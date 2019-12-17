/**
* \file hl1_studiomodel_render.cpp
* \brief Implementation for the HL1 Studio model render class.
*/

#include "pch.h"
#include "hl1_studiomodel_render.h"
#include "bbox_builder.h"
#include "glprogram.h"

#define MAXSTUDIOBONES  128

namespace hl_mdlviewer {
namespace hl1 {

struct GlobalUniformBlock
{
    glm::mat4 scene_transform;
    glm::vec4 light_color;
    glm::vec4 light_direction;
    float ambient_light;
    float shade_light;
    float lambert;
    bool lighting_enabled;
};

struct MatricesUniformBlock
{
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 model;
};

struct BoneMatricesUniformBlock
{
    glm::mat4 bone_matrices[MAXSTUDIOBONES];
};

StudioModelRender::StudioModelRender(
    StudioModel* studio_model,
    FileSystem* file_system) :
    studio_model_(studio_model),
    settings_(),
    view_settings_(),
    file_system_(file_system),
    projection_matrix_(),
    flat_program_(),
    smooth_program_(),
    textured_program_(),
    normal_program_(),
    matrices_uniform_buffer_(),
    bone_matrices_uniform_buffer_(),
    global_uniform_buffer_(),
    default_colors_(),
    angles_(),
    pan_(),
    render_data_(),
    studio_model_buffer_()
{
}

void StudioModelRender::initialize()
{
    load_shaders();
    setup_uniform_buffers();
}

void StudioModelRender::dispose()
{
    delete_resources();
}

void StudioModelRender::on_model_changed()
{
    render_data_.initialize(0, studio_model_->bodyparts.size());

    // Reset bodypart and model.
    set_model(0, 0);

    update_offset_matrices();
}

void StudioModelRender::reset()
{
    render_data_.clear();
    studio_model_buffer_.clear();
    reset_camera();
}

void StudioModelRender::load_shaders()
{
    add_shader_program(flat_program_, {
        { "flat.vs", GL_VERTEX_SHADER },
        { "flat.fs", GL_FRAGMENT_SHADER }
    });

    add_shader_program(smooth_program_, {
        { "smooth.vs", GL_VERTEX_SHADER },
        { "smooth.fs", GL_FRAGMENT_SHADER }
    });

    add_shader_program(textured_program_, {
        { "textured.vs", GL_VERTEX_SHADER },
        { "textured.fs", GL_FRAGMENT_SHADER }
    });

    add_shader_program(normal_program_, {
        { "normal.vs", GL_VERTEX_SHADER },
        { "normal.gs", GL_GEOMETRY_SHADER },
        { "normal.fs", GL_FRAGMENT_SHADER }
    });

    std::for_each(shader_programs_.begin(), shader_programs_.end(),
        [&](glprogram* program)
        {
            program->bind_uniform_index("Global", 1);
            program->bind_uniform_index("Matrices", 2);
            program->bind_uniform_index("BoneMatrices", 3);
            program->bind_uniform_index("BoneOffsetMatrices", 4);
        });

    default_colors_ = {
        { 1, 0, 0, 1 },
        { 0, 1, 0, 1},
        { 1, 1, 0, 1 },
        { 0, 0, 1, 1 },
        { 1, 0, 1, 1 },
        { 0, 1, 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 0, 0.5, 1 },
        { 1, 0.5, 0, 1 },
        { 0.5, 1, 0, 1 },
        { 0.5, 1, 0.5, 1 }
    };
}

void StudioModelRender::setup_uniform_buffers()
{
    global_uniform_buffer_.initialize(
        1,
        sizeof(GlobalUniformBlock),
        GL_DYNAMIC_DRAW);

    matrices_uniform_buffer_.initialize(
        2,
        sizeof(MatricesUniformBlock),
        GL_DYNAMIC_DRAW);

    bone_matrices_uniform_buffer_.initialize(
        3,
        sizeof(BoneMatricesUniformBlock),
        GL_DYNAMIC_DRAW);

    bone_offset_matrices_uniform_buffer_.initialize(
        4,
        sizeof(BoneMatricesUniformBlock),
        GL_DYNAMIC_DRAW);

    const glm::vec3 light_color(1, 1, 1);
    const glm::vec3 light_dir(0, -1, 0);
    const float ambient_light = 32.0f;
    const float shade_light = 192.0f;
    const float lambert = 1.5f;

    global_uniform_buffer_.bind();
    global_uniform_buffer_.set_data_unbinded(
        offsetof(GlobalUniformBlock, GlobalUniformBlock::light_color),
        light_color);
    global_uniform_buffer_.set_data_unbinded(
        offsetof(GlobalUniformBlock, GlobalUniformBlock::light_direction),
        light_dir);
    global_uniform_buffer_.set_data_unbinded(
        offsetof(GlobalUniformBlock, GlobalUniformBlock::ambient_light),
        ambient_light);
    global_uniform_buffer_.set_data_unbinded(
        offsetof(GlobalUniformBlock, GlobalUniformBlock::shade_light),
        shade_light);
    global_uniform_buffer_.set_data_unbinded(
        offsetof(GlobalUniformBlock, GlobalUniformBlock::lambert),
        lambert);
    global_uniform_buffer_.set_data(
        offsetof(GlobalUniformBlock, GlobalUniformBlock::lighting_enabled),
        settings_.lighting_enabled);
    global_uniform_buffer_.unbind();
}

void StudioModelRender::add_shader_program(glprogram& program,
    typename glprogram::ShaderInitializerList&& shader_initializer_list)
{
    program.initialize_with_files(std::move(shader_initializer_list), file_system_);
    shader_programs_.push_back(&program);
}

void StudioModelRender::delete_resources()
{
    studio_model_buffer_.clear();

    for (auto prog : shader_programs_)
        prog->delete_program();
    shader_programs_.clear();
}

void StudioModelRender::setup_projection_matrix(int width, int height)
{
    projection_matrix_ = glm::perspectiveFov(
        static_cast<float>(view_settings_.fov_radians),
        static_cast<float>(width),
        static_cast<float>(height),
        view_settings_.znear,
        view_settings_.zfar);
}

void StudioModelRender::setup_view()
{
    glm::mat4 v = glm::lookAt(
        glm::vec3(0, 0, view_settings_.zdistance),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0));

    // Apply pan to view matrix.
    glm::mat4 p = glm::translate(glm::mat4(1.0f), glm::vec3(pan_.x, pan_.y, 0));

    v = p * v;

    glm::mat4 m = glm::eulerAngleXYZ(angles_.x, angles_.y, angles_.z);

    matrices_uniform_buffer_.bind();
    matrices_uniform_buffer_.set_data_unbinded(
        offsetof(MatricesUniformBlock, MatricesUniformBlock::projection),
        projection_matrix_);
    matrices_uniform_buffer_.set_data_unbinded(
        offsetof(MatricesUniformBlock, MatricesUniformBlock::view),
        v);
    matrices_uniform_buffer_.set_data_unbinded(
        offsetof(MatricesUniformBlock, MatricesUniformBlock::model),
        m);
    matrices_uniform_buffer_.unbind();
}


void StudioModelRender::set_bones_transform(const std::vector<glm::mat4>& bones_transform)
{
    size_t num_bones = std::min(bones_transform.size(), static_cast<size_t>(MAXSTUDIOBONES));

    bone_matrices_uniform_buffer_.bind();
    bone_matrices_uniform_buffer_.set_data_unbinded_with_size(
        offsetof(BoneMatricesUniformBlock, bone_matrices),
        bones_transform.data(),
        num_bones * sizeof(glm::mat4));
    bone_matrices_uniform_buffer_.unbind();
}

void StudioModelRender::set_sequence_bounds(const glm::vec3& bbmin, const glm::vec3& bbmax)
{
    BBoxBuilder bbox_builder;

    // Update sequence bbox vertices positions.
    bbox_builder.build_vertices(
        bbmin,
        bbmax,
        0);

    studio_model_buffer_.buffer.set_vertices(
        studio_model_buffer_.sequence_bbox.front(),
        bbox_builder.get_vertices());
}

void StudioModelRender::set_scene_transform(const glm::mat4& transform)
{
    global_uniform_buffer_.set_data(
        offsetof(GlobalUniformBlock, GlobalUniformBlock::scene_transform),
        transform);
}

void StudioModelRender::update_angles(const glm::vec2& delta)
{
    angles_.x = clamp(
        angles_.x + delta[1] * view_settings_.rotate_sensitivity, 
        -HALF_PI, HALF_PI);

    angles_.y += delta[0] * view_settings_.rotate_sensitivity;

    if (angles_.y > M_PI_F)
        angles_.y -= TWO_PI;
    else if (angles_.y < -M_PI_F)
        angles_.y += TWO_PI;
}

void StudioModelRender::update_pan(const glm::vec2& delta)
{
    /*
        Adjust the translation distance proportionally based on how far
        away the camera is from the model so that the farest away from
        the model, the faster the panning.

            ? 
         -------    where ? is the panning value we are looking for.
      ^ |      /    where t is fov in radians / 2
      | |     /     where a is zDistance
      | |    /
      a |-- /
      | |t /
      | | /
      v |/
     
    */

    float adjacent = std::abs(view_settings_.zdistance);
    float opposite = adjacent * std::tan(view_settings_.fov_radians * 0.5f);

    pan_.x += opposite * delta[0] * view_settings_.pan_sensitivity;
    pan_.y -= opposite * delta[1] * view_settings_.pan_sensitivity;

}

void StudioModelRender::update_camera_distance(const float distance)
{
    set_zdistance(view_settings_.zdistance + distance);
}

void StudioModelRender::update_camera_distance_zoom_step(const bool forward)
{
    set_zdistance(view_settings_.zdistance + (forward
        ? -view_settings_.zoom_step_sensitivity
        : view_settings_.zoom_step_sensitivity));
}

void StudioModelRender::reset_camera()
{
    set_zdistance(view_settings_.zdistance);
    angles_ = glm::vec3(0, 0, 0);
    pan_ = glm::vec2(0, 0);
}

void StudioModelRender::render()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PRIMITIVE_RESTART);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    studio_model_buffer_.buffer.bind();
    glPrimitiveRestartIndex(PRIMITIVE_RESTART_INDEX);

    render_model();

    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_ALWAYS);

    if (settings_.draw_bones)
        render_bones();

    if (settings_.draw_attachments)
        render_attachments();

    if (settings_.draw_hitboxes)
        render_hitboxes();

    if (settings_.draw_sequence_bbox)
        render_sequence_bbox();

    studio_model_buffer_.buffer.unbind();

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);

    glDisable(GL_PRIMITIVE_RESTART);
    glDisable(GL_PROGRAM_POINT_SIZE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void StudioModelRender::render_model()
{
    glPolygonMode(GL_FRONT, GL_FILL);

    switch (settings_.render_mode)
    {
    case RenderMode::SMOOTH:
        render_model_smooth();
        break;
    case RenderMode::TEXTURED:
    {
        render_model_textured();
        render_model_textured_additive();
    }
    break;
    };

    if (settings_.render_wireframe)
        render_model_wireframe();

    if (settings_.draw_normals)
        render_normals();
}

void StudioModelRender::render_bones()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    flat_program_.bind();

    flat_program_.set_uniform("color", settings_.bone_vertex_color);
    flat_program_.set_uniform("pointSize", 5.0f);
    flat_program_.set_uniform("applyBoneTransform", true);
    flat_program_.set_uniform("applyOffsetMatrix", false);

    studio_model_buffer_.buffer.draw_arrays_unbinded(GL_POINTS,
        studio_model_buffer_.bones);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    flat_program_.set_uniform("color", settings_.bone_segment_color);
    studio_model_buffer_.buffer.draw_indexed_unbinded(GL_LINE_STRIP,
        studio_model_buffer_.bones);
}

void StudioModelRender::render_attachments()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

    flat_program_.bind();

    flat_program_.set_uniform("color", settings_.attachment_color);
    flat_program_.set_uniform("pointSize", 5.0f);
    flat_program_.set_uniform("applyBoneTransform", true);
    flat_program_.set_uniform("applyOffsetMatrix", false);

    studio_model_buffer_.buffer.draw_arrays_unbinded(GL_POINTS,
        studio_model_buffer_.attachments);
}

void StudioModelRender::render_hitboxes()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    flat_program_.bind();
    flat_program_.set_uniform("applyBoneTransform", true);
    flat_program_.set_uniform("applyOffsetMatrix", false);

    for (auto it = studio_model_->hitboxes.cbegin(); it != studio_model_->hitboxes.cend(); ++it)
    {
        const glm::vec4& c = default_colors_[it->group % default_colors_.size()];
        flat_program_.set_uniform("color", c);
        studio_model_buffer_.buffer.draw_indexed_unbinded(GL_LINE_STRIP,
            studio_model_buffer_.hitboxes[it->index]);
    }
}

void StudioModelRender::render_normals()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    normal_program_.bind();
    normal_program_.set_uniform("color", settings_.normal_color);
    normal_program_.set_uniform("lineLength", 2.0f);

    for (auto mesh_index : meshes_to_render_)
    {
        studio_model_buffer_.buffer.draw_indexed_unbinded(
            GL_POINTS,
            studio_model_buffer_.meshes[mesh_index]);
    }
}

void StudioModelRender::render_sequence_bbox()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthFunc(GL_LEQUAL);

    flat_program_.bind();
    flat_program_.set_uniform("color", glm::vec4(1,1,0, 32.0f / 255.0f));
    flat_program_.set_uniform("pointSize", 15.0f);
    flat_program_.set_uniform("applyBoneTransform", false);
    flat_program_.set_uniform("applyOffsetMatrix", false);

    studio_model_buffer_.buffer.draw_indexed_unbinded(
        GL_TRIANGLE_FAN,
        studio_model_buffer_.sequence_bbox.front());

    glDepthFunc(GL_ALWAYS);
    flat_program_.set_uniform("color", glm::vec4(1, 0, 0, 1));
    studio_model_buffer_.buffer.draw_indexed_unbinded(
        GL_LINE_STRIP,
        studio_model_buffer_.sequence_bbox.back());

    glDisable(GL_BLEND);
}

void StudioModelRender::render_model_wireframe()
{
    flat_program_.bind();

    flat_program_.set_uniform("color", settings_.wireframe_color);
    flat_program_.set_uniform("applyBoneTransform", true);
    flat_program_.set_uniform("applyOffsetMatrix", true);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    for (auto mesh_index : meshes_to_render_)
    {
        studio_model_buffer_.buffer.draw_indexed_unbinded(
            GL_TRIANGLES,
            studio_model_buffer_.meshes[mesh_index]);
    }
}

void StudioModelRender::render_model_smooth()
{
    smooth_program_.bind();
    smooth_program_.set_uniform("color", settings_.smooth_color);
    smooth_program_.set_uniform("applyBoneTransform", true);
    smooth_program_.set_uniform("applyOffsetMatrix", true);

    Mesh* mesh = nullptr;

    for (auto mesh_index : meshes_to_render_)
    {
        mesh = &studio_model_->meshes[mesh_index];

        smooth_program_.set_uniform("useFlatShade",
            mesh->texture->shading_mode == aiShadingMode_Flat
            ? true : false);

        if (settings_.highlight_models)
        {
            const glm::vec4& c = default_colors_[mesh->model->index % default_colors_.size()];
            smooth_program_.set_uniform("color", c);
        }

        studio_model_buffer_.buffer.draw_indexed_unbinded(
            GL_TRIANGLES,
            studio_model_buffer_.meshes[mesh->index]);
    }
}

void StudioModelRender::render_meshes_textured(const std::list<size_t>& meshes)
{
    textured_program_.bind();
    textured_program_.set_uniform("applyBoneTransform", true);
    textured_program_.set_uniform("applyOffsetMatrix", true);
    if (!settings_.render_chrome_effects)
        textured_program_.set_uniform("useChrome", false);

    glPolygonMode(GL_FRONT, GL_FILL);

    Mesh* mesh = nullptr;
    Texture* texture = nullptr;

    for (auto mesh_index : meshes)
    {
        mesh = &studio_model_->meshes[mesh_index];

        texture = mesh->texture;
        if (render_data_.skin > 0 && texture->skin_textures.size())
            texture = texture->skin_textures[render_data_.skin - 1];

        studio_model_buffer_.gltextures[texture->index].bind();

        textured_program_.set_uniform("useFlatShade",
            mesh->texture->shading_mode == aiShadingMode_Flat
            ? true : false);

        if (settings_.render_chrome_effects)
        {
            textured_program_.set_uniform("useChrome",
                mesh->texture->type == Texture::Type::Chrome 
                ? true : false);
        }

        if (mesh->texture->flags & aiTextureFlags::aiTextureFlags_UseAlpha)
        {
            textured_program_.set_uniform("masked", true);
            textured_program_.set_uniform("maskColor", texture->mask_color);
        }
        else
            textured_program_.set_uniform("masked", false);

        studio_model_buffer_.buffer.draw_indexed_unbinded(
            GL_TRIANGLES,
            studio_model_buffer_.meshes[mesh->index]);
    }
}

void StudioModelRender::render_model_textured()
{
    render_meshes_textured(opaque_meshes_);
}

void StudioModelRender::render_model_textured_additive()
{
    if (additive_meshes_.empty())
        return;

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);

    render_meshes_textured(additive_meshes_);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void StudioModelRender::update_meshes_to_render()
{
    const Bodypart* bodypart = nullptr;
    const Model* model = nullptr;
    const Mesh* mesh = nullptr;

    meshes_to_render_.clear();

    for (size_t i = 0; i < studio_model_->bodyparts.size(); ++i)
    {
        bodypart = &studio_model_->bodyparts[i];

        for (size_t j = 0; j < bodypart->models.size(); ++j)
        {
            model = bodypart->models[j];

            if (render_data_.model[i] == j)
            {
                for (size_t k = 0; k < model->meshes.size(); ++k)
                    meshes_to_render_.push_back(model->meshes[k]->index);
            }
        }
    }

    update_opaque_and_additive_textures();
}

void StudioModelRender::update_opaque_and_additive_textures()
{
    const Mesh* mesh = nullptr;
    const Texture* texture = nullptr;

    additive_meshes_.clear();
    opaque_meshes_.clear();

    for (auto mesh_index : meshes_to_render_)
    {
        mesh = &studio_model_->meshes[mesh_index];

        texture = mesh->texture;
        if (render_data_.skin > 0 && texture->skin_textures.size())
            texture = texture->skin_textures[render_data_.skin - 1];

        if (texture->blend_mode == aiBlendMode::aiBlendMode_Additive)
            additive_meshes_.push_back(mesh->index);
        else
            opaque_meshes_.push_back(mesh->index);
    }
}

void StudioModelRender::update_offset_matrices()
{
    size_t num_bones = std::min(studio_model_->bones.size(), static_cast<size_t>(MAXSTUDIOBONES));

    std::vector<glm::mat4> offset_bone_matrices(num_bones);
    for (size_t i = 0; i < offset_bone_matrices.size(); i++)
        offset_bone_matrices[i] = studio_model_->bones[i].offset_matrix;

    bone_offset_matrices_uniform_buffer_.bind();
    bone_offset_matrices_uniform_buffer_.set_data_unbinded_with_size(
        offsetof(BoneMatricesUniformBlock, bone_matrices),
        offset_bone_matrices.data(),
        offset_bone_matrices.size() * sizeof(glm::mat4));
    bone_offset_matrices_uniform_buffer_.unbind();
}

void StudioModelRender::set_model(int bodypart, int model)
{
    render_data_.model[bodypart] = model;

    update_meshes_to_render();
}

void StudioModelRender::set_skin(int value)
{
    render_data_.skin = value;

    update_opaque_and_additive_textures();
}

void StudioModelRender::set_render_mode(RenderMode render_mode)
{
    settings_.render_mode = render_mode;
}
void StudioModelRender::set_show_normals(bool enabled)
{
    settings_.draw_normals = enabled;
}
void StudioModelRender::set_show_bones(bool enabled)
{
    settings_.draw_bones = enabled;
}
void StudioModelRender::set_show_attachments(bool enabled)
{
    settings_.draw_attachments = enabled;
}
void StudioModelRender::set_show_hitboxes(bool enabled)
{
    settings_.draw_hitboxes = enabled;
}
void StudioModelRender::set_show_sequence_bbox(bool enabled)
{
    settings_.draw_sequence_bbox = enabled;
}
void StudioModelRender::set_highlight_models(bool enabled)
{
    settings_.highlight_models = enabled;
}
void StudioModelRender::set_draw_wireframe(bool enabled)
{
    settings_.render_wireframe = enabled;
}
void StudioModelRender::set_draw_chrome_effects(bool enabled)
{
    settings_.render_chrome_effects = enabled;
}
void StudioModelRender::set_lighting_enabled(bool enabled)
{
    settings_.lighting_enabled = enabled;

    global_uniform_buffer_.set_data(
        offsetof(GlobalUniformBlock, GlobalUniformBlock::lighting_enabled),
        settings_.lighting_enabled);
}

}
}
