include .env

CFLAGS  = -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

# vertSources 	= 	$(shell find ./shaders -type f -name "*.vert")
# vertObjFiles 	= 	$(patsubst %.vert, %.vert.spv, $(vertSources))
# fragSources 	= 	$(shell find ./shaders -type f -name "*.frag")
# fragObjFiles 	= 	$(patsubst %.frag, %.frag.spv, $(fragSources))

app: main.cpp ./src/app.cpp ./src/window.cpp ./src/device.cpp ./src/model.cpp ./src/pipeline.cpp ./src/swap_chain.cpp
	g++ $(CFLAGS) -o app main.cpp ./src/app.cpp ./src/window.cpp ./src/device.cpp ./src/model.cpp ./src/pipeline.cpp ./src/swap_chain.cpp $(LDFLAGS)

.PHONY: test clean

test: app
	./app

clean:
	rm -f app