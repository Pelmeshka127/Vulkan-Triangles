#include <GLFW/glfw3.h>
#include "camera.hpp"
#include "keyboard.hpp"

namespace Vulkan
{

    Camera::Camera(const glm::vec3& position, const glm::vec3& rotation) : 
        m_position(position),
        m_rotation(rotation)
    {
        update_view_matrix();
        update_projection_matrix();
    }

    const glm::mat4& Camera::get_view_matrix()
    {
        if (m_update_view_matrix)
        {
            update_view_matrix();
            m_update_view_matrix = false;
        }
        return m_view_matrix;
    }

    void Camera::update_view_matrix()
    {
        const float roll_in_radians  = glm::radians(m_rotation.x);
        const float pitch_in_radians = glm::radians(m_rotation.y);
        const float yaw_in_radians   = glm::radians(m_rotation.z);

        const glm::mat3 rotate_matrix_x(1, 0, 0,
            0,  std::cos(roll_in_radians), std::sin(roll_in_radians),
            0, -std::sin(roll_in_radians), std::cos(roll_in_radians));

        const glm::mat3 rotate_matrix_y(std::cos(pitch_in_radians), 0, -std::sin(pitch_in_radians),
            0, 1, 0,
            std::sin(pitch_in_radians), 0, std::cos(pitch_in_radians));

        const glm::mat3 rotate_matrix_z(std::cos(yaw_in_radians), std::sin(yaw_in_radians), 0,
            -std::sin(yaw_in_radians), std::cos(yaw_in_radians), 0,
            0, 0, 1);

        const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;
        m_direction = glm::normalize(euler_rotate_matrix * s_world_forward);
        m_right = glm::normalize(euler_rotate_matrix * s_world_right);
        m_up = glm::cross(m_right, m_direction);

        m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_up);
    }

    void Camera::update_projection_matrix()
    {
        m_projection_matrix = glm::perspective(glm::radians(m_field_of_view), m_viewport_width / m_viewport_height, m_near_clip_plane, m_far_clip_plane);
    }

    void Camera::set_viewport_size(const float width, const float height)
    {
        m_viewport_width = width;
        m_viewport_height = height;
        update_projection_matrix();
    }


    void Camera::move_forward(const float delta)
    {
        m_position += m_direction * delta;
        m_update_view_matrix = true;
    }

    void Camera::move_right(const float delta)
    {
        m_position += m_right * delta;
        m_update_view_matrix = true;
    }

    void Camera::move_up(const float delta)
    {
        m_position += s_world_up * delta;
        m_update_view_matrix = true;
    }

    void Camera::add_movement_and_rotation(const glm::vec3& movement_delta,
                                             const glm::vec3& rotation_delta)
    {
        m_position += m_direction * movement_delta.x;
        m_position += m_right     * movement_delta.y;
        m_position += m_up        * movement_delta.z;
        m_rotation += rotation_delta;
        m_update_view_matrix = true;
    }
} // end of Vulkan namespace