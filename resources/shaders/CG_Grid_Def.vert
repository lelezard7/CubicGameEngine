#version 330 core

in vec4 in_gridPosition;
uniform mat4 u_transformationMatrix;

void main(void)
{
    gl_Position = u_transformationMatrix * in_gridPosition;
}
