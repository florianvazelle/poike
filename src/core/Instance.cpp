// clang-format off
#include <poike/core/Instance.hpp>
#include <stdint.h>                        // for uint32_t
#include <poike/core/DebugUtilsMessenger.hpp>  // for DebugUtilsMessenger
#include <poike/core/Window.hpp>               // for Window
#include <cstring>                         // for strcmp
#include <stdexcept>                       // for runtime_error
// clang-format on

using namespace vkl;

const std::vector<const char*> Instance::ValidationLayers = {"VK_LAYER_KHRONOS_validation"};
const std::vector<const char*> Instance::DeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

Instance::Instance(const std::string& appName, const std::string& engineName, bool validationLayers)
    : m_instance(VK_NULL_HANDLE), m_enableValidationLayers(validationLayers) {
#ifdef __ANDROID__
  // This place is the first place for samples to use Vulkan APIs.
  // Here, we are going to open Vulkan.so on the device and retrieve function pointers using
  // vulkan_wrapper helper.
  if (!InitVulkan()) {
    throw std::runtime_error("Error initialization failed!");
  }
#endif

  if (validationLayers && !CheckValidationLayerSupport()) {
    throw std::runtime_error("validation layers requested, but not available!");
  }

  // appInfo permet de décrire notre application
  const VkApplicationInfo appInfo = {
      .sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName   = appName.c_str(),
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName        = engineName.c_str(),
      .engineVersion      = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion         = VK_API_VERSION_1_0,
  };

  // createInfo est utilisé pour informer Vulkan de nos informations d'application, des couches que
  // nous utiliserons et des extensions que nous voulons.
  VkInstanceCreateInfo createInfo = {
      .sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &appInfo,
  };

  std::vector<const char*> extensions;
  GetRequiredExtensions(extensions, validationLayers);
  createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {
      .pNext = nullptr,
  };

  if (validationLayers) {
    createInfo.enabledLayerCount   = static_cast<uint32_t>(Instance::ValidationLayers.size());
    createInfo.ppEnabledLayerNames = Instance::ValidationLayers.data();

    DebugUtilsMessenger::PopulateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
  }

  const VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);

  if (result == VK_ERROR_INCOMPATIBLE_DRIVER) {
    throw std::runtime_error(
        "Cannot find a compatible Vulkan installable client "
        "driver (ICD). Please make sure your driver supports "
        "Vulkan before continuing. The call to vkCreateInstance failed.");
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error(
        "The call to vkCreateInstance failed. Please make sure "
        "you have a Vulkan installable client driver (ICD) before "
        "continuing.");
  }
}

Instance::~Instance() { vkDestroyInstance(m_instance, nullptr); }

bool Instance::CheckValidationLayerSupport() {
  uint32_t layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char* layerName : Instance::ValidationLayers) {
    bool layerFound = false;

    for (const auto& layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

void Instance::GetRequiredExtensions(std::vector<const char*>& extensions, bool validationLayers) {
  Window::GetRequiredExtensions(extensions);

  if (validationLayers) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
}