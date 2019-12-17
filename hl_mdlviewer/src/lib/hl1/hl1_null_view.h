/**
* \file hl1_null_view.h
* \brief Declaration and implementation for the null view class.
*        See the 'null' object pattern for more info.
*/

#ifndef HLMDLVIEWER_HL1_NULL_VIEW_H_
#define HLMDLVIEWER_HL1_NULL_VIEW_H_

#include <string>
#include "hl1_mdlviewer_view.h"

namespace hl_mdlviewer {
namespace hl1 {

class HL1MDLViewerViewNull : public HL1MDLViewerView
{
public:

    virtual void set_presenter(HL1MDLViewerPresenter* presenter) {}
    virtual void initialize() {}
    virtual void run() {}
    virtual void invalidate() {}
    virtual void dispose() {}

    virtual void on_model_loading_success() {}
    virtual void on_model_loading_failure(const std::string& model_file_path) {}
    virtual void enable_model_interaction() {}
    virtual void disable_model_interaction() {}

    virtual void set_bodypart_range(int min, int max) {}
    virtual void set_bodypart(int value, const char* name) {}
    virtual void set_model_range(int min, int max) {}
    virtual void set_model(int bodypart_index, int value, const char* name) {}
    virtual void set_skin_range(int min, int max) {}
    virtual void set_skin(const int value) {}
    virtual void set_bone_controller(int index, float value) {}
    virtual void set_sequence(int value, const char* name) {}
    virtual void set_playback_rate(float value) {}
    virtual void set_blend_controller(int index, float value) {}
    virtual void set_render_mode(RenderMode render_mode) {}
    virtual void set_show_normals(bool enabled) {}
    virtual void set_show_bones(bool enabled) {}
    virtual void set_show_attachments(bool enabled) {}
    virtual void set_show_hitboxes(bool enabled) {}
    virtual void set_show_sequence_bbox(bool enabled) {}
    virtual void set_highlight_models(bool enabled) {}
    virtual void set_draw_wireframe(bool enabled) {}
    virtual void set_draw_chrome_effects(bool enabled) {}
    virtual void set_lighting_enabled(bool enabled) {}

    virtual void setup_ui(const UIData& ui_data) {}
};

}
}

#endif // HLMDLVIEWER_HL1_NULL_VIEW_H_
