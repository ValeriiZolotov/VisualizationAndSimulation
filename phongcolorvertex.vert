#version 410 core

/*phong shader for the textured objects */


layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec3 normal;

out vec3 fragmentPosition;
//out vec2 UV;
out vec3 Normal;

uniform mat4 matrix;
uniform mat4 pmatrix;
uniform mat4 vmatrix;



void main() {

   fragmentPosition = vec3(matrix * posAttr);
   Normal = mat3(transpose(inverse(matrix))) * normal;

   gl_Position = pmatrix * vmatrix * matrix * posAttr;
}
