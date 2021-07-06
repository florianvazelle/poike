/**
 * @file Instance.hpp
 * @brief Define Instance class
 */

#ifndef INSTANCE_HPP
#define INSTANCE_HPP

// clang-format off
#include <common/VulkanHeader.hpp>  // for VkInstance, VkInstance_T
#include <common/NoCopy.hpp>       // for NoCopy
#include <string>                // for string
#include <vector>                // for vector
// clang-format on

namespace vkl {

  /**
   * @brief A class that allows you to manage the instance.
   * A VkInstance is an object that contains all of the information our Vulkan application needs to function.
   */
  class Instance : public NoCopy {
  public:
    Instance(const std::string& appName, const std::string& engineName, bool validationLayers);
    Instance() = delete;
    ~Instance();

    inline const VkInstance& handle() const { return m_instance; }
    inline bool validationLayersEnabled() const { return m_enableValidationLayers; }

    static const std::vector<const char*> ValidationLayers;
    static const std::vector<const char*> DeviceExtensions;

  private:
    VkInstance m_instance;
    bool m_enableValidationLayers;

    /**
     * @brief Checks that the validation layers specified in Instance :: ValidationLayers are well supported.
     * @return
     */
    static bool CheckValidationLayerSupport();
    static void GetRequiredExtensions(std::vector<const char*>& extensions, bool validationLayers);
  };

}  // namespace vkl

#endif  // INSTANCE_HPP