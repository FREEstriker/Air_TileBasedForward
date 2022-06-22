#include "Camera/CameraBase.h"
#include "Core/Logic/Object/GameObject.h"
#include <glm/vec4.hpp>
#include "Utils/Log.h"
#include <rttr/registration>
#include "Core/Graphic/Instance/Buffer.h"
#include "Core/Graphic/Command/CommandBuffer.h"
#include "Asset/Mesh.h"
#include "Core/Graphic/Shader.h"
#include "Asset/TextureCube.h"
#include "Core/Graphic/Instance/Image.h"
#include "Core/Graphic/Instance/ImageSampler.h"
RTTR_REGISTRATION
{
	using namespace rttr;
	registration::class_<AirEngine::Camera::CameraBase>("AirEngine::Camera::Camera");
}

glm::mat4 AirEngine::Camera::CameraBase::ViewMatrix()
{
	glm::vec3 eye = _modelMatrix * glm::vec4(0, 0, 0, 1);
	glm::vec3 center = glm::normalize(glm::vec3(_modelMatrix * glm::vec4(0, 0, -1, 1)));
	glm::vec3 up = glm::normalize(glm::vec3(_modelMatrix * glm::vec4(0, 1, 0, 0)));

	return glm::lookAt(eye, center, up);
}

const glm::mat4* AirEngine::Camera::CameraBase::ModelMatrix()
{
	return &_modelMatrix;
}

const glm::mat4* AirEngine::Camera::CameraBase::ProjectionMatrix()
{
	OnSetProjectionMatrix(_projectionMatrix);
	return &_projectionMatrix;
}

const glm::vec4* AirEngine::Camera::CameraBase::ClipPlanes()
{
	OnSetClipPlanes(_cameraData.clipPlanes);
	return _cameraData.clipPlanes;
}

void AirEngine::Camera::CameraBase::RefreshCameraData()
{
	_cameraData.type = static_cast<int>(cameraType);
	_cameraData.nearFlat = nearFlat;
	_cameraData.farFlat = farFlat;
	_cameraData.aspectRatio = aspectRatio;
	_cameraData.position = _modelMatrix * glm::vec4(0, 0, 0, 1);
	OnSetParameter(_cameraData.parameter);
	_cameraData.forward = glm::normalize(glm::vec3(_modelMatrix * glm::vec4(0, 0, -1, 0)));
	_cameraData.right = glm::normalize(glm::vec3(_modelMatrix * glm::vec4(1, 0, 0, 0)));
	OnSetClipPlanes(_cameraData.clipPlanes);

	_buffer->WriteData(&_cameraData, sizeof(CameraData));
}

AirEngine::Core::Graphic::Instance::Buffer* AirEngine::Camera::CameraBase::CameraDataBuffer()
{
	return _buffer;
}

void AirEngine::Camera::CameraBase::OnUpdate()
{
	_modelMatrix = _gameObject->transform.ModelMatrix();
}

AirEngine::Camera::CameraBase::CameraBase(CameraType cameraType)
	: Component(ComponentType::CAMERA)
	, cameraType(cameraType)
	, nearFlat(3.0f)
	, farFlat(100.0f)
	, aspectRatio(16.0f / 9.0f)
	, _cameraData()
	, _modelMatrix(glm::mat4(1.0f))
	, _projectionMatrix(glm::mat4(1.0f))
	, _buffer(new Core::Graphic::Instance::Buffer(sizeof(CameraData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
{
}

AirEngine::Camera::CameraBase::~CameraBase()
{
	delete _buffer;
}