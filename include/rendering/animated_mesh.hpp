#ifndef __RENDERING_ANIMATED_MESH_HPP__
#define __RENDERING_ANIMATED_MESH_HPP__

#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace core {
    class engine;
}

namespace rendering {
    class skeleton;
    class skinned_mesh;
    struct track;
    class renderer;
}

namespace rendering {
    class animated_mesh {
        struct frame {
            int tkey = 0;
            int qkey = 0;
            int skey = 0;
        };

        core::engine& engine_;
        renderer& renderer_;
        std::shared_ptr<const skinned_mesh> mesh_;
        std::shared_ptr<const skeleton> skeleton_;
        std::string current_state_;
        std::string next_state_;
        float elapsed_time_;
        std::vector<frame> current_keyframes_;
        std::vector<frame> next_keyframes_;
        std::vector<glm::mat4> pose_;
        bool blend_;
        bool blend_next_;
    public:
        animated_mesh(core::engine& engine, renderer& renderer, const std::string& mesh, const std::string& skeleton);
        ~animated_mesh() noexcept;

        void draw() const noexcept;

        void update(float delta_time);

        auto mesh() const noexcept {
            return mesh_;
        }

        void load(const std::string& mesh);

        auto skeleton() const noexcept {
            return skeleton_;
        }

        void load_skeleton(const std::string& skeleton);

        auto& pose() const noexcept {
            return pose_;
        }

        void change_state(const std::string& state) {
            current_state_ = state;
            for (auto& k : current_keyframes_) {
                k.tkey = 0;
                k.qkey = 0;
                k.skey = 0;
            }
            elapsed_time_ = 0.f;
        }

        void change_state_blend(const std::string& state_b) {
            next_state_ = state_b;
            next_keyframes_.assign(next_keyframes_.size(), frame{});
            blend_next_ = true;
        }

        bool is_playing(const std::string& state) const {
            return current_state_ == state;
        }

        bool is_blending() const {
            return blend_;
        }

    private:
        glm::vec3 interpolate_translation_keyframe(const track& track, frame& current_frame, frame& next_frame, float fps);
        glm::quat interpolate_quaternion_keyframe(const track& track, frame& current_frame, frame& next_frame, float fps);
    };
}

#endif
