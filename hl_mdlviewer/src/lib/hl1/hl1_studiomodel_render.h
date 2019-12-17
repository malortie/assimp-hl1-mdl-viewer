/**
* \file hl1_studiomodel_render.h
* \brief Declaration for the HL1 Studio model render class.
*/

#ifndef HLMDLVIEWER_HL1_STUDIOMODEL_RENDER_H_
#define HLMDLVIEWER_HL1_STUDIOMODEL_RENDER_H_

#include "hl1_studiomodel.h"
#include "hl1_model_render_settings.h"
#include "render_view_settings.h"
#include "hl1_studiomodel_render_data.h"
#include "hl1_studiomodel_buffer.h"
#include "glprogram.h"
#include "gluniformbuffer.h"
#include "file_system.h"

namespace hl_mdlviewer {
namespace hl1 {

/** \brief The HL1 model renderer class. */
class StudioModelRender
{
public:
    StudioModelRender(
        StudioModel* studio_model,
        FileSystem* file_system);
    StudioModelRender(const StudioModelRender&) = delete;

    void initialize();
    void dispose();

    void reset();

    StudioModelBuffer* get_buffer() { return &studio_model_buffer_; }

    const StudioModelRenderData* render_data() const { return &render_data_; }
    const ModelRenderSettings* render_settings() const { return &settings_; }

    /** \brief Apply rotation to the model. 
    * \param[in] delta The rotation delta to apply to the model, in radians.
    */
    void update_angles(const glm::vec2& delta);
    
    /** \brief Apply pan to the model.
    * \param[in] delta The delta by which to translate the model.
    */
    void update_pan(const glm::vec2& delta);

    /** \brief Increase or decrease the distance between
    *          the camera and the model.
    * \param[in] distance The distance to apply.
    */
    void update_camera_distance(const float distance);

    /** \brief Increase or decrease the distance between
    *          the camera and the model by 'step'.
    * \param[in] forward Whether or not to move the camera toward the model.
    */
    void update_camera_distance_zoom_step(const bool forward);

    void reset_camera();

    void setup_projection_matrix(int width, int height);
    void setup_view();

    /** \brief Set the bone transforms to be used by the renderer.
    * \param[in] bones_transform The bone transforms.
    */
    void set_bones_transform(const std::vector<glm::mat4>& bones_transform);
    void set_sequence_bounds(const glm::vec3& bbmin, const glm::vec3& bbmax);

    void set_scene_transform(const glm::mat4& transform);

    void set_model(int bodypart, int model);
    void set_skin(int value);
    void set_render_mode(RenderMode render_mode);
    void set_show_normals(bool enabled);
    void set_show_bones(bool enabled);
    void set_show_attachments(bool enabled);
    void set_show_hitboxes(bool enabled);
    void set_show_sequence_bbox(bool enabled);
    void set_highlight_models(bool enabled);
    void set_draw_wireframe(bool enabled);
    void set_draw_chrome_effects(bool enabled);
    void set_lighting_enabled(bool enabled);

    inline void set_zdistance(float distance) { 
        view_settings_.zdistance = clamp(distance, view_settings_.znear, view_settings_.zfar);
    }

    /** \brief Render the 3D scene. */
    void render();

    void on_model_changed();

protected:

    void load_shaders();
    void setup_uniform_buffers();

    void add_shader_program(glprogram& reference,
        typename glprogram::ShaderInitializerList&& shader_initializer_list);

    void render_model();
    void render_model_wireframe();
    void render_model_smooth();
    void render_model_textured();
    void render_model_textured_additive();

    void render_bones();
    void render_attachments();
    void render_hitboxes();
    void render_normals();
    void render_sequence_bbox();

    void update_meshes_to_render();
    void update_opaque_and_additive_textures();
    void update_offset_matrices();

private:
    void render_meshes_textured(const std::list<size_t>& meshes);
    void delete_resources();

    /** \brief A pointer to the Studiomodel. */
    StudioModel* studio_model_;

    ModelRenderSettings settings_;
    RenderViewSettings view_settings_;
    StudioModelRenderData render_data_;
    StudioModelBuffer studio_model_buffer_;
    FileSystem* file_system_;

    /** \brief The current projection matrix. */
    glm::mat4 projection_matrix_;

    /** \brief The current model angles. */
    glm::vec3 angles_;

    /** \brief The current camera pan translation. */
    glm::vec2 pan_;

    glprogram flat_program_;
    glprogram smooth_program_;
    glprogram textured_program_;
    glprogram normal_program_;
    std::vector<glprogram*> shader_programs_;

    gluniformbuffer matrices_uniform_buffer_;
    gluniformbuffer bone_matrices_uniform_buffer_;
    gluniformbuffer bone_offset_matrices_uniform_buffer_;
    gluniformbuffer global_uniform_buffer_;
    gluniformbuffer global2_uniform_buffer_;

    std::vector<glm::vec4> default_colors_;

    std::list<size_t> meshes_to_render_;

    /** \brief A list of meshes to render after the opaque meshes. */
    std::list<size_t> additive_meshes_;
    std::list<size_t> opaque_meshes_;
};

}
}

#endif // HLMDLVIEWER_HL1_STUDIOMODEL_RENDER_H_
