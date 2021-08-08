#ifndef CAMERA_H
#define CAMERA_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "glm/glm.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera{
    glm::vec3 m_front;
    glm::vec3 m_position;
    glm::vec3 m_up;

    double m_pitch;
    double m_yaw;

public:
    Camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float pitch=0.0f, float yaw=-90.f) :
        m_position {position}, m_front {front}, m_up {up}, m_pitch{pitch}, m_yaw {yaw}
    {
    }

    glm::vec3 getFront() const {
        return m_front;
    }

    glm::vec3 getPosition() const {
        return m_position;
    }

    glm::mat4 getView()  const {
        return glm::lookAt(m_position, m_front + m_position, m_up);
    }

    void moveFront(float cameraSpeed) {
        m_position += m_front * cameraSpeed;
    }

    void moveSide(float cameraSpeed){
        m_position += glm::normalize(glm::cross(m_front, m_up)) * cameraSpeed;
    }

    void rotate(double xOffset, double yOffset){
        float sensitivity {0.1f};

        m_yaw += sensitivity * xOffset;
        m_pitch += sensitivity * yOffset;

        if(m_pitch > 89.0f)
            m_pitch = 89.0f;
        if(m_pitch < -89.0f)
            m_pitch = -89.0f;

        m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front.y = sin(glm::radians(m_pitch));
        m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    }

};

#endif