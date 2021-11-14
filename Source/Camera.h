#pragma once
#include <iostream>
#include <glm\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class ECameraType
{
	Perspective,
	Orthographic
};
struct FCameraDefinition
{
	ECameraType CameraType = ECameraType::Perspective;
	float FOV{45.f};
	float AspectRatio{};
	float NearPlane{0.1f};
	float FarPlane{1000.f};
	float Orthographic_Left{-100.f};
	float Orthographic_Right{+100.f};
	float Orthographic_Bottom{-100.f};
	float Orthographic_Top{100.f};
};

class Camera
{
    friend class ACameraController;

	FCameraDefinition CameraDefinition;

	glm::vec3 CameraUp = glm::vec3(0, 1, 0);
    glm::vec3 CameraRight = glm::vec3(1, 0, 0);
    glm::vec3 CameraForward = glm::vec3(0, 0, -1);
    glm::vec3 CameraPosition = glm::vec3(0, 0, 20);

public:
	Camera();
	explicit Camera(const FCameraDefinition& CameraSpecs);

	//Getters
    inline const glm::vec3& GetUpVector() const{ return CameraUp; }
    inline const glm::vec3& GetRightVector() const{ return CameraRight; }
    inline const glm::vec3& GetForwardVector() const{ return CameraForward; }
    inline const glm::vec3& GetPositionVector() const{ return CameraPosition; }
    inline glm::vec3 GetZBasisVector() const{ return -CameraForward; }

    inline glm::mat4 GetProjectionMatrix() const{ return MakeProjectionMatrix(CameraDefinition); }
    inline glm::mat4 GetViewMatrix() const { return glm::lookAt(CameraPosition, CameraPosition + CameraForward, CameraUp);}

	//Basically what the 'lookat' method is doing but using transpose for the inverse because it's computationally less expensive
	// glm::mat4 GetViewMatrix() const 
	// { 
	// 	glm::mat4 CameraMatrix(1.f);
	// 	CameraMatrix[0] = glm::vec4(CameraRight, 0.f);
	// 	CameraMatrix[1] = glm::vec4(CameraUp, 0.f);
	// 	CameraMatrix[2] = glm::vec4(-CameraForward, 0.f);
	// 	CameraMatrix[3] = glm::vec4(CameraPosition, 1.f);
		
	// 	return glm::inverse(CameraMatrix);
	// 	return glm::lookAt(CameraPosition, CameraPosition + CameraForward, CameraUp);
	// };


	inline const FCameraDefinition& GetCameraSpecification() const { return CameraDefinition; }

	//Setters
	void SetCameraSpecification(const FCameraDefinition& NewSpec);

    void SetCameraPosition(const glm::vec3 &cameraPosition);
    void SetCameraUp(const glm::vec3 &cameraUp);
    void SetCameraRight(const glm::vec3 &cameraRight);
    void SetCameraForward(const glm::vec3 &cameraForward);

	static glm::mat4 MakeProjectionMatrix(const FCameraDefinition& CameraSpecs);
    static bool SaveStateToFile(const Camera& Cam,  const std::string& FilePath);
    static bool LoadStateFromFile(Camera& Cam, const std::string& FilePath);
};