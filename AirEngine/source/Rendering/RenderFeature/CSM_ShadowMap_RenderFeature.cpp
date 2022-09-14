#include "Rendering/RenderFeature/CSM_ShadowMap_RenderFeature.h"
#include "Core/Graphic/CoreObject/Instance.h"
#include "Core/Graphic/Manager/RenderPassManager.h"
#include "Core/Graphic/Rendering/FrameBuffer.h"
#include "Camera/CameraBase.h"
#include "Core/Graphic/Command/CommandBuffer.h"
#include "Core/Graphic/Command/ImageMemoryBarrier.h"
#include "Asset/Mesh.h"
#include "Renderer/Renderer.h"
#include "Utils/OrientedBoundingBox.h"
#include "Core/Logic/Object/GameObject.h"
#include "Core/Logic/Object/Transform.h"
#include "Core/Graphic/Rendering/Material.h"
#include "Core/Graphic/Manager/LightManager.h"
#include "Core/Graphic/CoreObject/Instance.h"
#include <set>
#include "Light/LightBase.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderPass>("AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderPass")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		;
	rttr::registration::class_<AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderFeatureData>("AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderFeatureData")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		;
	rttr::registration::class_<AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature>("AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		;
}

void AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderPass::OnPopulateRenderPassSettings(RenderPassSettings& creator)
{
	creator.AddDepthAttachment(
		"DepthAttachment",
		VK_FORMAT_D32_SFLOAT,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
		VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	);
	creator.AddSubpass(
		"DrawSubpass",
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		{},
		"DepthAttachment"
	);
	creator.AddDependency(
		"VK_SUBPASS_EXTERNAL",
		"DrawSubpass",
		VkPipelineStageFlagBits::VK_PIPELINE_STAGE_TRANSFER_BIT,
		VkPipelineStageFlagBits::VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
		VkAccessFlagBits::VK_ACCESS_TRANSFER_WRITE_BIT,
		VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT
	);
	creator.AddDependency(
		"DrawSubpass",
		"VK_SUBPASS_EXTERNAL",
		VkPipelineStageFlagBits::VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
		VkPipelineStageFlagBits::VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
		VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
		VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT
	);
}

AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderPass::CSM_ShadowMap_RenderPass()
	: RenderPassBase()
{
}

AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderPass::~CSM_ShadowMap_RenderPass()
{
}

AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderFeatureData::CSM_ShadowMap_RenderFeatureData()
	: RenderFeatureDataBase()
	, shadowImages()
	, shadowFrameBuffers()
	, lightCameraInfoBuffer(nullptr)
	, lightCameraInfoStagingBuffer(nullptr)
	, frustumSplitScales()
	, lightCameraCompensationDistances()
	, shadowImageResolutions()
{
	frustumSplitScales.fill(1.0 / CASCADE_COUNT);
	lightCameraCompensationDistances.fill(20);
	shadowImageResolutions.fill(1024);
}

AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderFeatureData::~CSM_ShadowMap_RenderFeatureData()
{
}

AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderFeature()
	: RenderFeatureBase()
	, _shadowMapRenderPass(Core::Graphic::CoreObject::Instance::RenderPassManager().LoadRenderPass<CSM_ShadowMap_RenderPass>())
	, _shadowMapRenderPassName(rttr::type::get<CSM_ShadowMap_RenderPass>().get_name().to_string())
{

}

AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::~CSM_ShadowMap_RenderFeature()
{
	Core::Graphic::CoreObject::Instance::RenderPassManager().UnloadRenderPass<CSM_ShadowMap_RenderPass>();
}

void AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::CSM_ShadowMap_RenderFeatureData::Refresh()
{
	for (auto i = 0; i < CASCADE_COUNT; i++)
	{
		VkExtent2D extent = { shadowImageResolutions[i], shadowImageResolutions[i]};
		delete shadowImages[i];
		delete shadowFrameBuffers[i];
		shadowImages[i] = Core::Graphic::Instance::Image::Create2DImage(
			extent,
			VkFormat::VK_FORMAT_D32_SFLOAT,
			VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT,
			VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT
		);
		shadowFrameBuffers[i] = new Core::Graphic::Rendering::FrameBuffer(
			shadowMapRenderPass,
			{
				{"DepthAttachment", shadowImages[i]},
			}
		);
	}
}

AirEngine::Core::Graphic::Rendering::RenderFeatureDataBase* AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::OnCreateRenderFeatureData(Camera::CameraBase* camera)
{
	auto featureData = new CSM_ShadowMap_RenderFeatureData();
	featureData->shadowMapRenderPass = _shadowMapRenderPass;

	featureData->lightCameraInfoBuffer = new Core::Graphic::Instance::Buffer(
		sizeof(LightCameraInfo),
		VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	);
	featureData->lightCameraInfoStagingBuffer = new Core::Graphic::Instance::Buffer(
		sizeof(LightCameraInfo) * CASCADE_COUNT,
		VkBufferUsageFlagBits::VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
	);

	return featureData;
}

void AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::OnResolveRenderFeatureData(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Camera::CameraBase* camera)
{
	auto featureData = static_cast<CSM_ShadowMap_RenderFeatureData*>(renderFeatureData);
	featureData->Refresh();
}

void AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::OnDestroyRenderFeatureData(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData)
{
	auto featureData = static_cast<CSM_ShadowMap_RenderFeatureData*>(renderFeatureData);
	delete featureData->lightCameraInfoBuffer;
	delete featureData->lightCameraInfoStagingBuffer;
	for (auto i = 0; i < CASCADE_COUNT; i++)
	{
		delete featureData->shadowImages[i];
		delete featureData->shadowFrameBuffers[i];
	}
	delete featureData;
}

void AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::OnPrepare(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData)
{

}

void AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::OnExcute(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Core::Graphic::Command::CommandBuffer* commandBuffer, Camera::CameraBase* camera, std::vector<AirEngine::Renderer::Renderer*> const* rendererComponents)
{
	auto featureData = static_cast<CSM_ShadowMap_RenderFeatureData*>(renderFeatureData);
	
	glm::vec3 angularPointVPositions[8];
	camera->GetAngularPointVPosition(angularPointVPositions);

	glm::vec3 subAngularPointVPositions[CASCADE_COUNT][8];
	{
		std::array<float, CASCADE_COUNT> frustumSplitRatio = featureData->frustumSplitScales;
		for (int i = 1; i < CASCADE_COUNT; i++)
		{
			frustumSplitRatio[i] = frustumSplitRatio[i - 1] + featureData->frustumSplitScales[i];
		}

		for (int i = 0; i < CASCADE_COUNT; i++)
		{
			if (i == 0)
			{
				subAngularPointVPositions[i][0] = angularPointVPositions[0];
				subAngularPointVPositions[i][1] = angularPointVPositions[1];
				subAngularPointVPositions[i][2] = angularPointVPositions[2];
				subAngularPointVPositions[i][3] = angularPointVPositions[3];
			}
			else
			{
				subAngularPointVPositions[i][0] = (1 - frustumSplitRatio[i - 1]) * angularPointVPositions[0] + frustumSplitRatio[i - 1] * angularPointVPositions[4];
				subAngularPointVPositions[i][1] = (1 - frustumSplitRatio[i - 1]) * angularPointVPositions[1] + frustumSplitRatio[i - 1] * angularPointVPositions[5];
				subAngularPointVPositions[i][2] = (1 - frustumSplitRatio[i - 1]) * angularPointVPositions[2] + frustumSplitRatio[i - 1] * angularPointVPositions[6];
				subAngularPointVPositions[i][3] = (1 - frustumSplitRatio[i - 1]) * angularPointVPositions[3] + frustumSplitRatio[i - 1] * angularPointVPositions[7];
			}

			if (i == CASCADE_COUNT - 1)
			{
				subAngularPointVPositions[i][4] = angularPointVPositions[4];
				subAngularPointVPositions[i][5] = angularPointVPositions[5];
				subAngularPointVPositions[i][6] = angularPointVPositions[6];
				subAngularPointVPositions[i][7] = angularPointVPositions[7];
			}
			else
			{
				subAngularPointVPositions[i][4] = (1 - frustumSplitRatio[i]) * angularPointVPositions[0] + frustumSplitRatio[i] * angularPointVPositions[4];
				subAngularPointVPositions[i][5] = (1 - frustumSplitRatio[i]) * angularPointVPositions[1] + frustumSplitRatio[i] * angularPointVPositions[5];
				subAngularPointVPositions[i][6] = (1 - frustumSplitRatio[i]) * angularPointVPositions[2] + frustumSplitRatio[i] * angularPointVPositions[6];
				subAngularPointVPositions[i][7] = (1 - frustumSplitRatio[i]) * angularPointVPositions[3] + frustumSplitRatio[i] * angularPointVPositions[7];
			}
		}
	}

	glm::vec3 sphereCenterVPositions[CASCADE_COUNT];
	float sphereRadius[CASCADE_COUNT];
	{
		for (int i = 0; i < CASCADE_COUNT; i++)
		{
			float len = std::abs(subAngularPointVPositions[i][0].z - subAngularPointVPositions[i][4].z);
			float a2 = glm::dot(subAngularPointVPositions[i][0] - subAngularPointVPositions[i][2], subAngularPointVPositions[i][0] - subAngularPointVPositions[i][2]);
			float b2 = glm::dot(subAngularPointVPositions[i][4] - subAngularPointVPositions[i][6], subAngularPointVPositions[i][4] - subAngularPointVPositions[i][6]);
			float x = len / 2 - (a2 - b2) / 8 / len;

			sphereCenterVPositions[i] = glm::vec3(0, 0, subAngularPointVPositions[i][0].z - x);
			sphereRadius[i] = std::sqrt(a2 / 4 + x * x);
		}
	}

	glm::vec3 lightVPositions[CASCADE_COUNT];
	{
		auto lightInfo = Core::Graphic::CoreObject::Instance::LightManager().MainLightInfo();
		glm::mat4 cameraV = camera->ViewMatrix();
		glm::vec3 lightVPosition = cameraV * glm::vec4(lightInfo.position, 1);
		glm::vec3 lightVDirection = glm::normalize(glm::vec3(cameraV * glm::vec4(lightInfo.position + lightInfo.direction, 1)) - lightVPosition);

		for (int i = 0; i < CASCADE_COUNT; i++)
		{
			lightVPositions[i] = sphereCenterVPositions[i] - lightVDirection * (sphereRadius[i] + featureData->lightCameraCompensationDistances[i]);
		}
	}

	LightCameraInfo lightCameraInfos[CASCADE_COUNT];
	{
		auto light = Core::Graphic::CoreObject::Instance::LightManager().MainLight();
		glm::mat4 cameraV = camera->ViewMatrix();
		glm::mat4 lightM = light->GameObject()->transform.ModelMatrix();
		glm::vec3 lightVPosition = cameraV * lightM * glm::vec4(0, 0, 0, 1);
		glm::vec3 lightVUp = glm::normalize(glm::vec3(cameraV * lightM * glm::vec4(0, 1, 0, 1)) - lightVPosition);
		for (int i = 0; i < CASCADE_COUNT; i++)
		{
			float halfWidth = sphereRadius[i];
			float flatDistence = sphereRadius[i] + sphereRadius[i] + featureData->lightCameraCompensationDistances[i];
			lightCameraInfos[i].projection = glm::mat4(
				1.0 / halfWidth, 0, 0, 0,
				0, 1.0 / halfWidth, 0, 0,
				0, 0, -1.0 / flatDistence, 0,
				0, 0, 0, 1
			);

			auto m = glm::lookAt(lightVPositions[i], sphereCenterVPositions[i], lightVUp);
			lightCameraInfos[i].view = m * cameraV;

			lightCameraInfos[i].viewProjection = lightCameraInfos[i].projection * lightCameraInfos[i].view;
		}

		featureData->lightCameraInfoStagingBuffer->WriteData(&lightCameraInfos, sizeof(LightCameraInfo) * CASCADE_COUNT);
	}

	struct RendererWrapper
	{
		Core::Graphic::Rendering::Material* material;
		Asset::Mesh* mesh;
	};
	std::vector<RendererWrapper> wrappers = std::vector<RendererWrapper>();
	for (const auto& rendererComponent : *rendererComponents)
	{
		auto material = rendererComponent->GetMaterial(_shadowMapRenderPassName);
		if (material == nullptr) continue;

		material->SetUniformBuffer("lightCameraInfo", featureData->lightCameraInfoBuffer);
		material->SetUniformBuffer("meshObjectInfo", rendererComponent->ObjectInfoBuffer());

		wrappers.push_back({ material , rendererComponent->mesh });
	}

	commandBuffer->Reset();
	commandBuffer->BeginRecord(VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	for (int i = 0; i < CASCADE_COUNT; i++)
	{
		commandBuffer->CopyBuffer(featureData->lightCameraInfoStagingBuffer, sizeof(LightCameraInfo) * i, featureData->lightCameraInfoBuffer, 0, sizeof(LightCameraInfo));

		VkClearValue depthClearValue{};
		depthClearValue.depthStencil.depth = 1.0f;
		commandBuffer->BeginRenderPass(
			_shadowMapRenderPass,
			featureData->shadowFrameBuffers[i],
			{
				{"DepthAttachment", depthClearValue}
			}
		);

		for (const auto& wrapper : wrappers)
		{
			commandBuffer->DrawMesh(wrapper.mesh, wrapper.material);
		}

		commandBuffer->EndRenderPass();
	}

	commandBuffer->EndRecord();
}

void AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::OnSubmit(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Core::Graphic::Command::CommandBuffer* commandBuffer)
{
	commandBuffer->Submit();
}

void AirEngine::Rendering::RenderFeature::CSM_ShadowMap_RenderFeature::OnFinish(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Core::Graphic::Command::CommandBuffer* commandBuffer)
{
	commandBuffer->WaitForFinish();
}
