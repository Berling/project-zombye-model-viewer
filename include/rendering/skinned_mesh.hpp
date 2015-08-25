#ifndef __RENDERING_SKINNED_MESH_HPP__
#define __RENDERING_SKINNED_MESH_HPP__

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <glcw/buffer.hpp>
#include <glcw/vertex_array.hpp>

#include <rendering/mesh.hpp>

namespace core {
    class engine;
}

namespace rendering {
    class renderer;
}

namespace rendering {
    struct skinned_vertex {
        glm::vec3 pos;
        glm::vec2 tex;
        glm::vec3 nor;
        glm::vec3 tan;
        glm::ivec4 index;
        glm::vec4 weight;
    };

    class skinned_mesh {
        std::vector<submesh> submeshes_;
        glcw::vertex_array vao_;
        glcw::vertex_buffer vbo_;
        glcw::index_buffer ibo_;
        bool parallax_mapping_;
    public:
        skinned_mesh(core::engine& engine, renderer& renderer, const std::vector<char>& source, const std::string& file_name) noexcept;
        skinned_mesh(const skinned_mesh& other) = delete;
        skinned_mesh(skinned_mesh&& other) noexcept = default;
        ~skinned_mesh() = default;
        skinned_mesh& operator=(const skinned_mesh other) = delete;
        skinned_mesh& operator=(skinned_mesh&& other) noexcept = default;

        void draw() const noexcept;

        auto& vao() const noexcept {
            return vao_;
        }

        auto parallax_mapping() const {
            return parallax_mapping_;
        }
    };
}

#endif
