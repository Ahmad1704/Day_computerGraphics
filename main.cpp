#include <iostream>
#include <gl\glew.h>
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


GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint VBO, BasiceprogramId;
DrawingMode Current_DrawingMode = DrawingMode::Lines;


const int numPoints = 30;

GLfloat TriangleVertices[numPoints * 6];  




void initPoints() {
	float angleStep = 2.0f * 3.14159f / numPoints;


	for (int i = 0; i <= numPoints; ++i) {
		float angle = i * angleStep;

		
		TriangleVertices[i * 6] = cos(angle);  
		TriangleVertices[i * 6 + 1] = sin(angle); 
		TriangleVertices[i * 6 + 2] = 0.0f;        

		
		TriangleVertices[i * 6 + 3] = cos(angle); // Red
		TriangleVertices[i * 6 + 4] = sin(angle); // Green
		TriangleVertices[i * 6 + 5] = .5; // Blue
	}
}

void CreateColoredTriangle()
{
	initPoints();
	
	// create buffer object
	glGenBuffers(1, &VBO);

	// binding buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriangleVertices), TriangleVertices, GL_STATIC_DRAW);

	// shader
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(GL_FLOAT), (char*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
}

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_namering, GLuint& programId)
{
	programId = InitShader(vertex_shader_file_name, fragment_shader_file_namering);
	glUseProgram(programId);
}

int Init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Error";
		getchar();
		return 1;
	}
	else
	{
		if (GLEW_VERSION_3_0)
			cout << "Driver support OpenGL 3.0\nDetails:\n";
	}
	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	CompileShader("VS.glsl", "FS.glsl", BasiceprogramId);
	CreateColoredTriangle();
	glClearColor(0, 0.5, 0.5, 1);

	return 0;
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	switch (Current_DrawingMode)
	{
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
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 30);
}

float theta = 0;
void Update()
{
	// add all tick code
	theta += 0.01f;

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