#ifndef IMGUIRENDERPASS_HPP
#define IMGUIRENDERPASS_HPP

#include <poike/core/RenderPass.hpp>
#include <poike/core/VulkanHeader.hpp>
#include <stdexcept>

namespace vkl {
  class ImGuiRenderPass : public RenderPass {
  public:
    ImGuiRenderPass(const Device& device, const SwapChain& swapChain);

  private:
    void createRenderPass();
  };
}  // namespace vkl

#endif  // IMGUIRENDERPASS_HPP