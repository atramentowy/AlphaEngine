#version 330

in vec3 fragPosition;

out vec4 fragColor;

void main() {
    vec4 darkBlue = vec4(0.0, 0.0, 0.5, 1.0);   // Dark blue
    vec4 lightBlue = vec4(0.5, 0.7, 1.0, 1.0);  // Light blue
    vec4 white = vec4(1.0, 1.0, 1.0, 1.0);      // White

    // Normalize Y [-1, 1] -> [0, 1], and adjust to control transition points
    float normalizedY = (fragPosition.y + 1.0) * 0.5;  // Scale Y to [0, 1]

    // Interpolate between dark blue and light blue over the lower half of the height
    float t1 = smoothstep(0.0, 0.5, normalizedY); // Transition between dark blue and light blue
    
    // Interpolate between light blue and white over the upper half of the height
    float t2 = smoothstep(0.5, 1.0, normalizedY); // Transition between light blue and white
    
    // First mix: dark blue to light blue
    vec4 color1 = mix(darkBlue, lightBlue, t1);
    // Second mix: light blue to white
    vec4 color2 = mix(lightBlue, white, t2);

    // Final color is a combination of both mixes
    fragColor = mix(color1, color2, t2); // Smoothly blend between both transitions
}
