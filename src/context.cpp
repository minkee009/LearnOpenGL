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
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	  // 1
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 2
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 3
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // 4
	};

	uint32_t indices[] = { // note that we start from 0!
		0, 3, 1, // first triangle
		3, 2, 1, // second triangle
	};

	m_vertexLayout = VertexLayout::Create();
	m_vertexBuffer = Buffer::CreateWithData(
		GL_ARRAY_BUFFER, GL_STATIC_DRAW,
		vertices, sizeof(float) * 32);

	m_vertexLayout->SetAttrib(0, 3, GL_FLOAT, GL_FALSE,
							  sizeof(float) * 8, 0);
	m_vertexLayout->SetAttrib(1, 3, GL_FLOAT, GL_FALSE,
							  sizeof(float) * 8, sizeof(float) * 3);
	m_vertexLayout->SetAttrib(2, 2, GL_FLOAT, GL_FALSE,
							  sizeof(float) * 8, sizeof(float) * 6);

	//인덱스버퍼 생성
	m_indexBuffer = Buffer::CreateWithData(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(uint32_t) * 24);

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
	glUniform1i(glGetUniformLocation(m_program->Get(), "tex"), 0);
	glUniform1i(glGetUniformLocation(m_program->Get(), "tex2"), 1);

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