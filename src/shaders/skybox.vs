#version 330

layout(location = 0) in vec3 vertexPosition;

uniform mat4 projection;
uniform mat4 view;

out vec3 fragPosition;

void main() {
    fragPosition = vertexPosition;  // Pass position to fragment shader
    gl_Position = projection * view * vec4(vertexPosition, 1.0);
}