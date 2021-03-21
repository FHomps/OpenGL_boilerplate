#version 330 core
out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoords;

uniform sampler2D tex;

void main() {
    FragColor = texture(tex, texCoords) * vec4(vertexColor, 1.0);
}