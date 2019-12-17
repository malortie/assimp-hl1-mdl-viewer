/**
* \file hl1_studiomodel_animation_data.h
*/

#ifndef HLMDLVIEWER_HL1_STUDIOMODEL_ANIMATION_DATA_H_
#define HLMDLVIEWER_HL1_STUDIOMODEL_ANIMATION_DATA_H_

#include <memory>
#include "hl1_studiomodel.h"

namespace hl_mdlviewer {
namespace hl1 {

/** \brief A structure to represent a single bone controller channel. */
struct BoneControllerChannel
{
    float controller_value;
    float adj_value;

    void reset()
    {
        controller_value = 128.0f;
        adj_value = 0.0f;
    }

    /** \brief Store the bone controller value and calculate 
    * the adjusted rotation/translation value.
    * \param[in] bone_controller The bone controller.
    * \param[in] value The value of \p bone_controller.
    */
    void set_value(const BoneController* bone_controller, float value)
    {
        controller_value = value;

        // check for 360% wrapping
        if (bone_controller->wraps)
        {
            value = value * (360.0f / 256.0f) + bone_controller->start;
        }
        else
        {
            value = clamp(value / 255.0f, 0.0f, 1.0f);
            value = (1.0f - value) * bone_controller->start + value * bone_controller->end;
        }

        if (bone_controller->motion_type == MotionType::Rotation)
        {
            adj_value = value * M_PI_F / 180.0f;
        }
        else if (bone_controller->motion_type == MotionType::Position)
        {
            adj_value = value;
        }
    }
};

/** \brief A structure that contains data used by the animation class. */
struct StudioModelAnimationData
{
public:
    StudioModelAnimationData() :
        sequence(0),
        frame(0.0f),
        playback_rate(1.0f),
        blend_controllers(),
        bone_controllers()
    {
    }

    void clear()
    {
        sequence = 0;
        frame = 0;
        blend_controllers.clear();
        bone_controllers.clear();
    }

    void initialize(int sequence, float frame, 
        const size_t num_bone_controllers, 
        const size_t num_blend_controllers)
    {
        clear();

        this->sequence = sequence;
        this->frame = frame;

        if (num_bone_controllers)
            bone_controllers.resize(num_bone_controllers);
        for (size_t i = 0; i < num_bone_controllers; ++i)
            bone_controllers[i].reset();

        if (num_blend_controllers)
            blend_controllers.resize(num_blend_controllers);
        for (size_t i = 0; i < num_blend_controllers; ++i)
            blend_controllers[i] = 128; // rest value.

    }

    int sequence;
    float frame;
    float playback_rate;
    std::vector<BoneControllerChannel> bone_controllers;
    std::vector<uint8_t> blend_controllers;
};

}
}

#endif // HLMDLVIEWER_HL1_STUDIOMODEL_ANIMATION_DATA_H_
