/**
 * @file RenderPass.hpp
 * @brief Define RenderPass base class
 */

#ifndef RENDERPASS_HPP
#define RENDERPASS_HPP

// clang-format off
#include <stddef.h>              // for size_t
#include <stdint.h>              // for uint32_t
#include <poike/core/VulkanHeader.hpp>  // for VkFramebuffer, VkRenderPass, VkRende...
#include <poike/meta/NoCopy.hpp>       // for NoCopy
#include <iostream>              // for operator<<, cout, ostream
#include <memory>                // for unique_ptr
#include <vector>                // for vector
#include <poike/core/image/Attachment.hpp>
namespace vkl { class Device; }
namespace vkl { class SwapChain; }
// clang-format on

namespace vkl {
  /**
   * Note Exposé : Le render pass permet d'indiquer combien chaque framebuffer
   * aura de buffers de couleur et de profondeur, combien de samples il faudra utiliser avec chaque
   * frambuffer et comment les utiliser tout au long des opérations de rendu.
   */
  class RenderPass : public NoCopy {
  public:
    RenderPass(const Device& device, const SwapChain& swapChain);
    ~RenderPass();

    inline const VkRenderPass& handle() const { return m_renderPass; }
    inline const VkFramebuffer& frameBuffer(uint32_t index) const { return m_frameBuffers[index]; }
    inline const std::vector<std::unique_ptr<Attachment>>& attachments() const { return m_depthAttachments; };
    inline size_t size() const { return m_frameBuffers.size(); }

    void recreate();

  protected:
    VkRenderPass m_renderPass;

    std::vector<VkFramebuffer> m_frameBuffers;
    std::vector<std::unique_ptr<Attachment>>
        m_depthAttachments;  // TODO : transfomer ca en vector d'Attachment ou Attachment
                             // serait composer de plusieur Attachment (color, depth ...)

    const Device& m_device;
    const SwapChain& m_swapChain;

    virtual void createRenderPass() = 0;
    virtual void createFrameBuffers();
    void destroyRenderPass();
    void destroyFrameBuffers();
  };
}  // namespace vkl

#endif  // RENDERPASS_HPP
