#include "Core/Graphic/RenderPass/OpaqueRenderPass.h"
#include "Core/Graphic/Command/CommandBuffer.h"
#include "Core/Graphic/Command/CommandPool.h"
#include "Core/Graphic/CoreObject/Instance.h"
#include "Core/Graphic/Manager/RenderPassManager.h"
#include "Core/Graphic/CoreObject/Window.h"
#include "Core/Graphic/Command/ImageMemoryBarrier.h"
#include "Core/Graphic/Instance/FrameBuffer.h"
#include "Renderer/Renderer.h"
#include "Core/Graphic/Material.h"

void AirEngine::Core::Graphic::RenderPass::OpaqueRenderPass::OnPopulateRenderPassSettings(RenderPassSettings& creator)
{
	creator.AddColorAttachment(
		"ColorAttachment",
		VK_FORMAT_R8G8B8A8_SRGB,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
		VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	);
	creator.AddDepthAttachment(
		"DepthAttachment",
		VK_FORMAT_D32_SFLOAT,
		VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT,
		VK_ATTACHMENT_LOAD_OP_CLEAR,
		VK_ATTACHMENT_STORE_OP_STORE,
		VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	);
	creator.AddSubpass(
		"DrawSubpass",
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		{ "ColorAttachment" },
		"DepthAttachment"
	);
	creator.AddDependency(
		"VK_SUBPASS_EXTERNAL",
		"DrawSubpass",
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		VkPipelineStageFlagBits::VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
		0,
		VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT
	);
}

void AirEngine::Core::Graphic::RenderPass::OpaqueRenderPass::OnPopulateCommandBuffer(Command::CommandPool* commandPool, std::multimap<float, Renderer::Renderer*>& renderDistanceTable, Manager::RenderPassObject* renderPassObject)
{
	_renderCommandPool = commandPool;

	_renderCommandBuffer = commandPool->CreateCommandBuffer("OpaqueCommandBuffer", VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	_renderCommandBuffer->Reset();

	//Render
	_renderCommandBuffer->Reset();
	_renderCommandBuffer->BeginRecord(VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
	//Render queue attachment to attachment layout
	{
		Command::ImageMemoryBarrier depthAttachmentAcquireBarrier = Command::ImageMemoryBarrier
		(
			renderPassObject->_frameBuffers[renderPassObject->_indexMap[Name()]]->Attachment("DepthAttachment"),
			VK_IMAGE_LAYOUT_UNDEFINED,
			VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
			0,
			VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT
		);

		_renderCommandBuffer->AddPipelineBarrier(
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
			{ &depthAttachmentAcquireBarrier }
		);
	}
	{
		Command::ImageMemoryBarrier colorAttachmentAcquireBarrier = Command::ImageMemoryBarrier
		(
			renderPassObject->_frameBuffers[renderPassObject->_indexMap[Name()]]->Attachment("ColorAttachment"),
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL,
			0,
			VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
		);

		_renderCommandBuffer->AddPipelineBarrier(
			VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			{ &colorAttachmentAcquireBarrier }
		);
	}
	VkClearValue colorClearValue{};
	colorClearValue.color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	VkClearValue depthClearValue{};
	depthClearValue.depthStencil.depth = 1.0f;
	_renderCommandBuffer->BeginRenderPass(
		this,
		renderPassObject,
		{ colorClearValue, depthClearValue }
	);
	for (const auto& rendererDistencePair : renderDistanceTable)
	{
		auto& renderer = rendererDistencePair.second;

		_renderCommandBuffer->BindMaterial(renderer->material);
		_renderCommandBuffer->BindMesh(renderer->mesh);
		_renderCommandBuffer->Draw();
	}
	_renderCommandBuffer->EndRenderPass();
	_renderCommandBuffer->EndRecord();
}

void AirEngine::Core::Graphic::RenderPass::OpaqueRenderPass::OnSubmit()
{
	_renderCommandBuffer->Submit({}, {}, { Semaphore() });
}

void AirEngine::Core::Graphic::RenderPass::OpaqueRenderPass::OnClear()
{
	_renderCommandPool->DestoryCommandBuffer("OpaqueCommandBuffer");
}

AirEngine::Core::Graphic::RenderPass::OpaqueRenderPass::OpaqueRenderPass()
	: RenderPassBase("OpaqueRenderPass", 2000)
	, _renderCommandBuffer(nullptr)
	, _renderCommandPool(nullptr)
{
}

AirEngine::Core::Graphic::RenderPass::OpaqueRenderPass::~OpaqueRenderPass()
{
}
