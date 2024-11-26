#include <iostream>
#include <gl/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include<glm\glm.hpp>

using namespace std;
using namespace glm;

enum DrawingMode
{
	Points,
	Lines,
	FilledTriangle
};

struct Vertex
{
	vec3 Position;
	vec3 COlor;
};

const int MAX_RECURSION_DEPTH = 5;

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, BasiceprogramId;
DrawingMode Current_DrawingMode = DrawingMode::Lines;


void GenerateSierpinski(vector<float>& vertices, float x, float y, float size, int depth) {
	if (depth == 0) {
		// Base case: Create a single triangle
		// Triangle vertices (clockwise order)
		vertices.insert(vertices.end(), { x, y, 0.0f });              // Bottom-left vertex
		vertices.insert(vertices.end(), { x + size, y, 0.0f });       // Bottom-right vertex
		vertices.insert(vertices.end(), { x + size / 2, y + size, 0.0f }); // Top vertex
	}
	else {
		// Recursive case: Divide triangle into 3 smaller triangles
		float halfSize = size / 2.0f;

		// Bottom-left triangle
		GenerateSierpinski(vertices, x, y, halfSize, depth - 1);

		// Bottom-right triangle
		GenerateSierpinski(vertices, x + halfSize, y, halfSize, depth - 1);

		// Top triangle
		GenerateSierpinski(vertices, x + halfSize / 2, y + halfSize, halfSize, depth - 1);
	}
}

void CreateColoredTriangle()
{
	vector<float> vertices;
	GenerateSierpinski(vertices, -0.8f, -0.8f, 1.6f, MAX_RECURSION_DEPTH);

	// Add color data (e.g., white for simplicity)
	vector<float> fullData;
	for (size_t i = 0; i < vertices.size(); i += 3) {
		fullData.insert(fullData.end(), { vertices[i], vertices[i + 1], vertices[i + 2] }); // Position
		fullData.insert(fullData.end(), { 1.0f, 1.0f, 1.0f });                             // Color (white)
	}

	// Create buffer object
	glGenBuffers(1, &VBO);

	// Bind buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, fullData.size() * sizeof(float), fullData.data(), GL_STATIC_DRAW);

	// Vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0); // Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color
	glEnableVertexAttribArray(1);
}

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_namering, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_namering);
	glUseProgram(programId);
}

int Init() {
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		cout << "Error initializing GLEW" << endl;
		return 1;
	}

	cout << "OpenGL Initialized" << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	CompileShader("VS.glsl", "FS.glsl", BasiceprogramId);
	CreateColoredTriangle();

	glClearColor(0, 0.5, 0.5, 1);
	return 0;
}
void Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	switch (Current_DrawingMode) {
	case Points:
		glPointSize(10);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		break;
	case Lines:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case FilledTriangle:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	default:
		break;
	}

	// Draw based on recursion depth
	int vertexCount = pow(3, MAX_RECURSION_DEPTH) * 3; // Total vertices
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
float theta = 0;
void Update()
{
	// add all tick code
	//theta += 0.0001f;

	GLuint Theta_Location = glGetUniformLocation(BasiceprogramId, "theta");
	glUniform1f(Theta_Location, theta);
}

int main()
{
	sf::ContextSettings context;
	context.depthBits = 24;
	sf::Window window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Close, context);

	if (Init()) return 1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
			{
				window.close();
				break;
			}
			case sf::Event::KeyPressed:
			{
				if (event.key.code == sf::Keyboard::Num1)
				{
					Current_DrawingMode = DrawingMode::Points;
				}
				if (event.key.code == sf::Keyboard::Num2)
				{
					Current_DrawingMode = DrawingMode::Lines;
				}
				if (event.key.code == sf::Keyboard::Num3)
				{
					Current_DrawingMode = DrawingMode::FilledTriangle;
				}
				break;
			}
			}
		}

		Update();
		Render();

		window.display();
	}
	return 0;
}