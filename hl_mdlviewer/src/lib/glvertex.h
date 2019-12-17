/**
* \file glvertex.h
*/

#ifndef HLMDLVIEWER_GLVERTEX_H_
#define HLMDLVIEWER_GLVERTEX_H_

#include <glm/glm.hpp>

namespace hl_mdlviewer {

/** \brief Represents a single vertex. */
struct glvertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
    int boneid;
};

const unsigned int PRIMITIVE_RESTART_INDEX = UINT_MAX;

}

#endif // HLMDLVIEWER_GLVERTEX_H_
