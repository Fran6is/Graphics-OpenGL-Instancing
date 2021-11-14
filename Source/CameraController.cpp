#include "CameraController.h"

std::ostream& operator<<(std::ostream& Cout, const glm::vec3& Vector);
std::ostream& operator<<(std::ostream& Cout, const glm::mat4& Matrix);
std::ostream& operator<<(std::ostream& Cout, const glm::vec4& Vector);

void ACameraController::MountACamera(Camera* Camera)
{
	MountedCamera = Camera;
    if (MountedCamera)
    {
        //Need these for camera translational motion so we can at least move the camera pending when the camera rotation function 
        //gets called to calculate and overwrite all vector value
        CameraForward = MountedCamera->CameraForward;
        CameraRight = MountedCamera->CameraRight;
        CameraUp = MountedCamera->CameraUp;
        CameraPosition = MountedCamera->CameraPosition;

        //Most important code here is this one since we can get the basis vectors from it. 
        //Moreover, we're overriding previous vector values with each delta mouse position.
        //
        CurrentRotation.Yaw   = glm::degrees( std::atan2f(CameraForward.z, CameraForward.x) );
        CurrentRotation.Pitch = glm::degrees( std::asinf(CameraForward.y) );

        CameraFOV = MountedCamera->CameraDefinition.FOV;
    }
    else
    {
        CameraPosition = CameraRight = CameraUp = CameraForward = glm::vec3(0);

        CurrentRotation.Yaw = CurrentRotation.Pitch = 0;

        CameraFOV = 1;
    }
}

void ACameraController::Tick()
{
	if (!MountedCamera)
    {
	    std::cerr << "ACameraController::Tick - Can't control a Camera without mounting one fool! \n";
	    return;
    }

    glfwSetInputMode(Window, GLFW_CURSOR, (LeftMouseButtonIsDown() ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL));

	MountedCamera->CameraUp = CameraUp;
	MountedCamera->CameraRight = CameraRight;
	MountedCamera->CameraForward = CameraForward;
	MountedCamera->CameraPosition = CameraPosition;
    MountedCamera->CameraDefinition.FOV = CameraFOV;
}

void ACameraController::ResetToDefaultView()
{
	CurrentRotation.Yaw   = -120.f;
	CurrentRotation.Pitch = -21.f;

	CameraForward.x = std::cos(glm::radians(CurrentRotation.Yaw)) * std::cos(glm::radians(CurrentRotation.Pitch));
	CameraForward.z = std::sin(glm::radians(CurrentRotation.Yaw)) * std::cos(glm::radians(CurrentRotation.Pitch));
	CameraForward.y = std::sin(glm::radians(CurrentRotation.Pitch));

	CameraForward  = glm::normalize(CameraForward);
	CameraRight    = glm::vec3(0.87f, 0.f, -0.5f);
	CameraUp       = glm::vec3(-0.18f, 0.93f, -0.32f);
	CameraPosition = glm::vec3(24.54f, 22.25f, 39.68f); 

    CameraFOV = 45;
	//I logged the camera's movements per frame to get a good starting location and angle, then i copied and pasted them here default
}

void ACameraController::Statics_MoveForwardBackward(float Scale, float DeltaTime)
{
    if(!CameraControllerRef || !CameraControllerRef->MountedCamera) return;
    
    float Speed = CameraControllerRef->TranslationalSpeed * Scale * DeltaTime;

    CameraControllerRef->CameraPosition += CameraControllerRef->CameraForward * Speed;

    //std::cout << "Move Forward or Backward = " << Scale << "\t DeltaTime = " << DeltaTime << "\n";
    //std::cout << CameraControllerRef->CameraPosition << "\n";
}

void ACameraController::Statics_MoveLeftRight(float Scale, float DeltaTime)
{
    if(!CameraControllerRef || !CameraControllerRef->MountedCamera) return;

    float Speed = CameraControllerRef->TranslationalSpeed * Scale * DeltaTime;

    CameraControllerRef->CameraPosition += CameraControllerRef->CameraRight * Speed;

    //std::cout << "Move Left or Right = " << Scale << "\t DeltaTime = " << DeltaTime << "\n";

}

void ACameraController::Statics_MoveUpDown(float Scale, float DeltaTime)
{
    if(!CameraControllerRef || !CameraControllerRef->MountedCamera) return;

    float Speed = CameraControllerRef->TranslationalSpeed * Scale * DeltaTime;

    CameraControllerRef->CameraPosition += CameraControllerRef->GlobalUp * Speed;

    //std::cout << "Move up or down = " << Scale << "\t DeltaTime = " << DeltaTime << "\n";
}


void ACameraController::Statics_OnMouseScrollWheel(GLFWwindow* Window, double XOffset, double YOffset) 
{
    if(!CameraControllerRef || !CameraControllerRef->MountedCamera) return;

    CameraControllerRef->CameraFOV -= static_cast<float>(YOffset);
    CameraControllerRef->CameraFOV = glm::clamp(CameraControllerRef->CameraFOV, 1.f, CameraControllerRef->MaxFov);

    //std::cout << "On mouse scroll wheel [ " << XOffset << ", " << YOffset << "]\n";
    // std::cout << "FOV = " << CameraFOV << "\n";
}

void ACameraController::Statics_OnMousePositionChange(GLFWwindow* Window, double MouseX, double MouseY)
{
    if(!CameraControllerRef || !CameraControllerRef->MountedCamera) return;


    if (CameraControllerRef->MouseInfo.bFirstTime)
    {
        CameraControllerRef->MouseInfo.LastX = MouseX;
        CameraControllerRef->MouseInfo.LastY = MouseY;
        CameraControllerRef->MouseInfo.bFirstTime = false;
    }

	//Every Rotation been applied here is Frame rate independent since we're apply rotations based on delta mouse position
	auto DeltaX = static_cast<float>(MouseX - CameraControllerRef->MouseInfo.LastX);
	auto DeltaY = static_cast<float>(MouseY - CameraControllerRef->MouseInfo.LastY);

	DeltaY *= -1.f; //y invert

	//Basically don't apply a Delta Rotation if the left mouse button is not down, 
	//but do update the mouse position to the current values so we don't drastically "jump rotate" on the next click because the delta angle was too large
	if (CameraControllerRef->LeftMouseButtonIsDown()) 
	{
        float FOVScale = CameraControllerRef->CameraFOV / CameraControllerRef->MaxFov; //Later added this to scale mouse sensitivity based on current FOV. 0 to 1 

		CameraControllerRef->CurrentRotation.Yaw   += DeltaX * (CameraControllerRef->MouseSensitivity * FOVScale);
		CameraControllerRef->CurrentRotation.Pitch += DeltaY * (CameraControllerRef->MouseSensitivity * FOVScale);
	}
	CameraControllerRef->CurrentRotation.Pitch = glm::clamp(CameraControllerRef->CurrentRotation.Pitch, -89.f, 89.f);

    //std::cout << "Mouse pos = [ " << MouseX << "px, " << MouseY << "px ]";

	CameraControllerRef->MouseInfo.LastX = MouseX;
	CameraControllerRef->MouseInfo.LastY = MouseY;

	CameraControllerRef->RotateCamera();
}

void ACameraController::RotateCamera()
{
    if(!MountedCamera) return;

    CameraForward.x = cos(glm::radians(CurrentRotation.Yaw)) * std::cos(glm::radians(CurrentRotation.Pitch));
    CameraForward.z = sin(glm::radians(CurrentRotation.Yaw)) * std::cos(glm::radians(CurrentRotation.Pitch));
    CameraForward.y = sin(glm::radians(CurrentRotation.Pitch));

    CameraForward = glm::normalize(CameraForward);
    CameraRight   = glm::normalize(glm::cross(CameraForward, GlobalUp));
    CameraUp      = glm::normalize(glm::cross(CameraRight, CameraForward));
}

void ACameraController::SetMouseSensitivity(const float& Sensitivity)
{
	MouseSensitivity = glm::clamp(Sensitivity, 0.f, 1.0f);
}

void ACameraController::SetTranslationalSpeed(const float& SpeedPerSecond)
{
	TranslationalSpeed = SpeedPerSecond;
}


void ACameraController::SetMaxFOV(const float& MaxFOV) 
{
    this->MaxFov = MaxFOV;
}


bool ACameraController::LeftMouseButtonIsDown() const
{
    return  glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

ACameraController* ACameraController::CameraControllerRef = nullptr;
