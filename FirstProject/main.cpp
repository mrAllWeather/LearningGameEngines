// TODO: Refactor into multiple files; This is getting rather fat!

#include <iostream>
#include <string>
#include <vector>

#include "ObjectLoader.h"
#include "ShaderLoader.h"

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// Window Dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// Function Prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
	// Init GLFW
	glfwInit();
	// Set GLFW req options
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create window
	/* We can create as many windows as we like via this method; remember that each window will need to be made 
	current context to have actions applied to it, so really you should have a pointer / function that can be
	reused to perform the same actions if you want multiple windows.*/ 
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr); // Window 1
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// I.E This is where the following actions shall occur
	glfwMakeContextCurrent(window);

	// Set required callbacks: Guessing this becomes a large switch which will then be converted into a hash (after all you always want rebindable keys!)
	// Following from that; I wonder how one does context... State machine?
	glfwSetKeyCallback(window, key_callback);

	// Initialise GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}

	// Set up viewport
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	// Load Shaders into program
	GLuint ShaderProgram = BuildShaderProgram();

	// Load Shapes
	#ifdef _WIN32 || _WIN64
	LoadedVertexObjects MeshData(".//Shapes//");
	#elif __linux__
	LoadedVertexObjects MeshData("./Shapes/");
	#endif

	/** Local Variables **/
	GLclampf RedBit = 0.0f;

	// Program Loop
	while (!glfwWindowShouldClose(window))
	{
		// Check and call events
		glfwPollEvents();

		// Rendering Commands here
		glClearColor(RedBit, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		if (RedBit < 1.0f) {
			RedBit += 0.001f;
		}
		else {
			RedBit = 0.0f;
		}

		/*	TODO: Implement a 'view object' system. 
			On KeyPress (Say <ENTER>) call a new thread 
			This thread lists files loaded and askes which asks for input
			of which to load. On load the current object is replaced with
			the selected object and the thread dies
		*/
		glUseProgram(ShaderProgram);

		for (unsigned int i = 0; i < MeshData.GetShapes()->size(); ++i) {
			glBindVertexArray(MeshData.GetShapes()->at(i)->VAO);
			glDrawArrays(GL_TRIANGLES, 0, MeshData.GetShapes()->at(i)->vertices);
			glBindVertexArray(0);
		}

		// Swap Buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When user presses the escape key, we set the windowShouldClose property to true,
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
