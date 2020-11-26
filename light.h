#ifndef LIGHT_H
#define LIGHT_H


#include "visualobject.h"
#include "mesh.h"

class Light : public VisualObject
{
public:
    Light()
        :m_LightColour(vec3(0.f,0.f,0.f)),m_AmbientIntensity(1.0f),m_DiffuseIntesity(1.f){}

    Light(const QVector4D &colourValue, float dIntesity)
        :m_LightColour(vec3(colourValue.x(),colourValue.y(),colourValue.z())),
          m_AmbientIntensity(colourValue.w()),
          m_DiffuseIntesity(dIntesity) {}

    virtual void init(GLint i) override;
    virtual void draw() override;
    virtual ~Light() override;

    void useLight(GLint ambientColourUniform, GLint ambientIntensityUnifor,
                  GLint diffuseIntensityUniform, GLint lightPositionUniform);
    vec3 getLightColour(){return m_LightColour;}
    GLfloat getAmbientIntensity(){return m_AmbientIntensity;}

private:
    vec3 m_LightColour;
    GLfloat m_AmbientIntensity;
    GLfloat m_rotate{0.f};
    vec3 m_LightPosition{0.f,15.f,0.f};
    GLfloat m_DiffuseIntesity;



};


#endif // LIGHT_H
