#ifndef __RENDERING_MESH_HPP__
#define __RENDERING_MESH_HPP__

#include <memory>
#include <vector>

#include <glm/glm.hpp>

#include <glcw/buffer.hpp>
#include <glcw/vertex_array.hpp>
#include <glcw/texture.hpp>

#include <rendering/vertex.hpp>

namespace core {
    class engine;
}

namespace rendering {
    class renderer;
}

namespace rendering {
    struct header {
        uint32_t magic;
        uint64_t vertex_count;
        uint64_t index_count;
        uint64_t submesh_count;
        bool parallax_mapping;
    };

    struct submesh {
        uint64_t index_count;
        uint64_t offset;
        std::shared_ptr<const glcw::texture> diffuse;
        std::shared_ptr<const glcw::texture> normal;
        std::shared_ptr<const glcw::texture> material;
    };

    class mesh {
        std::vector<submesh> submeshes_;
        glcw::vertex_array vao_;
        glcw::vertex_buffer vbo_;
        glcw::index_buffer ibo_;
        bool parallax_mapping_;
    public:
        mesh(core::engine& engine, renderer& renderer, const std::vector<char>& source, const std::string& file_name) noexcept;
        mesh(const mesh& other) = delete;
        mesh(mesh&& other) = default;
        ~mesh() noexcept = default;
        mesh& operator=(const mesh& other) = delete;
        mesh& operator=(mesh&& other) noexcept = default;

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
