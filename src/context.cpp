#include "context.h"
#include "image.h"

ContextUPtr Context::Create() {
	auto context = ContextUPtr(new Context());
	if (!context->Init())
		return nullptr;
	return std::move(context);
}

bool Context::Init() {

	float vertices[] = {
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,	  // top right, red
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom right, green
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom left, blue
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  // top left, yellow
	};

	uint32_t indices[] = { // note that we start from 0!
		0, 3, 1, // first triangle
		3, 2, 1, // second triangle
	};

	//VAO 생성
	m_vertexLayout = VertexLayout::Create();

	//정점버퍼 생성
	m_vertexBuffer = Buffer::CreateWithData(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(float) * 24);

	//Attribute 설정
	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, sizeof(float) * 3);

	//인덱스버퍼 생성
	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 24);

	ShaderPtr vertShader = Shader::CreateFromFile("./shader/per_vertex_color.vs", GL_VERTEX_SHADER);
	ShaderPtr fragShader = Shader::CreateFromFile("./shader/per_vertex_color.fs", GL_FRAGMENT_SHADER);
	if (!vertShader || !fragShader)
		return false;
	SPDLOG_INFO("vertex shader id: {}", vertShader->Get());
	SPDLOG_INFO("fragment shader id: {}", fragShader->Get());
	m_program = Program::Create({ fragShader, vertShader });
	if (!m_program)
		return false;
	SPDLOG_INFO("program id: {}", m_program->Get());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	auto image = Image::Load("./resources/container.jpg");
	if (!image)
		return false;
	SPDLOG_INFO("image: {}x{}, {} channels",
		image->GetWidth(), image->GetHeight(), image->GetChannelCount());

	return true;
}

void Context::Render() {
	glClear(GL_COLOR_BUFFER_BIT);

	//static float time = 0.0f;
	//float t = sinf(time) * 0.5f + 0.5f;
	//static GLint loc = glGetUniformLocation(m_program->Get(),"color");

	m_program->Use();
	//glUniform4f(loc, t*t, 2.0f * t * (1.0f - t),(1.0f - t) * (1.0f - t), 1.0f);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//time += 0.016f;
}