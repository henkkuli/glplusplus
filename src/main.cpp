#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <stdlib.h>

#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vao.h"

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
 vec3 points[] = {
	vec3(0.0f,  0.5f,  0.0f),
	vec3(0.5f, -0.5f,  0.0f),
	vec3(-0.5f, -0.5f,  0.0f)
};

void test() {
}

int main() {
	try {
		initGL();
		atexit(exit);
		
		shader v("shaders/defaultShader.v", vertexShader);
		shader f("shaders/defaultShader.f", fragmentShader);
		program p;
		p.attachShader(v);
		p.attachShader(f);
		p.compile();

		buffer vbo;
		vbo.setData(points, 3);
		vao vao;
		vao.setAttribute(0, 3, vbo);

		// Main loop
		while (!glfwWindowShouldClose(window)) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			p.use();
			vao.bind();

			glEnable (GL_PROGRAM_POINT_SIZE);
			glDrawArrays(GL_POINTS, 0, 3);
			glDisable(GL_PROGRAM_POINT_SIZE);

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
