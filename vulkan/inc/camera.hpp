#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Vulkan
{

//-------------------------------------------------------------------------------//

// class Camera
// {

//     public:

//         void SetOrthographProjection(float left, float right, float top, float bottom, float near, float far)
//         {
//             projection_matrix_ = glm::ortho(left, right, top, bottom, near, far);
//         }

//         void SetPerspectiveProjection(float fovy, float aspect, float near, float far)
//         {
//             projection_matrix_ = glm::perspectiveLH_ZO(fovy, aspect, near, far);
//         }

//         void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f))
//         {
//             view_matrix_ = glm::lookAt(position, direction, up);
//         }

//         void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f))
//         {
//             SetViewDirection(position, target - position, up);
//         }

//         void SetViewYXZ(glm::vec3 position, glm::vec3 rotation)
//         {
//             const float c3 = glm::cos(rotation.z);
//             const float s3 = glm::sin(rotation.z);
//             const float c2 = glm::cos(rotation.x);
//             const float s2 = glm::sin(rotation.x);
//             const float c1 = glm::cos(rotation.y);
//             const float s1 = glm::sin(rotation.y);
            
//             const glm::vec3 u{(c1 * c3 + s1 * s2 * s3), (c2 * s3), (c1 * s2 * s3 - c3 * s1)};
//             const glm::vec3 v{(c3 * s1 * s2 - c1 * s3), (c2 * c3), (c1 * c3 * s2 + s1 * s3)};
//             const glm::vec3 w{(c2 * s1), (-s2), (c1 * c2)};
            
//             view_matrix_ = glm::mat4{1.f};

//             view_matrix_ = glm::rotate(view_matrix_, rotation.y, v);
//             view_matrix_ = glm::rotate(view_matrix_, rotation.x, u);
//             view_matrix_ = glm::rotate(view_matrix_, rotation.z, w);
//             view_matrix_ = glm::transpose(view_matrix_);
//             view_matrix_ = glm::translate(view_matrix_, -position);
//         }

//         const glm::mat4& GetProjection() const { return projection_matrix_; }

//         const glm::mat4& GetView() const { return view_matrix_; }

//     private:

//         glm::mat4 projection_matrix_{1.f};

//         glm::mat4 view_matrix_{1.f};

// };

//-------------------------------------------------------------------------------//

class Camera
{

    public:

        Camera(const glm::vec3& position = {0,0,0}, const glm::vec3& rotation = {0,0,0});

        void SetOrthographProjection(float left, float right, float top, float bottom, float near, float far)
        {
            projection_matrix_ = glm::ortho(left, right, top, bottom, near, far);
        }

        void SetPerspectiveProjection(float fovy, float aspect, float near, float far)
        {
            projection_matrix_ = glm::perspectiveLH_ZO(fovy, aspect, near, far);
        }

        void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f))
        {
            view_matrix_ = glm::lookAt(position, direction, up);
        }

        void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3(0.f, -1.f, 0.f))
        {
            SetViewDirection(position, target - position, up);
        }

        void SetViewYXZ(glm::vec3 position, glm::vec3 rotation)
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

        
        const glm::mat4& GetViewMatrix();
        
        const glm::mat4& GetProjectionMatrix()  const { return projection_matrix_; }
        
        const float GetFarClipPlane()           const { return far_clip_plane_; }
        
        const float GetNearClipPlane()          const { return near_clip_plane_; }
        
        const float GetFieldOfvView()           const { return field_of_view_; }

        
        void MoveForward(const float delta)
        {
            position_ += direction_ * delta;

            update_view_matrix_ = true;
        }
        
        void MoveRight(const float delta)
        {
            position_ += right_ * delta;

            update_view_matrix_ = true;
        }
        
        void MoveUp(const float delta)
        {
            position_ += up_ * delta;

            update_view_matrix_ = true;
        }

        const glm::vec3& GetPosition() const { return position_; }
        
        const glm::vec3& GetRotation() const { return rotation_; }

        void AddMovementAndRotation(const glm::vec3& movement_delta, const glm::vec3& rotation_delta, glm::vec3 dir, glm::vec3 right, glm::vec3 up);

    private:

        void UpdateViewMatrix();

        void UpdateProjectionMatrix()
        {
            projection_matrix_ = glm::perspective(glm::radians(field_of_view_), viewport_width_ / viewport_height_, near_clip_plane_, far_clip_plane_);
        }

        glm::vec3 position_;
        
        glm::vec3 rotation_;

        glm::vec3 direction_;
        
        glm::vec3 right_;
        
        glm::vec3 up_;
        
        float far_clip_plane_  { 100.f };
        
        float near_clip_plane_ { 0.1f };
        
        float viewport_width_  { 800.f };
        
        float viewport_height_ { 600.f };
        
        float field_of_view_   { 60.f };

        const glm::vec3 s_world_up{ 0.f, 0.f, 1.f };
        
        const glm::vec3 s_world_right{ 0.f, -1.f, 0.f };
        
        const glm::vec3 s_world_forward{ 1.f, 0.f, 0.f };

        glm::mat4 view_matrix_;
        
        glm::mat4 projection_matrix_;
        
        bool update_view_matrix_ = false;
};

//-------------------------------------------------------------------------------//

} // end of Vulkan namespace

#endif