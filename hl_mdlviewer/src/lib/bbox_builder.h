/**
* \file bbox_builder.h
* \brief Declaration for the bounding box builder class.
*/

#ifndef HLMDLVIEWER_BBOX_BUILDER_H_
#define HLMDLVIEWER_BBOX_BUILDER_H_

#include <vector>
#include "glvertex.h"
#include <glm/glm.hpp>

namespace hl_mdlviewer {
    
/** A class that helps build bounding box vertices and indices
* for buffer to use. */
class BBoxBuilder
{
public:
    BBoxBuilder();

    /** \brief Build the outline of the bounding box.
    * \param[in] bbmin The min extent of the bounding volume.
    * \param[in] bbmax The max extent of the bounding volume.
    * \param[in] primitive_restart_index The value to use to specify 
    *            primitive restart.
    * \param[in] bone_index The bone associated to this bounding box.
    */
    inline void build_line_strip(
        const glm::vec3& bbmin,
        const glm::vec3& bbmax,
        unsigned int primitive_restart_index,
        int bone_index)
    {
        build_vertices(bbmin, bbmax, bone_index);
        build_line_strip_indices(primitive_restart_index);
    }

    /** \brief Build the indices for the outline of the bounding box.
    * \param[in] primitive_restart_index The value to use to specify
    *            primitive restart.
    */
    void build_line_strip_indices(const unsigned int primitive_restart_index);

    /** \brief Build the faces of the bounding box.
    * \param[in] bbmin The min extent of the bounding volume.
    * \param[in] bbmax The max extent of the bounding volume.
    * \param[in] primitive_restart_index The value to use to specify
    *            primitive restart.
    * \param[in] bone_index The bone associated to this bounding box.
    */
    inline void build_triangle_fan(
        const glm::vec3& bbmin,
        const glm::vec3& bbmax,
        unsigned int primitive_restart_index,
        int bone_index) {
        build_vertices(bbmin, bbmax, bone_index);
        build_triangle_fan_indices(primitive_restart_index);
    }

    /** \brief Build the vertices for the bounding box.
    * \param[in] bbmin The min extent of the bounding volume.
    * \param[in] bbmax The max extent of the bounding volume.
    * \param[in] bone_index The bone associated to this bounding box.
    */
    virtual void build_vertices(
        const glm::vec3& bbmin,
        const glm::vec3& bbmax,
        int bone_index);

    inline const std::vector<glvertex>& get_vertices() const { return vertices_; }
    inline const std::vector<unsigned int>& get_indices() const { return indices_; }

protected:

    /** \brief Build the indices for the faces of the bounding box.
    * \param[in] primitive_restart_index The value to use to specify
    *            primitive restart.
    */
    void build_triangle_fan_indices(unsigned int primitive_restart_index);

    /** \brief The resulting vertices. */
    std::vector<glvertex> vertices_;

    /** \brief The resulting indices. */
    std::vector<unsigned int> indices_;
};


}

#endif // HLMDLVIEWER_BBOX_BUILDER_H_
