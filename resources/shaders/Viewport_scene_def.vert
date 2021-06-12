#version 330 core
layout (location = 0) in vec3 in_position;

//uniform mat4 uni_model;
//uniform mat4 uni_view;
uniform mat4 uni_projection;

void main()
{
    gl_Position = uni_projection * vec4(in_position, 1.0);
}









//attribute vec4 qt_Vertex;
//attribute vec4 qt_MultiTexCoord0;
//uniform mat4 qt_ModelViewProjectionMatrix;
//varying vec4 qt_TexCoord0;

//void main(void)
//{
//    gl_Position = qt_ModelViewProjectionMatrix * qt_Vertex;
//    qt_TexCoord0 = qt_MultiTexCoord0;
//}
