/**
* \file hl1_ui_setup.h
*/

#ifndef HLMDLVIEWER_HL1_UI_DATA_H_
#define HLMDLVIEWER_HL1_UI_DATA_H_

namespace hl_mdlviewer {
namespace hl1 {

struct UIModel
{
    int index;
    std::string name;
};

struct UIBodypart
{
    int index;
    std::string name;

    std::vector<UIModel> models;
};

struct UISequence
{
    int index;
    std::string name;
};

struct UIBoneController
{
    int index;
    float min;
    float max;
    bool is_mouth;
    float rest_value;
};

struct UIBlendController
{
    int index;
    float min;
    float max;
    float rest_value;
};

/** \brief A structure used to keep loose coupling between 
*          the Studiomodel and the view. */
struct UIData
{
    std::vector<UIBodypart> bodyparts;
    std::vector<UISequence> sequences;
    std::vector<UIBoneController> bone_controllers;
    std::vector<UIBlendController> blend_controllers;
    uint32_t num_skin_families;
};

}
}

#endif // HLMDLVIEWER_HL1_UI_DATA_H_
