/**
* \file hl1_studiomodel_animation.cpp
* \brief Implementation for the HL1 Studio model animation class.
*/

#include "pch.h"
#include "hl1_studiomodel_animation.h"
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace hl_mdlviewer {
namespace hl1 {

StudioModelAnimation::StudioModelAnimation(
    StudioModel* studio_model,
    AnimationEventHandler* animation_event_handler,
    FrameInterpolation* frame_interpolation) :
    studio_model_(studio_model),
    animation_event_handler_(animation_event_handler),
    frame_interpolation_(frame_interpolation),
    listeners_(),
    animation_data_()
{
    add_listener(animation_event_handler_);
}

void StudioModelAnimation::initialize()
{
}

void StudioModelAnimation::dispose()
{
}

void StudioModelAnimation::add_listener(SequenceListener* listener)
{
    listeners_.push_back(listener);
}

void StudioModelAnimation::reset()
{
    animation_data_.clear();
}

void StudioModelAnimation::on_model_changed()
{
    animation_data_.initialize(0, 0.0f,
        studio_model_->bone_controllers.size(),
        studio_model_->stats.num_blend_contollers);

    bones_transform_.resize(studio_model_->bones.size());
}

bool StudioModelAnimation::model_has_sequences() const
{
    return studio_model_->sequences.size();
}

void StudioModelAnimation::interpolate_bone_at_frame(
    const Bone* bone,
    int frame,
    float s,
    int blend,
    glm::vec3& result_position,
    glm::quat& result_orientation)
{
    const aiNodeAnim* pNodeAnim = studio_model_->sequences[animation_data_.sequence].blends[blend]->mChannels[bone->index];

    glm::vec3 vFrom = to_glm_vec3(pNodeAnim->mPositionKeys[frame].mValue);
    glm::quat qFrom = to_glm_quat(pNodeAnim->mRotationKeys[frame].mValue);
    glm::vec3 vTo = to_glm_vec3(pNodeAnim->mPositionKeys[frame + 1].mValue);
    glm::quat qTo = to_glm_quat(pNodeAnim->mRotationKeys[frame + 1].mValue);

    result_position = vFrom * (1.0f - s) + vTo * s;
    result_orientation = glm::slerp(qFrom, qTo, s);

    for (BoneController* bone_contoller : bone->bone_controllers)
        apply_bone_controller_transform(bone_contoller, result_position, result_orientation);
}

void StudioModelAnimation::apply_bone_controller_transform(
    const BoneController* bone_controller,
    glm::vec3& result_position,
    glm::quat& result_orientation)
{
    glm::vec3 angles = glm::eulerAngles(result_orientation);

    if (bone_controller->motion_type == MotionType::Rotation)
    {
        angles[bone_controller->motion_axis] += animation_data_.bone_controllers[bone_controller->index].adj_value;
    }
    else if (bone_controller->motion_type == MotionType::Position)
    {
        result_position[bone_controller->motion_axis] += animation_data_.bone_controllers[bone_controller->index].adj_value;
    }

    result_orientation = glm::quat(glm::vec3(angles.x, angles.y, angles.z));
}

void StudioModelAnimation::setup_animated_bone_transform(
    const Bone* bone, 
    const Sequence* sequence, 
    int frame, 
    float s, 
    glm::mat4& transform)
{
    glm::vec3 vInterpolated;
    glm::quat qInterpolated;
    interpolate_bone_at_frame(bone, frame, s, 0, vInterpolated, qInterpolated);

    if (sequence->blends.size() > 1)
    {
        float t1 = clamp(animation_data_.blend_controllers[0] / 255.0f, 0.0f, 1.0f);
        float t2 = 1.0f - t1;

        glm::vec3 vInterpolated2;
        glm::quat qInterpolated2;
        interpolate_bone_at_frame(bone, frame, s, 1, vInterpolated2, qInterpolated2);
        qInterpolated = glm::slerp(qInterpolated, qInterpolated2, t1);

        vInterpolated = vInterpolated * t2 + vInterpolated2 * t1;

        if (sequence->blends.size() == 4)
        {
            glm::vec3 vInterpolated3, vInterpolated4;
            glm::quat qInterpolated3, qInterpolated4;
            interpolate_bone_at_frame(bone, frame, s, 2, vInterpolated3, qInterpolated3);
            interpolate_bone_at_frame(bone, frame, s, 3, vInterpolated4, qInterpolated4);

            t1 = clamp(animation_data_.blend_controllers[0] / 255.0f, 0.0f, 1.0f);
            t2 = 1.0f - t1;

            qInterpolated3 = glm::slerp(qInterpolated3, qInterpolated4, t1);

            vInterpolated3 = vInterpolated3 * t2 + vInterpolated4 * t1;

            t1 = clamp(animation_data_.blend_controllers[1] / 255.0f, 0.0f, 1.0f);
            t2 = 1.0f - t1;

            qInterpolated = glm::slerp(qInterpolated, qInterpolated3, t1);

            vInterpolated = vInterpolated * t2 + vInterpolated3 * t1;
        }
    }

    transform = glm::mat4_cast(qInterpolated);
    transform[3][0] = vInterpolated.x;
    transform[3][1] = vInterpolated.y;
    transform[3][2] = vInterpolated.z;
}

void StudioModelAnimation::advance_frame(const Sequence* sequence, const float frame_time)
{
    bool sequence_finished = false;
    animation_data_.frame = frame_interpolation_->advance_frame(
        sequence,
        animation_data_.frame,
        animation_data_.playback_rate,
        frame_time,
        &sequence_finished);

    if (sequence_finished)
    {
        for (auto listener : listeners_)
            listener->on_sequence_finished(sequence);
    }
}

void StudioModelAnimation::process_animation_events(const Sequence* sequence, int frame)
{
    animation_event_handler_->process_events(sequence, frame);
}

void StudioModelAnimation::setup_bind_pose_bone_transform(const Bone* bone, glm::mat4& transform)
{
    glm::quat local_quat(bone->local_quat);
    glm::vec3 local_position(bone->local_position);

    for (BoneController* bone_contoller : bone->bone_controllers)
        apply_bone_controller_transform(bone_contoller, local_position, local_quat);

    transform = glm::mat4_cast(local_quat);
    transform[3][0] = local_position.x;
    transform[3][1] = local_position.y;
    transform[3][2] = local_position.z;
}

void StudioModelAnimation::apply_bone_parent_transform(const Bone* bone, glm::mat4& transform)
{
    transform = bones_transform_[bone->parent_index] * transform;
}

void StudioModelAnimation::update(const float frame_time)
{
    if (model_has_sequences())
    {
        const Sequence* sequence = &studio_model_->sequences[animation_data_.sequence];

        const unsigned int num_frames = sequence->num_frames;

        if (num_frames <= 1)
            animation_data_.frame = 0;

        int iFrame = (int)animation_data_.frame;

        float s = animation_data_.frame - iFrame;

        for (auto& bone : studio_model_->bones)
        {
            setup_animated_bone_transform(&bone, sequence, iFrame, s, bones_transform_[bone.index]);
            if (bone.parent)
                apply_bone_parent_transform(&bone, bones_transform_[bone.index]);
        }

        advance_frame(sequence, frame_time);

        if (sequence->events.size())
            process_animation_events(sequence, iFrame);
    }
    else
    {
        for (auto& bone : studio_model_->bones)
        {
            setup_bind_pose_bone_transform(&bone, bones_transform_[bone.index]);
            if (bone.parent)
                apply_bone_parent_transform(&bone, bones_transform_[bone.index]);
        }
    }
}

void StudioModelAnimation::set_sequence(int value)
{
    const int old_sequence = animation_data_.sequence;

    animation_data_.sequence = value;
    animation_data_.frame = 0;

    for (auto listener : listeners_) 
    {
        listener->on_change_sequence(
            &studio_model_->sequences[old_sequence],
            &studio_model_->sequences[value]);
    }

}

void StudioModelAnimation::set_playback_rate(float value)
{
    animation_data_.playback_rate = value;
}

void StudioModelAnimation::set_frame(float value)
{
    animation_data_.frame = value;
}

void StudioModelAnimation::set_blending(int index, uint8_t value)
{
    animation_data_.blend_controllers[index] = value;
}

void StudioModelAnimation::set_bone_controller(int index, float value)
{
    animation_data_.bone_controllers[index].set_value(
        &studio_model_->bone_controllers[index],
        value);
}

}
}
