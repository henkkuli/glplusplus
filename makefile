CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LD_FLAGS := -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -lGL -lGLEW -g
CC_FLAGS := -I ./include -g -static -Wall

compile: $(OBJ_FILES)
	g++ -o gl++ $^ $(LD_FLAGS)

obj/%.o: src/%.cpp
	@mkdir -p obj
	g++ $(CC_FLAGS) -c -o $@ $<

clean:
	@rm -rf obj

all: clean compile
