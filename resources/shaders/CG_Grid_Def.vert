#version 330 core

in vec4 in_gridPosition;
uniform mat4 u_modelViewProjectionMatrix;

void main(void)
{
    gl_Position = u_modelViewProjectionMatrix * in_gridPosition;
}
