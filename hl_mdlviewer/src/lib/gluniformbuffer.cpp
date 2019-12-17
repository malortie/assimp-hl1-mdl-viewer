/**
* \file gluniformbuffer.cpp
* \brief Implementation for the OpenGL uniform buffer class.
*/

#include "pch.h"
#include "gluniformbuffer.h"

namespace hl_mdlviewer
{

gluniformbuffer::gluniformbuffer() :
    id_(0)
{
}

gluniformbuffer::~gluniformbuffer()
{
}

void gluniformbuffer::initialize(
    GLuint block_index,
    size_t size_in_bytes,
    GLenum usage)
{
    glGenBuffers(1, &id_);
    glBindBuffer(GL_UNIFORM_BUFFER, id_);
    glBufferData(GL_UNIFORM_BUFFER, size_in_bytes, NULL, usage);
    glBindBufferRange(GL_UNIFORM_BUFFER, block_index, id_, 0, size_in_bytes);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

}
