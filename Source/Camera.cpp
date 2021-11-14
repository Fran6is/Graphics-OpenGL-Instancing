#include "Camera.h"
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

extern unsigned int SCR_WIDTH;
extern unsigned int SCR_HEIGHT;

Camera::Camera() : Camera(FCameraDefinition{ ECameraType::Perspective, 45.f, 1.f, 0.1f, 100.f })
{

}
Camera::Camera(const FCameraDefinition& CameraSpecs) : CameraDefinition{CameraSpecs}
{

}

void Camera::SetCameraSpecification(const FCameraDefinition& NewSpec)
{
	CameraDefinition = NewSpec;
}

glm::mat4 Camera::MakeProjectionMatrix(const FCameraDefinition& CameraSpecs)
{
	if (CameraSpecs.CameraType == ECameraType::Perspective)
	{
		return glm::perspective(glm::radians(CameraSpecs.FOV), (float)SCR_WIDTH/SCR_HEIGHT, CameraSpecs.NearPlane, CameraSpecs.FarPlane);
	}
	else
	{
		return glm::ortho(
			CameraSpecs.Orthographic_Left, CameraSpecs.Orthographic_Right, 
			CameraSpecs.Orthographic_Bottom, CameraSpecs.Orthographic_Top
			,CameraSpecs.NearPlane, CameraSpecs.FarPlane
		);
	}
}

void Camera::SetCameraUp(const glm::vec3 &cameraUp)
{
    CameraUp.x = cameraUp.x;
    CameraUp.y = cameraUp.y;
    CameraUp.z = cameraUp.z;
}

void Camera::SetCameraRight(const glm::vec3 &cameraRight)
{
    CameraRight.x = cameraRight.x;
    CameraRight.y = cameraRight.y;
    CameraRight.z = cameraRight.z;
}

void Camera::SetCameraForward(const glm::vec3 &cameraForward)
{
    CameraForward.x = cameraForward.x;
    CameraForward.y = cameraForward.y;
    CameraForward.z = cameraForward.z;
}

void Camera::SetCameraPosition(const glm::vec3 &cameraPosition)
{
    CameraPosition.x = cameraPosition.x;
    CameraPosition.y = cameraPosition.y;
    CameraPosition.z = cameraPosition.z;
}

bool Camera::SaveStateToFile(const Camera &Cam, const std::string &FilePath)
{
    std::ofstream File(FilePath, std::ios::trunc);
    if (!File.is_open())
    {
        std::cout << "Unable to open or find file at \"" << FilePath << "\" \tACameraController::SaveStateToFile\n";
        return false;
    }

    File << Cam.CameraForward.x << " " << Cam.CameraForward.y << " " << Cam.CameraForward.z << " \tCamera forward \n";
    File << Cam.CameraRight.x   << " " << Cam.CameraRight.y   << " " << Cam.CameraRight.z   << " \tCamera right \n";
    File << Cam.CameraUp.x      << " " << Cam.CameraUp.y      << " " << Cam.CameraUp.z      << " \tCamera up \n";
    File << Cam.CameraPosition.x  << " " << Cam.CameraPosition.y      << " " << Cam.CameraPosition.z      << " \tCamera Position \n";
    File << Cam.CameraDefinition.FOV  << " 0 0 " << " \tCamera FOV \n";

    std::cout << "\nCamera state saved to " << FilePath << "\n";

    File.close();
    return true;
}

bool Camera::LoadStateFromFile(Camera &Cam, const std::string &FilePath)
{
    std::ifstream File{ FilePath };

    if (!File.is_open())
    {
        std::cout << "Unable to open or find file at \"" << FilePath << "\" \tACameraController::LoadStateFromFile\n";
        return false;
    }
    std::string temp{};

    std::vector <glm::vec3> BasisVectorsAndPosition;
    BasisVectorsAndPosition.reserve(5);

    while (std::getline(File, temp))
    {
        std::istringstream iss{temp};
        glm::vec3 Retrieved(0.f);
        iss >> Retrieved.x >> Retrieved.y >> Retrieved.z;

        //iss.ignore(1000, '\n');
        //std::cout << Retrieved << "\n";

        BasisVectorsAndPosition.emplace_back(Retrieved);
        if (BasisVectorsAndPosition.size() == 5) break;
    }

    if (BasisVectorsAndPosition.size() == 5) //Must have emplaced/created 5 vectors, i.e read from 5 lines in the file
    {
        Cam.CameraForward  = BasisVectorsAndPosition.at(0);
        Cam.CameraRight    = BasisVectorsAndPosition.at(1);
        Cam.CameraUp       = BasisVectorsAndPosition.at(2);
        Cam.CameraPosition = BasisVectorsAndPosition.at(3);

        Cam.CameraDefinition.FOV = BasisVectorsAndPosition.at(4).x;
    }

    File.close();
    return true;
}


