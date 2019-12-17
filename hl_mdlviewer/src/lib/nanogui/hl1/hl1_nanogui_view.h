/**
* \file hl1_nanogui_view.h
* \brief Declaration for the HL1 nanogui view for the model viewer.
*/

#ifndef HLMDLVIEWER_HL1_NANOGUI_VIEW_H_
#define HLMDLVIEWER_HL1_NANOGUI_VIEW_H_

#include "hl1_mdlviewer_view.h"
#include "hl1_nanogui_view_glcanvas.h"

#include <nanogui/nanogui.h>

namespace hl_mdlviewer {
namespace hl1 {

template<typename Derived, typename Callback = void(Derived*)>
class IndexedControl
{
public:
    IndexedControl(int index) :
        mIndex(index)
    {
    }

    void setIndexedControlCallback(const std::function<Callback>& callback) { mIndexedControlCallback = callback; }

    const int& index() const { return mIndex; }

    void setIndex(const int index) { mIndex = index; }

protected:
    int mIndex;
    std::function<Callback> mIndexedControlCallback;
};




class IndexedButton : public nanogui::Button, public IndexedControl<IndexedButton>
{
public:
    IndexedButton(Widget* parent,
        int index,
        const std::string& caption = "Untitled",
        int icon = 0) :
        Button(parent, caption, icon),
        IndexedControl<IndexedButton>(index)
    {
        setCallback([&]() {
            if (mIndexedControlCallback != nullptr)
                mIndexedControlCallback(this);
            });
    }
};

class IndexedSlider;
using IndexedSliderCallbackFunction = void(IndexedSlider*, float);

class IndexedSlider : public nanogui::Slider,
    public IndexedControl<IndexedSlider, IndexedSliderCallbackFunction>
{
public:
    IndexedSlider(Widget* parent, int index) :
        Slider(parent),
        IndexedControl<IndexedSlider, IndexedSliderCallbackFunction>(index)
    {
        setCallback([&](float value) {
            if (mIndexedControlCallback != nullptr)
                mIndexedControlCallback(this, value);
            });
    }
};

class HL1MDLViewerPresenter;

class HL1NanoGUIView : public HL1MDLViewerView
{
public:
    HL1NanoGUIView();

    virtual void set_presenter(HL1MDLViewerPresenter* presenter);

    virtual void initialize();
    virtual void dispose();
    virtual void run();
    virtual void invalidate();

    virtual void set_bodypart_range(int min, int max);
    virtual void set_bodypart(int value, const char* name);
    virtual void set_model_range(int min, int max);
    virtual void set_model(int bodypart_index, int value, const char* name);
    virtual void set_skin_range(int min, int max);
    virtual void set_skin(const int value);
    virtual void set_bone_controller(int index, float value);
    virtual void set_sequence(int index, const char* name);
    virtual void set_playback_rate(float value);
    virtual void set_blend_controller(int index, float value);
    virtual void set_render_mode(RenderMode render_mode);
    virtual void set_show_normals(bool enabled);
    virtual void set_show_bones(bool enabled);
    virtual void set_show_attachments(bool enabled);
    virtual void set_show_hitboxes(bool enabled);
    virtual void set_show_sequence_bbox(bool enabled);
    virtual void set_highlight_models(bool enabled);
    virtual void set_draw_wireframe(bool enabled);
    virtual void set_draw_chrome_effects(bool enabled);
    virtual void set_lighting_enabled(bool enabled);

    virtual void on_model_loading_success();
    virtual void on_model_loading_failure(const std::string& model_file_path);
    virtual void enable_model_interaction();
    virtual void disable_model_interaction();

    virtual void setup_ui(const UIData& ui_data);

private:

    HL1MDLViewerPresenter* presenter_;

    nanogui::ref<nanogui::Screen> screen_;

    nanogui::GLCanvas* glcanvas_;
    nanogui::IntBox<int>* bodypart_;
    nanogui::IntBox<int>* model_;
    nanogui::IntBox<int>* skin_;
    nanogui::PopupButton* sequence_button_;
    nanogui::Widget* sequence_panel_;
    nanogui::Widget* bone_controller_panel_;
    nanogui::Widget* blend_panel_;
    nanogui::ComboBox* rendermode_;
    nanogui::CheckBox* show_normals_;
    nanogui::CheckBox* show_bones_;
    nanogui::CheckBox* show_attachments_;
    nanogui::CheckBox* show_hitboxes_;
    nanogui::CheckBox* show_sequence_bbox_;
    nanogui::CheckBox* highlight_models_;
    nanogui::CheckBox* wireframe_;
    nanogui::CheckBox* enable_lighting_;
    nanogui::CheckBox* enable_chrome_effects_;
    nanogui::Slider* playback_rate_;
    std::vector<IndexedSlider*> bone_controllers_;
    std::vector<IndexedSlider*> blenders_;
    nanogui::TabWidget* tab_;
};

}
}

#endif // HLMDLVIEWER_HL1_NANOGUI_VIEW_H_
