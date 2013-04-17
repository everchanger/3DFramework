#ifndef CAMERA__H__
#define CAMERA__H__

#include <glm/glm.hpp>
#include "IObject.hpp"

/**
    A first-person shooter type of camera.

    Set the properties of the camera, then use the `matrix` method to get the camera matrix for
    use in the vertex shader.

    Includes the perspective projection matrix.
    */
class Camera : public IObject {
	public:
		Camera();

		/**
			The position of the camera.
		*/
		void setPosition(const glm::vec3& position);

		void MoveForward(float deltaTime);
		void MoveBackward(float deltaTime);
		void MoveLeft(float deltaTime);
		void MoveRight(float deltaTime);
		void MoveUp(float deltaTime);
		void MoveDown(float deltaTime);
		void Move(glm::vec3& rDirection, float deltaTime);

		/**
			The vertical viewing angle of the camera, in degrees.

			Determines how "wide" the view of the camera is. Large angles appear to be zoomed out,
			as the camera has a wide view. Small values appear to be zoomed in, as the camera has a
			very narrow view.

			The value must be between 0 and 180.
			*/
		void setFieldOfView(float fieldOfView);

		/**
			Sets the near and far plane distances.

			Everything between the near plane and the var plane will be visible. Everything closer
			than the near plane, or farther than the far plane, will not be visible.

			@param nearPlane  Minimum visible distance from camera. Must be > 0
			@param farPlane   Maximum visible distance from vamera. Must be > nearPlane
			*/
		void setNearAndFarPlanes(float nearPlane, float farPlane);

		/*
			Offsets the cameras orientation.

			The verticle angle is constrained between 85deg and -85deg to avoid gimbal lock.

			@param upAngle     the angle (in degrees) to offset upwards. Negative values are downwards.
			@param rightAngle  the angle (in degrees) to offset rightwards. Negative values are leftwards.
			*/
		void offsetOrientation(float upAngle, float rightAngle);

		/*
			The width divided by the height of the screen/window/viewport

			Incorrect values will make the 3D scene look stretched.
		*/
		void setViewportAspectRatio(float viewportAspectRatio);

		/** A unit vector representing the direction the camera is facing */
		glm::vec3 forward();

		/** A unit vector representing the direction to the right of the camera*/
		glm::vec3 right();

		/** A unit vector representing the direction out of the top of the camera*/
		glm::vec3 up();

		/**
			The combined camera transformation matrix, including perspective projection.

			This is the complete matrix to use in the vertex shader.
			*/
		glm::mat4 CameraMatrix(void) { return m_CameraMatrix; }

		void HandleInput(float deltaTime);
		void Update(float deltaTime);

		glm::vec3& GetPosition(void) {return m_Position;}

	private:
		

		float m_fieldOfView;
		float m_nearPlane;
		float m_farPlane;
		float m_viewportAspectRatio;
		float m_Speed;

		bool m_RecalculateView;

		glm::vec3 m_Rotation;
		glm::vec3 m_Position;

		glm::mat4 m_ViewProj;

		glm::mat4 m_CameraMatrix;
};

#endif