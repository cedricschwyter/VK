cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/VK")

include_directories ("${CMAKE_SOURCE_DIR}/VK/ext/stbimage")
include_directories ("${CMAKE_SOURCE_DIR}/VK/ext/SDL/include")
include_directories ("${CMAKE_SOURCE_DIR}/VK/ext/glfw/include")
include_directories ("${CMAKE_SOURCE_DIR}/VK/ext/VulkanSDK/1.1.85.0/Include")

add_executable (VK "Main.cpp" "ASSERT.cpp" "ConsoleColor.hpp" "GraphicsPipeline.cpp" "GraphicsPipeline.hpp" "LoadingScreen.cpp" "LoadingScreen.hpp" "LOG_TYPE.cpp" "Logger.cpp" "Logger.hpp" "LOGGER_STATUS_CODE.cpp" "QueueFamilies.cpp" "SDL_STATUS_CODE.cpp" "SwapchainDetails.cpp" "Version.hpp" "VertFragShaderStages.cpp" "VertFragShaderStages.hpp" "VK.hpp" "VK_STATUS_CODE.hpp" "VKEngine.cpp" "VKEngine.hpp")

target_link_libraries(VK ${CMAKE_SOURCE_DIR}/VK/ext/SDL/lib/x64/SDL2.lib)
target_link_libraries(VK ${CMAKE_SOURCE_DIR}/VK/ext/SDL/lib/x64/SDL2_image.lib)
target_link_libraries(VK ${CMAKE_SOURCE_DIR}/VK/ext/glfw/lib-vc2019/glfw3.lib)
target_link_libraries(VK ${CMAKE_SOURCE_DIR}/VK/ext/VulkanSDK/1.1.85.0/Lib/vulkan-1.lib)
