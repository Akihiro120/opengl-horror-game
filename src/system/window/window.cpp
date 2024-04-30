#include "window.h"
#include <stdexcept>

#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

Window::Window(int width, int height)
{
	m_WIDTH = width;
	m_HEIGHT = height;

	initWindow(width, height);
	openglSetup();
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	app->setWidth(width);
	app->setHeight(height);
	app->rebuildWindow = true;
}

void Window::initWindow(int width, int height)
{
	// start glfw
	glfwInit();

	// prepare window hint
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create the window object
	m_Window = glfwCreateWindow(width, height, "Cardifice", nullptr, nullptr);
	if (!m_Window) throw std::runtime_error("Failed to Create window Object");

	// frame buffer
	glfwSetWindowUserPointer(m_Window, this);
	glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallback);

	// set as current context
	glfwMakeContextCurrent(m_Window);

	// vsync 
	glfwSwapInterval(0);

	// set mouse mode
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// load imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init("#version 460 core");
}

void Window::openglSetup()
{
	// load
	gladLoadGL();

	// enables
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glEnable(GL_CLIP_DISTANCE0);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
}

void Window::clearScreen(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::updateWindow()
{
	rebuildWindow = false;
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void Window::cleanUp()
{
	// destroy window and terminate
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

bool Window::shouldWindowClose()
{
	return glfwWindowShouldClose(m_Window);
}

int Window::getWidth()
{
	return m_WIDTH;
}

int Window::getHeight()
{
	return m_HEIGHT;
}

GLFWwindow* Window::getWindow()
{
	return m_Window;
}

void Window::setWidth(int width)
{
	this->m_WIDTH = width;
}

void Window::setHeight(int height)
{
	this->m_HEIGHT = height;
}