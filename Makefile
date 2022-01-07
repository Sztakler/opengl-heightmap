GPP = g++
NAME =  heightmap
DEBUG_FLAGS = -g -Wall -I.
LINKER_FLAGS = -lGLEW  -lGL -lglfw
OBJ_DIR = obj
SRC_DIR = src
OBJ = $(OBJ_DIR)/main.o \
	$(OBJ_DIR)/drawable.o \
	$(OBJ_DIR)/shader.o \
	$(OBJ_DIR)/vao.o \
	$(OBJ_DIR)/vbo.o \
	$(OBJ_DIR)/ebo.o \
	$(OBJ_DIR)/camera.o \
	$(OBJ_DIR)/box.o \
	$(OBJ_DIR)/heightmap.o \
	$(OBJ_DIR)/map_loader.o \


OBJ_PATH = ./$(OBJ_DIR)/
SRC_PATH = ./$(SRC_DIR)/


$(NAME): $(OBJ_DIR) $(OBJ)
	$(GPP) $(DEBUG_FLAGS) $(OBJ_PATH)*.o $(LINKER_FLAGS) -o $(NAME)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_PATH)main.o: $(SRC_PATH)main.cpp
	$(GPP) -c -o $@ $<

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp $(SRC_PATH)%.h
	$(GPP) -c -o $@ $<

clean:
	rm -rf $(OBJ_DIR)
	rm $(NAME)