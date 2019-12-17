/**
* \file hl1_studiomodel.h
*/

#ifndef HLMDLVIEWER_HL1_STUDIOMODEL_H_
#define HLMDLVIEWER_HL1_STUDIOMODEL_H_

#include <vector>
#include <string>

#include "hl1_model_stats.h"
#include "hl1_studiomodel_defines.h"

#include <assimp/anim.h>
#include <assimp/material.h>

namespace hl_mdlviewer {
namespace hl1 {

struct BoneController;
struct Bodypart;
struct Model;
struct Mesh;
struct Texture;

/** \brief Represent a Studiomodel mesh. */
struct Mesh
{
    int index;
    Model* model;
    Texture* texture;
};

/** \brief Represent a Studiomodel sub model. */
struct Model
{
    int index;
    std::string name;
    Bodypart* bodypart;

    std::vector<Mesh*> meshes;
};

/** \brief Represent a Studiomodel bodypart. */
struct Bodypart
{
    int index;
    std::string name;
    std::vector<Model*> models;
};

/** \brief Represent a Studiomodel bone. */
struct Bone
{
    int index;
    int parent_index;
    std::string name;
    Bone* parent;
    glm::quat local_quat;
    glm::vec3 local_position;
    glm::mat4 offset_matrix;
    std::vector<Bone*> children;
    std::vector<BoneController*> bone_controllers;
};

/** \brief Represent a Studiomodel animation event. */
struct AnimationEvent
{
    int frame;
    int event;
    std::string options;
};

/** \brief Represent a Studiomodel sequence. */
struct Sequence
{
    int index;
    std::string name;
    float fps;
    int num_frames;
    glm::vec3 bbmin;
    glm::vec3 bbmax;
    std::vector<aiAnimation*> blends;
    std::vector<AnimationEvent> events;
};

/** \brief Represent a Studiomodel attachment. */
struct Attachment
{
    int index;
    Bone* bone;
    glm::vec3 position;
};

/** \brief Represent a Studiomodel hitbox. */
struct Hitbox
{
    int index;
    Bone* bone;
    int group;
    glm::vec3 bbmin;
    glm::vec3 bbmax;
};

/** \brief The bone controller motion type. */
enum class MotionType
{
    Position = 0,
    Rotation
};


/** \brief The bone controller motion axis. */
enum MotionAxis
{
    AxisX = 0,
    AxisY,
    AxisZ
};

/** \brief Converts a set of motion flags of a motion type to
* an integral motion type.
*
* \param[in] motion_flags Motion flags.
* \return A MotionType value.
*/
inline MotionType motion_flags_to_motion_type(int motion_flags)
{
    motion_flags = motion_flags & STUDIO_TYPES;

    if (motion_flags & STUDIO_X ||
        motion_flags & STUDIO_Y || 
        motion_flags & STUDIO_Z ||
        motion_flags & STUDIO_LX ||
        motion_flags & STUDIO_LY ||
        motion_flags & STUDIO_LZ ||
        motion_flags & STUDIO_AX ||
        motion_flags & STUDIO_AY ||
        motion_flags & STUDIO_AZ)
        return MotionType::Position;
    else if (motion_flags & STUDIO_XR ||
        motion_flags & STUDIO_YR ||
        motion_flags & STUDIO_ZR ||
        motion_flags & STUDIO_AXR ||
        motion_flags & STUDIO_AYR ||
        motion_flags & STUDIO_AZR)
        return MotionType::Rotation;
    else
        throw std::exception(std::string("Unknown motion flags " + std::to_string(motion_flags)).c_str());
}

/** \brief Determines the motion axis from the a set of motion flags.
*
* \param[in] motion_type Motion flags.
* \return A MotionAxis type.
*/
inline MotionAxis motion_type_to_motion_axis(int motion_flags)
{
    motion_flags = motion_flags & STUDIO_TYPES;

    if (motion_flags & STUDIO_X ||
        motion_flags & STUDIO_LX ||
        motion_flags & STUDIO_AX ||
        motion_flags & STUDIO_XR ||
        motion_flags & STUDIO_AXR)
        return MotionAxis::AxisX;

    if (motion_flags & STUDIO_Y ||
        motion_flags & STUDIO_LY ||
        motion_flags & STUDIO_AY ||
        motion_flags & STUDIO_YR ||
        motion_flags & STUDIO_AYR)
        return MotionAxis::AxisY;

    if (motion_flags & STUDIO_Z ||
        motion_flags & STUDIO_LZ ||
        motion_flags & STUDIO_AZ ||
        motion_flags & STUDIO_ZR ||
        motion_flags & STUDIO_AZR)
        return MotionAxis::AxisZ;

    throw std::exception(std::string("Unknown motion flags " + std::to_string(motion_flags)).c_str());
}

/** \brief Represent a Studiomodel bone controller. */
struct BoneController
{
    int index;
    Bone* bone;
    int bone_index;
    MotionAxis motion_axis;
    MotionType motion_type;
    float start;
    float end;
    bool is_mouth;
    bool wraps;
};

/** \brief Represent a Studiomodel texture. */
struct Texture
{
    enum class Type
    {
        Default = 0,
        Chrome,
    };

    Texture() :
        index(0),
        type(Type::Default),
        shading_mode(static_cast<aiShadingMode>(0)),
        blend_mode(aiBlendMode_Default),
        mask_color(),
        skin_textures()
    {
    }

    int index;
    Type type;
    aiShadingMode shading_mode;
    aiBlendMode blend_mode;
    aiTextureFlags flags;
    glm::vec3 mask_color;
    std::vector<Texture*> skin_textures;
};

/** \brief A structure that holds all model information. */
struct StudioModel
{
    void clear()
    {
        bodyparts.clear();
        models.clear();
        meshes.clear();
        bones.clear();
        attachments.clear();
        hitboxes.clear();
        bone_controllers.clear();
        sequences.clear();
        textures.clear();

        stats.reset();
    }

    std::vector<Bodypart> bodyparts;
    std::vector<Model> models;
    std::vector<Mesh> meshes;
    std::vector<Bone> bones;
    std::vector<BoneController> bone_controllers;
    std::vector<Attachment> attachments;
    std::vector<Hitbox> hitboxes;
    std::vector<Sequence> sequences;
    std::vector<Texture> textures;

    ModelStats stats;
};

}
}

#endif // HLMDLVIEWER_HL1_STUDIOMODEL_H_
