#include <poike/core/DebugUtilsMessenger.hpp>
#include <poike/core/Instance.hpp>

#include <iostream>
#include <stdexcept>

using namespace poike;

DebugUtilsMessenger::DebugUtilsMessenger(const Instance& instance, bool exitOnError)
    : m_debugMessenger(VK_NULL_HANDLE), m_exitOnError(exitOnError), m_instance(instance) {
  if (instance.validationLayersEnabled()) {
    // Setup the debug messenger
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {
        .pUserData = &m_exitOnError,
    };
    PopulateDebugMessengerCreateInfo(createInfo);

    // Get pointer to extension function
    auto createFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance.handle(),
                                                                                "vkCreateDebugUtilsMessengerEXT");

    if (createFunc == nullptr) {
      throw std::runtime_error("Debug Utils Messenger extension not available");
    }

    // Create the messenger
    VkResult result = createFunc(instance.handle(), &createInfo, nullptr, &m_debugMessenger);

    if (result != VK_SUCCESS) {
      throw std::runtime_error("Failed to setup Debug Utils Messenger");
    }
  }
}

DebugUtilsMessenger::~DebugUtilsMessenger() noexcept(false) {
  if (m_instance.validationLayersEnabled()) {
    auto destroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_instance.handle(),
                                                                                  "vkDestroyDebugUtilsMessengerEXT");

    if (destroyFunc == nullptr) {
      throw std::runtime_error("Failed to destroy Debug Utils Messenger");
    }

    destroyFunc(m_instance.handle(), m_debugMessenger, nullptr);
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL
DebugUtilsMessenger::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                   VkDebugUtilsMessageTypeFlagsEXT messageType,
                                   const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                   void* pUserData) {
  static const std::string yellow("\033[0;33m");
  static const std::string red("\033[0;31m");
  static const std::string reset("\033[0m");

  bool is_error = false;

  if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    std::cout << yellow << "Warning: ";
  } else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    std::cerr << red << "Error: ";
    is_error = true;
  }

  std::cerr << reset << pCallbackData->pMessageIdName << "\n" << pCallbackData->pMessage << "\n" << std::endl;

  bool* userData = (bool*)pUserData;  // cast it to an int

  if (is_error && (*userData)) {
    exit(1);
  }

  return VK_FALSE;
}

void DebugUtilsMessenger::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity
      = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType
      = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = DebugCallback;
}
