#pragma once
#include "Camera.h"
#include <GLFW/glfw3.h>


class ACameraController
{
	struct FMouseMovement
	{
		double LastX = 0.0;
		double LastY = 0.0;
		bool bFirstTime = true;
	} MouseInfo;

	struct EulerAngles
	{
		float Yaw = 0.0f;
		float Pitch = 0.0f;
	} CurrentRotation;

private:
	Camera* MountedCamera = nullptr;

	float CameraFOV = 1;

	glm::vec3 CameraForward = glm::vec3(0);
	glm::vec3 CameraUp = glm::vec3(0);
	glm::vec3 CameraRight = glm::vec3(0);

	glm::vec3 CameraPosition = glm::vec3(0);
	glm::vec3 GlobalUp = glm::vec3(0, 1, 0);

	float MouseSensitivity   = 0.1f; //To multiply our delta mouse movement
	float TranslationalSpeed = 5.f;
	float MaxFov = 45;

    static ACameraController* CameraControllerRef;

public:
    GLFWwindow* Window = nullptr;

	static void Statics_OnMousePositionChange(GLFWwindow* Window, double XPosition, double YPosition);
	static void Statics_OnMouseScrollWheel(GLFWwindow* Window, double XOffset, double YOffset);

	static void Statics_MoveForwardBackward(float Scale, float DeltaTime);
	static void Statics_MoveLeftRight(float Scale, float DeltaTime);
	static void Statics_MoveUpDown(float Scale, float DeltaTime);

private:
    inline bool LeftMouseButtonIsDown() const;
	void RotateCamera();

public:

    ACameraController()
    {
       CameraControllerRef = this; 
    }
	
    void MountACamera(Camera* Camera);

	inline bool HasACamera() const{ return MountedCamera;}

	//Will multiply the delta x and y mouse displacement. Default = 0.1 ( 10% ) of delta mouse movement
	void SetMouseSensitivity(const float& Sensitivity); 

	//Translational speed (per second). Default = 5units/s
	void SetTranslationalSpeed(const float& Persecond); 

	void SetMaxFOV(const float& MaxFOV);

	void Tick();

	void ResetToDefaultView();

	inline const glm::vec3& GetCameraForwardVector() const { return CameraForward; }
	inline const glm::vec3& GetCameraRightVector() const { return CameraRight; }
	inline const glm::vec3& GetCameraUpVector() const { return CameraUp; }
	inline const glm::vec3& GetCameraPosition() const { return CameraPosition; }
	inline glm::vec3 GetCameraZBasisVector() const { return -CameraForward; }
	inline const Camera* GetControlledCamera() const { return MountedCamera; }



};


