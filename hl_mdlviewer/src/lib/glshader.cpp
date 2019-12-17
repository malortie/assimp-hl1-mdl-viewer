/**
* \file glshader.cpp
* \brief Implementation for the OpenGL shader class.
*/

#include "pch.h"
#include "glshader.h"

#include <fstream>
#include <stdexcept>

namespace hl_mdlviewer
{

glshader::glshader() : id_(0)
{
}
glshader::~glshader()
{
}

void glshader::create_from_string(const std::string& str, GLenum type)
{
    id_ = glCreateShader(type);

    const GLchar* const shader_source = str.c_str();
    glShaderSource(id_, 1, &shader_source, nullptr);

    glCompileShader(id_);

    GLint status;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint log_length;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_length);

        std::string error_message;
        error_message.resize(log_length);
        GLsizei num_read_characters;
        glGetShaderInfoLog(id_, log_length, &num_read_characters, &error_message[0]);
        throw std::runtime_error(error_message);
    }
}

void glshader::load(const std::string& file_path, GLenum type)
{
    std::ifstream ifs(file_path, std::ifstream::in);

    std::string s;
    while (ifs.good())
    {
        std::string temp;
        std::getline(ifs, temp);
        s += temp + '\n';
    }

    create_from_string(s, type);
}

void glshader::delete_shader()
{
    glDeleteShader(id_);
    id_ = 0;
}

}
