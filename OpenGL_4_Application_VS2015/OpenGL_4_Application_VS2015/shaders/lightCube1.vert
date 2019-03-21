#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

uniform mat4 model1;
uniform mat4 view1;
uniform mat4 projection1;

void main() 
{
	gl_Position = projection1 * view1 * model1 * vec4(vPosition, 1.0f);
}
