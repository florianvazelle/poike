#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

// clang-format off
#include <poike/core/VulkanHeader.hpp>  // for VkSemaphore, VkSemaphore_T
#include <poike/meta/NoCopy.hpp>       // for NoCopy
namespace poike { class Device; }
// clang-format on

namespace poike {

  class Semaphore : public NoCopy {
  public:
    Semaphore(const Device& device);
    Semaphore() = delete;
    ~Semaphore();

    inline const VkSemaphore& handle() const { return m_semaphore; }

  private:
    VkSemaphore m_semaphore;

    const Device& m_device;
  };

}  // namespace poike

#endif  // SEMAPHORE_HPP