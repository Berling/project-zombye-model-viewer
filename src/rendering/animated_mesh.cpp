#include <stack>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>

#include <glcw/texture.hpp>

#include <core/engine.hpp>
#include <rendering/animated_mesh.hpp>
#include <rendering/renderer.hpp>
#include <rendering/skinned_mesh.hpp>
#include <rendering/skeleton.hpp>

#include <glm/gtx/string_cast.hpp>

namespace rendering {
    animated_mesh::animated_mesh(core::engine& engine, renderer& renderer, const std::string& mesh, const std::string& skeleton)
    : engine_{engine}, renderer_{renderer}, current_state_{""}, elapsed_time_{0.f}, blend_{false}, blend_next_{false} {
        load(mesh);
        load_skeleton(skeleton);
    }

    animated_mesh::~animated_mesh() noexcept {}

    void animated_mesh::update(float delta_time) {
        static std::vector<glm::mat4> pose(skeleton_->bones().size(), glm::mat4{1.f});
        static const float fps = 1.f / 24.f;
        pose.assign(skeleton_->bones().size(), glm::mat4{1.f});

        if (current_state_ != "") {
            auto& bones = skeleton_->bones();
            auto& bone_hierachy = skeleton_->bone_hierachy_;

            elapsed_time_ += delta_time;

            if (blend_ && next_state_ != "") {
                auto& animation = skeleton_->animation(current_state_);
                auto length = animation.length * fps;
                auto& next_animation = skeleton_->animation(next_state_);
                auto next_animation_length = next_animation.length * fps;
                auto delta_factor = next_animation_length <= length ? 1.f / next_animation_length : 1.f / length;

                if (elapsed_time_ <= next_animation_length) {
                    if (elapsed_time_ >= length) {
                        current_state_ = next_state_;
                        next_state_ = "";
                        blend_ = false;
                        current_keyframes_ = next_keyframes_;
                        return;
                    } else {
                        std::stack<int> traversal;
                        traversal.push(0);
                        while(!traversal.empty()) {
                            auto i = traversal.top();
                            traversal.pop();

                            auto& cs_track = animation.tracks.at(i);

                            auto& cs_current_frame = current_keyframes_.at(i);
                            auto cs_next_frame = cs_current_frame;
                            ++cs_next_frame.tkey;
                            ++cs_next_frame.qkey;
                            ++cs_next_frame.skey;

                            auto iv_current = interpolate_translation_keyframe(cs_track, cs_current_frame, cs_next_frame, fps);
                            auto iq_current = interpolate_quaternion_keyframe(cs_track, cs_current_frame, cs_next_frame, fps);

                            auto& ns_track = next_animation.tracks.at(i);

                            auto& ns_current_frame = next_keyframes_.at(i);
                            auto ns_next_frame = ns_current_frame;
                            ++ns_next_frame.tkey;
                            ++ns_next_frame.qkey;
                            ++ns_next_frame.skey;

                            auto iv_next = interpolate_translation_keyframe(ns_track, ns_current_frame, ns_next_frame, fps);
                            auto iq_next = interpolate_quaternion_keyframe(ns_track, ns_current_frame, ns_next_frame, fps);

                            auto delta = elapsed_time_ * delta_factor;
                            delta = delta > 1.f ? 1.f : delta;
                            auto iv = glm::lerp(iv_current, iv_next, delta);
                            auto iq = glm::lerp(iq_current, iq_next, delta);

                            auto p = glm::toMat4(iq);
                            p[3].x = iv.x;
                            p[3].y = iv.y;
                            p[3].z = iv.z;

                            pose[i] *= bones.at(i).relative_transform * p;
                            if (bone_hierachy.find(i) != bone_hierachy.end()) {
                                for (auto node = bone_hierachy.begin(i); node != bone_hierachy.end(i); ++node) {
                                    auto& child = node->second;
                                    pose[child] *= pose[i];
                                    traversal.push(child);
                                }
                            }
                        }
                    }
                } else {
                    elapsed_time_ = 0.f;
                    current_keyframes_.assign(current_keyframes_.size(), frame{});
                    next_keyframes_.assign(next_keyframes_.size(), frame{});
                    current_state_ = next_state_;
                    next_state_ = "";
                    blend_ = false;
                    return;
                }
            } else {
                auto& animation = skeleton_->animation(current_state_);
                auto length = animation.length * fps;
                if (elapsed_time_ <= length) {
                    std::stack<int> traversal;
                    traversal.push(0);
                    while(!traversal.empty()) {
                        auto i = traversal.top();
                        traversal.pop();

                        auto& track = animation.tracks.at(i);

                        auto& current_frame = current_keyframes_.at(i);
                        auto next_frame = current_frame;
                        ++next_frame.tkey;
                        ++next_frame.qkey;
                        ++next_frame.skey;

                        auto iv = interpolate_translation_keyframe(track, current_frame, next_frame, fps);
                        auto iq = interpolate_quaternion_keyframe(track, current_frame, next_frame, fps);

                        auto p = glm::toMat4(iq);
                        p[3].x = iv.x;
                        p[3].y = iv.y;
                        p[3].z = iv.z;

                        pose[i] *= bones.at(i).relative_transform * p;
                        if (bone_hierachy.find(i) != bone_hierachy.end()) {
                            for (auto node = bone_hierachy.begin(i); node != bone_hierachy.end(i); ++node) {
                                auto& child = node->second;
                                pose[child] *= pose[i];
                                traversal.push(child);
                            }
                        }
                    }
                } else {
                    elapsed_time_ = 0.f;
                    for (auto& k : current_keyframes_) {
                        k.tkey = 0;
                        k.qkey = 0;
                        k.skey = 0;
                    }
                    if (blend_next_) {
                        blend_ = true;
                        blend_next_ = false;
                    }
                    return;
                }
            }
        } else {
            auto& bones = skeleton_->bones();
            for (auto i = 0u; i < bones.size(); ++i) {
                pose[i] = glm::inverse(bones.at(i).absolute_transform);
            }
        }

        auto& bones = skeleton_->bones();
        for (auto i = 0; i < pose_.size(); ++i) {
            pose_[i] = pose[i] * bones.at(i).absolute_transform;
        }
    }

    void animated_mesh::draw() const noexcept {
        mesh_->draw();
    }

    void animated_mesh::load(const std::string& mesh) {
        mesh_ = renderer_.skinned_mesh_manager().load(mesh);
        if (!mesh_) {
            throw std::runtime_error{"could not load mesh from file " + mesh};
        }
    }

    void animated_mesh::load_skeleton(const std::string& skeleton) {
        skeleton_ = renderer_.skeleton_manager().load(skeleton);
        if (!skeleton_) {
            throw std::runtime_error{"could not load skeleton " + skeleton};
        }

        auto& bones = skeleton_->bones();
        for (auto i = 0u; i < bones.size(); ++i) {
            pose_.emplace_back(bones[i].absolute_transform);
        }

        current_keyframes_.resize(bones.size());
        next_keyframes_.resize(bones.size());
    }

    glm::vec3 animated_mesh::interpolate_translation_keyframe(const track& track, frame& current_frame, frame& next_frame, float fps) {
        if (track.tkeys.size() != 1) {
            auto t1 = track.tkeys[current_frame.tkey].time * fps;
            auto t2 = track.tkeys[next_frame.tkey].time * fps;
            if (elapsed_time_ >= t2 && next_frame.tkey < track.tkeys.size() - 1) {
                ++current_frame.tkey;
                ++next_frame.tkey;
                t1 = track.tkeys[current_frame.tkey].time * fps;
                t2 = track.tkeys[next_frame.tkey].time * fps;
            }

            auto v1 = track.tkeys[current_frame.tkey].translate;
            auto v2 = track.tkeys[next_frame.tkey].translate;

            auto delta = (elapsed_time_ - t1) / (t2 - t1);
            delta = delta > 1.f ? 1.f : delta;

            return glm::lerp(v1, v2, delta);
        } else {
            return track.tkeys[current_frame.tkey].translate;
        }
    }

    glm::quat animated_mesh::interpolate_quaternion_keyframe(const track& track, frame& current_frame, frame& next_frame, float fps) {
        if (track.qkeys.size() != 1) {
            auto t1 = track.qkeys[current_frame.qkey].time * fps;
            auto t2 = track.qkeys[next_frame.qkey].time * fps;
            if (elapsed_time_ >= t2 && next_frame.qkey < track.qkeys.size() - 1) {
                ++current_frame.qkey;
                ++next_frame.qkey;
                t1 = track.qkeys[current_frame.qkey].time * fps;
                t2 = track.qkeys[next_frame.qkey].time * fps;
            }

            auto q1 = track.qkeys[current_frame.qkey].rotate;
            q1 = glm::normalize(q1);
            auto q2 = track.qkeys[next_frame.qkey].rotate;
            q2 = glm::normalize(q2);

            auto delta = (elapsed_time_ - t1) / (t2 - t1);
            delta = delta > 1.f ? 1.f : delta;

            return glm::normalize(glm::lerp(q1, q2, delta));
        } else {
            return track.qkeys[current_frame.qkey].rotate;
        }
    }
}
