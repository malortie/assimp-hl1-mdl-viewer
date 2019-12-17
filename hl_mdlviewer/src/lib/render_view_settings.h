/**
* \file render_view_settings.h
*/

#ifndef HLMDLVIEWER_RENDER_VIEW_SETTINGS_H_
#define HLMDLVIEWER_RENDER_VIEW_SETTINGS_H_

namespace hl_mdlviewer {

/** \brief A structure that holds the view render settings that
    will be given to the model viewer. */
struct RenderViewSettings
{
    RenderViewSettings(
        float fov_radians = 75.0f * M_PI_F / 180.0f,
        float zdistance = 50.0f, 
        float znear = 0.1f, 
        float zfar = 1000.0f,
        float rotate_sensitivity = 0.05f,
        float pan_sensitivity = 0.01f,
        float zoom_step_sensitivity = 15.0f) :
        fov_radians(fov_radians),
        znear(znear),
        zfar(zfar),
        zdistance(zdistance),
        rotate_sensitivity(rotate_sensitivity),
        pan_sensitivity(pan_sensitivity),
        zoom_step_sensitivity(zoom_step_sensitivity)
    {
    }

    /** The field of view. */
    float fov_radians;

    /** Near plane distance. */
    float znear;

    /** Far plane distance. */
    float zfar;

    /** The distance away from the model. */
    float zdistance;

    /** Self explanatory. */
    float rotate_sensitivity;

    /** Self explanatory. */
    float pan_sensitivity;

    /** Zoom step (scroll) sensitivity. */
    float zoom_step_sensitivity;
};

}

#endif // HLMDLVIEWER_RENDER_VIEW_SETTINGS_H_
