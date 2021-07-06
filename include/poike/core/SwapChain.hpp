/**
 * @file SwapChain.hpp
 * @brief Define SwapChain class and SwapChainSupportDetails struct
 */

#ifndef SWAPCHAIN_HPP
#define SWAPCHAIN_HPP

// clang-format off
#include <stddef.h>              // for size_t
#include <stdint.h>              // for uint32_t
#include <poike/core/VulkanHeader.hpp>  // for VkSurfaceFormatKHR, VkPresentModeKHR
#include <poike/meta/NoCopy.hpp>       // for NoCopy
#include <vector>                // for vector
namespace vkl { class Device; }
namespace vkl { class Window; }
// clang-format on

namespace vkl {
  struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
  };

  /**
   * Note Exposé : Une SwapChain est un tableau d'images prêtes à être présentées.
   * Une des utilisations est le contrôle du frame rate.
   * Le GPU effectue un rendu complet en une seule image, puis l'affiche. Une fois qu'il a fini de
   * dessiner le premier cadre, il commence à dessiner la deuxième frame.
   */

  class SwapChain : public NoCopy {
  public:
    explicit SwapChain(const Device& device, const Window& window);
    ~SwapChain();

    auto recreate() -> void;
    auto cleanupOld() -> void;

    inline const VkSwapchainKHR& handle() const { return m_swapChain; }
    inline const VkFormat& imageFormat() const { return m_imageFormat; }
    inline const VkExtent2D& extent() const { return m_extent; }
    inline size_t numImages() const { return m_images.size(); }
    inline size_t numImageViews() const { return m_imageViews.size(); }

    inline const SwapChainSupportDetails& supportDetails() const { return m_supportDetails; }
    inline VkImageView imageView(uint32_t index) const { return m_imageViews[index]; }

    static SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);
    static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const Window& window);

  private:
    SwapChainSupportDetails m_supportDetails;
    VkSwapchainKHR m_swapChain;
    VkSwapchainKHR m_oldSwapChain;

    // Swap chain image handles
    std::vector<VkImage> m_images;
    std::vector<VkImageView> m_imageViews;

    VkFormat m_imageFormat;
    VkExtent2D m_extent;

    const Device& m_device;
    const Window& m_window;

    void createSwapChain();
    void createImageViews();

    void destroyImageViews();

    static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
  };
}  // namespace vkl

#endif  // SWAPCHAIN_HPP
