/**
* \file glshader.h
* \brief Declaration for the OpenGL shader class.
*/

#ifndef HLMDLVIEWER_GLSHADER_H_
#define HLMDLVIEWER_GLSHADER_H_

#include <string>
#include "glad.h"

namespace hl_mdlviewer {

/** A class that wraps an OpenGL shader. */
class glshader
{
public:
    glshader();
    ~glshader();

    void create_from_string(const std::string& str, GLenum type);
    void load(const std::string& file_path, GLenum type);

    void delete_shader();

    inline const GLuint id() const { return id_; }

private:
    GLuint id_;
};

}

#endif // HLMDLVIEWER_GLSHADER_H_
