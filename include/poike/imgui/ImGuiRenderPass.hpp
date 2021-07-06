#ifndef IMGUIRENDERPASS_HPP
#define IMGUIRENDERPASS_HPP

#include <poike/core/RenderPass.hpp>
#include <poike/core/VulkanHeader.hpp>
#include <stdexcept>

namespace poike {
  class ImGuiRenderPass : public RenderPass {
  public:
    ImGuiRenderPass(const Device& device, const SwapChain& swapChain);

  private:
    void createRenderPass();
  };
}  // namespace poike

#endif  // IMGUIRENDERPASS_HPP