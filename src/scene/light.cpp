#include "common.hpp"
#include "scene/light.hpp"
#include "util/mathfunc.hpp"


constexpr int LIGHT_NUM_CASCADES = 3;
constexpr int LIGHT_SHADOW_MAP_SIZE = 2048;
constexpr int LIGHT_FRUSTUM_CORNERS = 8;

Base::ShadowMap::ShadowMap(Size2Di size)
{
    this->size = size;

    // Create depth texture
    Color borderColor(1.f);
    glGenTextures(1, &glTexture);
    glBindTexture(GL_TEXTURE_2D, glTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float*)&borderColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, size.width, size.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    
    // Create framebuffer
    glGenFramebuffers(1, &glFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, glFramebuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, glTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    matBiasVP = Mat4f::identity();
}

Base::Light::Light()
{
    // Create shadowmaps
    for (uint i = 0; i < LIGHT_NUM_CASCADES; i++)
        shadowMaps.add(new ShadowMap({ LIGHT_SHADOW_MAP_SIZE, LIGHT_SHADOW_MAP_SIZE }));
}

Base::Light::~Light()
{
    for (ShadowMap* map : shadowMaps)
        delete map;
}

void Base::Light::setPosition(const Vec3f& position)
{
    pos = position;
    dir = pos.normalize();
}

Base::Light* Base::Light::translate(const Vec3f& translate)
{
    pos += translate;
    dir = pos.normalize();
    return this;
}

void Base::Light::prepareShadowMaps(const Camera* sceneCamera)
{
    // Matrix for converting from -1->1 to 0->1 in the shader
    static Mat4f matBias = Mat4f(
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, 0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 0.5f, 0.5f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    // Get light view matrix
    Mat4f lightMatV = Mat4f::viewLookAt(dir, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
    
    // Get camera properties
    Mat4f camMatInv = sceneCamera->getView().inverse();
    float tanHfov   = dtan(sceneCamera->getFovH() / 2.f);
    float tanVfov   = dtan(sceneCamera->getFovV() / 2.f);
    float zNear     = 5.f;
    float zFar      = 2000.f;
    float zDis      = zFar - zNear;

    for (uint i = 0; i < shadowMaps.size(); i++)
    {
        float zn = zNear + LIGHT_CASCADES[i] * zDis;
        float zf = zNear + LIGHT_CASCADES[i + 1] * zDis;
        float xn = zn * tanHfov;
        float yn = zn * tanVfov;
        float xf = zf * tanHfov;
        float yf = zf * tanVfov;

        // Calculate the 8 points (4 + 4 for the near and far planes) that
        // make up this sub-section of the camera frustum.

        Vec4f frustumCorners[LIGHT_FRUSTUM_CORNERS] = {
            Vec4f( xn,  yn, -zn, 1.f),
            Vec4f(-xn,  yn, -zn, 1.f),
            Vec4f( xn, -yn, -zn, 1.f),
            Vec4f(-xn, -yn, -zn, 1.f),
            Vec4f( xf,  yf, -zf, 1.f),
            Vec4f(-xf,  yf, -zf, 1.f),
            Vec4f( xf, -yf, -zf, 1.f),
            Vec4f(-xf, -yf, -zf, 1.f) 
        };

        // Calculate box for the boundaries of the sub-frustum
        float minX =  10000000.f; //maxLimit<float>();
        float maxX = -10000000.f; //minLimit<float>();
        float minY =  10000000.f; //maxLimit<float>();
        float maxY = -10000000.f; //minLimit<float>();
        float minZ =  10000000.f; //maxLimit<float>();
        float maxZ = -10000000.f; //minLimit<float>();

        for (uint j = 0; j < LIGHT_FRUSTUM_CORNERS; j++)
        {
            // Convert the points from camera space to world space, and then into light space
            Vec4f cornerWorldSpace = (camMatInv * frustumCorners[j]);
            Vec4f cornerLightSpace = lightMatV * cornerWorldSpace;
            minX = min(minX, cornerLightSpace.x);
            maxX = max(maxX, cornerLightSpace.x);
            minY = min(minY, cornerLightSpace.y);
            maxY = max(maxY, cornerLightSpace.y);
            minZ = min(minZ, cornerLightSpace.z);
            maxZ = max(maxZ, cornerLightSpace.z);
        }

        // Use the box as the orthographic projection of this shadow map
        Mat4f mapMatP = Mat4f::ortho(minX, maxX, minY, maxY, minZ, maxZ);
        //Vec3f frusCentre = { (maxX + minX) / 2, (maxY + minY) / 2, (maxZ + minZ) / 2};
        //Mat4f mapMatV = lightMatV * Mat4f::translate(-frusCentre);

        // Create matrices to use in the shaders
        shadowMaps[i]->matVP  = mapMatP * lightMatV;
        shadowMaps[i]->matBiasVP = matBias * shadowMaps[i]->matVP;

        // End depth in clipspace
        Vec4f vView = { 0.f, 0.f, -(zNear + (LIGHT_CASCADES[i + 1] * zDis)), 1.f };
        Vec4f vClip = sceneCamera->getProjection() * vView;
        shadowMaps[i]->cascadeEndClipSpaceDepth = vClip.z;
    }
}