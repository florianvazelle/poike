// clang-format off
#include <poike/imgui/ImGuiCommandBuffers.hpp>
#include <imgui.h>                    // for GetDrawData
#include <imgui_impl_vulkan.h>        // for ImGui_ImplVulkan_RenderDrawData
#include <poike/core/VulkanHeader.hpp>       // for VkCommandBuffer, VkCommandBuffer_T
#include <poike/core/CommandBuffers.hpp>  // for CommandBuffers
#include <poike/core/CommandPool.hpp>     // for CommandPool, vkl
#include <poike/core/DescriptorSets.hpp>  // for DescriptorSets
#include <poike/core/Device.hpp>          // for Device
#include <poike/core/RenderPass.hpp>      // for RenderPass, RenderPass
#include <poike/core/SwapChain.hpp>       // for SwapChain
#include <stdexcept>                  // for runtime_error
#include <vector>                     // for vector
// clang-format on

using namespace vkl;

ImGuiCommandBuffers::ImGuiCommandBuffers(const Device& device,
                                         const RenderPass& renderPass,
                                         const SwapChain& swapChain,
                                         const GraphicsPipeline& graphicsPipeline,
                                         const CommandPool& commandPool)
    : CommandBuffersBase(device, renderPass, swapChain, graphicsPipeline, commandPool) {
  createCommandBuffers();
}

void ImGuiCommandBuffers::recreate() {
  destroyCommandBuffers();
  createCommandBuffers();
}

void ImGuiCommandBuffers::createCommandBuffers() {
  m_commandBuffers.resize(m_renderPass.size());

  const VkCommandBufferAllocateInfo allocInfo = {
      .sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool        = m_commandPool.handle(),
      .level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size()),
  };

  if (vkAllocateCommandBuffers(m_device.logical(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
    throw std::runtime_error("Unable to allocate UI command buffers!");
  }
}

void ImGuiCommandBuffers::recordCommandBuffers(uint32_t bufferIdx) {
  const VkCommandBufferBeginInfo cmdBufferBegin = {
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
  };

  if (vkBeginCommandBuffer(m_commandBuffers[bufferIdx], &cmdBufferBegin) != VK_SUCCESS) {
    throw std::runtime_error("Unable to start recording UI command buffer!");
  }

  const VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
  const VkRenderPassBeginInfo renderPassBeginInfo = {
      .sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      .renderPass               = m_renderPass.handle(),
      .framebuffer              = m_renderPass.frameBuffer(bufferIdx),
      .renderArea = {
          .extent = {
              .width  = m_swapChain.extent().width,
              .height = m_swapChain.extent().height,
          },
      },
      .clearValueCount          = 1,
      .pClearValues             = &clearColor,
  };

  vkCmdBeginRenderPass(m_commandBuffers[bufferIdx], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

  // Grab and record the draw data for Dear Imgui
  ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_commandBuffers[bufferIdx]);

  // End and submit render pass
  vkCmdEndRenderPass(m_commandBuffers[bufferIdx]);

  if (vkEndCommandBuffer(m_commandBuffers[bufferIdx]) != VK_SUCCESS) {
    throw std::runtime_error("Failed to record command buffers!");
  }
}