#include "Rendering/Renderer/TBForwardRenderer.h"
#include "Rendering/RenderFeature/Geometry_RenderFeature.h"
#include "Rendering/RenderFeature/Background_RenderFeature.h"
#include <glm/glm.hpp>
#include "Camera/CameraBase.h"
#include "Rendering/RenderFeature/TBForward_LightList_RenderFeature.h"
#include "Rendering/RenderFeature/TBForward_Opaque_RenderFeature.h"
#include "Rendering/RenderFeature/TBForward_OIT_DepthPeeling_RenderFeature.h"
#include "Rendering/RenderFeature/SSAO_Occlusion_RenderFeature.h"
#include "Rendering/RenderFeature/AO_Blur_RenderFeature.h"
#include "Rendering/RenderFeature/AO_Cover_RenderFeature.h"
#include "Rendering/RenderFeature/TBForward_OIT_AlphaBuffer_RenderFeature.h"
#include "Rendering/RenderFeature/HBAO_Occlusion_RenderFeature.h"
#include "Rendering/RenderFeature/GTAO_Occlusion_RenderFeature.h"

RTTR_REGISTRATION
{
	rttr::registration::class_<AirEngine::Rendering::Renderer::TBForwardRenderer>("AirEngine::Rendering::Renderer::TBForwardRenderer")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		;
	rttr::registration::class_<AirEngine::Rendering::Renderer::TBForwardRenderer::TBForwardRendererData>("AirEngine::Rendering::Renderer::TBForwardRenderer::TBForwardRendererData")
		.constructor<>()
		(
			rttr::policy::ctor::as_raw_ptr
		)
		;
}

AirEngine::Rendering::Renderer::TBForwardRenderer::TBForwardRenderer()
	: Core::Graphic::Rendering::RendererBase()
{
	UseRenderFeature("Geometry_RenderFeature", new RenderFeature::Geometry_RenderFeature());
	UseRenderFeature("TBForward_LightList_RenderFeature", new RenderFeature::TBForward_LightList_RenderFeature());
	UseRenderFeature("TBForward_Opaque_RenderFeature", new RenderFeature::TBForward_Opaque_RenderFeature());
	UseRenderFeature("Background_RenderFeature", new RenderFeature::Background_RenderFeature());
	UseRenderFeature("TBForward_OIT_DepthPeeling_RenderFeature", new RenderFeature::TBForward_OIT_DepthPeeling_RenderFeature());
	UseRenderFeature("TBForward_OIT_AlphaBuffer_RenderFeature", new RenderFeature::TBForward_OIT_AlphaBuffer_RenderFeature());
	//UseRenderFeature("SSAO_Occlusion_RenderFeature", new RenderFeature::SSAO_Occlusion_RenderFeature());
	UseRenderFeature("HBAO_Occlusion_RenderFeature", new RenderFeature::HBAO_Occlusion_RenderFeature());
	UseRenderFeature("GTAO_Occlusion_RenderFeature", new RenderFeature::GTAO_Occlusion_RenderFeature());
	UseRenderFeature("AO_Blur_RenderFeature", new RenderFeature::AO_Blur_RenderFeature());
	UseRenderFeature("AO_Cover_RenderFeature", new RenderFeature::AO_Cover_RenderFeature());
}

AirEngine::Rendering::Renderer::TBForwardRenderer::~TBForwardRenderer()
{
	delete static_cast<RenderFeature::Geometry_RenderFeature*>(RenderFeature("GeometryRenderFeature"));
}

AirEngine::Rendering::Renderer::TBForwardRenderer::TBForwardRendererData::TBForwardRendererData()
	: Core::Graphic::Rendering::RendererDataBase()
{
}

AirEngine::Rendering::Renderer::TBForwardRenderer::TBForwardRendererData::~TBForwardRendererData()
{
}

AirEngine::Core::Graphic::Rendering::RendererDataBase* AirEngine::Rendering::Renderer::TBForwardRenderer::OnCreateRendererData(Camera::CameraBase* camera)
{
	return new TBForwardRendererData();
}

void AirEngine::Rendering::Renderer::TBForwardRenderer::OnResolveRendererData(Core::Graphic::Rendering::RendererDataBase* rendererData, Camera::CameraBase* camera)
{
	rendererData->RenderFeatureData<RenderFeature::Background_RenderFeature::Background_RenderFeatureData>("Background_RenderFeature")->needClearColorAttachment = true;
	auto geometryFeatureData = rendererData->RenderFeatureData<RenderFeature::Geometry_RenderFeature::Geometry_RenderFeatureData>("Geometry_RenderFeature");
	auto lightListFeatureData = rendererData->RenderFeatureData<RenderFeature::TBForward_LightList_RenderFeature::TBForward_LightList_RenderFeatureData>("TBForward_LightList_RenderFeature");
	auto opaqueFeatureData = rendererData->RenderFeatureData<RenderFeature::TBForward_Opaque_RenderFeature::TBForward_Opaque_RenderFeatureData>("TBForward_Opaque_RenderFeature");
	auto depthPeelingFeatureData = rendererData->RenderFeatureData<RenderFeature::TBForward_OIT_DepthPeeling_RenderFeature::TBForward_OIT_DepthPeeling_RenderFeatureData>("TBForward_OIT_DepthPeeling_RenderFeature");
	auto alphaBufferFeatureData = rendererData->RenderFeatureData<RenderFeature::TBForward_OIT_AlphaBuffer_RenderFeature::TBForward_OIT_AlphaBuffer_RenderFeatureData>("TBForward_OIT_AlphaBuffer_RenderFeature");
	//auto ssaoFeatureData = rendererData->RenderFeatureData<RenderFeature::SSAO_Occlusion_RenderFeature::SSAO_Occlusion_RenderFeatureData>("SSAO_Occlusion_RenderFeature");
	auto hbaoFeatureData = rendererData->RenderFeatureData<RenderFeature::HBAO_Occlusion_RenderFeature::HBAO_Occlusion_RenderFeatureData>("HBAO_Occlusion_RenderFeature");
	auto gtaoFeatureData = rendererData->RenderFeatureData<RenderFeature::GTAO_Occlusion_RenderFeature::GTAO_Occlusion_RenderFeatureData>("GTAO_Occlusion_RenderFeature");
	auto aoBlurFeatureData = rendererData->RenderFeatureData<RenderFeature::AO_Blur_RenderFeature::AO_Blur_RenderFeatureData>("AO_Blur_RenderFeature");
	auto aoCoverFeatureData = rendererData->RenderFeatureData<RenderFeature::AO_Cover_RenderFeature::AO_Cover_RenderFeatureData>("AO_Cover_RenderFeature");

	//ssaoFeatureData->depthTexture = geometryFeatureData->depthTexture;
	//ssaoFeatureData->normalTexture = geometryFeatureData->normalTexture;

	hbaoFeatureData->depthTexture = geometryFeatureData->depthTexture;
	gtaoFeatureData->depthTexture = geometryFeatureData->depthTexture;

	aoBlurFeatureData->normalTexture = geometryFeatureData->normalTexture;
	//aoBlurFeatureData->occlusionTexture = ssaoFeatureData->occlusionTexture;
	//aoBlurFeatureData->occlusionTexture = hbaoFeatureData->occlusionTexture;
	aoBlurFeatureData->occlusionTexture = gtaoFeatureData->occlusionTexture;
	aoBlurFeatureData->iterateCount = 2;

	//aoCoverFeatureData->occlusionTexture = ssaoFeatureData->occlusionTexture;
	aoCoverFeatureData->occlusionTexture = hbaoFeatureData->occlusionTexture;
	aoCoverFeatureData->occlusionTexture = gtaoFeatureData->occlusionTexture;
	aoCoverFeatureData->intensity = 1.5f;

	lightListFeatureData->depthTexture = geometryFeatureData->depthTexture;

	opaqueFeatureData->opaqueLightIndexListsBuffer = lightListFeatureData->opaqueLightIndexListsBuffer;

	depthPeelingFeatureData->transparentLightIndexListsBuffer = lightListFeatureData->transparentLightIndexListsBuffer;
	depthPeelingFeatureData->depthTexture = geometryFeatureData->depthTexture;

	alphaBufferFeatureData->transparentLightIndexListsBuffer = lightListFeatureData->transparentLightIndexListsBuffer;
}

void AirEngine::Rendering::Renderer::TBForwardRenderer::OnDestroyRendererData(Core::Graphic::Rendering::RendererDataBase* rendererData)
{
	delete static_cast<TBForwardRendererData*>(rendererData);
}

void AirEngine::Rendering::Renderer::TBForwardRenderer::PrepareRenderer(Core::Graphic::Rendering::RendererDataBase* rendererData)
{
	PrepareRenderFeature("Background_RenderFeature", rendererData);
	PrepareRenderFeature("Geometry_RenderFeature", rendererData);
	//PrepareRenderFeature("SSAO_Occlusion_RenderFeature", rendererData);
	PrepareRenderFeature("HBAO_Occlusion_RenderFeature", rendererData);
	PrepareRenderFeature("GTAO_Occlusion_RenderFeature", rendererData);
	PrepareRenderFeature("AO_Blur_RenderFeature", rendererData);
	PrepareRenderFeature("TBForward_LightList_RenderFeature", rendererData);
	PrepareRenderFeature("TBForward_Opaque_RenderFeature", rendererData);
	PrepareRenderFeature("AO_Cover_RenderFeature", rendererData);
	switch (static_cast<TBForwardRendererData*>(rendererData)->oitType)
	{
		case OitType::DEPTH_PEELING:
		{
			PrepareRenderFeature("TBForward_OIT_DepthPeeling_RenderFeature", rendererData);
			break;
		}
		case OitType::ALPHA_BUFFER:
		{
			PrepareRenderFeature("TBForward_OIT_AlphaBuffer_RenderFeature", rendererData);
			break;
		}
	}
}

void AirEngine::Rendering::Renderer::TBForwardRenderer::ExcuteRenderer(Core::Graphic::Rendering::RendererDataBase* rendererData, Camera::CameraBase* camera, std::vector<AirEngine::Renderer::Renderer*> const* rendererComponents)
{
	ExcuteRenderFeature("Background_RenderFeature", rendererData, camera, rendererComponents);
	ExcuteRenderFeature("Geometry_RenderFeature", rendererData, camera, rendererComponents);
	//ExcuteRenderFeature("SSAO_Occlusion_RenderFeature", rendererData, camera, rendererComponents);
	ExcuteRenderFeature("HBAO_Occlusion_RenderFeature", rendererData, camera, rendererComponents);
	ExcuteRenderFeature("GTAO_Occlusion_RenderFeature", rendererData, camera, rendererComponents);
	ExcuteRenderFeature("AO_Blur_RenderFeature", rendererData, camera, rendererComponents);
	ExcuteRenderFeature("TBForward_LightList_RenderFeature", rendererData, camera, rendererComponents);
	ExcuteRenderFeature("TBForward_Opaque_RenderFeature", rendererData, camera, rendererComponents);
	ExcuteRenderFeature("AO_Cover_RenderFeature", rendererData, camera, rendererComponents);
	switch (static_cast<TBForwardRendererData*>(rendererData)->oitType)
	{
	case OitType::DEPTH_PEELING:
	{
		ExcuteRenderFeature("TBForward_OIT_DepthPeeling_RenderFeature", rendererData, camera, rendererComponents);
		break;
	}
	case OitType::ALPHA_BUFFER:
	{
		ExcuteRenderFeature("TBForward_OIT_AlphaBuffer_RenderFeature", rendererData, camera, rendererComponents);
		break;
	}
	}
}

void AirEngine::Rendering::Renderer::TBForwardRenderer::SubmitRenderer(Core::Graphic::Rendering::RendererDataBase* rendererData)
{
	SubmitRenderFeature("Background_RenderFeature", rendererData);
	SubmitRenderFeature("Geometry_RenderFeature", rendererData);
	//SubmitRenderFeature("SSAO_Occlusion_RenderFeature", rendererData);
	SubmitRenderFeature("HBAO_Occlusion_RenderFeature", rendererData);
	SubmitRenderFeature("GTAO_Occlusion_RenderFeature", rendererData);
	SubmitRenderFeature("AO_Blur_RenderFeature", rendererData);
	SubmitRenderFeature("TBForward_LightList_RenderFeature", rendererData);
	SubmitRenderFeature("TBForward_Opaque_RenderFeature", rendererData);
	SubmitRenderFeature("AO_Cover_RenderFeature", rendererData);
	switch (static_cast<TBForwardRendererData*>(rendererData)->oitType)
	{
		case OitType::DEPTH_PEELING:
		{
			SubmitRenderFeature("TBForward_OIT_DepthPeeling_RenderFeature", rendererData);
			break;
		}
		case OitType::ALPHA_BUFFER:
		{
			SubmitRenderFeature("TBForward_OIT_AlphaBuffer_RenderFeature", rendererData);
			break;
		}
	}
}

void AirEngine::Rendering::Renderer::TBForwardRenderer::FinishRenderer(Core::Graphic::Rendering::RendererDataBase* rendererData)
{
	FinishRenderFeature("Background_RenderFeature", rendererData);
	FinishRenderFeature("Geometry_RenderFeature", rendererData);
	//FinishRenderFeature("SSAO_Occlusion_RenderFeature", rendererData);
	FinishRenderFeature("HBAO_Occlusion_RenderFeature", rendererData);
	FinishRenderFeature("GTAO_Occlusion_RenderFeature", rendererData);
	FinishRenderFeature("AO_Blur_RenderFeature", rendererData);
	FinishRenderFeature("TBForward_LightList_RenderFeature", rendererData);
	FinishRenderFeature("TBForward_Opaque_RenderFeature", rendererData);
	FinishRenderFeature("AO_Cover_RenderFeature", rendererData);
	switch (static_cast<TBForwardRendererData*>(rendererData)->oitType)
	{
		case OitType::DEPTH_PEELING:
		{
			FinishRenderFeature("TBForward_OIT_DepthPeeling_RenderFeature", rendererData);
			break;
		}
		case OitType::ALPHA_BUFFER:
		{
			FinishRenderFeature("TBForward_OIT_AlphaBuffer_RenderFeature", rendererData);
			break;
		}
	}
}
