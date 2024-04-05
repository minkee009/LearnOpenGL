#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string& filename,GLenum shaderType)
{
    auto shader = ShaderUPtr(new Shader());
    if (!shader->LoadFile(filename, shaderType))
        return nullptr;
    return std::move(shader);
}