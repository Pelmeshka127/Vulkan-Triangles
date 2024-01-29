#include "camera.hpp"

#include <cassert>
#include <limits>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

void Camera::SetOrthographProjection(float left, float right, float top, float bottom, float near, float far) 
{
    projection_matrix_ = glm::ortho(left, right, top, bottom, near, far);
}

//-------------------------------------------------------------------------------//
 
void Camera::SetPerspectiveProjection(float fovy, float aspect, float near, float far) 
{
    projection_matrix_ = glm::perspectiveLH_ZO(fovy, aspect, near, far);
}

//-------------------------------------------------------------------------------//

void Camera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up) 
{
    view_matrix_ = glm::lookAt(position, direction, up);
}

//-------------------------------------------------------------------------------//

void Camera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up) 
{
    SetViewDirection(position, target - position, up);
}

//-------------------------------------------------------------------------------//

void Camera::SetViewYXZ(glm::vec3 position, glm::vec3 rotation)
{
    const float c3 = glm::cos(rotation.z);
    const float s3 = glm::sin(rotation.z);
    const float c2 = glm::cos(rotation.x);
    const float s2 = glm::sin(rotation.x);
    const float c1 = glm::cos(rotation.y);
    const float s1 = glm::sin(rotation.y);
    
    const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
    const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
    const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
    
    view_matrix_ = glm::mat4{1.f};

    view_matrix_ = glm::rotate(view_matrix_, rotation.y, v);
    view_matrix_ = glm::rotate(view_matrix_, rotation.x, u);
    view_matrix_ = glm::rotate(view_matrix_, rotation.z, w);
    view_matrix_ = glm::transpose(view_matrix_);
    view_matrix_ = glm::translate(view_matrix_, -position);
}

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace