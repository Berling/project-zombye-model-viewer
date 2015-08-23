#include <stdexcept>
#include <string>

#include <glm/gtc/matrix_transform.hpp>

#include <SDL2/SDL.h>
#include <utils/logger.hpp>

#include <core/engine.hpp>
#include <graphics/graphics_system.hpp>
#include <rendering/renderer.hpp>

namespace core {
	engine::engine(const std::string& file_name) noexcept
	: quit_{false} {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			auto sdl_error = std::string{SDL_GetError()};
			SDL_ClearError();
			throw std::runtime_error{"could not initialize SDL" + sdl_error};
		}
		graphics_system_ = std::make_unique<graphics::graphics_system>(*this);
		renderer_ = std::make_unique<rendering::renderer>(*this, file_name);

		utils::log(utils::log_level::LOG_INFO) << "load model " << file_name << std::endl;

		scene_radius_ = 4.f;
		camera_position_ = glm::vec3{0.f};
		center_ = scene_radius_ / sinf(glm::radians(45.f) / 2.f);
		zoom_ = 5.f;
		translate_ = glm::vec2{0.f};

		dragging_ = false;
		zooming_ = false;
		rotating_ = false;

		camera_.place(glm::vec2{400.f, 300.f}, 0.5f * sqrtf(800.f * 800.f + 600.f * 600.f));
	}

	engine::~engine() {
		SDL_Quit();
	}

	void engine::update(float delta_time) {
		auto rotation_matrix = glm::inverse(camera_.rotation_matrix());
		auto translation_matrix = glm::translate(glm::mat4{1.f},
			glm::vec3{translate_ * scene_radius_ / zoom_, -center_ * scene_radius_ / zoom_});
		renderer_->view_matrix(translation_matrix * rotation_matrix);

		graphics_system_->begin();
		renderer_->render(delta_time);
		graphics_system_->end(delta_time);
	}

	void engine::run() {
		auto event = SDL_Event{};
		auto last_time = 0.f;
		auto current_time = SDL_GetTicks() / 1000.f;
		auto delta_time = 0.f;

		while (!quit_) {
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					quit_ = true;
				} else if (event.type == SDL_KEYDOWN) {
					static auto debug_render = false;
					if (event.key.keysym.scancode == SDL_SCANCODE_E) {
						debug_render = !debug_render;
						renderer_->debug_mode(debug_render);
					}
				} else if (event.type == SDL_MOUSEBUTTONDOWN) {
					auto x = float(event.button.x);
					auto y = float(event.button.y);
					mouse_down_ = glm::vec2{x / 800.f, y / 600.f};

					if (event.button.button == SDL_BUTTON_LEFT) {
						camera_.mouse_position(glm::vec2{x, 600.f - 1.f - y});
						camera_.start_dragging();
						rotating_ = true;
					} else if (event.button.button == SDL_BUTTON_MIDDLE) {
						dragging_ = true;
						zooming_ = false;
					} else if (event.button.button == SDL_BUTTON_RIGHT) {
						dragging_ = false;
						zooming_ = true;
					}
				} else if (event.type == SDL_MOUSEBUTTONUP) {
					if (event.button.button == SDL_BUTTON_LEFT) {
						camera_.finish_dragging();
						rotating_ = false;
					} else if (event.button.button == SDL_BUTTON_MIDDLE) {
						dragging_ = false;
					} else if (event.button.button == SDL_BUTTON_RIGHT) {
						zooming_ = false;
					}
				} else if (event.type == SDL_MOUSEMOTION) {
					auto x = float(event.button.x);
					auto y = float(event.button.y);
					auto pos = glm::vec2{x / 800.f, y / 600.f};
					auto drag = pos - mouse_down_;

					if (rotating_) {
						camera_.mouse_position(glm::vec2{x, 600.f - 1.f - y});
					} else if (dragging_) {
						translate_ += glm::vec2{10.f * drag.x, -10.f * drag.y};

						if (translate_.x > zoom_) translate_.x = zoom_;
						if (translate_.x < -zoom_) translate_.x = -zoom_;
						if (translate_.y > zoom_) translate_.y = zoom_;
						if (translate_.y < -zoom_) translate_.y = -zoom_;
					} else if (zooming_) {
						auto zoom = 10.f * drag.x;
						zoom_ += zoom;

						if (zoom_ < 0.05f) zoom_ = 0.05f;
						if (zoom_ > 10.f) zoom_ = 10.f;
						if (translate_.x > zoom_) translate_.x = zoom_;
						if (translate_.x < -zoom_) translate_.x = -zoom_;
						if (translate_.y > zoom_) translate_.y = zoom_;
						if (translate_.y < -zoom_) translate_.y = -zoom_;
					}

					mouse_down_ = pos;
				}
			}
			last_time = current_time;
			current_time = SDL_GetTicks() / 1000.f;
			delta_time = current_time - last_time;

			update(delta_time);
		}
	}

	void engine::quit() noexcept {
		quit_ = true;
	}
}
