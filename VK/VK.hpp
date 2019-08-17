/**
    Prototypes the vk namespace

    @author       D3PSI
    @version      0.0.1 02.12.2019

    @file         VK.hpp
    @brief        Prototype of the vk namespace
*/
#ifndef VK_HPP
#define VK_HPP
#include "Version.hpp"

#include <vulkan/vulkan.h>

#include <iostream>

#include "VKEngine.hpp"
#include "VK_STATUS_CODE.hpp"


/**
    Prototypes a namespace to prevent global naming conflicts
*/
namespace vk {

    extern VK_STATUS_CODE                       errorCodeBuffer;
    
    extern VKEngine*                            engine;
    extern const unsigned int                   HEIGHT;
    extern const unsigned int                   WIDTH;
    extern const char*                          TITLE;
    extern const unsigned int                   MAX_IN_FLIGHT_FRAMES;
    extern VkQueue                              transferQueue;
    extern VkCommandPool                        transferCommandPool;

    // Starting/Default camera state values
    extern const double                         YAW;
    extern const double                         PITCH;
    extern const double                         ROLL;
    extern const double                         SPEED;
    extern const double                         SENS;
    extern const double                         FOV;

    extern VkCommandPool                        graphicsCommandPool;
    extern VkQueue                              graphicsQueue;
    extern VkFence                              graphicsFence;
    extern std::mutex                           graphicsMutex;
    extern VkQueue                              transferQueue;
    extern VkCommandPool                        transferCommandPool;
    extern VkFence                              transferFence;
    extern std::mutex                           transferMutex;
    extern std::mutex                           commandBufferMutex;

    extern std::mutex                           loadingMutex;

    /**
        Initializes the VKEngine object

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE init(void);

    /**
        Handles main initialization of everything

        @return        Returns VK_SC_SUCCESS on success
        @return        Returns VK_SC_UNKNOWN_ERROR on error
    */
    VK_STATUS_CODE run(void);

    /**
        Helper function to create a VkDebugUtilsMessengerEXT

        @param        instance_                The valid VkInstance handle
        @param        pCreateInfo_            Pointer to messenger create info
        @param        pAllocator_                Pointer to memory allocator
        @param        pDebugMessenger_        The handle in which the messenger will be stored

        @return        Returns VK_SUCCESS on success
        @return        Returns VK_ERROR_EXTENSION NOT PRESENT if the "VK_EXT_debug_utils" extension has not been enabled
    */
    VkResult createDebugUtilsMessenger(
        VkInstance                                        instance_,
        const VkDebugUtilsMessengerCreateInfoEXT*         pCreateInfo_,
        const VkAllocationCallbacks*                      pAllocator_,
        VkDebugUtilsMessengerEXT*                         pDebugMessenger_
        );

    /**
        Helper function to destroy a VkDebugUtilsMessengerEXT

        @param         instance_                  The valid VkInstance handle
        @param         debugMessenger_            The debug messenger handle
        @param         pAllocator_                Pointer to memory allocator

        @return        Returns VK_SC_SUCCESS on success
        @return        Returns VK_SC_DEBUG_UTILS_MESSENGER_CLEANUP_ERROR on error
    */
    VK_STATUS_CODE destroyDebugUtilsMessenger(
        VkInstance                            instance_,
        VkDebugUtilsMessengerEXT              debugMessenger_,
        const VkAllocationCallbacks*          pAllocator_
        );

    /**
        Makes the console invisible
    */
    void hideConsole(void);
    
    /**
        Makes the console visible
    */
    void showConsole(void);
    
    /**
        Checks whether the console window is visible at the moment

        @return        Returns true if the console window is visible
        @return        Returns false if the console window is not visible
    */
    bool isConsoleVisible(void);

    /**
        Loads a file from disc and returns its contents in a char array

        @param         filePath_        The (relative) file path to the desired file

        @return        Returns an std::vector< char > containing the binary content of the specified input file
    */
    const std::vector< char > loadFile(const std::string& filePath_);

    /**
        Copies one buffer into the memory of another

        @param      srcBuf_     The source buffer
        @param      dstBuf_     The destination buffer
        @param      size_       The buffer size in bytes
    */
    void copyBuffer(VkBuffer srcBuf_, VkBuffer dstBuf_, VkDeviceSize size_);

    /**
        Starts a command buffer

        @param      queue_          The queue to submit the command buffer on

        @return     Returns a command buffer handle
    */
    VkCommandBuffer startCommandBuffer(Queue queue_);

    /**
        Ends a command buffer

        @param      commandBuffer_      The command buffer to end
        @param      queue_              The queue to submit the command buffer on
    */
    void endCommandBuffer(VkCommandBuffer commandBuffer_, Queue queue_);

    /**
        Executes an image layout transition operation

        @param      image_          The image to transition
        @param      format_         The image format
        @param      oldLayout_      The old layout
        @param      newLayout_      The new layout
        @param      mipLevels       The amount of mip levels
    */
    void imageLayoutTransition(
        VkImage         image_,
        VkFormat        format_,
        VkImageLayout   oldLayout_,
        VkImageLayout   newLayout_,
        uint32_t        mipLevels_
        );

    /**
        Copies a specific buffer area to an image

        @param      buffer_     The buffer to read from
        @param      image_      The image to write to
        @param      width_      The width of the area
        @param      height_     The height of the area
    */
    void copyBufferToImage(
        VkBuffer        buffer_,
        VkImage         image_,
        uint32_t        width_,
        uint32_t        height_
        );

    /**
        Creates a VkImageView handle

        @param      image_              The VkImage handle to create an image view from
        @param      format_             The image format
        @param      aspectFlags_        The aspect mask to specify in the image view creation process
        @param      mipLevels_          The amount of mip levels

        @return     Returns a valid VkImageView handle
    */
    VkImageView createImageView(
        VkImage                 image_,
        VkFormat                format_,
        VkImageAspectFlags      aspectFlags_,
        uint32_t                mipLevels_
        );

    /**
        Evaluates the best buffer format to use

        @param      candidates_         A reference to an std::vector of VkFormats to evaluate from
        @param      tiling_             VkImageTiling flags
        @param      features_           Feature flags the format must have

        @return     Returns the best VkFormat to use
    */
    VkFormat enumerateSupportedBufferFormat(const std::vector< VkFormat >& candidates_, VkImageTiling tiling_, VkFormatFeatureFlags features_);

    /**
        Finds the appropriate memory type to use for the specified operation

        @param         typeFilter_                    Filter for the right memory type
        @param         memoryPropertyFlags_           The necessary memory properties that must be supported by the memory

        @return        Returns an index for a memory type
    */
    uint32_t enumerateSuitableMemoryType(uint32_t typeFilter_, VkMemoryPropertyFlags memoryPropertyFlags_);

    /**
        Creates a VkImage handle

        @param      width_              The width of the image
        @param      height_             The height of the image
        @param      mipLevels_          The number of mipmaps to generate
        @param      format_             The image format
        @param      tiling_             The image tiling flags
        @param      usage_              The image usage flags
        @param      properties_         Image memory property flags
        @param      samples_            The number of samples per pixel
        @param      image_              The handle where the image will be stored
        @param      imageMemory_        The handle where the image's device memory will be stored

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE createImage(
        uint32_t                    width_,
        uint32_t                    height_,
        uint32_t                    mipLevels_,
        VkFormat                    format_,
        VkImageTiling               tiling_,
        VkImageUsageFlags           usage_,
        VkMemoryPropertyFlags       properties_,
        VkSampleCountFlagBits       samples_,
        VkImage&                    img_,
        VkDeviceMemory&             imgMem_
        );

    /**
        Generates mipmaps for texture images

        @param      image_              The VkImage to generate mipmaps from
        @param      imageFormat_        The format of the original image
        @param      width_              The width of the original image
        @param      height_             The height of the original image
        @param      mipLevels_          The amount of mipmaps to generate

        @return     Returns VK_SC_SUCCESS on success
     */
    VK_STATUS_CODE generateImageMipmaps(
        VkImage         image_, 
        VkFormat        imageFormat_,
        int32_t         width_, 
        int32_t         height_, 
        uint32_t        mipLevels_
        );

    /**
        Checks whether a VkFormat has a stencil component

        @param      format_         The format in question

        @return     Returns true if the format utilizes a stencil component
    */
    bool hasStencilBufferComponent(VkFormat format_);

    /**
        Adds a model to the model loading queue

        @param      path_       The path to the model

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE push(const char* path_);

    /**
        Adds a model to the model loading queue

        @param      path_       The path to the model

        @return     Returns VK_SC_SUCCESS on success
    */
    VK_STATUS_CODE push(ModelInfo info_);

    /**
        Waits on a queue to signal their fence

        @param      queue_      The queue to wait on
    */
    void waitForQueue(Queue queue_);

}
#endif  // VK_HPP
