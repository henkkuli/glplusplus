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

int windowWidth = 640;
int windowHeight = 480;

void windowResizeCallback(GLFWwindow *window, int width, int height) {
	windowWidth = width;
	windowHeight = height;
}

void initGL() {
	if (!glfwInit())
		throw "Couldn't start GLFW";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(windowWidth, windowHeight, "Window!!!", 0, 0);
	if (!window) {
		glfwTerminate();
		throw "Couldn't open a window";
	}

	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	glewExperimental = GL_TRUE;
	glewInit();
	
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "Version: " << version << endl;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void stopGL() {
	glfwTerminate();
}

void exit() {
	stopGL();
}

#define PARTICLES_COUNT (1024*4*4)
 struct particle {
	 vec4 position;
	 vec4 velocity;
	 particle() {}
 } particles[PARTICLES_COUNT];

int main() {
	try {
		// Initialize 
		initGL();
		atexit(exit);
		
		// Load shaders
		program renderProgram;
		renderProgram.attachShader(shader("shaders/particleShader_vs.glsl", vertexShader));
		renderProgram.attachShader(shader("shaders/particleShader_fs.glsl", fragmentShader));
		renderProgram.attachShader(shader("shaders/particleShader_gs.glsl", geometryShader));
		renderProgram.link();
		uniform renderTimeUniform = renderProgram.getUniform("time");

		program computeProgram;
		computeProgram.attachShader(shader("shaders/particleShader_cs.glsl", computeShader));
		computeProgram.link();
		uniform deltatimeUniform = computeProgram.getUniform("dtime");
		uniform computeTimeUniform = computeProgram.getUniform("time");

		// Setup particle system
		for (int i = 0; i < PARTICLES_COUNT; ++i) {
			particle &p = particles[i];		// Shortcut
			p.position = vec4(0, 0, 0, 1);
			p.velocity = vec4(0, 0, 0, ((float) i) * (1.0 / PARTICLES_COUNT));
		}

		buffer particleBuffer(shaderStorageBuffer);
		particleBuffer.allocate(particles, PARTICLES_COUNT);

		// Setup vao for rendering
		vao vao;
		vao.setAttribute(0, 4, particleBuffer);
		vao.setAttribute(1, 4, particleBuffer, sizeof(vec4));

		// For debugging
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		// Timing
		double lastUpdate = glfwGetTime();

		// Main loop
		while (!glfwWindowShouldClose(window)) {
			glViewport(0, 0, windowWidth, windowHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			double now = glfwGetTime();
			double delta = now - lastUpdate;
			lastUpdate = now;

			cout << now << endl;

			// Compute new particle locations
			computeProgram.use();
			deltatimeUniform = delta;
			computeTimeUniform = now;
			particleBuffer.bind(shaderStorageBuffer, 0);
			computeProgram.dispatch(PARTICLES_COUNT / 128, 1, 1);
			computeProgram.barrier(vertexAttribArrayBarrier);

			// Rendering
			renderProgram.use();
			renderTimeUniform = now;
			vao.bind();

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glDrawArrays(GL_POINTS, 0, PARTICLES_COUNT);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);

			// Update window
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
