#include "camera.hpp"

#include <iostream>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

Camera::Camera(const glm::vec3& position, const glm::vec3& rotation) : position_(position), rotation_(rotation)
{
    UpdateViewMatrix();
    
    UpdateProjectionMatrix();
}

//-------------------------------------------------------------------------------//

void Camera::UpdateViewMatrix()
{
    std::cout << "Updating view matrix" << std::endl;
    const float roll_in_radians  = glm::radians(rotation_.x);
    const float pitch_in_radians = glm::radians(rotation_.y);
    const float yaw_in_radians   = glm::radians(rotation_.z);

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
    
    direction_ = glm::normalize(euler_rotate_matrix * s_world_forward);
    
    right_ = glm::normalize(euler_rotate_matrix * s_world_right);
    
    up_ = glm::cross(right_, direction_);

    view_matrix_ = glm::lookAt(position_, position_ + direction_, up_);
}

//-------------------------------------------------------------------------------//

const glm::mat4& Camera::GetViewMatrix()
{
    if (update_view_matrix_)
    {
        UpdateViewMatrix();

        update_view_matrix_ = false;
    }

    return view_matrix_;
}

//-------------------------------------------------------------------------------//

void Camera::AddMovementAndRotation(const glm::vec3& movement_delta, const glm::vec3& rotation_delta, glm::vec3 dir, glm::vec3 right, glm::vec3 up)
{
    position_ += dir * movement_delta.x;
    
    position_ += right     * movement_delta.y;
    
    position_ += up        * movement_delta.z;
    
    rotation_ += rotation_delta;

    std::cout << "Moving and rotation" << std::endl;
    
    update_view_matrix_ = true;
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace