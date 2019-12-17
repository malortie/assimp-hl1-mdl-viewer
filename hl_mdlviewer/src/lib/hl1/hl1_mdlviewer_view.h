/**
* \file hl1_mdlviewer_view.h
* \brief Interface for views that interact with HL1 models.
*/

#ifndef HLMDLVIEWER_HL1_MDLVIEWER_VIEW_H_
#define HLMDLVIEWER_HL1_MDLVIEWER_VIEW_H_

#include "mdlviewer_view.h"
#include "hl1_model_render_settings.h"
#include "hl1_ui_data.h"

namespace hl_mdlviewer {
namespace hl1 {

class HL1MDLViewerPresenter;

class HL1MDLViewerView : public hl_mdlviewer::MDLViewerView<HL1MDLViewerPresenter>
{
public:
    virtual void set_bodypart_range(int min, int max) = 0;
    virtual void set_bodypart(int value, const char* name) = 0;
    virtual void set_model_range(int min, int max) = 0;
    virtual void set_model(int bodypart_index, int value, const char* name) = 0;
    virtual void set_skin_range(int min, int max) = 0;
    virtual void set_skin(const int value) = 0;
    virtual void set_bone_controller(int index, float value) = 0;
    virtual void set_sequence(int value, const char* name) = 0;
    virtual void set_playback_rate(float value) = 0;
    virtual void set_blend_controller(int index, float value) = 0;
    virtual void set_render_mode(RenderMode render_mode) = 0;
    virtual void set_show_normals(bool enabled) = 0;
    virtual void set_show_bones(bool enabled) = 0;
    virtual void set_show_attachments(bool enabled) = 0;
    virtual void set_show_hitboxes(bool enabled) = 0;
    virtual void set_show_sequence_bbox(bool enabled) = 0;
    virtual void set_highlight_models(bool enabled) = 0;
    virtual void set_draw_wireframe(bool enabled) = 0;
    virtual void set_draw_chrome_effects(bool enabled) = 0;
    virtual void set_lighting_enabled(bool enabled) = 0;

    virtual void setup_ui(const UIData& ui_data) = 0;
};

}
}

#endif // HLMDLVIEWER_HL1_MDLVIEWER_VIEW_H_
