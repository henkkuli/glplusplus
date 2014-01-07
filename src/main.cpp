#include <gl\glew.h>
#include <gl\glfw3.h>
#include <iostream>

#include "shader.h"
#include "program.h"

using namespace std;

GLFWwindow *window;

void initGL() {
	if (!glfwInit())
		throw "Couldn't start GLFW";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Window!!!", 0, 0);
	if (!window) {
		glfwTerminate();
		throw "Couldn't open a window";
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "Version: " << version << endl;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void stopGL() {
	glfwTerminate();
}

void exit() {
	stopGL();
}
 float points[] = {
	0.0f,  0.5f,  0.0f,
	0.5f, -0.5f,  0.0f,
	-0.5f, -0.5f,  0.0f
};
 unsigned int vbo = 0, vao = 0,p=0;

void test() {
}

int main() {
	try {
		initGL();
		atexit(exit);
		
		shader v("defaultShader.v", vertexShader);
		shader f("defaultShader.f", fragmentShader);
		program p;
		p.attachShader(v);
		p.attachShader(f);
		p.compile();

		// TODO Remove
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), points,GL_STATIC_DRAW);

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Main loop
		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			p.use();
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}

		stopGL();
	} catch (const char *str) {
		stopGL();
		cerr << str << endl;
		cin.get();
		return 1;
	}
}