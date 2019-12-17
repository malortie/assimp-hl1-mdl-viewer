/**
* \file hl1_ui_setup.cpp
* \brief Implementation for the HL1 UI setup class.
*/

#include "pch.h"
#include "hl1_ui_setup.h"

namespace hl_mdlviewer {
namespace hl1 {

UISetup::UISetup()
{
}

void UISetup::setup(
    const StudioModel* studio_model,
    const StudioModelAnimationData* studio_animation_data,
    UIData& ui_data)
{
    const Bodypart* bodypart = nullptr;
    const BoneController* bone_controller = nullptr;

    ui_data.bodyparts.resize(studio_model->bodyparts.size());
    for (size_t i = 0; i < studio_model->bodyparts.size(); ++i)
    {
        bodypart = &studio_model->bodyparts[i];
        ui_data.bodyparts[i] = { bodypart->index, bodypart->name };
        ui_data.bodyparts[i].models.resize(bodypart->models.size());
        for (size_t j = 0; j < bodypart->models.size(); ++j)
        {
            ui_data.bodyparts[i].models[j] =
            {
                bodypart->models[j]->index,
                bodypart->models[j]->name,
            };
        }
    }

    ui_data.num_skin_families = studio_model->stats.num_skin_families;

    ui_data.sequences.resize(studio_model->sequences.size());
    for (size_t i = 0; i < studio_model->sequences.size(); ++i)
    {
        ui_data.sequences[i] =
        {
            studio_model->sequences[i].index,
            studio_model->sequences[i].name,
        };
    }

    ui_data.bone_controllers.resize(studio_model->bone_controllers.size());
    for (size_t i = 0; i < studio_model->bone_controllers.size(); ++i)
    {
        bone_controller = &studio_model->bone_controllers[i];
        ui_data.bone_controllers[i] =
        {
            bone_controller->index,
            0.0f,
            255.0f,
            bone_controller->is_mouth,
            128.0f
        };
    }

    ui_data.blend_controllers.resize(studio_animation_data->blend_controllers.size());
    for (size_t i = 0; i < ui_data.blend_controllers.size(); ++i)
    {
        ui_data.blend_controllers[i] =
        {
            static_cast<int>(i),
            0.0f,
            255.0f,
            128.0f
        };
    }
}

}
}
