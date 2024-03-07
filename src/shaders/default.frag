#version 410 core
layout (location=0) out vec4 fragColor;
#extension GL_ARB_shading_language_include: require
void main() {
    fragColor = vec4(0.5, 0.3, 0.5, 1.0);
}