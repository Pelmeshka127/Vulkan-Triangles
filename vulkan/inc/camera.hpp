#ifndef CAMERA_HPP_
#define CAMERA_HPP_
#include <iostream>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>

namespace Vulkan
{

class Camera
{
    public:

        Camera(const glm::vec3& position = { 0, 0, 0 },
                const glm::vec3& rotation = { 0, 0, 0 });

        void set_viewport_size(const float width, const float height);

        const glm::mat4& get_view_matrix();
        const glm::mat4& get_projection_matrix() const { return m_projection_matrix; }
        const float get_far_clip_plane() const { return m_far_clip_plane; }
        const float get_near_clip_plane() const { return m_near_clip_plane; }
        const float get_field_of_view() const { return m_field_of_view; }

        void move_forward(const float delta);
        void move_right(const float delta);
        void move_up(const float delta);

        const glm::vec3& get_position() const { return m_position; }
        const glm::vec3& get_rotation() const { return m_rotation; }

        void add_movement_and_rotation(const glm::vec3& movement_delta,
                                        const glm::vec3& rotation_delta);

    private:
        void update_view_matrix();
        void update_projection_matrix();

        glm::vec3 m_position;
        glm::vec3 m_rotation; 

        glm::vec3 m_direction;
        glm::vec3 m_right;
        glm::vec3 m_up;
        float m_far_clip_plane  { 100.f };
        float m_near_clip_plane { 0.1f };
        float m_viewport_width  { 800.f };
        float m_viewport_height { 600.f };
        float m_field_of_view   { 60.f };

        static constexpr glm::vec3 s_world_up{ 0.f, 0.f, 1.f };
        static constexpr glm::vec3 s_world_right{ 0.f, -1.f, 0.f };
        static constexpr glm::vec3 s_world_forward{ 1.f, 0.f, 0.f };

        glm::mat4 m_view_matrix;
        glm::mat4 m_projection_matrix;
        bool m_update_view_matrix = false;
};

} // end of Vulkan namespcae

#endif