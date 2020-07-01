/**
* \file hl1_studiomodel_setup.cpp
* \brief Implementation for the HL1 Studio model setup class.
*/

#include "pch.h"
#include "hl1_studiomodel_setup.h"
#include "glvertex.h"
#include "bbox_builder.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../code/AssetLib/MDL/HalfLife/HL1ImportDefinitions.h"

#include <fstream>
#include <sstream>
#include <glm/gtx/matrix_decompose.hpp>

namespace hl_mdlviewer { 
namespace hl1 { 

StudioModelSetup::StudioModelSetup() :
    studio_model_(nullptr),
    studio_model_buffer_(nullptr),
    scene_(nullptr),
    bone_map_(),
    scene_bones_(nullptr),
    buffer_builder_()
{
}

void StudioModelSetup::setup_model(
    const aiScene* scene,
    StudioModel* studio_model,
    StudioModelBuffer* studio_model_buffer,
    glm::mat4& scene_transform)
{
    scene_ = scene;
    studio_model_ = studio_model;
    studio_model_buffer_ = studio_model_buffer;
    bone_map_.clear();

    scene_transform = to_glm_mat4(scene_->mRootNode->mTransformation);

    scene_bones_ = scene_->mRootNode->FindNode(AI_MDL_HL1_NODE_BONES);
    scene_global_info_ = scene_->mRootNode->FindNode(AI_MDL_HL1_NODE_GLOBAL_INFO);

    setup_model_stats();

    setup_model_data();

    setup_model_buffers();

    studio_model_buffer_->buffer.initialize(
        buffer_builder_.get_vertices(),
        buffer_builder_.get_indices());
}

void StudioModelSetup::setup_model_data()
{
    setup_bones();
    setup_bone_controllers();
    setup_sequences();
    setup_textures();
    setup_skins();
    setup_attachments();
    setup_hitboxes();
    setup_meshes();
}
void StudioModelSetup::setup_model_buffers()
{
    setup_buffer_meshes();
    setup_buffer_bones();
    setup_buffer_attachments();
    setup_buffer_hitboxes();
    setup_buffer_sequence_bbox();
    setup_buffer_gltextures();
}

void StudioModelSetup::setup_bones()
{
    if (!scene_bones_)
        return;

    for (unsigned int i = 0; i < scene_bones_->mNumChildren; ++i)
    {
        bone_map_.insert_or_assign(scene_bones_->mChildren[i]->mName.C_Str(), static_cast<int>(i));
    }

    // Collect mOffsetMatrix (Inverse bind pose matrix) of each bone
    // that affect a specific vertex.
    std::vector<aiMatrix4x4> offset_matrices(scene_bones_->mNumChildren);
    std::vector<bool> offset_matrices_collected(scene_bones_->mNumChildren);

    for (unsigned int i = 0; i < scene_->mNumMeshes; ++i)
    {
        const aiMesh* scene_mesh = scene_->mMeshes[i];
        for (unsigned int j = 0; j < scene_mesh->mNumBones; ++j)
        {
            const aiBone* scene_mesh_bone = scene_mesh->mBones[j];
            int boneid = bone_map_[scene_mesh_bone->mName.C_Str()];
            if (!offset_matrices_collected[boneid])
            {
                offset_matrices_collected[boneid] = true;
                offset_matrices[boneid] = scene_mesh_bone->mOffsetMatrix;
            }
        }
    }

    studio_model_->bones.resize(scene_bones_->mNumChildren);

    for (unsigned int i = 0; i < scene_bones_->mNumChildren; ++i)
    {
        const aiNode* scene_bone = scene_bones_->mChildren[i];

        Bone* studio_bone = &studio_model_->bones[i];
        studio_bone->index = static_cast<int>(i);
        studio_bone->name = scene_bone->mName.C_Str();

        if (scene_bone->mParent != scene_->mRootNode)
        {
            studio_bone->parent_index = bone_map_[scene_bone->mParent->mName.C_Str()];
            studio_bone->parent = &studio_model_->bones[studio_bone->parent_index];
            studio_model_->bones[studio_bone->parent_index].children.push_back(studio_bone);
        }
        else
        {
            studio_bone->parent_index = -1;
            studio_bone->parent = nullptr;
        }

        // Store local space position and orientation.
        aiVector3D local_position;
        aiQuaternion local_orientation;
        scene_bone->mTransformation.DecomposeNoScaling(local_orientation, local_position);
        studio_bone->local_quat = to_glm_quat(local_orientation);
        studio_bone->local_position = to_glm_vec3(local_position);
        studio_bone->offset_matrix = to_glm_mat4(offset_matrices[studio_bone->index]);
    }
}

void StudioModelSetup::setup_bone_controllers()
{
    const aiNode* const scene_bone_controllers = scene_->mRootNode->FindNode(AI_MDL_HL1_NODE_BONE_CONTROLLERS);
    if (!scene_bone_controllers)
        return;

    studio_model_->bone_controllers.resize(scene_bone_controllers->mNumChildren);

    for (unsigned int i = 0; i < scene_bone_controllers->mNumChildren; ++i)
    {
        const aiNode* scene_bone_controller = scene_bone_controllers->mChildren[i];
        BoneController* studio_bone_controller = &studio_model_->bone_controllers[i];

        studio_bone_controller->index = static_cast<int>(i);

        int original_bone_controller_channel = -1;
        scene_bone_controller->mMetaData->Get("Channel", original_bone_controller_channel);
        studio_bone_controller->is_mouth = original_bone_controller_channel == 4;

        aiString bone_name;
        scene_bone_controller->mMetaData->Get("Bone", bone_name);
        studio_bone_controller->bone_index = bone_map_[bone_name.C_Str()];
        studio_bone_controller->bone = &studio_model_->bones[studio_bone_controller->bone_index];
        studio_bone_controller->bone->bone_controllers.push_back(studio_bone_controller);

        int motion_flags;
        scene_bone_controller->mMetaData->Get("MotionFlags", motion_flags);
        studio_bone_controller->motion_type = motion_flags_to_motion_type(motion_flags);
        studio_bone_controller->motion_axis = motion_type_to_motion_axis(motion_flags);

        studio_bone_controller->wraps = motion_flags & STUDIO_RLOOP ? true : false;

        scene_bone_controller->mMetaData->Get("Start", studio_bone_controller->start);
        scene_bone_controller->mMetaData->Get("End", studio_bone_controller->end);
    }
}

void add_bone_and_children_bones_to_set(Bone* bone, std::set<int>& bones_set)
{
    bones_set.insert(bone->index);

    for (auto it = bone->children.begin(); it != bone->children.end(); ++it)
        add_bone_and_children_bones_to_set(*it, bones_set);
}

void StudioModelSetup::setup_sequences()
{
    if (scene_->mNumAnimations == 0)
        return;

    const aiNode* const scene_sequences_infos = scene_->mRootNode->FindNode(AI_MDL_HL1_NODE_SEQUENCE_INFOS);
    if (!scene_sequences_infos)
        return;

    studio_model_->sequences.resize(scene_sequences_infos->mNumChildren);

    for (unsigned int i = 0; i < scene_sequences_infos->mNumChildren; ++i)
    {
        aiNode* scene_sequence_info = scene_sequences_infos->mChildren[i];
        Sequence* studio_sequence = &studio_model_->sequences[i];

        studio_sequence->index = i;
        studio_sequence->name = scene_sequence_info->mName.C_Str();

        scene_sequence_info->mMetaData->Get("FramesPerSecond", studio_sequence->fps);
        scene_sequence_info->mMetaData->Get("NumFrames", studio_sequence->num_frames);

        aiVector3D bbmin, bbmax;
        scene_sequence_info->mMetaData->Get("BBMin", bbmin);
        scene_sequence_info->mMetaData->Get("BBMax", bbmax);

        studio_sequence->bbmin = to_glm_vec3(bbmin);
        studio_sequence->bbmax = to_glm_vec3(bbmax);

        int numblends;
        scene_sequence_info->mMetaData->Get("NumBlends", numblends);
        studio_sequence->blends.resize(numblends);

        int animation_index = 0;
        scene_sequence_info->mMetaData->Get("AnimationIndex", animation_index);
        for (int j = 0; j < numblends; ++j)
            studio_sequence->blends[j] = scene_->mAnimations[animation_index + j];

        const aiNode* sequence_info_events = scene_sequence_info->FindNode(AI_MDL_HL1_NODE_ANIMATION_EVENTS);
        if (sequence_info_events != nullptr)
        {
            studio_sequence->events.resize(sequence_info_events->mNumChildren);

            for (unsigned int j = 0; j < sequence_info_events->mNumChildren; ++j)
            {
                const aiNode* pEventNode = sequence_info_events->mChildren[j];
                AnimationEvent* studio_event = &studio_sequence->events[j];
                pEventNode->mMetaData->Get("Frame", studio_event->frame);
                pEventNode->mMetaData->Get("ScriptEvent", studio_event->event);

                aiString options;
                pEventNode->mMetaData->Get("Options", options);
                studio_event->options = options.C_Str();
            }
        }
    }
}

void StudioModelSetup::setup_textures()
{
    if (scene_->mNumMaterials == 0)
        return;

    studio_model_->textures.resize(scene_->mNumMaterials);

    const aiMaterial* scene_material = NULL;
    Texture* studio_texture = NULL;

    for (unsigned int i = 0; i < scene_->mNumMaterials; ++i)
    {
        scene_material = scene_->mMaterials[i];
        studio_texture = &studio_model_->textures[i];

        studio_texture->index = i;
        scene_material->Get(AI_MATKEY_SHADING_MODEL, studio_texture->shading_mode);

        scene_material->Get(AI_MATKEY_BLEND_FUNC, studio_texture->blend_mode);

        int chrome;
        if (AI_SUCCESS == scene_material->Get(AI_MDL_HL1_MATKEY_CHROME(aiTextureType_DIFFUSE, 0), chrome))
        {
            studio_texture->type = chrome
                ? Texture::Type::Chrome
                : Texture::Type::Default;
        }

        if (AI_SUCCESS == scene_material->Get(AI_MATKEY_TEXFLAGS_DIFFUSE(0), studio_texture->flags))
        {
            if (studio_texture->flags & aiTextureFlags_UseAlpha)
            {
                aiColor3D mask_color;
                if (AI_SUCCESS == scene_material->Get(AI_MATKEY_COLOR_TRANSPARENT, mask_color))
                {
                    // Ensure that the transparency color uses the same convention
                    // as texture colors.
                    mask_color.r /= 255.0f;
                    mask_color.g /= 255.0f;
                    mask_color.b /= 255.0f;
                    studio_texture->mask_color = to_glm_vec3(mask_color);
                }
            }
        }
    }
}

void StudioModelSetup::setup_skins()
{
    const aiTexture* scene_texture = NULL;
    const aiMaterial* scene_material = NULL;

    for (unsigned int i = 0; i < scene_->mNumMaterials; ++i)
    {
        scene_material = scene_->mMaterials[i];
        const unsigned int num_material_textures = scene_material->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE);

        // Start at first skin.
        for (unsigned int j = 1; j < num_material_textures; ++j)
        {
            aiString path;
            scene_material->GetTexture(aiTextureType::aiTextureType_DIFFUSE, j, &path);

            for (unsigned int k = 0; k < scene_->mNumTextures; ++k)
            {
                scene_texture = scene_->mTextures[k];

                if (path == scene_texture->mFilename)
                {
                    studio_model_->textures[i].skin_textures.push_back(&studio_model_->textures[k]);
                    break;
                }
            }
        }
    }
}

void StudioModelSetup::setup_attachments()
{
    const aiNode* const scene_attachments = scene_->mRootNode->FindNode(AI_MDL_HL1_NODE_ATTACHMENTS);
    if (!scene_attachments)
        return;

    studio_model_->attachments.resize(scene_attachments->mNumChildren);
    for (unsigned int i = 0; i < scene_attachments->mNumChildren; ++i)
    {
        const aiNode* scene_attachment = scene_attachments->mChildren[i];
        Attachment* studio_attachment = &studio_model_->attachments[i];
        studio_attachment->index = static_cast<int>(i);

        aiVector3D origin;
        scene_attachment->mMetaData->Get("Position", origin);
        studio_attachment->position = to_glm_vec3(origin);

        aiString bone_name;
        scene_attachment->mMetaData->Get("Bone", bone_name);
        studio_attachment->bone = &studio_model_->bones[bone_map_[bone_name.C_Str()]];
    }
}

void StudioModelSetup::setup_hitboxes()
{
    const aiNode* const scene_hitboxes = scene_->mRootNode->FindNode(AI_MDL_HL1_NODE_HITBOXES);
    if (!scene_hitboxes)
        return;

    studio_model_->hitboxes.resize(scene_hitboxes->mNumChildren);
    for (unsigned int i = 0; i < scene_hitboxes->mNumChildren; ++i)
    {
        const aiNode* scene_hitbox = scene_hitboxes->mChildren[i];
        Hitbox* studio_hitbox = &studio_model_->hitboxes[i];
        studio_hitbox->index = static_cast<int>(i);

        aiString bone_name;
        scene_hitbox->mMetaData->Get("Bone", bone_name);
        studio_hitbox->bone = &studio_model_->bones[bone_map_[bone_name.C_Str()]];

        scene_hitbox->mMetaData->Get("HitGroup", studio_hitbox->group);

        aiVector3D bbmin, bbmax;
        scene_hitbox->mMetaData->Get("BBMin", bbmin);
        scene_hitbox->mMetaData->Get("BBMax", bbmax);
        studio_hitbox->bbmin = to_glm_vec3(bbmin);
        studio_hitbox->bbmax = to_glm_vec3(bbmax);
    }
}

void StudioModelSetup::setup_meshes()
{
    const aiNode* const scene_bodyparts = scene_->mRootNode->FindNode(AI_MDL_HL1_NODE_BODYPARTS);
    if (!scene_bodyparts)
        return;

    unsigned int num_total_bodygroups = scene_bodyparts->mNumChildren;
    unsigned int num_total_models = 0;
    unsigned int num_total_meshes = scene_->mNumMeshes;

    for (unsigned int i = 0; i < scene_bodyparts->mNumChildren; ++i)
    {
        const auto* scene_bodypart = scene_bodyparts->mChildren[i];
        num_total_models += scene_bodypart->mNumChildren;
    }

    studio_model_->bodyparts.resize(num_total_bodygroups);
    studio_model_->models.resize(num_total_models);
    studio_model_->meshes.resize(num_total_meshes);

    unsigned int model_index = 0;

    unsigned int vertex_index = 0;
    unsigned int indice_index = 0;

    for (unsigned int i = 0; i < scene_bodyparts->mNumChildren; ++i)
    {
        const auto* scene_bodypart = scene_bodyparts->mChildren[i];
        Bodypart* studio_bodypart = &studio_model_->bodyparts[i];
        studio_bodypart->index = i;
        studio_bodypart->models.resize(scene_bodypart->mNumChildren);
        studio_bodypart->name = scene_bodyparts->mName.C_Str();

        for (unsigned int j = 0; j < scene_bodypart->mNumChildren; ++j, ++model_index)
        {
            const auto* scene_model = scene_bodypart->mChildren[j];
            Model* studio_model = &studio_model_->models[model_index];

            studio_bodypart->models[j] = studio_model;

            studio_model->index = model_index;
            studio_model->bodypart = studio_bodypart;
            studio_model->name = scene_model->mName.C_Str();
            studio_model->meshes.resize(scene_model->mNumMeshes);

            for (unsigned int k = 0; k < scene_model->mNumMeshes; ++k)
            {
                const unsigned int mesh_index = scene_model->mMeshes[k];
                aiMesh* scene_mesh = scene_->mMeshes[mesh_index];
                Mesh* studio_mesh = &studio_model_->meshes[mesh_index];

                studio_model->meshes[k] = studio_mesh;

                studio_mesh->index = mesh_index;
                studio_mesh->model = studio_model;
                studio_mesh->texture = &studio_model_->textures[scene_mesh->mMaterialIndex];
            }
        }
    }
}

void StudioModelSetup::setup_buffer_gltextures()
{
    studio_model_buffer_->gltextures.resize(scene_->mNumTextures);

    const aiTexture* scene_texture = NULL;

    for (unsigned int i = 0; i < scene_->mNumTextures; ++i)
    {
        scene_texture = scene_->mTextures[i];
        studio_model_buffer_->gltextures[i].create_from_data(
            scene_texture->mWidth,
            scene_texture->mHeight,
            GL_RGBA,
            GL_BGRA,
            (unsigned char*)scene_texture->pcData);
    }
}

void StudioModelSetup::setup_buffer_meshes()
{
    const aiNode* const scene_bodyparts = scene_->mRootNode->FindNode(AI_MDL_HL1_NODE_BODYPARTS);
    if (!scene_bodyparts)
        return;

    unsigned int num_total_vertices = 0;
    unsigned int num_total_indices = 0;

    for (unsigned int i = 0; i < scene_->mNumMeshes; ++i)
    {
        num_total_vertices += scene_->mMeshes[i]->mNumVertices;

        for (unsigned int j = 0; j < scene_->mMeshes[i]->mNumFaces; ++j)
            num_total_indices += scene_->mMeshes[i]->mFaces[j].mNumIndices;
    }

    studio_model_buffer_->meshes.resize(studio_model_->meshes.size());
    buffer_builder_.reserve(num_total_vertices, num_total_indices);

    std::vector<glvertex> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < studio_model_->meshes.size(); ++i)
    {
        const Mesh* studio_mesh = &studio_model_->meshes[i];
        const aiMesh* scene_mesh = scene_->mMeshes[studio_mesh->index];

        unsigned int num_indices = 0;
        for (unsigned int f = 0; f < scene_mesh->mNumFaces; ++f)
            num_indices += scene_mesh->mFaces[f].mNumIndices;

        vertices.resize(scene_mesh->mNumVertices);
        indices.resize(num_indices);

        for (unsigned int v = 0; v < scene_mesh->mNumVertices; ++v)
        {
            vertices[v].position = to_glm_vec3(scene_mesh->mVertices[v]);
            vertices[v].normal = to_glm_vec3(scene_mesh->mNormals[v]);
            vertices[v].uv = to_glm_vec2(scene_mesh->mTextureCoords[0][v]);
        }

        size_t indice_index = 0;
        for (unsigned int f = 0; f < scene_mesh->mNumFaces; ++f)
        {
            const aiFace* pFace = &scene_mesh->mFaces[f];
            for (unsigned int fi = 0; fi < pFace->mNumIndices;
                ++fi, ++indice_index)
            {
                indices[indice_index] = pFace->mIndices[fi];
            }
        }

        for (unsigned int b = 0; b < scene_mesh->mNumBones; ++b)
        {
            const aiBone* scene_bone = scene_mesh->mBones[b];
            for (unsigned int w = 0; w < scene_bone->mNumWeights; ++w)
            {
                unsigned int vertex_id = scene_bone->mWeights[w].mVertexId;
                vertices[vertex_id].boneid = bone_map_[scene_bone->mName.C_Str()];
            }
        }

        buffer_builder_.append(
            vertices, indices, 
            PRIMITIVE_RESTART_INDEX,
            studio_model_buffer_->meshes[i]);
    }
}

void StudioModelSetup::setup_buffer_bones()
{
    std::vector<glvertex> vertices(studio_model_->bones.size());

    for (size_t i = 0; i < studio_model_->bones.size(); ++i)
    {
        const Bone* bone = &studio_model_->bones[i];
        vertices[i].boneid = bone->index;
    }

    std::vector<unsigned int> indices;
    build_bone_segments(indices);

    buffer_builder_.append(vertices, indices,
        PRIMITIVE_RESTART_INDEX,
        studio_model_buffer_->bones);
}

void StudioModelSetup::build_bone_segments(std::vector<unsigned int>& bone_segments)
{
    bone_segments.reserve(studio_model_->bones.size());

    int parent_index = studio_model_->bones[0].parent_index;
    int segment_index = 0;

    for (size_t i = 0; i < studio_model_->bones.size(); ++i)
    {
        const Bone* bone = &studio_model_->bones[i];

        if (bone->parent_index == parent_index)
        {
            bone_segments.push_back(bone->index);
        }
        else
        {
            bone_segments.push_back(PRIMITIVE_RESTART_INDEX);
            bone_segments.push_back(bone->parent_index);
            bone_segments.push_back(bone->index);
        }
        parent_index = bone->index;
    }
}

void StudioModelSetup::setup_buffer_attachments()
{
    std::vector<glvertex> vertices(studio_model_->attachments.size());
    std::vector<unsigned int> indices(studio_model_->attachments.size());
    std::iota(indices.begin(), indices.end(), 0);

    Attachment* studio_attachment = nullptr;
    for (size_t i = 0; i < studio_model_->attachments.size(); ++i)
    {
        studio_attachment = &studio_model_->attachments[i];
        vertices[i].position = studio_attachment->position;
        vertices[i].boneid = studio_attachment->bone->index;
    }

    buffer_builder_.append(vertices, indices,
        PRIMITIVE_RESTART_INDEX,
        studio_model_buffer_->attachments);
}

void StudioModelSetup::setup_buffer_hitboxes()
{
    BBoxBuilder bbox_builder;

    std::vector<glvertex> vertices;
    std::vector<unsigned int> indices;

    studio_model_buffer_->hitboxes.resize(studio_model_->hitboxes.size());

    for (auto it = studio_model_->hitboxes.cbegin(); it != studio_model_->hitboxes.cend(); ++it)
    {
        bbox_builder.build_line_strip(
            it->bbmin,
            it->bbmax,
            PRIMITIVE_RESTART_INDEX,
            it->bone->index);

        buffer_builder_.append(
            bbox_builder.get_vertices(),
            bbox_builder.get_indices(),
            PRIMITIVE_RESTART_INDEX,
            studio_model_buffer_->hitboxes[it->index]);
    }
}

void StudioModelSetup::setup_model_stats()
{
    if (!scene_global_info_)
        return;

    get_global_info_value<int>("NumBodyparts", studio_model_->stats.num_bodyparts);
    get_global_info_value<int>("NumModels", studio_model_->stats.num_models);
    get_global_info_value<int>("NumBones", studio_model_->stats.num_bones);
    get_global_info_value<int>("NumAttachments", studio_model_->stats.num_attachments);
    get_global_info_value<int>("NumSkinFamilies", studio_model_->stats.num_skin_families);
    get_global_info_value<int>("NumHitboxes", studio_model_->stats.num_hitboxes);
    get_global_info_value<int>("NumBoneControllers", studio_model_->stats.num_bone_controllers);
    get_global_info_value<int>("NumSequences", studio_model_->stats.num_sequences);
    get_global_info_value<int>("NumBlendControllers", studio_model_->stats.num_blend_contollers);
    studio_model_->stats.num_textures = static_cast<decltype(studio_model_->stats.num_textures)>(scene_->mNumTextures);
}

void StudioModelSetup::setup_buffer_sequence_bbox()
{
    BBoxBuilder bbox_builder;

    studio_model_buffer_->sequence_bbox.resize(2);

    bbox_builder.build_triangle_fan(
        glm::vec3(0, 0, 0),
        glm::vec3(0, 0, 0),
        PRIMITIVE_RESTART_INDEX,
        0);

    buffer_builder_.append(
        bbox_builder.get_vertices(),
        bbox_builder.get_indices(),
        PRIMITIVE_RESTART_INDEX,
        studio_model_buffer_->sequence_bbox[0]);

    // Build the outline for the bbox, but reuse the same vertices
    // from the first sequence_bbox buffer entry.
    bbox_builder.build_line_strip_indices(PRIMITIVE_RESTART_INDEX);

    buffer_builder_.append_indices(
        studio_model_buffer_->sequence_bbox[0],
        bbox_builder.get_indices(),
        PRIMITIVE_RESTART_INDEX,
        studio_model_buffer_->sequence_bbox[1]);
}

}
}
