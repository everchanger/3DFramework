#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glfw.h>

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock


Camera::Camera() :
    m_Position(0,0,1),
    m_Rotation(0,0,0),
    m_fieldOfView(50.0f),
    m_nearPlane(0.01f),
    m_farPlane(100.0f),
	m_Speed(2.0f),
    m_viewportAspectRatio(4.0f/3.0f),
	m_RecalculateView(true)
{
}

void Camera::setPosition(const glm::vec3& position) 
{
    m_Position = position;
}

void Camera::MoveForward(float deltaTime) 
{
	Move(forward(), deltaTime); 
}

void Camera::MoveBackward(float deltaTime) 
{
	Move(-forward(), deltaTime);
}

void Camera::MoveLeft(float deltaTime) 
{
	Move(-right(), deltaTime);
}

void Camera::MoveRight(float deltaTime)
{
	Move(right(), deltaTime);
}

void Camera::MoveUp(float deltaTime) 
{
	Move(up(), deltaTime);
}

void Camera::MoveDown(float deltaTime) 
{
	Move(-up(), deltaTime);
}

void Camera::Move(glm::vec3& rDirection, float deltaTime) 
{
	m_Position += (deltaTime * m_Speed * rDirection);
}

void Camera::setFieldOfView(float fieldOfView) 
{
    assert(fieldOfView > 0.0f && fieldOfView < 180.0f);

    m_fieldOfView = fieldOfView;

	m_RecalculateView = true;
}

void Camera::setNearAndFarPlanes(float nearPlane, float farPlane) 
{
    assert(nearPlane > 0.0f);
    assert(farPlane > nearPlane);

    m_nearPlane = nearPlane;
    m_farPlane = farPlane;

	m_RecalculateView = true;
}

void Camera::offsetOrientation(float upAngle, float rightAngle) 
{
	m_Rotation.y += rightAngle;

    while(m_Rotation.y > 360.0f) { 
		m_Rotation.y -= 360.0; 
	}
    while(m_Rotation.y < 0.0f) { 
		m_Rotation.y += 360.0;
	}

    m_Rotation.x += upAngle;

	if(m_Rotation.x > MaxVerticalAngle) { 
		m_Rotation.x = MaxVerticalAngle;
	}
	if(m_Rotation.x < -MaxVerticalAngle) {
		m_Rotation.x = -MaxVerticalAngle; 
	}
}

void Camera::setViewportAspectRatio(float viewportAspectRatio) 
{
    assert(viewportAspectRatio > 0.0);

    m_viewportAspectRatio = viewportAspectRatio;

	m_RecalculateView = true;
}

glm::vec3 Camera::forward() 
{
    glm::mat4& rCam = m_transform.WorldMatrix();
    return glm::normalize(-glm::vec3(rCam[2][0],rCam[2][1],rCam[2][2]));
}

glm::vec3 Camera::right() 
{
	glm::mat4& rCam = m_transform.WorldMatrix();
    return glm::normalize(glm::vec3(rCam[0][0],rCam[0][1],rCam[0][2]));
}

glm::vec3 Camera::up() 
{
    glm::mat4& rCam = m_transform.WorldMatrix();
    return glm::normalize(glm::vec3(rCam[1][0],rCam[1][1],rCam[1][2]));
}

void Camera::HandleInput(float deltaTime) 
{
	//move position of camera based on WASD keys, and XZ keys for up and down
    if(glfwGetKey('S')){
		MoveBackward(deltaTime);
    } else if(glfwGetKey('W')){
        MoveForward(deltaTime);
    }
    if(glfwGetKey('A')){
       MoveLeft(deltaTime);
    } else if(glfwGetKey('D')){
        MoveRight(deltaTime);
    }
    if(glfwGetKey('Z')){
        MoveUp(deltaTime);
    } else if(glfwGetKey('X')){
        MoveDown(deltaTime);
    }

    //rotate camera based on mouse movement
    const float mouseSensitivity = 0.1f;
    int mouseX, mouseY;
    glfwGetMousePos(&mouseX, &mouseY);
    offsetOrientation(mouseSensitivity * -mouseY, mouseSensitivity * -mouseX);
    glfwSetMousePos(0, 0); //reset the mouse, so it doesn't go out of the window	
}

void Camera::Update(float deltaTime)
{
	if(IsUpToDate()) {
		return;
	}

	HandleInput(deltaTime);

	if(m_RecalculateView) {
		m_ViewProj = glm::perspective(m_fieldOfView, m_viewportAspectRatio, m_nearPlane, m_farPlane);
		m_RecalculateView = false;
	}

	/*http://www.arcsynthesis.org/gltut/Positioning/Tut06%20Fun%20with%20Matrices.html*/
	glm::mat4 orientation;

	// translate
	orientation = glm::translate(orientation, m_Position);

	// rotate
	orientation = glm::rotate(orientation, m_Rotation.y, glm::vec3(0,1,0));
	orientation = glm::rotate(orientation, m_Rotation.x, glm::vec3(1,0,0));
	m_transform.LocalMatrix()  = glm::rotate(orientation, m_Rotation.z, glm::vec3(0,0,1));
	
	IObject::Update(deltaTime);
	
	// use the inverse of the transform to create a view matrix.
	orientation = glm::inverse(m_transform.WorldMatrix());
	m_CameraMatrix = m_ViewProj*orientation;
}