#ifndef ATTACHMENT_HPP
#define ATTACHMENT_HPP

#include <poike/core/SwapChain.hpp>
#include <poike/core/image/Image.hpp>

namespace vkl {

  class Attachment : public Image {
  public:
    Attachment(const Device& device, const SwapChain& swapChain, VkFormat format, VkImageUsageFlags usage)
        : Image(device), m_format(format), m_usage(usage) {
      createImage(swapChain.extent().width, swapChain.extent().height);
      allocateMemory();
      createImageView();
      createSampler();
    };

  private:
    VkFormat m_format;
    VkImageUsageFlags m_usage;

    void createImage(uint32_t width, uint32_t height) final {
      const VkImageCreateInfo imageInfo = {
          .sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
          .imageType     = VK_IMAGE_TYPE_2D,
          .format        = m_format,
          .extent        = {
            .width  = width,
            .height = height,
            .depth  = 1,
          },
          .mipLevels     = 1,
          .arrayLayers   = 1,
          .samples       = VK_SAMPLE_COUNT_1_BIT,
          .tiling        = VK_IMAGE_TILING_OPTIMAL,
          // VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT flag is required for input attachments,
          .usage         = m_usage, // | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
          .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      };

      if (vkCreateImage(m_device.logical(), &imageInfo, nullptr, &m_image) != VK_SUCCESS) {
        throw std::runtime_error("vkCreateImage failed");
      }
    };

    void createImageView() final {
      VkImageAspectFlags aspectMask = 0;

      if (m_usage & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) {
        aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      }

      if (m_usage & VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT) {
        aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
      }

      const VkImageViewCreateInfo imageViewInfo = {
          .sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
          .image                           = m_image,
          .viewType                        = VK_IMAGE_VIEW_TYPE_2D,
          .format                          = m_format,
          .subresourceRange                = {
              .aspectMask     = aspectMask,
              .baseMipLevel   = 0,
              .levelCount     = 1,
              .baseArrayLayer = 0,
              .layerCount     = 1,
          },
      };

      if (vkCreateImageView(m_device.logical(), &imageViewInfo, nullptr, &m_imageView) != VK_SUCCESS) {
        throw std::runtime_error("vkCreateImageView failed");
      }
    };

    void createSampler() final {
      // Create sampler to sample from to depth attachment
      // Used to sample in the fragment shader for shadowed rendering
      const VkFilter filter = misc::formatIsFilterable(m_device.physical(), m_format, VK_IMAGE_TILING_OPTIMAL,
                                                       VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)
                                  ? VK_FILTER_LINEAR
                                  : VK_FILTER_NEAREST;

      const VkSamplerCreateInfo sampler = {
          .sType         = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
          .magFilter     = filter,
          .minFilter     = filter,
          .mipmapMode    = VK_SAMPLER_MIPMAP_MODE_LINEAR,
          .addressModeU  = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
          .addressModeV  = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
          .addressModeW  = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
          .mipLodBias    = 0.0f,
          .maxAnisotropy = 1.0f,
          .minLod        = 0.0f,
          .maxLod        = 1.0f,
          .borderColor   = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE,
      };

      if (vkCreateSampler(m_device.logical(), &sampler, nullptr, &m_sampler) != VK_SUCCESS) {
        throw std::runtime_error("Sampler creation failed");
      }
    };
  };

}  // namespace vkl

#endif  // ATTACHMENT_HPP
