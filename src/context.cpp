#include "context.h"
#include "image.h"

ContextUPtr Context::Create(GLFWwindow* window) {
	auto context = ContextUPtr(new Context());
	if (!context->Init())
		return nullptr;
	context->m_window = window;
	return std::move(context);
}

bool Context::Init() {

	float vertices[] = {
	  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

	  -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

	   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

	  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

	  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
	   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
	   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
	};

	uint32_t indices[] = {
	   0,  2,  1,  2,  0,  3,
	   4,  5,  6,  6,  7,  4,
	   8,  9, 10, 10, 11,  8,
	  12, 14, 13, 14, 12, 15,
	  16, 17, 18, 18, 19, 16,
	  20, 22, 21, 22, 20, 23,
	};

	m_vertexLayout = VertexLayout::Create();
	m_vertexBuffer = Buffer::CreateWithData(
		GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		vertices, sizeof(float) * 120);

	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);


	//인덱스버퍼 생성
	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 36);

	ShaderPtr vertShader = Shader::CreateFromFile("./shader/texture.vs", GL_VERTEX_SHADER);
	ShaderPtr fragShader = Shader::CreateFromFile("./shader/texture.fs", GL_FRAGMENT_SHADER);
	if (!vertShader || !fragShader)
		return false;
	SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
	SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
	m_program = Program::Create({ fragShader, vertShader });
	if (!m_program)
		return false;
	SPDLOG_INFO("program id: {}", m_program->Get());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	auto image = Image::Image::Load("./resources/container.jpg");
	m_texture = Texture::CreateFromImage(image.get());

	auto image2 = Image::Load("./resources/awesomeface.png");
	m_texture2 = Texture::CreateFromImage(image2.get());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->Get());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture2->Get());

	m_program->Use();
	m_program->SetUniform("tex", 0);
	m_program->SetUniform("tex2", 1);

	return true;
}

void Context::Render() {
	std::vector<glm::vec3> cubePositions = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f),
	};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//static float time = 0.0f;
	//float t = sinf(time) * 0.5f + 0.5f;
	//static GLint loc = glGetUniformLocation(m_program->Get(),"color");

	m_program->Use();
	//glUniform4f(loc, t*t, 2.0f * t * (1.0f - t),(1.0f - t) * (1.0f - t), 1.0f);


	// 종횡비 4:3, 세로화각 45도의 원근 투영
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	float aspectRatio = (float)width / (float)height;
	auto projection = glm::perspective(glm::radians(45.0f),
		aspectRatio, 0.01f, 20.0f);

	// 카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
	float x = sinf((float)glfwGetTime() * glm::pi<float>() * 2.0f) * 3.0f;
	auto cameraPos = glm::vec3(x, 0.0f, 3.0f);
	auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	auto cameraZ = glm::normalize(cameraPos - cameraTarget);
	auto cameraX = glm::normalize(glm::cross(cameraUp, cameraZ));
	auto cameraY = glm::cross(cameraZ, cameraX);

	auto cameraMat = glm::mat4(
		glm::vec4(cameraX, 0.0f),
		glm::vec4(cameraY, 0.0f),
		glm::vec4(cameraZ, 0.0f),
		glm::vec4(cameraPos, 1.0f));

	auto view = glm::inverse(cameraMat);

	for (size_t i = 0; i < cubePositions.size(); i++) {
		auto& pos = cubePositions[i];

		glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
		model = glm::rotate(model, glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i), glm::vec3(1.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		/*auto model = glm::translate(glm::mat4(1.0f), pos);
		model = glm::rotate(model,
			glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),
			glm::vec3(1.0f, 0.5f, 0.0f));*/
		auto transform = projection * view * model;
		m_program->SetUniform("transform", transform);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	//time += 0.016f;
}