#include "common.hpp"
#include "scene/light.hpp"
#include "util/mathfunc.hpp"


constexpr int LIGHT_NUM_CASCADES = 3;
constexpr int LIGHT_SHADOW_MAP_SIZE = 2048;
constexpr int LIGHT_FRUSTUM_CORNERS = 8;

EXPORT Base::ShadowMap::ShadowMap(Size2Di size)
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

EXPORT Base::Light::Light()
{
    Color debugColors[] = {
        Color(255, 0, 0, 128),
        Color(0, 255, 0, 128),
        Color(0, 0, 255, 128),
    };

    // Create shadowmaps
    for (uint i = 0; i < LIGHT_NUM_CASCADES; i++)
    {
        ShadowMap* map = new ShadowMap({ LIGHT_SHADOW_MAP_SIZE, LIGHT_SHADOW_MAP_SIZE });
        shadowMaps.add(map);
        map->debugMaterial = new Material(Image::createSingleColor(debugColors[i]));
        map->debugCamFrustumMesh = new TriangleMesh();
        map->debugOrthoBoxMesh = new TriangleMesh();
        map->debugCamFrustum = new Model(map->debugCamFrustumMesh, map->debugMaterial);
        map->debugOrthoBox = new Model(map->debugOrthoBoxMesh, map->debugMaterial);
    }
}

EXPORT Base::Light::~Light()
{
    for (ShadowMap* map : shadowMaps)
        delete map;
}

EXPORT void Base::Light::setPosition(const Vec3f& position)
{
    pos = position;
    dir = pos.normalize();
}

EXPORT Base::Light* Base::Light::translate(const Vec3f& translate)
{
    pos += translate;
    dir = pos.normalize();
    return this;
}

EXPORT void Base::Light::prepareShadowMaps(const Scene* scene)
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
    Camera* camera = scene->camera;
    Mat4f camView  = camera->getView();
    float tanHfov  = dtan(camera->getFovH() / 2.f);
    float tanVfov  = dtan(camera->getFovV() / 2.f);
    float zNear    = 5.f;
    float zFar     = 1000.f;
    float zDis     = zFar - zNear;

    for (uint i = 0; i < shadowMaps.size(); i++)
    {
        ShadowMap* map = shadowMaps[i];
        TriangleMesh* frustumMesh = map->debugCamFrustumMesh;
        TriangleMesh* orthoMesh = map->debugOrthoBoxMesh;
        float zn = zNear + LIGHT_CASCADES[i] * zDis;
        float zf = zNear + LIGHT_CASCADES[i + 1] * zDis;

        // Calculate the 8 points (4 + 4 for the near and far planes) that
        // make up this sub-section of the camera frustum.
        Mat4f camProj = Mat4f::perspective(tanVfov, camera->getRatio(), zn, zf);
        Mat4f camViewProjInv = (camProj * camView).inverse();
        Vec4f frustumCorners[LIGHT_FRUSTUM_CORNERS] = {
            camViewProjInv * Vec4f(-1,  1,  1, 1.f),
            camViewProjInv * Vec4f( 1,  1,  1, 1.f),
            camViewProjInv * Vec4f( 1, -1,  1, 1.f),
            camViewProjInv * Vec4f(-1, -1,  1, 1.f),
            camViewProjInv * Vec4f(-1,  1, -1, 1.f),
            camViewProjInv * Vec4f( 1,  1, -1, 1.f),
            camViewProjInv * Vec4f( 1, -1, -1, 1.f),
            camViewProjInv * Vec4f(-1, -1, -1, 1.f)
        };

        // Calculate box for the boundaries of the sub-frustum
        float minX = maxLimit<float>();
        float maxX = minLimit<float>();
        float minY = maxLimit<float>();
        float maxY = minLimit<float>();
        float minZ = maxLimit<float>();
        float maxZ = minLimit<float>();

        frustumMesh->clear();
        orthoMesh->clear();
        for (uint j = 0; j < LIGHT_FRUSTUM_CORNERS; j++)
        {
            // Convert the points from camera space to world space, and then into light space
            Vec3f cornerWorldSpace = frustumCorners[j].homogenize();
            Vec4f cornerLightSpace = lightMatV * cornerWorldSpace;
            minX = min(minX, cornerLightSpace.x);
            maxX = max(maxX, cornerLightSpace.x);
            minY = min(minY, cornerLightSpace.y);
            maxY = max(maxY, cornerLightSpace.y);
            minZ = min(minZ, cornerLightSpace.z);
            maxZ = max(maxZ, cornerLightSpace.z);
            frustumMesh->addVertex(Vertex3Df(cornerWorldSpace));
        }
        
        // Debug frustum
        frustumMesh->addTriangle({0, 4, 5}); frustumMesh->addTriangle({5, 1, 0}); // X-
        frustumMesh->addTriangle({7, 3, 2}); frustumMesh->addTriangle({2, 6, 7}); // X+
        frustumMesh->addTriangle({2, 1, 5}); frustumMesh->addTriangle({5, 6, 2}); // Y-
        frustumMesh->addTriangle({7, 4, 0}); frustumMesh->addTriangle({0, 3, 7}); // Y+
        frustumMesh->addTriangle({0, 1, 2}); frustumMesh->addTriangle({2, 3, 0}); // Z-
        frustumMesh->addTriangle({4, 7, 6}); frustumMesh->addTriangle({6, 5, 4}); // Z+
        frustumMesh->update();
        
        // Build frustum for culling occluders
        map->matP = Mat4f::ortho(minX, maxX, minY, maxY, -zFar, -minZ);
        map->matVP = map->matP * lightMatV;
        map->buildFrustum();

        // Extend Z to include occluders
        for (Object* obj : scene->objects)
        {
            if (!obj->getOcclude())
                continue;
            
            const BoundingBox& box = obj->getBoundingBox();
            if (!map->boxVisible(box))
                continue;

            for (uint i = 0; i < 8; i++)
            {
                // Convert bounding box from world space to
                // light space and shift the ortho box to include the object.
                Vec4f boxCornerLightSpace = lightMatV * box[i];
                maxZ = max(maxZ, boxCornerLightSpace.z);
            }
        }

        // Debug ortho box
        Mat4f lightMatVinv = lightMatV.inverse();
        Vec3f lightPoints[] = {
            { minX, maxY, maxZ },
            { minX, minY, maxZ },
            { maxX, minY, maxZ },
            { maxX, maxY, maxZ },
            { minX, maxY, minZ },
            { minX, minY, minZ },
            { maxX, minY, minZ },
            { maxX, maxY, minZ }
        };
        for (uint j = 0; j < LIGHT_FRUSTUM_CORNERS; j++)
        {
            Vec3f lightPointWorldSpace = lightMatVinv * lightPoints[j];
            orthoMesh->addVertex(Vertex3Df(Vec3f(lightPointWorldSpace.x, lightPointWorldSpace.y, lightPointWorldSpace.z)));
        }
        orthoMesh->addTriangle({0, 4, 5}); orthoMesh->addTriangle({5, 1, 0}); // X-
        orthoMesh->addTriangle({7, 3, 2}); orthoMesh->addTriangle({2, 6, 7}); // X+
        orthoMesh->addTriangle({2, 1, 5}); orthoMesh->addTriangle({5, 6, 2}); // Y-
        orthoMesh->addTriangle({7, 4, 0}); orthoMesh->addTriangle({0, 3, 7}); // Y+
        orthoMesh->addTriangle({0, 1, 2}); orthoMesh->addTriangle({2, 3, 0}); // Z-
        orthoMesh->addTriangle({4, 7, 6}); orthoMesh->addTriangle({6, 5, 4}); // Z+
        orthoMesh->update();

        // Use the box as the orthographic projection of this shadow map
        map->matP = Mat4f::ortho(minX, maxX, minY, maxY, -maxZ, -minZ);
        map->matVP  = map->matP * lightMatV;
        map->matBiasVP = matBias * shadowMaps[i]->matVP;
        map->buildFrustum();

        // End depth in clipspace
        Vec4f vView = { 0.f, 0.f, -(zNear + (LIGHT_CASCADES[i + 1] * zDis)), 1.f };
        Vec4f vClip = camera->getProjection() * vView;
        map->cascadeEndClipSpaceDepth = vClip.z;
    }
}