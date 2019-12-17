/**
* \file gluniformbuffer.h
* \brief Declaration for the OpenGL uniform buffer class.
*/

#ifndef HLMDLVIEWER_GLUNIFORMBUFFER_H_
#define HLMDLVIEWER_GLUNIFORMBUFFER_H_

#include "glad.h"
#include <type_traits>

namespace hl_mdlviewer {

/** A class that wraps an OpenGL uniform buffer. */
class gluniformbuffer
{
public:
    gluniformbuffer();
    ~gluniformbuffer();

    inline const GLuint id() const { return id_; }

    void initialize(
        GLuint block_index,
        size_t size_in_bytes, 
        GLenum usage = GL_STATIC_DRAW);

    inline void bind() {
        glBindBuffer(GL_UNIFORM_BUFFER, id_);
    }

    inline void unbind() {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    inline void set_data_unbinded(GLintptr offset, bool value) {
        internal_set_data_unbinded<
            bool,
            sizeof(bool)>
            (offset, &value);
    }

    inline void set_data_unbinded(GLintptr offset, float value) {
        internal_set_data_unbinded<
            float,
            sizeof(float)>
            (offset, &value);
    }

    inline void set_data_unbinded(GLintptr offset, const glm::vec2& value) {
        internal_set_data_unbinded<
            typename glm::vec2::value_type,
            sizeof(glm::vec2)>
            (offset, &value.x);
    }

    inline void set_data_unbinded(GLintptr offset, const glm::vec3& value) {
        internal_set_data_unbinded<
            typename glm::vec3::value_type,
            sizeof(glm::vec3)>
            (offset, glm::value_ptr(value));
    }

    inline void set_data_unbinded(GLintptr offset, const glm::vec4& value) {
        internal_set_data_unbinded<
            typename glm::vec4::value_type,
            sizeof(glm::vec4)>
            (offset, glm::value_ptr(value));
    }

    inline void set_data_unbinded(GLintptr offset, const glm::mat2& value) {
        internal_set_data_unbinded<
            typename glm::mat2::value_type,
            sizeof(glm::mat2)>
            (offset, glm::value_ptr(value));
    }

    inline void set_data_unbinded(GLintptr offset, const glm::mat3& value) {
        internal_set_data_unbinded<
            typename glm::mat3::value_type,
            sizeof(glm::mat3)>
            (offset, glm::value_ptr(value));
    }

    inline void set_data_unbinded(GLintptr offset, const glm::mat4& value) {
        internal_set_data_unbinded<
            typename glm::mat4::value_type,
            sizeof(glm::mat4)>
            (offset, glm::value_ptr(value));
    }

    inline void set_data(GLintptr offset, bool value) {
        internal_set_data<
            bool,
            sizeof(bool)>
            (offset, &value);
    }

    inline void set_data(GLintptr offset, float value) {
        internal_set_data<
            float,
            sizeof(float)>
            (offset, &value);
    }

    inline void set_data(GLintptr offset, const glm::vec2& value) {
        internal_set_data<
            typename glm::vec2::value_type,
            sizeof(glm::vec2)>
            (offset, glm::value_ptr(value));
    }

    inline void set_data(GLintptr offset, const glm::vec3& value) {
        internal_set_data<
            typename glm::vec3::value_type,
            sizeof(glm::vec3)>
            (offset, glm::value_ptr(value));
    }

    inline void set_data(GLintptr offset, const glm::vec4& value) {
        internal_set_data<
            typename glm::vec4::value_type,
            sizeof(glm::vec4)>
            (offset,  glm::value_ptr(value));
    }

    inline void set_data(GLintptr offset, const glm::mat2& value) {
        internal_set_data<
            typename glm::mat2::value_type,
            sizeof(glm::mat2)>
            (offset,  glm::value_ptr(value));
    }

    inline void set_data(GLintptr offset, const glm::mat3& value) {
        internal_set_data<
            typename glm::mat3::value_type,
            sizeof(glm::mat3)>
            (offset,  glm::value_ptr(value));
    }

    inline void set_data(GLintptr offset, const glm::mat4& value) {
        internal_set_data<
            typename glm::mat4::value_type,
            sizeof(glm::mat4)>
            (offset,  glm::value_ptr(value));
    }

    inline void set_data_with_size(GLintptr offset, const void* data, size_t size_in_bytes) {
        glBindBuffer(GL_UNIFORM_BUFFER, id_);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size_in_bytes, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    inline void set_data_unbinded_with_size(GLintptr offset, const void* data, size_t size_in_bytes) {
        glBufferSubData(GL_UNIFORM_BUFFER, offset, size_in_bytes, data);
    }


protected:

    template<typename T, size_t SizeInBytes>
    inline void internal_set_data_unbinded(GLintptr offset, const T* data) {
        glBufferSubData(GL_UNIFORM_BUFFER, offset, SizeInBytes, data);
    }

    template<typename T, size_t SizeInBytes>
    inline void internal_set_data(GLintptr offset, const T* data) {
        glBindBuffer(GL_UNIFORM_BUFFER, id_);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, SizeInBytes, data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

private:

    GLuint id_;
};

}

#endif // HLMDLVIEWER_GLUNIFORMBUFFER_H_
