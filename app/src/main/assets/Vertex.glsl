#version 300 es
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

out vec2 fragUV;

uniform mat4 uProjectionMatrix;
uniform mat4 uTranslationMatrix;
uniform mat4 uScaleMatrix;
uniform mat4 uRotationXMatrix;
uniform mat4 uRotationYMatrix;
uniform mat4 uRotationZMatrix;
uniform mat4 uViewMatrix;

void main() {
    fragUV = inUV;
    mat4 rotationMatrix = uRotationZMatrix * uRotationYMatrix * uRotationXMatrix;
    mat4 worldMatrix = uTranslationMatrix * rotationMatrix * uScaleMatrix;
    gl_Position = uProjectionMatrix * uViewMatrix * worldMatrix * vec4(inPosition, 1.0);
}