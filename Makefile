VULKAN_SDK_PATH=/home/tallen/vulkan/VulkanSDK/1.0.57.0/x86_64
CFLAGS = -std=c++14 -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

VulkanTest: main.cc
	clang++ $(CFLAGS) -o VulkanTest main.cc $(LDFLAGS)

.PHONY: test clean

test: VulkanTest
	./VulkanTest

clean:
	rm -f VulkanTest
