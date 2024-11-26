#include <iostream>
#include <gl/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

enum DrawingMode
{
    Points,
    Lines,
    FilledTriangle
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, VAO, BasiceprogramId;
DrawingMode Current_DrawingMode = DrawingMode::Lines;

const int NUM_POINTS = 250; // Number of points on the sine wave
vector<float> vertices;

// Function to generate x-coordinates for the sine wave
void GenerateSineWaveVertices() {
    float xStart = -1.0f;  // Start of the x range
    float xEnd = 1.0f;     // End of the x range
    float step = (xEnd - xStart) / (NUM_POINTS - 1); // Ensure correct range

    // Generate x-coordinates only (y will be computed in the vertex shader)
    for (int i = 0; i < NUM_POINTS; ++i) {
        float x = xStart + i * step;
        vertices.push_back(x);     // x-coordinate
    }
}

// Function to setup the sine wave
void SetupSineWave() {
    GenerateSineWaveVertices();

    // Generate and bind VAO and VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 1, GL_FLOAT, false, sizeof(float),0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Shader compilation and program linking
void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name, GLuint& programId) {
    programId = InitShader(vertex_shader_file_name, fragment_shader_file_name);
    glUseProgram(programId);
}

// Initialize OpenGL and compile shaders
int Init() {
    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        cout << "Error: " << glewGetErrorString(err) << endl;
        getchar();
        return 1;
    }
    cout << "Using GLEW: " << glewGetString(GLEW_VERSION) << endl;

    CompileShader("VS.glsl", "FS.glsl", BasiceprogramId);
    SetupSineWave();
    glClearColor(0, 0.5, 0.5, 1);
    return 0;
}

// Render the sine wave
void RenderSineWave() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(BasiceprogramId);
    glBindVertexArray(VAO);

    // Set the uniforms (amplitude and frequency)
    GLint amplitudeLoc = glGetUniformLocation(BasiceprogramId, "amplitude");
    GLint frequencyLoc = glGetUniformLocation(BasiceprogramId, "frequency");

    // Pass amplitude and frequency to the shader
    glUniform1f(amplitudeLoc, 0.5f);  // Example amplitude
    glUniform1f(frequencyLoc, 80.0f); // Example frequency

    // Draw the sine wave as a line strip
    glDrawArrays(GL_LINE_STRIP, 0, NUM_POINTS);

    glBindVertexArray(0);
}

// Main update function (handles time-based changes like animation)
void Update() {
    // For now, no dynamic updates are needed in the sine wave, but you can modify amplitude/frequency.
}

// Main program
int main() {
    sf::ContextSettings context;
    context.depthBits = 24;
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Sine Wave with OpenGL", sf::Style::Close, context);

    if (Init()) return 1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        Update();
        RenderSineWave();

        window.display();
    }
    return 0;
}
