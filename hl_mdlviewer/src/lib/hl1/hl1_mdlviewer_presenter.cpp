/**
* \file hl1_mdlviewer_presenter.cpp
* \brief Implementation for the HL1 model viewer presenter class.
*/

#include "pch.h"
#include <iostream>
#include "hl1_mdlviewer_presenter.h"
#include "hl1_mdlviewer_view.h"
#include "hl1_studiomodel_setup.h"
#include "hl1_ui_setup.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace hl_mdlviewer {
namespace hl1 {

HL1MDLViewerPresenter::HL1MDLViewerPresenter(HL1MDLViewerView* view) :
    file_system_(),
    sound_system_(&file_system_),
    view_(view),
    studio_model_(),
    event_handler_(&sound_system_),
    frame_interpolation_(),
    model_render_(&studio_model_, 
        &file_system_),
    model_animation_(&studio_model_,
        &event_handler_,
        &frame_interpolation_),
    model_loaded_(false),
    scene_(nullptr)
{
    view_->set_presenter(this);
}

void HL1MDLViewerPresenter::setup_default_search_paths()
{
    file_system_.add_search_path(HLMDLVIEWER_SHADERS_SEARCH_PATH);
    file_system_.add_search_path(HLMDLVIEWER_GAME_SOUNDS_SEARCH_PATH);
}

void HL1MDLViewerPresenter::initialize()
{
    setup_default_search_paths();
    
    view_->initialize();
    model_animation_.initialize();
    model_render_.initialize();
}

void HL1MDLViewerPresenter::run()
{
    view_->run();
}

void HL1MDLViewerPresenter::dispose()
{
    view_->dispose();
    model_animation_.dispose();
    model_render_.dispose();
}

void HL1MDLViewerPresenter::load_model(const std::string& file_path)
{
    unload_model();

    try
    {
        // Use Assimp importer to load the MDL file.
        scene_ = importer_.ReadFile(file_path, aiProcess_ValidateDataStructure | aiProcess_PopulateArmatureData);
        if (!scene_)
            throw std::runtime_error(importer_.GetErrorString());

        // Convert the loaded MDL file to Studiomodel data.
        StudioModelSetup model_setup;
        glm::mat4 scene_transform(1.0f);
        model_setup.setup_model(scene_, &studio_model_, model_render_.get_buffer(), scene_transform);

        // Notify of a new Studiomodel.
        model_animation_.on_model_changed();
        model_render_.on_model_changed();

        model_render_.set_scene_transform(scene_transform);

        // Set renderer sequence bounds.
        const auto* current_sequence = &studio_model_.sequences[animation_data()->sequence];
        model_render_.set_sequence_bounds(
            current_sequence->bbmin,
            current_sequence->bbmax);

        model_loaded_ = true;

        // Setup and prepare the ui for the view.
        UIData ui_data;
        UISetup ui_setup;
        ui_setup.setup(&studio_model_,
            animation_data(),
            ui_data);

        // Give the UI data to the view.
        view_->setup_ui(ui_data);

        // Use the view interface to update the view.
        set_bodypart(0);

        view_->set_skin(render_data()->skin);

        view_->set_draw_wireframe(render_settings()->render_wireframe);
        view_->set_draw_chrome_effects(render_settings()->render_chrome_effects);
        view_->set_lighting_enabled(render_settings()->lighting_enabled);
        view_->set_highlight_models(render_settings()->highlight_models);
        view_->set_render_mode(render_settings()->render_mode);
        view_->set_show_bones(render_settings()->draw_bones);
        view_->set_show_normals(render_settings()->draw_normals);
        view_->set_show_attachments(render_settings()->draw_attachments);
        view_->set_show_hitboxes(render_settings()->draw_hitboxes);
        view_->set_show_sequence_bbox(render_settings()->draw_sequence_bbox);
        
        // Setup UI bone controllers.
        for (int i = 0; i < static_cast<int>(animation_data()->bone_controllers.size()); ++i)
            view_->set_bone_controller(i, animation_data()->bone_controllers[i].controller_value);
        
        // Setup UI blend controllers.
        for (int i = 0; i < static_cast<int>(animation_data()->blend_controllers.size()); ++i)
            view_->set_blend_controller(i, animation_data()->blend_controllers[i]);

        // Notify the view of success and enable interaction.
        view_->on_model_loading_success();
        view_->enable_model_interaction();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;

        // On model failure, disable interaction with the model viewer.
        view_->on_model_loading_failure(file_path);
        view_->disable_model_interaction();
    }

    // Tell the view to redraw itself.
    view_->invalidate();
}

void HL1MDLViewerPresenter::unload_model()
{
    studio_model_.clear();

    model_animation_.reset();
    model_render_.reset();

    bodypart_ = 0;

    model_loaded_ = false;
}

void HL1MDLViewerPresenter::set_bodypart(int bodypart)
{
    bodypart_ = bodypart;

    // Update the view.
    const int bodypart_model_value = get_bodypart_model_value(bodypart);
    view_->set_bodypart(bodypart, get_bodypart_name(bodypart));
    view_->set_model_range(0,
        static_cast<int>(get_bodypart_model_count(bodypart) - 1));

    view_->set_model(bodypart_,
        bodypart_model_value,
        get_model_name(bodypart_, bodypart_model_value));
}

void HL1MDLViewerPresenter::set_bodypart_model(int model)
{
    model_render_.set_model(bodypart_, model);

    view_->set_model(bodypart_, model,
        studio_model_.bodyparts[bodypart_].models[model]->name.c_str());
}

void HL1MDLViewerPresenter::set_skin(int skin)
{
    model_render_.set_skin(skin);
    view_->set_skin(skin);
}

void HL1MDLViewerPresenter::set_bone_controller(int index, float value)
{
    model_animation_.set_bone_controller(index, value);
    view_->set_bone_controller(index, value);
}

void HL1MDLViewerPresenter::set_sequence(int value)
{
    model_animation_.set_sequence(value);
    model_render_.set_sequence_bounds(
        studio_model_.sequences[value].bbmin,
        studio_model_.sequences[value].bbmax);
    view_->set_sequence(value, studio_model_.sequences[value].name.c_str());
}

void HL1MDLViewerPresenter::set_playback_rate(float value)
{
    model_animation_.set_playback_rate(value);
    view_->set_playback_rate(value);
}

void HL1MDLViewerPresenter::set_blending(int index, uint8_t value)
{
    model_animation_.set_blending(index, value);
    view_->set_blend_controller(index, value);
}

void HL1MDLViewerPresenter::set_render_mode(RenderMode render_mode)
{
    model_render_.set_render_mode(render_mode);
    view_->set_render_mode(render_mode);
}

void HL1MDLViewerPresenter::set_show_normals(bool enabled)
{
    model_render_.set_show_normals(enabled);
    view_->set_show_normals(enabled);
}

void HL1MDLViewerPresenter::set_show_bones(bool enabled)
{
    model_render_.set_show_bones(enabled);
    view_->set_show_bones(enabled);
}

void HL1MDLViewerPresenter::set_show_attachments(bool enabled)
{
    model_render_.set_show_attachments(enabled);
    view_->set_show_attachments(enabled);
}

void HL1MDLViewerPresenter::set_show_hitboxes(bool enabled)
{
    model_render_.set_show_hitboxes(enabled);
    view_->set_show_hitboxes(enabled);
}

void HL1MDLViewerPresenter::set_show_sequence_bbox(bool enabled)
{
    model_render_.set_show_sequence_bbox(enabled);
    view_->set_show_sequence_bbox(enabled);
}

void HL1MDLViewerPresenter::set_highlight_models(bool enabled)
{
    model_render_.set_highlight_models(enabled);
    view_->set_highlight_models(enabled);
}

void HL1MDLViewerPresenter::set_draw_wireframe(bool enabled)
{
    model_render_.set_draw_wireframe(enabled);
    view_->set_draw_wireframe(enabled);
}

void HL1MDLViewerPresenter::set_draw_chrome_effects(bool enabled)
{
    model_render_.set_draw_chrome_effects(enabled);
    view_->set_draw_chrome_effects(enabled);
}

void HL1MDLViewerPresenter::set_lighting_enabled(bool enabled)
{
    model_render_.set_lighting_enabled(enabled);
    view_->set_lighting_enabled(enabled);
}

void HL1MDLViewerPresenter::draw_model(float frame_time)
{
    if (!model_loaded_)
        return;

    model_animation_.update(frame_time);

    model_render_.set_bones_transform(model_animation_.get_bone_transforms());
    model_render_.setup_view();
    model_render_.render();
}

void HL1MDLViewerPresenter::set_canvas_dimensions(int width, int height)
{
    model_render_.setup_projection_matrix(width, height);
}

void HL1MDLViewerPresenter::update_angles(const glm::vec2& delta)
{
    model_render_.update_angles(delta);
}

void HL1MDLViewerPresenter::update_pan(const glm::vec2& delta)
{
    model_render_.update_pan(delta);
}

void HL1MDLViewerPresenter::update_camera_distance(const float distance)
{
    model_render_.update_camera_distance(distance);
}

void HL1MDLViewerPresenter::update_camera_distance_zoom_step(const bool forward)
{
    model_render_.update_camera_distance_zoom_step(forward);
}

}
}
