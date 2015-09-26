#version 430 core

uniform sampler2D tex;
in vec2 fragTexCoord;
out vec4 fragColor;

void main()
{
	fragColor = texture(tex,fragTexCoord);
}