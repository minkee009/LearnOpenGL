#include "context.h"

ContextUPtr Context::Create() {
	auto context = ContextUPtr(new Context());
	if (!context->Init())
		return nullptr;
	return std::move(context);
}

bool Context::Init() {

	float vertices[] = {
		 0.5f,  0.5f, 0.0f, // top right
		 0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, // top left
	};

	uint32_t indices[] = { // note that we start from 0!
		0, 1, 3, // first triangle
		1, 2, 3, // second triangle
	};

	//VAO 생성
	m_vertexArrayObject = VertexLayout::Create();

	//정점버퍼 생성
	m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 12);

	//Attribute 설정
	m_vertexArrayObject->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);

	//인덱스버퍼 생성
	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 6);

	ShaderPtr vertShader = Shader::CreateFromFile("./shader/simple.vs", GL_VERTEX_SHADER);
	ShaderPtr fragShader = Shader::CreateFromFile("./shader/simple.fs", GL_FRAGMENT_SHADER);
	if (!vertShader || !fragShader)
		return false;
	SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
	SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
	m_program = Program::Create({ fragShader, vertShader });
	if (!m_program)
		return false;
	SPDLOG_INFO("program id: {}", m_program->Get());

	auto loc = glGetUniformLocation(m_program->Get(), "color");
	m_program->Use();
	glUniform4f(loc, 1.0f, 1.0f, 0.0f, 1.0f);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	return true;
}

void Context::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	static float time = 0.0f;
	float t = sinf(time) * 0.5f + 0.5f;
	static GLint loc = glGetUniformLocation(m_program->Get(),"color");

	m_program->Use();
	glUniform4f(loc, t*t, 2.0f * t * (1.0f - t),(1.0f - t) * (1.0f - t), 1.0f);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	time += 0.016f;
}