/**
* \file hl1_model_render_settings.h
*/

#ifndef HLMDLVIEWER_HL1_MODEL_RENDER_SETTINGS_H_
#define HLMDLVIEWER_HL1_MODEL_RENDER_SETTINGS_H_

#include <glm/glm.hpp>

namespace hl_mdlviewer {
namespace hl1 {

enum RenderMode
{
    SMOOTH,
    TEXTURED,
    FIRST_RENDER_MODE = SMOOTH,
    LAST_RENDER_MODE = TEXTURED,
    COUNT // Must be last.
};

struct ModelRenderSettings
{
    ModelRenderSettings() :
        render_mode(RenderMode::TEXTURED),
        highlight_models(false),
        draw_normals(false),
        draw_bones(false),
        draw_attachments(false),
        draw_hitboxes(false),
        draw_sequence_bbox(false),
        render_wireframe(false),
        render_chrome_effects(false),
        lighting_enabled(false),
        attachment_color(glm::vec4(1,1,1,1)),
        bone_segment_color(glm::vec4(1,1,0,1)),
        bone_vertex_color(glm::vec4(1,0,0,1)),
        flat_color(glm::vec4(1,1,1,1)),
        smooth_color(flat_color),
        normal_color(glm::vec4(1,1,0,1)),
        wireframe_color(glm::vec4(0,0,0,1))
    {
    }

    RenderMode render_mode;
    bool highlight_models;
    bool draw_normals;
    bool draw_bones;
    bool draw_attachments;
    bool draw_hitboxes;
    bool draw_sequence_bbox;
    bool render_wireframe;
    bool render_chrome_effects;
    bool lighting_enabled;
    glm::vec4 attachment_color;
    glm::vec4 bone_segment_color;
    glm::vec4 bone_vertex_color;
    glm::vec4 flat_color;
    glm::vec4 smooth_color;
    glm::vec4 normal_color;
    glm::vec4 wireframe_color;
};

}
}

#endif // HLMDLVIEWER_HL1_MODEL_RENDER_SETTINGS_H_
