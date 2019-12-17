/**
* \file bbox_builder.cpp
* \brief Implementation for the bounding box builder class.
*/

#include "pch.h"
#include "bbox_builder.h"

namespace hl_mdlviewer {

BBoxBuilder::BBoxBuilder()
{
}

void BBoxBuilder::build_vertices(const glm::vec3& bbmin,
    const glm::vec3& bbmax,
    int bone_index)
{
    const size_t NUM_VERTICES_PER_BBOX = 8;
    vertices_.resize(NUM_VERTICES_PER_BBOX);

    for (auto it = vertices_.begin(); it != vertices_.end(); ++it)
        (*it).boneid = bone_index;

    vertices_[0].position = glm::vec3(bbmin.x, bbmin.y, bbmin.z);
    vertices_[1].position = glm::vec3(bbmin.x, bbmin.y, bbmax.z);
    vertices_[2].position = glm::vec3(bbmax.x, bbmin.y, bbmax.z);
    vertices_[3].position = glm::vec3(bbmax.x, bbmin.y, bbmin.z);
    vertices_[4].position = glm::vec3(bbmin.x, bbmax.y, bbmin.z);
    vertices_[5].position = glm::vec3(bbmin.x, bbmax.y, bbmax.z);
    vertices_[6].position = glm::vec3(bbmax.x, bbmax.y, bbmax.z);
    vertices_[7].position = glm::vec3(bbmax.x, bbmax.y, bbmin.z);
}

void BBoxBuilder::build_line_strip_indices(
    const unsigned int primitive_restart_index)
{
    const size_t NUM_INDICES_PER_BBOX = 19;
    indices_.resize(NUM_INDICES_PER_BBOX);
    indices_[0] = 0;
    indices_[1] = 1;
    indices_[2] = 2;
    indices_[3] = 3;
    indices_[4] = 0;
    indices_[5] = 4;
    indices_[6] = 5;
    indices_[7] = 6;
    indices_[8] = 7;
    indices_[9] = 4;
    indices_[10] = primitive_restart_index;
    indices_[11] = 1;
    indices_[12] = 5;
    indices_[13] = primitive_restart_index;
    indices_[14] = 2;
    indices_[15] = 6;
    indices_[16] = primitive_restart_index;
    indices_[17] = 3;
    indices_[18] = 7;
}

void BBoxBuilder::build_triangle_fan_indices(
    unsigned int primitive_restart_index)
{
    const size_t NUM_INDICES_PER_BBOX = 17;
    indices_.resize(NUM_INDICES_PER_BBOX);
    indices_[0] = 0;
    indices_[1] = 1;
    indices_[2] = 2;
    indices_[3] = 3;
    indices_[4] = 7;
    indices_[5] = 4;
    indices_[6] = 5;
    indices_[7] = 1;
    indices_[8] = primitive_restart_index;
    indices_[9] = 6;
    indices_[10] = 5;
    indices_[11] = 4;
    indices_[12] = 7;
    indices_[13] = 3;
    indices_[14] = 2;
    indices_[15] = 1;
    indices_[16] = 5;
}

}
