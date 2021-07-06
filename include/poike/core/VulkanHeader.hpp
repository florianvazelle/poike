#pragma once

#ifdef __ANDROID__
// Include files for Android
#  include "vulkan_wrapper.h"  // Include Vulkan_wrapper and dynamically load symbols.
#endif

#include <vulkan/vulkan.h>