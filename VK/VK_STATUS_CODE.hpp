/**
    Implements the VK_STATUS_CODE enumeration

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         VK_STATUS_CODE.hpp
    @brief        Implementation of the VK_STATUS_CODE enumeration

*/
#ifndef VK_STATUS_CODE_HPP
#define VK_STATUS_CODE_HPP

/**
    Status code enumeration for easier exception-backtracing
*/
typedef enum VK_STATUS_CODE {

    VK_SC_RESOURCE_LOADING_ERROR                            = -53,
    VK_SC_MODEL_LOADING_ERROR_ASSIMP                        = -52,
    VK_SC_MSAA_BUFFER_CREATION_ERROR                        = -51,
    VK_SC_IMAGE_MEMORY_ALLOCATION_ERROR                     = -50,
    VK_SC_DEPTH_BUFFER_CREATION_ERROR                       = -49,
    VK_SC_IMAGE_CREATION_ERROR                              = -48,
    VK_SC_CAMERA_CREATION_ERROR                             = -47,
    VK_SC_SAMPLER_CREATION_ERROR                            = -46,
    VK_SC_IMAGE_VIEW_CREATION_ERROR                         = -45,
    VK_SC_TEXTURE_IMAGE_VIEW_CREATION_ERROR                 = -44,
    VK_SC_TEXTURE_IMAGE_CREATION_ERROR                      = -43,
    VK_SC_DESCRIPTOR_SET_CREATION_ERROR                     = -42,
    VK_SC_DESCRIPTOR_POOL_ERROR                             = -41,
    VK_SC_UNIFORM_BUFFER_UPDATE_ERROR                       = -40,
    VK_SC_UNIFORM_BUFFER_CREATION_ERROR                     = -39,
    VK_SC_DESCRIPTOR_ERROR                                  = -38,
    VK_SC_DESCRIPTOR_SET_LAYOUT_CREATION_ERROR              = -37,
    VK_SC_INDEX_BUFFER_MAP_ERROR                            = -36,
    VK_SC_VERTEX_BUFFER_MAP_ERROR                           = -35,
    VK_SC_BUFFER_BINDING_ERROR                              = -34,
    VK_SC_BUFFER_ALLOCATION_ERROR                           = -33,
    VK_SC_BUFFER_MEMORY_TYPE_CREATION_ERROR                 = -32,
    VK_SC_BUFFER_CREATION_ERROR                             = -31,
    VK_SC_FENCE_CREATION_ERROR                              = -30,
    VK_SC_SWAPCHAIN_CLEAN_ERROR                             = -29,
    VK_SC_SWAPCHAIN_IMAGE_ACQUIRE_ERROR                     = -28,
    VK_SC_PRESENTATION_ERROR                                = -27,
    VK_SC_QUEUE_SUBMISSION_ERROR                            = -26,
    VK_SC_SEMAPHORE_CREATION_ERROR                          = -25,
    VK_SC_SYNCHRONIZATION_OBJECT_INITIALIZATION_ERROR       = -24,
    VK_SC_COMMAND_BUFFER_RECORDING_ERROR                    = -23,
    VK_SC_COMMAND_BUFFER_ALLOCATION_ERROR                   = -22,
    VK_SC_COMMAND_POOL_ALLOCATION_ERROR                     = -21,
    VK_SC_FRAMEBUFFER_ALLOCATION_ERROR                      = -20,
    VK_SC_RENDER_PASS_CREATION_ERROR                        = -19,
    VK_SC_PIPELINE_LAYOUT_CREATION_ERROR                    = -18,
    VK_SC_SHADER_MODULE_CREATION_ERROR                      = -17,
    VK_SC_GRAPHICS_PIPELINE_CREATION_ERROR                  = -16,
    VK_SC_SWAPCHAIN_IMAGE_VIEWS_CREATION_ERROR              = -15,
    VK_SC_SWAPCHAIN_CREATION_ERROR                          = -14,
    VK_SC_SURFACE_CREATION_ERROR                            = -13,
    VK_SC_LOGICAL_DEVICE_CREATION_ERROR                     = -12,
    VK_SC_PHYSICAL_DEVICE_VULKAN_SUPPORT_ERROR              = -11,
    VK_SC_PHYSICAL_DEVICE_CREATION_ERROR                    = -10,
    VK_SC_DEBUG_UTILS_MESSENGER_CLEANUP_ERROR               = -9,
    VK_SC_DEBUG_UTILS_MESSENGER_CREATION_ERROR              = -8,
    VK_SC_INSTANCE_CREATON_ERROR                            = -7,
    VK_SC_CLEANUP_ERROR                                     = -6,
    VK_SC_VULKAN_RUNTIME_ERROR                              = -5,
    VK_SC_VULKAN_ERROR                                      = -4,
    VK_SC_WINDOW_ERROR                                      = -3,
    VK_SC_LOGGER_ERROR                                      = -2,
    VK_SC_UKNOWN_ERROR                                      = -1,
    VK_SC_SUCCESS                                           = 0,
    VK_SC_SWAPCHAIN_RECREATED                               = VK_SC_SUCCESS,
    VK_SC_ERROR_CODE_BUFFER_NOT_INITIALIZED                 = 1

} VK_STATUS_CODE;
#endif  // VK_STATUS_CODE_HPP
