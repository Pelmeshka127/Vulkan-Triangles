#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

class Camera
{

    public:

        void SetOrthographProjection(float left, float right, float top, float bottom, float near, float far);

        void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

        void SetViewDirection(glm::vec3 camera_position, glm::vec3 camera_direction, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f));

        void SetViewTarget(glm::vec3 camera_position, glm::vec3 camera_target, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f));

        void SetViewYXZ(glm::vec3 position, glm::vec3 rotation);

        const glm::mat4& GetProjection() const { return projection_matrix_; }

        const glm::mat4& GetView() const { return view_matrix_; }

    private:

        glm::mat4 projection_matrix_{1.f};

        glm::mat4 view_matrix_{1.f};

};

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace

#endif