#pragma once
#include "Core/Graphic/Rendering/RenderFeatureBase.h"
#include "Core/Graphic/Rendering/RenderPassBase.h"
#include "Core/Graphic/Instance/Buffer.h"
#include "Core/Graphic/Instance/Image.h"
#include <glm/vec2.hpp>

namespace AirEngine
{
	namespace Rendering
	{
		namespace RenderFeature
		{
			class ClearDepthAttachment_RenderFeature final : public Core::Graphic::Rendering::RenderFeatureBase
			{
			public:
				class ClearDepthAttachment_RenderFeatureData final : public Core::Graphic::Rendering::RenderFeatureDataBase
				{
					friend class ClearDepthAttachment_RenderFeature;
				public:
					float clearValue;
					CONSTRUCTOR(ClearDepthAttachment_RenderFeatureData)
					RTTR_ENABLE(Core::Graphic::Rendering::RenderFeatureDataBase)
				};

				CONSTRUCTOR(ClearDepthAttachment_RenderFeature)

			private:

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