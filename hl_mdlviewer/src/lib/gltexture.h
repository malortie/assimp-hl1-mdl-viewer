/**
* \file gltexture.h
* \brief Declaration for the OpenGL texture class.
*/

#ifndef HLMDLVIEWER_GLTEXTURE_H_
#define HLMDLVIEWER_GLTEXTURE_H_

#include <string>
#include "glad.h"

namespace hl_mdlviewer {

/** A class that wraps an OpenGL texture. */
class gltexture
{
public:
    gltexture();
    ~gltexture();

    void create_from_data(
        unsigned int width, unsigned int height,
        GLint internalFormat, GLint format,
        unsigned char* pixels);

    void delete_texture();

    inline const GLuint id() const { return id_; }

    inline void bind() {
        glBindTexture(GL_TEXTURE_2D, id_);
    }
    inline void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

private:
    GLuint id_;
};

}

#endif // HLMDLVIEWER_GLTEXTURE_H_
