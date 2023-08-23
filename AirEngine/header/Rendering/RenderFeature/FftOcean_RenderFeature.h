#pragma once
#include "Core/Graphic/Rendering/RenderFeatureBase.h"
#include "Core/Graphic/Rendering/RenderPassBase.h"
#include "Core/Graphic/Instance/Buffer.h"
#include "Core/Graphic/Instance/Image.h"
#include "Core/Graphic/Instance/ImageSampler.h"
#include <glm/vec2.hpp>
#include <glm/ext/vector_int2.hpp>

namespace AirEngine
{
	namespace Asset
	{
		class Mesh;
	}
	namespace Core
	{
		namespace Graphic
		{
			namespace Rendering
			{
				class Shader;
				class Material;
			}
			namespace Instance
			{
				class ImageSampler;
			}
		}
	}
	namespace Rendering
	{
		namespace RenderFeature
		{
			class FftOcean_RenderFeature final : public Core::Graphic::Rendering::RenderFeatureBase
			{
			public:
				class FftOcean_RenderPass final : public Core::Graphic::Rendering::RenderPassBase
				{
				private:
					void OnPopulateRenderPassSettings(RenderPassSettings& settings)override;
				public:
					CONSTRUCTOR(FftOcean_RenderPass)
					RTTR_ENABLE(Core::Graphic::Rendering::RenderPassBase)
				};
				struct GenerateFrequencyInfo
				{
					glm::ivec2 imageSize;
					glm::vec2 L;
					glm::ivec2 NM;
					glm::vec2 windDirection;
					float windSpeed;
					float time;
					float a;
					float windDependency;
				};
				class FftOcean_RenderFeatureData final : public Core::Graphic::Rendering::RenderFeatureDataBase
				{
					friend class FftOcean_RenderFeature;
				private:
					bool isInitialized;
					glm::ivec2 imageSize;

					Core::Graphic::Instance::Buffer* gaussianNoiseImageStagingBuffer;
					Core::Graphic::Instance::Image* gaussianNoiseImage;

					GenerateFrequencyInfo generateFrequencyInfo;
					Core::Graphic::Rendering::Shader* generateFrequencyShader;
					Core::Graphic::Rendering::Material* generateFrequencyMaterial;
					Core::Graphic::Instance::Image* heightFrequencyImage;
					Core::Graphic::Instance::Image* xyFrequencyImage;
					Core::Graphic::Instance::Image* xySlopeFrequencyImage;
				public:

					CONSTRUCTOR(FftOcean_RenderFeatureData)
					RTTR_ENABLE(Core::Graphic::Rendering::RenderFeatureDataBase)
				};

				CONSTRUCTOR(FftOcean_RenderFeature)

			private:
				Core::Graphic::Rendering::RenderPassBase* _renderPass;
				std::string _renderPassName;
				Core::Graphic::Instance::ImageSampler* _pointSampler;
				Core::Graphic::Instance::ImageSampler* _linearSampler;

				Core::Graphic::Rendering::RenderFeatureDataBase* OnCreateRenderFeatureData(Camera::CameraBase* camera)override;
				void OnResolveRenderFeatureData(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Camera::CameraBase* camera)override;
				void OnDestroyRenderFeatureData(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData)override;

				void OnPrepare(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData)override;
				void OnExcute(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Core::Graphic::Command::CommandBuffer* commandBuffer, Camera::CameraBase* camera, std::vector<AirEngine::Renderer::Renderer*>const* rendererComponents)override;
				void OnSubmit(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Core::Graphic::Command::CommandBuffer* commandBuffer)override;
				void OnFinish(Core::Graphic::Rendering::RenderFeatureDataBase* renderFeatureData, Core::Graphic::Command::CommandBuffer* commandBuffer)override;

				RTTR_ENABLE(Core::Graphic::Rendering::RenderFeatureBase)
			};
		}
	}
}