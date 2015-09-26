#version 430 core

uniform sampler2D tex;
in vec2 fragTexCoord;
out vec4 fragColor;

void main()
{
	fragColor = vec4(0.0,1.0,1.0,1.0);
}