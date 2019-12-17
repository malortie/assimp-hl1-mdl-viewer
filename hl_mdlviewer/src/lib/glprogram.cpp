/**
* \file glprogram.cpp
* \brief Implementation for the OpenGL program class.
*/

#include "pch.h"
#include "glprogram.h"
#include "shader_parser.h"
#include <initializer_list>

namespace hl_mdlviewer
{

glprogram::glprogram() : id_(0)
{
}

glprogram::~glprogram()
{
}

void glprogram::initialize_with_files(
    ShaderInitializerList&& shader_initializer_list,
    FileSystem* file_system)
{
    glshader temp_shader;
    ShaderParser parser(file_system);

    id_ = glCreateProgram();
    for (auto file_path_shader_type : std::move(shader_initializer_list))
    {
        std::string parsed_shader_string;
        parser.parse(file_path_shader_type.first, parsed_shader_string);
        temp_shader.create_from_string(parsed_shader_string, file_path_shader_type.second);
        glAttachShader(id_, temp_shader.id());
    }

    update();
}

void glprogram::initialize_with_shaders(std::initializer_list<glshader>&& shaders)
{
    id_ = glCreateProgram();
    add_shaders(std::move(shaders));
    update();
}

void glprogram::add_shader(const glshader& shader)
{
    glAttachShader(id_, shader.id());
}

void glprogram::add_shaders(std::initializer_list<glshader>&& shaders)
{
    for (auto shader : std::move(shaders))
        glAttachShader(id_, shader.id());
}

void glprogram::update()
{
    bind_attributes();
    link();
    validate_program();
}

void glprogram::delete_program()
{
    glDeleteProgram(id_);
    id_ = 0;
}

void glprogram::bind_attributes()
{
    glBindAttribLocation(id_, 0, "position");
    glBindAttribLocation(id_, 1, "normal");
    glBindAttribLocation(id_, 2, "uv");
    glBindAttribLocation(id_, 3, "boneid");
}

void glprogram::bind_uniform_index(const char* name, GLuint index)
{
    GLuint uniform_block_index = glGetUniformBlockIndex(id_, name);
    validate_uniform_block_index(uniform_block_index, name);
    glUniformBlockBinding(id_, uniform_block_index, index);
}

void glprogram::link()
{
    glLinkProgram(id_);

    GLint status;
    glGetProgramiv(id_, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
        throw_info_log_exception();
}

void glprogram::validate_program()
{
    glValidateProgram(id_);

    GLint status;
    glGetProgramiv(id_, GL_VALIDATE_STATUS, &status);

    if (status == GL_FALSE)
        throw_info_log_exception();
}

void glprogram::throw_info_log_exception()
{
    GLint log_length;
    glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &log_length);

    std::string error_message;
    error_message.resize(log_length);
    GLsizei num_read_characters;
    glGetProgramInfoLog(id_, log_length, &num_read_characters, &error_message[0]);
}

}
