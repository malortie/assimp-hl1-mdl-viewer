/**
* \file hl1_mdlviewer_presenter.h
* \brief Declaration for the HL1 model viewer presenter class.
*/

#ifndef HLMDLVIEWER_HL1_MDLVIEWER_PRESENTER_H_
#define HLMDLVIEWER_HL1_MDLVIEWER_PRESENTER_H_

#include "mdlviewer_presenter.h"
#include "hl1_studiomodel_animation.h"
#include "hl1_studiomodel_render.h"
#include "sound_system.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace hl_mdlviewer {
namespace hl1 {

class HL1MDLViewerView;

class HL1MDLViewerPresenter : public MDLViewerPresenter
{
public:
    HL1MDLViewerPresenter(HL1MDLViewerView* view);
    virtual void initialize();
    virtual void run();
    virtual void load_model(const std::string& file_path);
    virtual void dispose();

    virtual void setup_default_search_paths();

    virtual void set_bodypart(int value);
    virtual void set_bodypart_model(int model);
    virtual void set_skin(int value);
    virtual void set_bone_controller(int index, float value);
    virtual void set_sequence(int value);
    virtual void set_playback_rate(float value);
    virtual void set_blending(int blend_index, uint8_t value);
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

    virtual void draw_model(float frame_time);
    virtual void set_canvas_dimensions(int width, int height);
    virtual void update_angles(const glm::vec2& delta);
    virtual void update_pan(const glm::vec2& delta);
    virtual void update_camera_distance(const float distance);
    virtual void update_camera_distance_zoom_step(const bool forward);

protected:

    const StudioModelAnimationData* animation_data() const { return model_animation_.animation_data(); }
    const StudioModelRenderData* render_data() const { return model_render_.render_data(); }
    const ModelRenderSettings* render_settings() const { return model_render_.render_settings(); }

    inline int get_active_bodypart() const { return bodypart_; }
    inline int get_bodypart_model_value(int bodypart) const { return render_data()->model[bodypart]; }
    inline const char* get_bodypart_name(int bodypart) const { return studio_model_.bodyparts[bodypart].name.c_str(); }
    inline const char* get_model_name(int bodypart, int model) const { return studio_model_.bodyparts[bodypart].models[model]->name.c_str(); }
    inline size_t get_bodypart_model_count(int bodypart) const { return studio_model_.bodyparts[bodypart].models.size(); }

    void unload_model();

private:

    FileSystem file_system_;
    SoundSystem sound_system_;

    /** \brief A pointer to the view used by this presenter. */
    HL1MDLViewerView* view_;

    bool model_loaded_;

    /** \brief The active bodypart. */
    int bodypart_;

    /** \brief The Assimp importer. */
    Assimp::Importer importer_;

    /** \brief The scene that was loaded. */
    const aiScene* scene_;

    StudioModel studio_model_;
    StudioModelRender model_render_;
    StudioModelAnimation model_animation_;
    AnimationEventHandler event_handler_;
    FrameInterpolation frame_interpolation_;
};

}
}

#endif // HLMDLVIEWER_HL1_MDLVIEWER_PRESENTER_H_
