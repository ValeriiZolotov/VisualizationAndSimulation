#version 410 core

/*phong shader for the textured objects */

in vec2 UV;
in vec3 Normal;
in vec3 fragmentPosition;

uniform vec4 objectColor = vec4(0.7, 0.7, 0.7,1);

out vec4 fragColor;
uniform vec3 lightColour;
uniform float ambientIntesity;
uniform float diffuseIntensity;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;
uniform float specularIntensity;
uniform float specularExponent;

void main() {

    // Ambient light
    vec4 ambient = vec4(lightColour,1.0)*ambientIntesity;
    // Diffuse light
    vec3 lightDirection = vec3(lightPosition - fragmentPosition);
    float diffuseFactor = max(dot(normalize(Normal),normalize(lightDirection)),0);//angle of the light
    vec4 diffuseColour = vec4(lightColour,0.f)*diffuseIntensity*diffuseFactor;
   // Specular light
    vec4 specularColour = vec4(0,0,0,0);

    if(lightPosition.y>0.0f)
    {
        if(diffuseFactor>0.0f)
        {
           vec3 fragToCamera = normalize(cameraPosition - fragmentPosition);
           vec3 reflectedVertex = reflect(-normalize(lightDirection),normalize(Normal));
           float specularFactor = dot(fragToCamera,reflectedVertex);
           if(specularFactor > 0.f)
           {
               specularFactor = pow(specularFactor,specularExponent);
               specularColour = vec4(lightColour*specularIntensity*specularFactor,1);
           }

        }
      }

    fragColor = (ambient+diffuseColour+specularColour)*objectColor;
}
