#define _USE_MATH_DEFINES
#define MAX_SIZE 100
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
#include "TerrainStorage.h"
/**
 * Fields
 */

glm::vec4 clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.00f);
float colorBarTime = 3;
bool cursorMode = true;
int windowWidth = 1280, windowHeight = 720;

float lastFrame = 0.0f;
float lastMouseX, lastMouseY;
float firstMouse = true;
Scene* s;
float lastScrollTime = glfwGetTime();

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
void mouseCallback(GLFWwindow* window, double xpos, double ypos);


/**
 * Function implementation
 */
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
	//s->getActiveCamera().setFov(s->getActiveCamera().getFov() - (float)yoffset);
	s->getActivePlayer().setActiveColor(s->getActivePlayer().getActiveColor() + (int)yoffset);
	lastScrollTime = glfwGetTime();

}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse) {
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}
	if (cursorMode) {
		float xOffset = xpos - lastMouseX;
		float yOffset = lastMouseY - ypos;

		lastMouseX = xpos;
		lastMouseY = ypos;

		float senstivity = 0.1f;
		s->getActiveCamera().setYaw(s->getActiveCamera().getYaw() + xOffset * senstivity);
		s->getActiveCamera().setPitch(s->getActiveCamera().getPitch() + yOffset * senstivity);
	}

}


int main(int argc, char **argv)
{
	GLFWwindow* window = SetupGlfwWindow(windowWidth, windowHeight, "HexaPlanet");
	if (!window)
		return 1;

	int frameBufferWidth, frameBufferHeight;
	glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	Renderer renderer = Renderer(frameBufferWidth, frameBufferHeight);
	Scene scene = Scene();
	s = &scene;
	
	ImGuiIO& io = SetupDearImgui(window);
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetCursorPosCallback(window, mouseCallback);


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

	Camera& activeCamera = scene.getActiveCamera();
	Player& activePlayer = scene.getActivePlayer();

	if ((float)frameBufferHeight / frameBufferWidth != activeCamera.getAspectRatio()) {
		activeCamera.setAspectRatio((float)frameBufferHeight / frameBufferWidth);
	}
	//calculate delta time
	float currentFrame = glfwGetTime();
	float deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

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
			activePlayer.move(Direction::FORWARD, deltaTime);
		}
		if (io.KeysDown[GLFW_KEY_S])
		{
			activePlayer.move(Direction::BACKWARD, deltaTime);

		}
		if (io.KeysDown[GLFW_KEY_A])
		{
			activePlayer.move(Direction::LEFT, deltaTime);
			
		}
		if(io.KeysDown[GLFW_KEY_D])
		{
			activePlayer.move(Direction::RIGHT, deltaTime);

		}
		if (io.KeysDown[GLFW_KEY_SPACE]) {
			activePlayer.move(Direction::JUMP, deltaTime);
		}
		if (io.KeysDown[GLFW_KEY_ESCAPE])
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cursorMode = false;
		}
	}

	if (!io.WantCaptureMouse)
	{
		// TODO: Handle mouse events here
		if (io.MouseClicked[0] || io.MouseClicked[1]) {
			PixelInfo p = renderer.getPixelInfo(renderer.GetViewportWidth() / 2, renderer.GetViewportHeight() / 2);
			bool res; int neighbor;
			Voxel v = extractPixelInfo(p, &res, &neighbor);
			std::cout << "voxel: " << v.grid << " " << v.x << " " << v.y << " " << v.z << std::endl;
			std::cout << "neighbor: " << neighbor << std::endl << std::endl;

			if (io.MouseClicked[1])
			{
				// right mouse button is down
				Voxel v_n = scene.getActivePlanet().getNeighbor(v, neighbor);
				if (scene.getActivePlanet().isValidVoxel(v_n)) {
					scene.getActivePlanet().placeVoxel(v_n, (BlockType)(activePlayer.getActiveColor() + 1));
					std::vector <glm::vec3> tmp;
					if (scene.getActivePlayer().isCollidingWithVoxel(v_n, tmp)) {
						scene.getActivePlanet().placeVoxel(v_n, BlockType::None);

					}
				}

			}
			else {
				// Left mouse button is down
				if (!cursorMode) {
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					cursorMode = true;
					firstMouse = true;
				}
				scene.getActivePlanet().placeVoxel(v, BlockType::None);
			}

		}

	}
	// activePlayer.UpdateCameraUp();
	activePlayer.UpdatePos(deltaTime);

	if (activePlayer.isThirdPerson()) {
		scene.getGUI()->updateActive("crossHair", false);
	}
	else {
		scene.getGUI()->updateActive("crossHair", true);
	}
	if (lastFrame - lastScrollTime> colorBarTime) {
		scene.getGUI()->updateActive("selector", false);
		scene.getGUI()->updateActive("colorBar", false);
	}
	else {
		scene.getGUI()->updateActive("selector", true);
		scene.getGUI()->updateActive("colorBar", true);

	}
	scene.getGUI()->updateTranslation("selector", glm::vec2((float)(scene.getActivePlayer().getActiveColor() - 8) / 8.0f * 0.597f +0.03f, -0.80));

	renderer.Render(scene);

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
	float voxelQ, voxelR;
	Voxel voxel = s->getActivePlanet().getVoxel(c.GetTranslation(), &voxelQ, &voxelR);
	ImGui::Text("Root: %d, x:%d, y:%d, z:%d", voxel.grid, voxel.x, voxel.y, voxel.z);
	ImGui::Text("q:%f, r:%f", voxelQ, voxelR);

	float pitch = c.getPitch();
	float yaw = c.getYaw();
	ImGui::Text("Pitch: %f, Yaw: %f", pitch, yaw);
	glm::vec3 up = c.getCameraUp();
	ImGui::Text("Up: %f, %f, %f", up.x, up.y, up.z);
	glm::vec3 front = c.getCameraFront();
	ImGui::Text("Front: %f, %f, %f", front.x, front.y, front.z);

	glm::vec3 pos = c.GetTranslation();
	ImGui::Text("Position: %f, %f, %f", pos.x, pos.y, pos.z);

	glm::vec3 pole = c.getPole();
	ImGui::Text("Pole: %f, %f, %f", pole.x, pole.y, pole.z);


	bool ortho = c.IsOrthographic();
	ImGui::Checkbox("Orthographic", &ortho);
	c.SetOrthograpic(ortho);

	view_volume v = c.GetViewVolume();
	float l = v.l, r = v.r, b = v.b, t = v.t, n = v.n, f = v.f;

	if (ImGui::SliderFloat("left", &l, -1, 1)) 		c.SetViewVolume(l, r, b, t, n, f);
	if (ImGui::SliderFloat("right", &r, -1, 1))		c.SetViewVolume(l, r, b, t, n, f);
	if (ImGui::SliderFloat("bottom", &b, -1, 1))	c.SetViewVolume(l, r, b, t, n, f);
	if (ImGui::SliderFloat("top", &t, -1, 1))		c.SetViewVolume(l, r, b, t, n, f);
	if (ImGui::SliderFloat("near", &n, -1, 1))		c.SetViewVolume(l, r, b, t, n, f);
	if (ImGui::SliderFloat("far", &f, 0, 100))		c.SetViewVolume(l, r, b, t, n, f);
	ImGui::End();
}

void drawPlanetMenu(Planet& p) {
	ImGui::Begin("planet");
	static int activeLoad = -1;
	static char s[10];

	std::vector<std::string> t = TerrainStorage::getSavedTerrainNames();
	int numOfTerrains = t.size();
	const char* terrains[MAX_SIZE];
	for (int i = 0; i < numOfTerrains; i++) {
		terrains[i] = t[i].c_str();
	}

	ImGui::ListBox("Terrain", &activeLoad, terrains, numOfTerrains);
	if (ImGui::Button("Load")) {
		if (activeLoad >= 0) {
			TerrainStorage::loadTerrain(&p, t[activeLoad]);
		}
	}
	ImGui::SameLine(50);
	if (ImGui::Button("Delete")) {
		if (activeLoad >= 0) {
			TerrainStorage::deleteTerrain(t[activeLoad]);
		}
	}
	if (ImGui::Button("Save")) {
		TerrainStorage::saveTerrain(p, s);
	}
	ImGui::SameLine(50);
	ImGui::InputText("Terrain Name", s, 10);


	glm::vec3 center = p.getCenter();
	if (ImGui::SliderFloat3("Center", (float*)&center, -100, 100)) p.setCenter(center);
	float baseHeight = p.getBaseHeight();
	if (ImGui::SliderFloat("Base Height", (float*)&baseHeight, 0, 100)) p.setBaseHeight(baseHeight);
	float voxelHeight = p.getVoxelHeight();
	if (ImGui::SliderFloat("Voxel Height", (float*)&voxelHeight, 0, 10)) p.setVoxelHeight(voxelHeight);

	float gravity = p.getGravity();
	if (ImGui::SliderFloat("Gravity", (float*)&gravity, 0, 30)) p.setGravity(gravity);
	float airFriction = p.getAirFriction();
	if (ImGui::SliderFloat("Air Friction", (float*)&airFriction, 0, 30)) p.setAirFriction(airFriction);
	float groundFriction = p.getGroundFriction();
	if (ImGui::SliderFloat("Ground Friction", (float*)&groundFriction, 0, 30)) p.setGroundFriction(groundFriction);


	static int z = 0;
	ImGui::SliderInt("Z", &z, 0, p.getGrid(0)->getZ() - 1);
	if (ImGui::Button("Fill Z")) {
		p.fillSky(z);
	}

	int count = 0;
	for (bool* i = p.activeChunks; count < 20; i++) {
		ImGui::Checkbox(std::to_string(count).c_str(), i);
		count++;
	}

	ImGui::End();

}

void drawPlayerMenu(Player& player) {
	ImGui::Begin("player");

	glm::vec3 velocity = player.getVelocity();
	ImGui::Text("Velocity: %f, %f, %f", velocity.x, velocity.y, velocity.z);

	bool onGround = player.isOnGround();
	if (onGround) {
		ImGui::Text("On ground");
	}
	else {
		ImGui::Text("Not on ground");

	}

	float jumpForce = player.getJumpForce();
	if (ImGui::SliderFloat("Jump Force", (float*)&jumpForce, 0, 500)) player.setJumpForce(jumpForce);
	float mass = player.getMass();
	if (ImGui::SliderFloat("Mass", (float*)&mass, 0.01, 10)) player.setMass(mass);

	float speed = player.getSpeed();
	if (ImGui::SliderFloat("Speed", (float*)&speed, 0, 1000)) player.setSpeed(speed);
	bool jetPack = player.hasJetpack();
	if (ImGui::Checkbox("Jetpack", &jetPack)) player.setJetpack(jetPack);
	bool thirdPerson = player.isThirdPerson();
	if (ImGui::Checkbox("Third Person", &thirdPerson)) player.setThirdPerson(thirdPerson);
	bool flying = player.getMode() == PlayerMode::FLYING;
	if (ImGui::Checkbox("Creative Mode", &flying)) player.setMode(flying ? PlayerMode::FLYING : PlayerMode::WALKING);


	ImGui::End();
}

struct Window {
	int index;
	const char* name;
	const char* hotkey;
	bool isActive = false;
};

Window windows[] = {
		{0, "Info", "CTRL+I", true},
		{1, "Camera", "CTRL+C", false},
		{2, "Planet", "", false},
		{3, "Player", "", false}
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
			case 2:
				drawPlanetMenu(scene.getActivePlanet());
				break;
			case 3:
				drawPlayerMenu(scene.getActivePlayer());
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