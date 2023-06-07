CC = g++

BREW = $(shell brew --prefix)

CFLAGS = -g -std=c++11 -Wno-deprecated-declarations
INCFLAGS = -Iinclude -I$(BREW)/include -Iimgui -Iimgui/backends
LDFLAGS = -framework OpenGL -L$(BREW)/lib -lglfw

OBJECTS = main.o Camera.o Cube.o Shader.o Tokenizer.o Window.o ground.o particlesystem.o generalparticle.o
OBJECTS += imgui.o imgui_demo.o imgui_draw.o imgui_tables.o imgui_widgets.o
OBJECTS += imgui_impl_glfw.o imgui_impl_opengl3.o

RM = /bin/rm -f
all: menv
menv: $(OBJECTS)
	$(CC) -o menv $^ $(LDFLAGS)
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@
%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@
%.o: imgui/%.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@
%.o: imgui/backends/%.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

clean: 
	$(RM) *.o menv