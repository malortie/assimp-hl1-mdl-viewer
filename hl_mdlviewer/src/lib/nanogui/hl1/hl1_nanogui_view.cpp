/**
* \file hl1_nanogui_view.cpp
* \brief Implementation for the HL1 nanogui view for the model viewer.
*/

#include "pch.h"
#include "hl1_nanogui_view.h"
#include "hl1_mdlviewer_presenter.h"
#include "hl1_model_stats.h"
#include <nanogui/messagedialog.h>

namespace hl_mdlviewer {
namespace hl1 {

HL1NanoGUIView::HL1NanoGUIView() :
    presenter_(nullptr),
    glcanvas_(nullptr),
    bodypart_(nullptr),
    model_(nullptr),
    skin_(nullptr),
    sequence_button_(nullptr),
    sequence_panel_(nullptr),
    rendermode_(nullptr),
    show_normals_(nullptr),
    show_bones_(nullptr),
    show_attachments_(nullptr),
    show_hitboxes_(nullptr),
    highlight_models_(nullptr),
    wireframe_(nullptr),
    enable_lighting_(nullptr),
    enable_chrome_effects_(nullptr),
    playback_rate_(nullptr),
    bone_controller_panel_(nullptr),
    blend_panel_(nullptr),
    bone_controllers_(),
    blenders_()
{
}

void HL1NanoGUIView::set_presenter(HL1MDLViewerPresenter* presenter)
{
    presenter_ = presenter;
}

void HL1NanoGUIView::initialize()
{
    using namespace nanogui;

    nanogui::init();

    screen_ = new nanogui::Screen(Eigen::Vector2i(800, 600), "NanoGUI Test", true, false, 
        8, // color bits
        8, // alpha bits
        24, // depth bits
        8, // stencil bits
        0, // num samples
        3, // Major
        3); // Minor
    screen_->drawAll();
    screen_->setVisible(true);
    screen_->setResizeCallback([&](Vector2i dimensions){
        glcanvas_->setSize(Vector2i(screen_->width(), screen_->height()));
        screen_->performLayout();
        presenter_->set_canvas_dimensions(glcanvas_->width(), glcanvas_->height());
    });

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        throw std::runtime_error("Could not initialize GLAD!");

    glcanvas_ = new MyGLCanvas(presenter_, screen_);
    glcanvas_->setBackgroundColor({ 100, 100, 100, 255 });
    glcanvas_->setSize(Vector2i(screen_->width(), screen_->height()));

    Window* w = new Window(screen_, "test");
    w->setPosition(Vector2i(15, 15));
    w->setLayout(new BoxLayout(
        Orientation::Vertical,
        Alignment::Middle,
        5,
        5));

    Widget* p = new Widget(w);
    p->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 5, 5));

    Button* b = new Button(p, "Open file");

    b->setCallback([&]() {
        char current_directory[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, current_directory);
        std::string file_path = file_dialog({ {"mdl", "Valve .MDL file"} }, false);
        
        if (file_path != "")
        {
            SetCurrentDirectoryA(current_directory);

            try {
                presenter_->load_model(file_path.c_str());
            } catch (...) {
                tab_->setVisible(false);
                tab_->setEnabled(false);

                std::stringstream ss;
                ss << "Failed to open file ";
                ss << file_path.c_str();
                auto dlg = new MessageDialog(
                    screen_,
                    MessageDialog::Type::Information,
                    "Title",
                    ss.str());
            }
        }
    });

    tab_ = w->add<TabWidget>();
    tab_->setFixedWidth(300);
    tab_->setEnabled(false);
    tab_->setVisible(false);
    {
        Widget* layer = tab_->createTab("Rendering");
        layer->setWidth(screen_->width());
        layer->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 0, 0));

        p = new Widget(layer);
        GridLayout* layout =
            new GridLayout(Orientation::Horizontal, 2,
                Alignment::Fill, 15, 6);
        layout->setColAlignment({ Alignment::Minimum, Alignment::Fill });
        p->setLayout(layout);

        new Label(p, "Render mode", "sans-bold");
        rendermode_ = new ComboBox(p, { "Colored", "Textured" });
        rendermode_->setWidth(screen_->width());
        rendermode_->setEnabled(false);
        rendermode_->setSelectedIndex(static_cast<int>(RenderMode::TEXTURED));
        rendermode_->setCallback([&](int rendermode) {
            presenter_->set_render_mode(static_cast<RenderMode>(rendermode));
        });

        p = new Widget(layer);
        p->setLayout(new GroupLayout());

        wireframe_ = new CheckBox(p);
        wireframe_->setCaption("Wireframe");
        wireframe_->setCallback([&](bool checked) {
            presenter_->set_draw_wireframe(checked);
        });

        highlight_models_ = new CheckBox(p);
        highlight_models_->setCaption("Highlight models");
        highlight_models_->setCallback([&](bool checked) {
            presenter_->set_highlight_models(checked);
        });

        enable_lighting_ = new CheckBox(p);
        enable_lighting_->setCaption("Enable lighting");
        enable_lighting_->setCallback([&](bool checked) {
            presenter_->set_lighting_enabled(checked);
        });

        enable_chrome_effects_ = new CheckBox(p);
        enable_chrome_effects_->setChecked(true);
        enable_chrome_effects_->setCaption("Enable chrome effect");
        enable_chrome_effects_->setCallback([&](bool checked) {
            presenter_->set_draw_chrome_effects(checked);
        });
    }

    {

        Widget* layer = tab_->createTab("Model");
        layer->setWidth(screen_->width());
        layer->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 0, 0));
        p = new Widget(layer);
        GridLayout* layout =
            new GridLayout(Orientation::Horizontal, 2,
                Alignment::Fill, 15, 6);
        layout->setColAlignment({ Alignment::Minimum, Alignment::Fill });
        p->setLayout(layout);

        new Label(p, "Bodypart", "sans-bold");
        bodypart_ = new IntBox<int>(p);
        bodypart_->setWidth(screen_->width());
        bodypart_->setEnabled(false);
        bodypart_->setEditable(false);
        bodypart_->setSpinnable(false);
        bodypart_->setValue(0);
        bodypart_->setMinMaxValues(0, 50);
        bodypart_->setCallback([&](int value) {
            presenter_->set_bodypart(value);
        });

        new Label(p, "Model", "sans-bold");
        model_ = new IntBox<int>(p);
        model_->setWidth(screen_->width());
        model_->setCallback([&](int value) {
            presenter_->set_bodypart_model(value);
        });

        new Label(p, "Skin", "sans-bold");
        skin_ = new IntBox<int>(p);
        skin_->setWidth(screen_->width());
        skin_->setCallback([&](int value) {
            presenter_->set_skin(value);
        });

        p = new Widget(layer);
        p->setLayout(new GroupLayout());

        show_normals_ = new CheckBox(p);
        show_normals_->setCaption("Show normals");
        show_normals_->setCallback([&](bool checked) {
            presenter_->set_show_normals(checked);
        });

        show_bones_ = new CheckBox(p);
        show_bones_->setCaption("Show bones");
        show_bones_->setCallback([&](bool checked) {
            presenter_->set_show_bones(checked);
        });

        show_attachments_ = new CheckBox(p);
        show_attachments_->setCaption("Show attachments");
        show_attachments_->setCallback([&](bool checked) {
            presenter_->set_show_attachments(checked);
        });

        show_hitboxes_ = new CheckBox(p);
        show_hitboxes_->setCaption("Show hitboxes");
        show_hitboxes_->setCallback([&](bool checked) {
            presenter_->set_show_hitboxes(checked);
        });

        p = new Widget(layer);
        p->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 0, 0));
        bone_controller_panel_ = new Widget(p);
        layout = new GridLayout(Orientation::Horizontal, 2,
            Alignment::Fill, 15, 6);
        layout->setColAlignment({ Alignment::Minimum, Alignment::Fill });
        bone_controller_panel_->setLayout(layout);
    }
    {
        Widget* layer = tab_->createTab("Animation");
        layer->setWidth(screen_->width());
        layer->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Fill, 0, 0));
        p = new Widget(layer);
        GridLayout* layout =
            new GridLayout(Orientation::Horizontal, 2,
                Alignment::Fill, 15, 6);
        layout->setColAlignment({ Alignment::Minimum, Alignment::Fill });
        p->setLayout(layout);

        new Label(p, "Sequence", "sans-bold");

        sequence_button_ = new PopupButton(p, "untitled");
        sequence_button_->setWidth(screen_->width());
        sequence_button_->setEnabled(false);

        Popup* popup = sequence_button_->popup();
        VScrollPanel* vscroll = new VScrollPanel(popup);
        sequence_panel_ = new Widget(vscroll);
        sequence_panel_->setLayout(
            new BoxLayout(Orientation::Vertical,
                Alignment::Fill, 0, 0));

        new Label(p, "Playback rate", "sans-bold");
        playback_rate_ = new Slider(p);
        playback_rate_->setWidth(screen_->width());
        playback_rate_->setEnabled(false);
        playback_rate_->setValue(1.0f);
        playback_rate_->setRange({ 0.0f, 1.0f });
        playback_rate_->setFixedWidth(80);
        playback_rate_->setCallback([&](float value) {
            presenter_->set_playback_rate(value);
        });

        p = new Widget(layer);
        p->setLayout(new GroupLayout());

        show_sequence_bbox_ = new CheckBox(p);
        show_sequence_bbox_->setCaption("Show sequence bbox");
        show_sequence_bbox_->setCallback([&](bool checked) {
            presenter_->set_show_sequence_bbox(checked);
        });

        p = new Widget(layer);
        p->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 0, 0));

        blend_panel_ = new Widget(p);
        layout = new GridLayout(Orientation::Horizontal, 2,
            Alignment::Fill, 15, 6);
        layout->setColAlignment({ Alignment::Minimum, Alignment::Fill });
        blend_panel_->setLayout(layout);
    }

    tab_->setActiveTab(0);

    screen_->performLayout();

    presenter_->set_canvas_dimensions(glcanvas_->width(), glcanvas_->height());
}

void HL1NanoGUIView::run()
{
    nanogui::mainloop();
}

void HL1NanoGUIView::dispose()
{
    nanogui::shutdown();
}

void HL1NanoGUIView::invalidate()
{
    screen_->performLayout();
}

void HL1NanoGUIView::on_model_loading_success()
{
}

void HL1NanoGUIView::on_model_loading_failure(const std::string& model_file_path)
{
    std::stringstream ss;
    ss << "Failed to open file ";
    ss << model_file_path.c_str();
    auto dlg = new nanogui::MessageDialog(
        screen_,
        nanogui::MessageDialog::Type::Information,
        "Title",
        ss.str());
}

void HL1NanoGUIView::enable_model_interaction()
{
    rendermode_->setEnabled(true);
    wireframe_->setEnabled(true);
    highlight_models_->setEnabled(true);
    enable_lighting_->setEnabled(true);
    enable_chrome_effects_->setEnabled(true);

    show_normals_->setEnabled(true);
    show_bones_->setEnabled(true);
    show_attachments_->setEnabled(true);
    show_hitboxes_->setEnabled(true);

    playback_rate_->setEnabled(true);

    tab_->setEnabled(true);
    tab_->setVisible(true);
}

void HL1NanoGUIView::disable_model_interaction()
{
    tab_->setEnabled(false);
    tab_->setVisible(false);
}

void HL1NanoGUIView::setup_ui(const UIData& ui_data)
{
    using namespace nanogui;

    set_bodypart_range(0, static_cast<int>(ui_data.bodyparts.size() - 1));
    bodypart_->setValue(0);
    bodypart_->setEnabled(ui_data.bodyparts.size() > 1);
    bodypart_->setEditable(bodypart_->enabled());
    bodypart_->setSpinnable(bodypart_->enabled());

    set_model_range(0, static_cast<int>(ui_data.bodyparts.front().models.size() - 1));
    model_->setValue(0);
    model_->setEnabled(ui_data.bodyparts.front().models.size() > 1);
    model_->setEditable(model_->enabled());
    model_->setSpinnable(model_->enabled());

    set_skin_range(0, static_cast<int>(ui_data.num_skin_families - 1));
    skin_->setValue(0);
    skin_->setEnabled(ui_data.num_skin_families > 1);
    skin_->setEditable(skin_->enabled());
    skin_->setSpinnable(skin_->enabled());

    // Clear sequence list.
    while (sequence_panel_->childCount() > 0)
        sequence_panel_->removeChild(0);

    // Update sequence list.
    if (ui_data.sequences.size() > 0)
    {
        for (size_t i = 0; i < ui_data.sequences.size(); ++i)
        {
            IndexedButton* btn = new IndexedButton(sequence_panel_, 
                static_cast<int>(ui_data.sequences[i].index),
                ui_data.sequences[i].name);
            btn->setIndexedControlCallback([&](IndexedButton* b) {
                presenter_->set_sequence(b->index());
                sequence_button_->setCaption(b->caption());
                sequence_button_->setPushed(false);
            });
        }
        sequence_button_->setCaption(ui_data.sequences.front().name);
        sequence_button_->setEnabled(true);
    }
    else
    {
        sequence_button_->setCaption("");
        sequence_button_->setEnabled(false);
    }

    // Clear bone controller list.
    while (bone_controller_panel_->childCount() > 0)
        bone_controller_panel_->removeChild(0);

    if (ui_data.bone_controllers.size())
    {
        bone_controllers_.resize(ui_data.bone_controllers.size());

        for (size_t i = 0; i < ui_data.bone_controllers.size(); ++i)
        {
            const UIBoneController* bc = &ui_data.bone_controllers[i];

            if (bc->is_mouth)
                new Label(bone_controller_panel_, "Mouth", "sans-bold");
            else
                new Label(bone_controller_panel_, std::string("Bone Controller ") + std::to_string(i), "sans-bold");
            IndexedSlider* slider = bone_controllers_[i] = new IndexedSlider(bone_controller_panel_, static_cast<int>(i));
            slider->setRange({ bc->min, bc->max });
            slider->setValue(bc->rest_value);
            slider->setIndexedControlCallback([&](IndexedSlider* thisIndexedSlider, float value) {
                presenter_->set_bone_controller(thisIndexedSlider->index(), value);
            });
        }
    }

    // Clear blend list.
    while (blend_panel_->childCount() > 0)
        blend_panel_->removeChild(0);

    if (ui_data.blend_controllers.size())
    {
        blenders_.resize(ui_data.blend_controllers.size());

        for (size_t i = 0; i < ui_data.blend_controllers.size(); ++i)
        {
            const UIBlendController* b = &ui_data.blend_controllers[i];

            new Label(blend_panel_, std::string("Blend Controller ") + std::to_string(i), "sans-bold");
            IndexedSlider* slider = blenders_[i] = new IndexedSlider(blend_panel_, static_cast<int>(i));
            slider->setRange({ b->min, b->max });
            slider->setValue(b->rest_value);
            slider->setIndexedControlCallback([&](IndexedSlider* thisIndexedSlider, float value) {
                presenter_->set_blending(thisIndexedSlider->index(), static_cast<uint8_t>(value));
            });
        }
    }
}

void HL1NanoGUIView::set_bodypart_range(int min, int max)
{
    bodypart_->setMinMaxValues(min, max);
    bool usable = max > 0;
    if (usable != bodypart_->enabled())
    {
        bodypart_->setEnabled(usable);
        bodypart_->setEditable(usable);
        bodypart_->setSpinnable(usable);
    }
}

void HL1NanoGUIView::set_bodypart(int value, const char* name)
{
    bodypart_->setValue(value);
}

void HL1NanoGUIView::set_model_range(int min, int max)
{
    model_->setMinMaxValues(min, max);
    bool usable = max > 0;
    if (usable != model_->enabled())
    {
        model_->setEnabled(usable);
        model_->setEditable(usable);
        model_->setSpinnable(usable);
    }
}

void HL1NanoGUIView::set_model(int bodypart_index, int value, const char* name)
{
    model_->setValue(value);
}

void HL1NanoGUIView::set_skin_range(int min, int max)
{
    skin_->setMinMaxValues(min, max);
    bool usable = max > 0;
    if (usable != skin_->enabled())
    {
        skin_->setEnabled(usable);
        skin_->setEditable(usable);
        skin_->setSpinnable(usable);
    }
}

void HL1NanoGUIView::set_skin(const int skin)
{
    skin_->setValue(skin);
}

void HL1NanoGUIView::set_sequence(int value, const char* name)
{
    sequence_button_->setCaption(name);
    sequence_button_->setPushed(false);
}

void HL1NanoGUIView::set_bone_controller(int index, float value)
{
    bone_controllers_[index]->setValue(value);
}

void HL1NanoGUIView::set_playback_rate(float value)
{
    playback_rate_->setValue(value);
}

void HL1NanoGUIView::set_blend_controller(int index, float value)
{
    blenders_[index]->setValue(value);
}

void HL1NanoGUIView::set_render_mode(RenderMode render_mode)
{
    rendermode_->setSelectedIndex(static_cast<int>(render_mode));
}

void HL1NanoGUIView::set_show_normals(bool enabled)
{
    show_normals_->setChecked(enabled);
}

void HL1NanoGUIView::set_show_bones(bool enabled)
{
    show_bones_->setChecked(enabled);
}

void HL1NanoGUIView::set_show_attachments(bool enabled)
{
    show_attachments_->setChecked(enabled);
}

void HL1NanoGUIView::set_show_hitboxes(bool enabled)
{
    show_hitboxes_->setChecked(enabled);
}

void HL1NanoGUIView::set_show_sequence_bbox(bool enabled)
{
    show_sequence_bbox_->setChecked(enabled);
}

void HL1NanoGUIView::set_highlight_models(bool enabled)
{
    highlight_models_->setChecked(enabled);
}

void HL1NanoGUIView::set_draw_wireframe(bool enabled)
{
    wireframe_->setChecked(enabled);
}

void HL1NanoGUIView::set_draw_chrome_effects(bool enabled)
{
    enable_chrome_effects_->setChecked(enabled);
}

void HL1NanoGUIView::set_lighting_enabled(bool enabled)
{
    enable_lighting_->setChecked(enabled);
}

}
}
