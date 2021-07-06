// clang-format off
#include <common/RenderPass.hpp>
#include <common/Device.hpp>                 // for Device
#include <common/SwapChain.hpp>              // for SwapChain
#include <stdexcept>                         // for runtime_error
#include <common/QueueFamily.hpp>            // for vkl
// clang-format on

using namespace vkl;

RenderPass::RenderPass(const Device& device, const SwapChain& swapChain)
    : m_renderPass(VK_NULL_HANDLE), m_device(device), m_swapChain(swapChain) {}

RenderPass::~RenderPass() {
  destroyFrameBuffers();
  destroyRenderPass();
}

void RenderPass::recreate() {
  destroyFrameBuffers();
  destroyRenderPass();

  createRenderPass();
  createFrameBuffers();
}

void RenderPass::createFrameBuffers() {
  const size_t numImages = m_swapChain.numImages();

  m_frameBuffers.resize(numImages);

  // Create a framebuffer for each image view
  for (size_t i = 0; i < numImages; ++i) {
    const VkImageView attachments[] = {m_swapChain.imageView(i)};

    const VkFramebufferCreateInfo info = {
        .sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass      = m_renderPass,
        .attachmentCount = 1,
        .pAttachments    = attachments,
        .width           = m_swapChain.extent().width,
        .height          = m_swapChain.extent().height,
        .layers          = 1,
    };

    if (vkCreateFramebuffer(m_device.logical(), &info, nullptr, &m_frameBuffers.at(i)) != VK_SUCCESS) {
      throw std::runtime_error("Framebuffer creation failed");
    }
  }
}

void RenderPass::destroyRenderPass() { vkDestroyRenderPass(m_device.logical(), m_renderPass, nullptr); }

void RenderPass::destroyFrameBuffers() {
  for (VkFramebuffer& fb : m_frameBuffers) {
    vkDestroyFramebuffer(m_device.logical(), fb, nullptr);
  }

  m_depthAttachments.clear();
}
