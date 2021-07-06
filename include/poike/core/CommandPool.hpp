/**
 * @file CommandPool.hpp
 * @brief Define CommandPool class
 */

#ifndef COMMANDPOOL_HPP
#define COMMANDPOOL_HPP

#include <common/NoCopy.hpp>
#include <common/VulkanHeader.hpp>
#include <optional>

namespace vkl {
  class Device;

  /**
   * Note Exposé : Les command pools gèrent la mémoire utilisée par les command buffers, et ce sont
   * les command pools qui instancient les command buffers.
   */

  class CommandPool : public NoCopy {
  public:
    CommandPool(const Device& device,
                const VkCommandPoolCreateFlags& flags,
                std::optional<uint32_t> queueFamilyIndex = std::nullopt);
    ~CommandPool();

    inline const VkCommandPool& handle() const { return m_pool; };

  private:
    VkCommandPool m_pool;

    const Device& m_device;
  };
}  // namespace vkl

#endif  // COMMANDPOOL_HPP
