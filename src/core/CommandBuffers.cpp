#include <poike/core/CommandBuffers.hpp>
#include <poike/core/CommandPool.hpp>
#include <poike/core/Device.hpp>
#include <poike/core/GraphicsPipeline.hpp>
#include <poike/core/RenderPass.hpp>
#include <poike/core/SwapChain.hpp>

#include <stdexcept>

#define DEFAULT_FENCE_TIMEOUT 100000000000

using namespace vkl;

CommandBuffersBase::CommandBuffersBase(const Device& device,
                                       const RenderPass& renderPass,
                                       const SwapChain& swapChain,
                                       const GraphicsPipeline& graphicsPipeline,
                                       const CommandPool& commandPool)
    : m_device(device),
      m_renderPass(renderPass),
      m_swapChain(swapChain),
      m_graphicsPipeline(graphicsPipeline),
      m_commandPool(commandPool) {}

CommandBuffers::CommandBuffers(const Device& device,
                               const RenderPass& renderPass,
                               const SwapChain& swapChain,
                               const GraphicsPipeline& graphicsPipeline,
                               const CommandPool& commandPool,
                               const DescriptorSets& descriptorSets,
                               const std::vector<const IBuffer*>& buffers)
    : CommandBuffersBase(device, renderPass, swapChain, graphicsPipeline, commandPool),
      m_descriptorSets(descriptorSets),
      m_buffers(buffers) {}

CommandBuffersBase::~CommandBuffersBase() { destroyCommandBuffers(); }

void CommandBuffers::recreate() {
  destroyCommandBuffers();
  createCommandBuffers();
}

void CommandBuffersBase::destroyCommandBuffers() {
  vkFreeCommandBuffers(m_device.logical(), m_commandPool.handle(), static_cast<uint32_t>(m_commandBuffers.size()),
                       m_commandBuffers.data());
}

void CommandBuffers::SingleTimeCommands(const Device& device,
                                        const CommandPool& cmdPool,
                                        const VkQueue& queue,
                                        const std::function<void(const VkCommandBuffer&)>& func) {
  const VkCommandBufferAllocateInfo allocInfo = {
      .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool        = cmdPool.handle(),
      .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = 1,
  };

  VkCommandBuffer commandBuffer = {};
  if (vkAllocateCommandBuffers(device.logical(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }

  const VkCommandBufferBeginInfo beginInfo = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Could not create one-time command buffer!");
  }

  func(commandBuffer);

  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }

  const VkSubmitInfo submitInfo = {
      .sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO,
      .commandBufferCount = 1,
      .pCommandBuffers    = &commandBuffer,
  };

  VkFenceCreateInfo fenceInfo = {
      .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
  };
  VkFence fence;
  vkCreateFence(device.logical(), &fenceInfo, nullptr, &fence);

  // Submit to the queue
  vkQueueSubmit(queue, 1, &submitInfo, fence);
  // vkQueueWaitIdle(queue);

  // Wait for the fence to signal that command buffer has finished executing
  vkWaitForFences(device.logical(), 1, &fence, VK_TRUE, DEFAULT_FENCE_TIMEOUT);
  vkDestroyFence(device.logical(), fence, nullptr);

  vkFreeCommandBuffers(device.logical(), cmdPool.handle(), 1, &commandBuffer);
}
