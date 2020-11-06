#version 330 core

layout(location = 0) in vec4 posAttr;
layout(location = 1) in vec4 colAttr;


out vec4 col;
uniform mat4 matrix;
uniform mat4 pmatrix;
uniform mat4 vmatrix;


void main() {
   col = colAttr;
   gl_Position = pmatrix * vmatrix * matrix * posAttr;
}
