/**
* \file glprogram.h
* \brief Declaration for the OpenGL program class.
*/

#ifndef HLMDLVIEWER_GLPROGRAM_H_
#define HLMDLVIEWER_GLPROGRAM_H_

#include <string>
#include "glshader.h"
#include <glm/gtc/type_ptr.hpp>
#include "glad.h"
#include "file_system.h"

namespace hl_mdlviewer {

/** A class that wraps an OpenGL program. */
class glprogram
{
public:
    glprogram();
    ~glprogram();

    using ShaderInitializerList = std::initializer_list<std::pair<std::string, GLenum>>;

    void initialize_with_files(ShaderInitializerList&& shader_initializer_list,
        FileSystem* file_system);
    void initialize_with_shaders(std::initializer_list<glshader>&& shaders);

    void add_shader(const glshader& shader);
    void add_shaders(std::initializer_list<glshader>&& shaders);

    void update();
    void delete_program();

    inline const GLuint id() const { return id_; }

    void bind_uniform_index(const char* name, GLuint index);

    inline void bind() {
        glUseProgram(id_);
    }

    inline void unbind() {
        glUseProgram(0);
    }

    inline GLint get_attrib_location(const char* name) {
        return glGetAttribLocation(id_, name);
    }

    inline GLint get_uniform_location(const char* name) {
        return glGetUniformLocation(id_, name);
    }

    inline void set_uniform(const char* name, const glm::vec2& value) {
        GLint location = glGetUniformLocation(id_, name);
        validate_uniform_location(location, name);
        glUniform2fv(location, 1,  glm::value_ptr(value));
    }

    inline void set_uniform(const char* name, const glm::vec3& value) {
        GLint location = glGetUniformLocation(id_, name);
        validate_uniform_location(location, name);
        glUniform3fv(location, 1,  glm::value_ptr(value));
    }

    inline void set_uniform(const char* name, const glm::vec4& value) {
        GLint location = glGetUniformLocation(id_, name);
        validate_uniform_location(location, name);
        glUniform4fv(location, 1,  glm::value_ptr(value));
    }

    inline void set_uniform(const char* name, const glm::mat4& value, bool transpose = false) {
        GLint location = glGetUniformLocation(id_, name);
        validate_uniform_location(location, name);
        glUniformMatrix4fv(location, 1, transpose ? GL_TRUE : GL_FALSE,  glm::value_ptr(value));
    }

    inline void set_uniform(const char* name, const bool value) {
        GLint location = glGetUniformLocation(id_, name);
        validate_uniform_location(location, name);
        glUniform1i(location, value);
    }

    inline void set_uniform(const char* name, const GLint value) {
        GLint location = glGetUniformLocation(id_, name);
        validate_uniform_location(location, name);
        glUniform1i(location, value);
    }

    inline void set_uniform(const char* name, const GLuint value) {
        GLint location = glGetUniformLocation(id_, name);
        validate_uniform_location(location, name);
        glUniform1ui(location, value);
    }

    inline void set_uniform(const char* name, const GLfloat value) {
        GLint location = glGetUniformLocation(id_, name);
        validate_uniform_location(location, name);
        glUniform1f(location, value);
    }

protected:

    void bind_attributes();
    void link();
    void validate_program();
    void throw_info_log_exception();

    inline void validate_uniform_location(GLint location, const char* name)
    {
        if (location == -1)
            throw std::runtime_error("Could not find uniform location " + std::string(name));
    }

    inline void validate_uniform_block_index(GLuint index, const char* name)
    {
        if (index == GL_INVALID_INDEX)
            throw std::runtime_error("Could not find uniform block index " + std::string(name));
    }

private:

    GLuint id_;
};

}

#endif // HLMDLVIEWER_GLPROGRAM_H_
