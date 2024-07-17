#include "context.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
const char WINDOW_NAME[] = "Learn OpenGL - 01 : Create Window";

void OnFramebufferSizeChange(GLFWwindow* window, int width, int height) {
	SPDLOG_INFO("framebuffer size changed: ({} X {})", width, height);
	glViewport(0, 0, width, height);
}

void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {
	SPDLOG_INFO("key : {}, scancode : {}, action : {}, mods : {}{}{}",
		key,
		scancode,
		action == GLFW_PRESS ? "Pressed" :
		action == GLFW_RELEASE ? "Released" :
		action == GLFW_REPEAT ? "Repeat" : "Unknown",
		mods & GLFW_MOD_CONTROL ? "C" : "-",
		mods & GLFW_MOD_SHIFT ? "S" : "-",
		mods & GLFW_MOD_ALT ? "A" : "-");
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

#pragma region Joystick Method
void OnJoystickEvent(int jid, int event) {
	SPDLOG_INFO("jid: {}, event: {}",jid, event);
}

bool InitJosystick(const char* name, int* jid) {
	for(int i = 0; i < GLFW_JOYSTICK_LAST; i++) {
		if(glfwJoystickPresent(i)) {
			name = glfwGetJoystickName(i);
			SPDLOG_INFO("joystick1: {}",name);
			glfwSetJoystickCallback(OnJoystickEvent);
			*jid = i;
			return true;
		}
		else {
			SPDLOG_ERROR("조이스틱 정보를 가져오는데 실패했습니다.");
			return false;
		}
	}
}

void CheckJoystickState(int jid, GLFWgamepadstate* state) {
	if(!glfwGetGamepadState(jid, state)) {
		return;
	}
	
	for(int i = 0; i < 15; i++) {
		if(state->buttons[i] == GLFW_PRESS) {
			const char* buttonName;
			bool inValidName = false;

			switch(i) {
				case GLFW_GAMEPAD_BUTTON_A:
					buttonName = "Button A";
					break;
				case GLFW_GAMEPAD_BUTTON_B:
					buttonName = "Button B";
					break;
				case GLFW_GAMEPAD_BUTTON_X:
					buttonName = "Button X";
					break;
				case GLFW_GAMEPAD_BUTTON_Y:
					buttonName = "Button Y";
					break;
				default:
					SPDLOG_INFO("Pressed: {}", std::to_string(i).c_str());
					inValidName = true;
					break;
			}

			if(!inValidName)
				SPDLOG_INFO("Pressed: {}", buttonName);
		}
	}
}
#pragma endregion

int main() {
	SPDLOG_INFO("Start program");

	SPDLOG_INFO("Initialize glfw");
	if (!glfwInit()) {
		const char* description = nullptr;
		glfwGetError(&description);
		SPDLOG_ERROR("Failed to initialize glfw: {}", description);
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	SPDLOG_INFO("Create glfw window");
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, NULL, NULL);
	if (window == NULL) {
		SPDLOG_ERROR("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		SPDLOG_ERROR("Failed to initialize glad");
		glfwTerminate();
		return -1;
	}
	const GLubyte* glVersion = glGetString(GL_VERSION);
	SPDLOG_INFO("OpenGL context version: {}", (const char*)glVersion);

	auto context = Context::Create();
	if(!context){
		SPDLOG_ERROR("failed to create glContext");
		glfwTerminate();
		return -1;
	}

	OnFramebufferSizeChange(window, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, OnFramebufferSizeChange);
	glfwSetKeyCallback(window, OnKeyEvent);
	//const char* joystickName = nullptr;
	//int currentJid = -1;
	//bool isJoystickConnected = InitJosystick(joystickName, &currentJid);
	
	SPDLOG_INFO("Start Main Loop");
	//GLFWgamepadstate joystickState;
	while (!glfwWindowShouldClose(window)) {
		//if(isJoystickConnected)
			//CheckJoystickState(currentJid, &joystickState);
		
		context->Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	context.reset();

	glfwTerminate();
	return 0;
}
