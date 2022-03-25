#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui/imgui.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <nfd.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Scene.h"
#include "Utils.h"
#include <iostream>

/**
 * Fields
 */

glm::vec4 clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.00f);
int windowWidth = 1280, windowHeight = 720;


/**
 * Function declarations
 */
static void GlfwErrorCallback(int error, const char* description);
GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name);
ImGuiIO& SetupDearImgui(GLFWwindow* window);
void StartFrame();
void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io);
void Cleanup(GLFWwindow* window);
void DrawImguiMenus(ImGuiIO& io, Scene& scene);

/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	// TODO: Handle mouse scroll here
}

int main(int argc, char **argv)
{
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "HexaPlanet");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);

	glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);
	glEnable(GL_DEPTH_TEST);
	// glfwSwapInterval(0);


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
		StartFrame();
		DrawImguiMenus(io, scene);

		// Clear the screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		RenderFrame(window, scene, renderer, io);

    }

	Cleanup(window);
    return 0;
}


static void GlfwErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

GLFWwindow* SetupGlfwWindow(int w, int h, const char* window_name)
{
	glfwSetErrorCallback(GlfwErrorCallback);
	if (!glfwInit())
		return NULL;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	#if __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	
	GLFWwindow* window = glfwCreateWindow(w, h, window_name, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
						 // very importent!! initialization of glad
						 // https://stackoverflow.com/questions/48582444/imgui-with-the-glad-opengl-loader-throws-segmentation-fault-core-dumped

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	return window;
}

ImGuiIO& SetupDearImgui(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	ImGui::StyleColorsDark();
	return io;
}

void StartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}


void RenderFrame(GLFWwindow* window, Scene& scene, Renderer& renderer, ImGuiIO& io)
{
	ImGui::Render();
	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	
	if (frameBufferWidth != renderer.GetViewportWidth() || frameBufferHeight != renderer.GetViewportHeight())
	{
		renderer.SetViewport(frameBufferWidth, frameBufferHeight);
		windowWidth = frameBufferWidth;
		windowHeight = frameBufferHeight;
		glViewport(0, 0, windowWidth, windowHeight);
	}

	if (!io.WantCaptureKeyboard)
	{
		// TODO: Handle keyboard events here
		if (io.KeysDown[GLFW_KEY_UP])
		{

		}
		if (io.KeysDown[GLFW_KEY_DOWN])
		{

		}
		if (io.KeysDown[GLFW_KEY_RIGHT])
		{

		}
		if (io.KeysDown[GLFW_KEY_LEFT])
		{

		}
		if (io.KeysDown[GLFW_KEY_W])
		{

		}
		if (io.KeysDown[GLFW_KEY_S])
		{

		}
		if (io.KeysDown[GLFW_KEY_A])
		{

		}
		if(io.KeysDown[GLFW_KEY_D])
		{

		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseDown[0])
		{
			// Left mouse button is down
		}
	}

	// renderer.ClearColorBuffer(clear_color);
	// renderer.ClearZBuffer(1);
	renderer.Render(scene);
	// renderer.SwapBuffers();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwMakeContextCurrent(window);
	glfwSwapBuffers(window);
}

void Cleanup(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void DrawInfoMenu() {
	ImGui::Begin("Fps");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void DrawCameraMenu(Camera& c) {
	bool hasChanged = false;
	ImGui::Begin("camera");
	bool ortho = c.IsOrthographic();
	ImGui::Checkbox("Orthographic", &ortho);
	c.SetOrthograpic(ortho);

	view_volume v = c.GetViewVolume();
	float l = v.l, r = v.r, b = v.b, t = v.t, n = v.n, f = v.f;
	if (ImGui::SliderFloat3("translation", (float*)&c.translation, -5, 5)) c.UpdateViewTrans();
	if (ImGui::SliderFloat3("rotation", (float*)&c.rotation, 0.0f, 6.28f)) c.UpdateViewTrans();

	if (ImGui::SliderFloat("left", &l, -1, 1)) 		c.SetViewVolume(l, r, b, t, n, f);
	if (ImGui::SliderFloat("right", &r, -1, 1))		c.SetViewVolume(l, r, b, t, n, f);
	if (ImGui::SliderFloat("bottom", &b, -1, 1))	c.SetViewVolume(l, r, b, t, n, f);
	if (ImGui::SliderFloat("top", &t, -1, 1))		c.SetViewVolume(l, r, b, t, n, f);
	if (ImGui::SliderFloat("near", &n, -1, 1))		c.SetViewVolume(l, r, b, t, n, f);
	if (ImGui::SliderFloat("far", &f, 0, 100))		c.SetViewVolume(l, r, b, t, n, f);
	ImGui::End();
}

struct Window {
	int index;
	const char* name;
	const char* hotkey;
	bool isActive = false;
};

Window windows[] = {
		{0, "Info", "CTRL+I", false},
		{1, "Camera", "CTRL+C", false}
};

void DrawWindowDropdown(ImGuiIO& io)
{
	for (Window& w : windows) {
		if (ImGui::MenuItem(w.name, w.hotkey)) {
			w.isActive = !w.isActive;
		}
	}
}

void DrawWindows(ImGuiIO& io, Scene& scene)
{
	for (const Window& w : windows) {
		if (w.isActive) {
			switch (w.index)
			{
			case 0:
				DrawInfoMenu();
				break;
			case 1:
				DrawCameraMenu(scene.getActiveCamera());
				break;
			default:
				break;
			}
		}

	}
}

void DrawImguiMenus(ImGuiIO& io, Scene& scene)
{

	// Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Window")) {
			DrawWindowDropdown(io);
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	DrawWindows(io, scene);
}