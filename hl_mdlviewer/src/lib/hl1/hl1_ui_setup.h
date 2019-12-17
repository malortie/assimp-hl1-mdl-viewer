/**
* \file hl1_ui_setup.h
* \brief Declaration for the HL1 UI setup class.
*/

#ifndef HLMDLVIEWER_HL1_UI_SETUP_H_
#define HLMDLVIEWER_HL1_UI_SETUP_H_

#include "hl1_studiomodel.h"
#include "hl1_studiomodel_animation_data.h"
#include "hl1_ui_data.h"

namespace hl_mdlviewer {
namespace hl1 {

/** \brief A class to help setup the UI after a model has finished loading. */
class UISetup
{
public:
    UISetup();
    UISetup(const UISetup& other) = delete;

    virtual void setup(
        const StudioModel* studio_model, 
        const StudioModelAnimationData* studio_animation_data,
        UIData& ui_data);
};

}
}

#endif // HLMDLVIEWER_HL1_UI_SETUP_H_
