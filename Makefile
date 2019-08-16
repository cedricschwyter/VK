VULKAN_SDK_PATH = ext/vulkan/Linux
CFLAGS = -std=c++17 -I$(VULKAN_SDK_PATH)/include -Iext/SDL/include -Iext/tinyobjloader -Iext/stbimage
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan -lassimp -lSDL2 -lSDL2_image -lpthread

VKTest: VK/*.cpp
	$(CXX) $(CFLAGS) -v -o "bin/Linux/x64/VK by D3PSI" VK/*.cpp $(LDFLAGS)

.PHONY: run clean

run: VKTest
	./RUN.sh

clean:
	rm -f "VK/VK by D3PSI" "bin/Linux/x64/VK by D3PSI"
