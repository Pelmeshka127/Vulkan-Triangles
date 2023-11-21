include .env

CFLAGS  = -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include
LDFLAGS = -L$(VULKAN_SDK_PATH)/lib `pkg-config --static --libs glfw3` -lvulkan

app: main.cpp ./src/app.cpp ./src/window.cpp
	g++ $(CFLAGS) -o app main.cpp ./src/app.cpp ./src/window.cpp $(LDFLAGS)

.PHONY: test clean

test: app
	./app

clean:
	rm -f app