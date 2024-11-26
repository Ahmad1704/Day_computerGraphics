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

std::vector<float> vertices;  // To store mouse coordinates for drawing

// Function to update vertices with mouse positions
void UpdateMousePosition(float mouseX, float mouseY) {
    // Map mouse coordinates from window space to OpenGL space (-1 to 1)
    float x = (2.0f * mouseX / WIDTH) - 1.0f;
    float y = -(2.0f * mouseY / HEIGHT) + 1.0f; // Y axis is inverted in OpenGL
    vertices.push_back(x);
    vertices.push_back(y);
}

// Function to setup the drawing environment
void SetupDrawing() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
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
    if (err != GLEW_OK) {
        cout << "Error";
        getchar();
        return 1;
    }
    else {
        if (GLEW_VERSION_3_0)
            cout << "Driver support OpenGL 3.0\nDetails:\n";
    }
    cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
    cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
    cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
    cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
    cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    CompileShader("VS.glsl", "FS.glsl", BasiceprogramId);
    SetupDrawing();
    glClearColor(0, 0.5, 0.5, 1);
    return 0;
}

// Render the line based on vertices
void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(BasiceprogramId);
    glBindVertexArray(VAO);

    // Update the VBO with new vertices before drawing
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

    // Draw lines based on the mouse positions
    glDrawArrays(GL_LINE_STRIP, 0, vertices.size()/2 );

    glBindVertexArray(0);
}

// Main update function (handles time-based changes like animation)
void Update() {
    // Nothing to update for now, we are just drawing lines as the mouse moves
}

int main() {
    sf::ContextSettings context;
    context.depthBits = 24;
    sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "Mouse Line Drawing with OpenGL", sf::Style::Close, context);

    if (Init()) return 1;

    bool mousePressed = false;  // To track if mouse button is pressed

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    mousePressed = true;
                    UpdateMousePosition(event.mouseButton.x, event.mouseButton.y);
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    mousePressed = false;
                }
                break;
            case sf::Event::MouseMoved:
                if (mousePressed) {
                    UpdateMousePosition(event.mouseMove.x, event.mouseMove.y);
                }
                break;
            }
        }

        Update();
        Render();

        window.display();
    }
    return 0;
}
