#version 330 core

layout(location = 0) in float x;  // Input x-coordinate passed from the CPU

uniform float amplitude;  // Amplitude of the sine wave
uniform float frequency;  // Frequency of the sine wave

void main() {
    // Compute the y-coordinate using the sine function
    float y = amplitude * sin(frequency * x)* sin(frequency/4 * x);
    gl_Position = vec4(x, y, 0.0, 1.0);
}
