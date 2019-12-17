/**
* \file hl1_model_stats.h
*/

#ifndef HLMDLVIEWER_HL1_MODELSTATS_H_
#define HLMDLVIEWER_HL1_MODELSTATS_H_

namespace hl_mdlviewer {
namespace hl1 {

/** \brief A structure that holds stats about the loaded model. */
struct ModelStats
{
    ModelStats() :
        num_bodyparts(0),
        num_models(0),
        num_sequences(0),
        num_skin_families(0),
        num_textures(0),
        num_bones(0),
        num_bone_controllers(0),
        num_attachments(0),
        num_hitboxes(0),
        num_blend_contollers(0)
    {
    }

    void reset()
    {
        num_bodyparts = 0;
        num_models = 0;
        num_sequences = 0;
        num_skin_families = 0;
        num_textures = 0;
        num_bones = 0;
        num_bone_controllers = 0;
        num_attachments = 0;
        num_hitboxes = 0;
        num_blend_contollers = 0;
    }

    uint32_t num_bodyparts;
    uint32_t num_models;
    uint32_t num_sequences;
    uint32_t num_skin_families;
    uint32_t num_textures;
    uint32_t num_bones;
    uint32_t num_bone_controllers;
    uint32_t num_attachments;
    uint32_t num_hitboxes;
    uint32_t num_blend_contollers;
};

}
}

#endif // HLMDLVIEWER_HL1_MODELSTATS_H_
