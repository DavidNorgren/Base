#include "common.hpp"
#include "scene/light.hpp"
#include "util/mathfunc.hpp"


constexpr int LIGHT_NUM_CASCADES = 1;
constexpr int LIGHT_SHADOW_MAP_SIZE = 1024;
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

void Base::Light::buildMatrix(float ratio)
{
    static Mat4f matBias = Mat4f(
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, 0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 0.5f, 0.5f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    matP = Mat4f::ortho(
        -orthoSize.x / 2.f, orthoSize.x / 2.f,
        -(orthoSize.y / 2.f) / ratio, (orthoSize.y / 2.f) / ratio,
        -orthoSize.z / 2.f, orthoSize.z / 2.f
    );
    matV = Mat4f::viewLookAt(pos, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });
    matVP = matP * matV;
    
    //for (uint i = 0; i < shadowMaps.size(); i++)
    //    shadowMaps[i]->matBiasVP = matBias * matVP;
}

void Base::Light::startShadowMapPass(const Camera* sceneCamera)
{
    static Mat4f matBias = Mat4f(
        0.5f, 0.0f, 0.0f, 0.5f,
        0.0f, 0.5f, 0.0f, 0.5f,
        0.0f, 0.0f, 0.5f, 0.5f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    matV = Mat4f::viewLookAt(dir, { 0.f, 0.f, 0.f }, { 0.f, 1.f, 0.f });

    Mat4f camMatInv = sceneCamera->getView().inverse();
    float tanHfov   = dtan(sceneCamera->getFovH() / 2.f);
    float tanVfov   = dtan(sceneCamera->getFovV() / 2.f);
    float zNear     = sceneCamera->getZNear();
    float zFar      = 200.f;//sceneCamera->getZFar();
    float zDis      = zFar - zNear;

    for (uint i = 0; i < shadowMaps.size(); i++)
    {
        float xn = (zNear + LIGHT_CASCADES[i]     * zDis) * tanHfov;
        float yn = (zNear + LIGHT_CASCADES[i]     * zDis) * tanVfov;
        float xf = (zNear + LIGHT_CASCADES[i + 1] * zDis) * tanHfov;
        float yf = (zNear + LIGHT_CASCADES[i + 1] * zDis) * tanVfov;

        // Calculate the 8 points (4 + 4 for the near and far planes) that
        // make up this sub-section of the camera frustum.

        Vec4f frustumCorners[LIGHT_FRUSTUM_CORNERS] = {
            Vec4f( xn,  yn, -(zNear + LIGHT_CASCADES[i]     * zDis), 1.0),
            Vec4f(-xn,  yn, -(zNear + LIGHT_CASCADES[i]     * zDis), 1.0),
            Vec4f( xn, -yn, -(zNear + LIGHT_CASCADES[i]     * zDis), 1.0),
            Vec4f(-xn, -yn, -(zNear + LIGHT_CASCADES[i]     * zDis), 1.0),
            Vec4f( xf,  yf, -(zNear + LIGHT_CASCADES[i + 1] * zDis), 1.0),
            Vec4f(-xf,  yf, -(zNear + LIGHT_CASCADES[i + 1] * zDis), 1.0),
            Vec4f( xf, -yf, -(zNear + LIGHT_CASCADES[i + 1] * zDis), 1.0),
            Vec4f(-xf, -yf, -(zNear + LIGHT_CASCADES[i + 1] * zDis), 1.0) 
        };

        // Calculate ortho matrix from the boundaries of the sub-frustum

        float minX = maxLimit<float>();
        float maxX = minLimit<float>();
        float minY = maxLimit<float>();
        float maxY = minLimit<float>();
        float minZ = maxLimit<float>();
        float maxZ = minLimit<float>();

        for (uint j = 0; j < LIGHT_FRUSTUM_CORNERS; j++)
        {
            Vec4f cornerLightSpace = matV * (camMatInv * frustumCorners[j]);
            minX = min(minX, cornerLightSpace.x);
            maxX = max(maxX, cornerLightSpace.x);
            minY = min(minY, cornerLightSpace.y);
            maxY = max(maxY, cornerLightSpace.y);
            minZ = min(minZ, cornerLightSpace.z);
            maxZ = max(maxZ, cornerLightSpace.z);
        }

        // Create ortho matrix for this shadow map
        matP = Mat4f::ortho(minX, maxX, minY, maxY, minZ, maxZ);
        matVP  = matP * matV;

        shadowMaps[i]->matBiasVP = matBias * matVP;
    }
}