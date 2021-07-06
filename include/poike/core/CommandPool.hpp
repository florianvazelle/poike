/**
 * @file CommandPool.hpp
 * @brief Define CommandPool class
 */

#ifndef COMMANDPOOL_HPP
#define COMMANDPOOL_HPP

#include <poike/meta/NoCopy.hpp>
#include <poike/core/VulkanHeader.hpp>
#include <optional>

namespace poike {
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
}  // namespace poike

#endif  // COMMANDPOOL_HPP
