#!/bin/bash

echo "Installing dependencies..."

sudo apt update

sudo apt install g++ make gcc cmake xorg-dev libglu1-mesa-dev libglfw3-dev assimp-utils libglm-dev libsdl2-dev libsdl2-2.0-0 libjpeg9-dev libwebp-dev libtiff5-dev libsdl2-image-dev libsdl2-image-2.0-0 libvulkan1 mesa-vulkan-drivers vulkan-utils -y

echo "Successfully installed dependencies"