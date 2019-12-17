/**
* \file gltexture.cpp
* \brief Implementation for the OpenGL texture class.
*/

#include "pch.h"
#include "gltexture.h"

namespace hl_mdlviewer
{

gltexture::gltexture() : id_(0)
{
}

gltexture::~gltexture()
{
}

void gltexture::create_from_data(
    unsigned int width, unsigned int height,
    GLint internalFormat, GLint format,
    unsigned char* pixels)
{
    glGenTextures(1, &id_);
    glBindTexture(GL_TEXTURE_2D, id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(
        GL_TEXTURE_2D, 0,
        internalFormat,
        width, height,
        0, format,
        GL_UNSIGNED_BYTE,
        pixels);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void gltexture::delete_texture()
{
    glDeleteTextures(1, &id_);
    id_ = 0;
}

}
