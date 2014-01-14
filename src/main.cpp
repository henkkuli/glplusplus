#include <GL/glew.h>
#include <GL/glfw3.h>
#include <iostream>
#include <stdlib.h>

#include "uniform.h"
#include "shader.h"
#include "program.h"
#include "buffer.h"
#include "vao.h"
#include "texture.h"

using namespace std;

GLFWwindow *window;

int windowWidth = 640;
int windowHeight = 480;

mat4 projection;
mat4 view;
vec3 cameraPos(0, 0, -2);
float cameraHori = 0;
float cameraVert = 0;
bool cursorCaptured = false;

void windowResizeCallback(GLFWwindow *window, int width, int height) {
	windowWidth = width;
	windowHeight = height;
	projection = mat4::perspective(45, 1.0f*width/height, 0.1, 100);
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
	windowResizeCallback(window, windowWidth, windowHeight);

	glewExperimental = GL_TRUE;
	glewInit();
	
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "Version: " << version << endl;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void stopGL() {
	glfwTerminate();
}

void exit() {
	stopGL();
}

vec3 points[] = {
   vec3(0.5f, -0.5f,  0.0f),
   vec3(0.0f,  0.5f,  0.0f),
   vec3(-0.5f, -0.5f,  0.0f)
};

#define PARTICLES_COUNT (128)
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
		program particleRenderProgram;
		particleRenderProgram.attachShader(shader("shaders/particleShader_vs.glsl", vertexShader));
		particleRenderProgram.attachShader(shader("shaders/particleShader_fs.glsl", fragmentShader));
		particleRenderProgram.attachShader(shader("shaders/particleShader_gs.glsl", geometryShader));
		particleRenderProgram.link();
		//uniform particleTimeUniform = particleRenderProgram.getUniform("time");
		uniform particleProjectionUniform = particleRenderProgram.getUniform("proj");
		uniform particleViewUniform = particleRenderProgram.getUniform("view");
		//uniform particleSdUniform = particleRenderProgram.getUniform("screenDimensions");

		program computeProgram;
		computeProgram.attachShader(shader("shaders/particleShader_cs.glsl", computeShader));
		computeProgram.link();
		uniform deltatimeUniform = computeProgram.getUniform("dtime");
		uniform computeTimeUniform = computeProgram.getUniform("time");

		// Setup particle system
		for (int i = 0; i < PARTICLES_COUNT; ++i) {
			particle &p = particles[i];		// Shortcut
			p.position = vec4(0, 0, 0, 1);
			p.velocity = vec4(0, 0, 0, 0);
		}

		buffer particleBuffer(shaderStorageBuffer);
		particleBuffer.allocate(particles, PARTICLES_COUNT);

		// Setup vao for rendering
		vao particleVao;
		particleVao.setAttribute(0, 4, particleBuffer);
		particleVao.setAttribute(1, 4, particleBuffer, sizeof(vec4));


		// Triangle
		buffer triangleBuffer(arrayBuffer);
		triangleBuffer.allocate(points, 3);
		vao triangleVao;
		triangleVao.setAttribute(0, 3, triangleBuffer);
		program triangleRenderProgram;
		triangleRenderProgram.attachShader(shader("shaders/defaultShader_vs.glsl", vertexShader));
		triangleRenderProgram.attachShader(shader("shaders/defaultShader_fs.glsl", fragmentShader));
		triangleRenderProgram.link();
		//uniform triangleTimeUniform = triangleRenderProgram.getUniform("time");
		uniform triangleProjectionUniform = triangleRenderProgram.getUniform("proj");
		uniform triangleViewUniform = triangleRenderProgram.getUniform("view");

		// For debugging
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		
		// Timing
		double lastUpdate = glfwGetTime();
		// Main loop
		while (!glfwWindowShouldClose(window)) {
			// Setup OpenGL for newx frame
			glViewport(0, 0, windowWidth, windowHeight);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Compute delta time
			double now = glfwGetTime();
			double delta = now - lastUpdate;
			lastUpdate = now;
			//cout << now << endl;

			// Input
			if (glfwGetKey(window, GLFW_KEY_A)) {
				cameraPos += vec3(cos(cameraHori), 0, -sin(cameraHori)) * delta;
			}
			if (glfwGetKey(window, GLFW_KEY_D)) {
				cameraPos -= vec3(cos(cameraHori), 0, -sin(cameraHori)) * delta;
			}
			if (glfwGetKey(window, GLFW_KEY_W)) {
				cameraPos += vec3(sin(cameraHori), 0, cos(cameraHori)) * delta;
			}
			if (glfwGetKey(window, GLFW_KEY_S)) {
				cameraPos -= vec3(sin(cameraHori), 0, cos(cameraHori)) * delta;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
				cameraPos += vec3(0, 1, 0) * delta;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
				cameraPos -= vec3(0, 1, 0) * delta;
			}
			if (glfwGetKey(window, GLFW_KEY_ESCAPE)) {
				cursorCaptured = false;
			}
			if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
				cursorCaptured = true;
				glfwSetCursorPos(window, windowWidth/2, windowHeight/2);
			}
			if (cursorCaptured) {
				double mx, my;
				glfwGetCursorPos(window, &mx, &my);
				glfwSetCursorPos(window, windowWidth/2, windowHeight/2);
				cameraHori += (windowWidth/2 - mx) * delta;
				cameraVert += (windowHeight/2 - my) * delta;
			}
			const vec3 cameraDir = vec3(
				cos(cameraVert) * sin(cameraHori),
				sin(cameraVert),
				cos(cameraVert) * cos(cameraHori)
			);
			view = mat4::lookAt(cameraPos, cameraPos + cameraDir, vec3(0, 1, 0));
			cout<<cameraPos.h[0]<<" "<<cameraPos.h[1]<<" "<<cameraPos.h[2]<<endl;

			// Compute new particle locations
			computeProgram.use();
			deltatimeUniform = delta;
			computeTimeUniform = now;
			particleBuffer.bind(shaderStorageBuffer, 0);
			if (!glfwGetKey(window, GLFW_KEY_SPACE))
    		    computeProgram.dispatch(PARTICLES_COUNT / 128, 1, 1);
			computeProgram.barrier(vertexAttribArrayBarrier);

			// Rendering
			particleRenderProgram.use();
			//particleTimeUniform = now;
			//particleSdUniform = vec2(windowWidth, windowHeight);
			particleProjectionUniform = projection;
			particleViewUniform = view;

			particleVao.bind();

			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glDrawArrays(GL_POINTS, 0, PARTICLES_COUNT);
			glDisable(GL_BLEND);
			glEnable(GL_DEPTH_TEST);

			// Triangle
			triangleRenderProgram.use();
			//triangleTimeUniform = now;
			triangleProjectionUniform = projection;
			triangleViewUniform = view;

			triangleVao.bind();

			glDrawArrays(GL_TRIANGLES, 0, 3);

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
