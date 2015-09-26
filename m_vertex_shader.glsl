#version 430 core

uniform mat4 gWorld;
layout(location = 0) in vec3 vert;
layout(location = 1) in vec2 texCoord;
out vec2 fragTexCoord;

void main() {
    gl_Position = gWorld * vec4(vert, 1);
	fragTexCoord = texCoord;
}