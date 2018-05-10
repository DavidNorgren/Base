#pragma once

#include "render/image.hpp"
#include "render/colors.hpp"


namespace Base
{
    enum IlluminationModel
    {
        COLOR_ON_AMBIENT_OFF = 0,
        COLOR_ON_AMBIENT_ON = 1,
        HIGHLIGHT_ON = 2,
        REFL_ON_RT_ON = 3,
        TRANSP_GLASS_ON_REFL_RT_ON = 4,
        REFL_FRESNEL_ON_RT_ON = 5,
        TRANSP_REFR_ON_REFL_FRESNEL_OFF_RT_ON = 6,
        TRANSP_REFR_ON_REFL_FRESNEL_ON_RT_ON = 7,
        REFL_ON_RT_OFF = 8,
        TRANSP_GLASS_ON_REFLECTION_RT_OFF = 9,
        CASTS_SHADOWS_ONTO_INVISIBLE = 10
    };

    /* Material, based on the OBJ MTL format. */
    class Material
    {
      public:
        Material() {}
        Material(Image* texture) : diffuseTexture(texture) {}

        // Setters
        void setAmbient(const Color& ambient)           { this->ambient = ambient; }
        void setDiffuse(const Color& diffuse)           { this->diffuse = diffuse; }
        void setSpecular(const Color& specular)         { this->specular = specular; }
        void setSpecularExp(float specularExp)          { this->specularExp = specularExp; }
        void setTransmission(const Color& transmission) { this->transmission = transmission; }
        void setOpacity(float opacity)                  { this->opacity = opacity; }

        void setTexture(Image* texture)            { this->diffuseTexture = texture; }
        void setAmbientTexture(Image* texture)     { this->ambientTexture = texture; }
        void setDiffuseTexture(Image* texture)     { this->diffuseTexture = texture; }
        void setSpecularTexture(Image* texture)    { this->specularTexture = texture; }
        void setSpecularExpTexture(Image* texture) { this->specularExpTexture = texture; }
        void setOpacityTexture(Image* texture)     { this->opacityTexture = texture; }
        void setBumpTexture(Image* texture)        { this->bumpTexture = texture; }

        void setIllumModel(IlluminationModel illumModel) { this->illumModel = illumModel; }

        // Getters
        Color getAmbient() const      { return ambient; }
        Color getDiffuse() const      { return diffuse; }
        Color getSpecular() const     { return specular; }
        float getSpecularExp() const  { return specularExp; }
        Color getTransmission() const { return transmission; }
        float getOpacity() const      { return opacity; }

        EXPORT Image* getTexture() const;
        EXPORT Image* getAmbientTexture() const;
        EXPORT Image* getDiffuseTexture() const;
        EXPORT Image* getSpecularTexture() const;
        EXPORT Image* getSpecularExpTexture() const;
        EXPORT Image* getOpacityTexture() const;
        EXPORT Image* getBumpTexture() const;

        IlluminationModel getIllumModel() const { return illumModel; }

      private:
        Color ambient = Colors::WHITE;       // Ka
        Color diffuse = Colors::WHITE;       // Kd
        Color specular = Colors::WHITE;      // Ks
        float specularExp = 10.f;            // Ns
        Color transmission = Colors::WHITE;  // Tf
        float opacity = 1.f;                 // d

        Image* ambientTexture = nullptr;     // map_Ka
        Image* diffuseTexture = nullptr;     // map_Kd
        Image* specularTexture = nullptr;    // map_Ks
        Image* specularExpTexture = nullptr; // map_Ns
        Image* opacityTexture = nullptr;     // map_d
        Image* bumpTexture = nullptr;        // map_bump

        IlluminationModel illumModel = HIGHLIGHT_ON; // illum
    };
}