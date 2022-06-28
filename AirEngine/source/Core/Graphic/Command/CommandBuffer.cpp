#include "Core/Graphic/Command/CommandBuffer.h"
#include "Core/Graphic/Command/CommandPool.h"
#include "Core/Graphic/CoreObject/Instance.h"
#include "Utils/Log.h"
#include "Core/Graphic/Instance/Buffer.h"
#include "Core/Graphic/Instance/Image.h"
#include "Core/Graphic/Command/Semaphore.h"
#include "Core/Graphic/Shader.h"
#include "Core/Graphic/Material.h"
#include "Core/Graphic/Command/ImageMemoryBarrier.h"
#include "Asset/Mesh.h"
#include "Core/Graphic/RenderPass/RenderPassBase.h"
#include "Core/Graphic/Manager/RenderPassManager.h"
#include "Core/Graphic/Instance/FrameBuffer.h"

AirEngine::Core::Graphic::Command::CommandBuffer::CommandBuffer(std::string name, CommandPool* parentCommandPool, VkCommandBufferLevel level)
    : _name(name)
    , _parentCommandPool(parentCommandPool)
    , _commandData()
    , _vkCommandBuffer(VK_NULL_HANDLE)
    , _vkFence(VK_NULL_HANDLE)
{
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    Utils::Log::Exception("Failed to create synchronization objects for a command buffer.", vkCreateFence(Graphic::CoreObject::Instance::VkDevice_(), &fenceInfo, nullptr, &_vkFence));

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = parentCommandPool->_vkCommandPool;
    allocInfo.level = level;
    allocInfo.commandBufferCount = 1;

    Utils::Log::Exception("Failed to allocate command buffers.", vkAllocateCommandBuffers(Graphic::CoreObject::Instance::VkDevice_(), &allocInfo, &_vkCommandBuffer));

}
AirEngine::Core::Graphic::Command::CommandBuffer::~CommandBuffer()
{
    vkFreeCommandBuffers(Graphic::CoreObject::Instance::VkDevice_(), _parentCommandPool->_vkCommandPool, 1, &_vkCommandBuffer);
    vkDestroyFence(Graphic::CoreObject::Instance::VkDevice_(), _vkFence, nullptr);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::Reset()
{
    vkResetFences(Graphic::CoreObject::Instance::VkDevice_(), 1, &_vkFence);
    vkResetCommandBuffer(_vkCommandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::BeginRecord(VkCommandBufferUsageFlags flag)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = flag;

    vkBeginCommandBuffer(_vkCommandBuffer, &beginInfo);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::AddPipelineBarrier(VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, std::vector<ImageMemoryBarrier*> imageMemoryBarriers)
{
    std::vector< VkImageMemoryBarrier> vkBarriers = std::vector< VkImageMemoryBarrier>();
    for (const auto& imageMemoryBarrier : imageMemoryBarriers)
    {
        vkBarriers.insert(vkBarriers.end(), imageMemoryBarrier->VkImageMemoryBarriers().begin(), imageMemoryBarrier->VkImageMemoryBarriers().end());
    }
    vkCmdPipelineBarrier(
        _vkCommandBuffer,
        srcStageMask, dstStageMask,
        0,
        0, nullptr,
        0, nullptr,
        static_cast<uint32_t>(vkBarriers.size()), vkBarriers.data()
    );
}
void AirEngine::Core::Graphic::Command::CommandBuffer::AddPipelineBarrier(VkDependencyFlags dependencyFlag, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, std::vector<ImageMemoryBarrier*> imageMemoryBarriers)
{
    std::vector< VkImageMemoryBarrier> vkBarriers = std::vector< VkImageMemoryBarrier>();
    for (const auto& imageMemoryBarrier : imageMemoryBarriers)
    {
        vkBarriers.insert(vkBarriers.end(), imageMemoryBarrier->VkImageMemoryBarriers().begin(), imageMemoryBarrier->VkImageMemoryBarriers().end());
    }
    vkCmdPipelineBarrier(
        _vkCommandBuffer,
        srcStageMask, dstStageMask,
        dependencyFlag,
        0, nullptr,
        0, nullptr,
        static_cast<uint32_t>(vkBarriers.size()), vkBarriers.data()
    );

}

void AirEngine::Core::Graphic::Command::CommandBuffer::CopyBufferToImage(Instance::Buffer* srcBuffer, Instance::Image* dstImage, VkImageLayout dstImageLayout)
{
    auto layerCount = dstImage->LayerCount();
    auto layerSize = dstImage->PerLayerSize();
    auto subresources = dstImage->VkImageSubresourceLayers_();
    std::vector< VkBufferImageCopy> infos = std::vector<VkBufferImageCopy>(layerCount);
    for (uint32_t i = 0; i < layerCount; i++)
    {
        auto& region = infos[i];

        region.bufferOffset = layerSize * i;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource = subresources[i];
        region.imageOffset = { 0, 0, 0 };
        region.imageExtent = dstImage->VkExtent3D_();
    }

    vkCmdCopyBufferToImage(_vkCommandBuffer, srcBuffer->VkBuffer_(), dstImage->VkImage_(), dstImageLayout, static_cast<uint32_t>(layerCount), infos.data());
}

void AirEngine::Core::Graphic::Command::CommandBuffer::CopyBuffer(Instance::Buffer* srcBuffer, Instance::Buffer* dstBuffer)
{
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = srcBuffer->Offset();
    copyRegion.dstOffset = dstBuffer->Offset();
    copyRegion.size = srcBuffer->Size();
    vkCmdCopyBuffer(_vkCommandBuffer, srcBuffer->VkBuffer_(), dstBuffer->VkBuffer_(), 1, &copyRegion);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::CopyBuffer(Instance::Buffer* srcBuffer, VkDeviceSize srcOffset, Instance::Buffer* dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size)
{
    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = srcOffset;
    copyRegion.dstOffset = dstOffset;
    copyRegion.size = size;
    vkCmdCopyBuffer(_vkCommandBuffer, srcBuffer->VkBuffer_(), dstBuffer->VkBuffer_(), 1, &copyRegion);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::EndRecord()
{
    vkEndCommandBuffer(_vkCommandBuffer);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::Submit(std::vector<Command::Semaphore*> waitSemaphores, std::vector<VkPipelineStageFlags> waitStages, std::vector<Command::Semaphore*> signalSemaphores)
{
    std::vector <VkSemaphore> wait = std::vector <VkSemaphore>(waitSemaphores.size());
    for (uint32_t i = 0; i < waitSemaphores.size(); i++)
    {
        wait[i] = waitSemaphores[i]->VkSemphore_();
    }
    std::vector <VkSemaphore> signal = std::vector <VkSemaphore>(signalSemaphores.size());
    for (uint32_t i = 0; i < signalSemaphores.size(); i++)
    {
        signal[i] = signalSemaphores[i]->VkSemphore_();
    }
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_vkCommandBuffer;
    submitInfo.waitSemaphoreCount = static_cast<uint32_t>(wait.size());
    submitInfo.pWaitSemaphores = wait.data();
    submitInfo.pWaitDstStageMask = waitStages.data();
    submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signal.size());
    submitInfo.pSignalSemaphores = signal.data();

    {
        std::unique_lock<std::mutex> lock(CoreObject::Instance::Queue_(_parentCommandPool->_queueName)->mutex);

        vkQueueSubmit(CoreObject::Instance::Queue_(_parentCommandPool->_queueName)->queue, 1, &submitInfo, _vkFence);
    }
}

void AirEngine::Core::Graphic::Command::CommandBuffer::Submit(std::vector<Command::Semaphore*> signalSemaphores)
{
    std::vector <VkSemaphore> signal = std::vector <VkSemaphore>(signalSemaphores.size());
    for (uint32_t i = 0; i < signalSemaphores.size(); i++)
    {
        signal[i] = signalSemaphores[i]->VkSemphore_();
    }
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_vkCommandBuffer;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signal.size());
    submitInfo.pSignalSemaphores = signal.data();

    {
        std::unique_lock<std::mutex> lock(CoreObject::Instance::Queue_(_parentCommandPool->_queueName)->mutex);

        vkQueueSubmit(CoreObject::Instance::Queue_(_parentCommandPool->_queueName)->queue, 1, &submitInfo, _vkFence);
    }
}

void AirEngine::Core::Graphic::Command::CommandBuffer::Submit()
{
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &_vkCommandBuffer;
    submitInfo.waitSemaphoreCount = 0;
    submitInfo.pWaitSemaphores = nullptr;
    submitInfo.pWaitDstStageMask = nullptr;
    submitInfo.signalSemaphoreCount = 0;
    submitInfo.pSignalSemaphores = nullptr;

    {
        std::unique_lock<std::mutex> lock(CoreObject::Instance::Queue_(_parentCommandPool->_queueName)->mutex);

        vkQueueSubmit(CoreObject::Instance::Queue_(_parentCommandPool->_queueName)->queue, 1, &submitInfo, _vkFence);
    }
}

void AirEngine::Core::Graphic::Command::CommandBuffer::WaitForFinish()
{
    vkWaitForFences(Graphic::CoreObject::Instance::VkDevice_(), 1, &_vkFence, VK_TRUE, UINT64_MAX);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::BeginRenderPass(Graphic::RenderPass::RenderPassBase* renderPass, Graphic::Manager::RenderPassObject* renderPassObject, std::vector<VkClearValue> clearValues)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass->VkRenderPass_();
    renderPassInfo.framebuffer = renderPassObject->FrameBuffer(renderPass->Name())->VkFramebuffer_();
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = renderPassObject->Extent();
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(_vkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::EndRenderPass()
{
    vkCmdEndRenderPass(_vkCommandBuffer);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::DrawMesh(Asset::Mesh* mesh)
{
    VkBuffer vertexBuffers[] = { mesh->VertexBuffer().VkBuffer_() };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(_vkCommandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(_vkCommandBuffer, mesh->IndexBuffer().VkBuffer_(), 0, VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(_vkCommandBuffer, static_cast<uint32_t>(mesh->Indices().size()), 1, 0, 0, 0);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::BindMaterial(Material* material)
{
    vkCmdBindPipeline(_vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material->Shader()->VkPipeline_());
    auto sets = material->VkDescriptorSets();
    vkCmdBindDescriptorSets(_vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material->PipelineLayout(), 0, static_cast<uint32_t>(sets.size()), sets.data(), 0, nullptr);
}

void AirEngine::Core::Graphic::Command::CommandBuffer::Blit(Instance::Image* srcImage, VkImageLayout srcImageLayout, Instance::Image* dstImage, VkImageLayout dstImageLayout, VkFilter filter)
{
    auto src = srcImage->VkExtent3D_();
    auto dst = dstImage->VkExtent3D_();
    auto layerCount = std::min(srcImage->LayerCount(), dstImage->LayerCount());
    auto srcSubresources = srcImage->VkImageSubresourceLayers_();
    auto dstSubresources = dstImage->VkImageSubresourceLayers_();
    std::vector< VkImageBlit> infos = std::vector< VkImageBlit>(layerCount);
    for (uint32_t i = 0; i < layerCount; i++)
    {
        auto& blit = infos[i];

        blit.srcSubresource = srcSubresources[i];
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = *reinterpret_cast<VkOffset3D*>(&src);
        blit.dstSubresource = dstSubresources[i];
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = *reinterpret_cast<VkOffset3D*>(&dst);
    }

    vkCmdBlitImage(_vkCommandBuffer, srcImage->VkImage_(), srcImageLayout, dstImage->VkImage_(), dstImageLayout, static_cast<uint32_t>(layerCount), infos.data(), filter);
}
