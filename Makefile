CC ?= gcc
STD ?= c99

ifeq ($(OS), Windows_NT)
  CWARNINGS := -Wall -Wextra
  TARGET = $(BUILD_DIR)/solar.exe
  OPENGL_LIBS := -lglfw3 -lGLEW -lopengl32 -lgdi32
  OTHER_INCLUDES := -I./lib
else
  CWARNINGS := -Wall -Wextra -Wpedantic -Wshadow
  TARGET = $(BUILD_DIR)/solar
  ifeq ($(shell uname -s), Linux)
    OPENGL_INCLUDE := -I/usr/include/GLFW -I/usr/include/GL
    OPENGL_LIBS := -lglfw -lGLEW -lGL
  else ifeq ($(shell uname -s), Darwin)
    OPENGL_LIBS := -lglfw -framework Cocoa -framework OpenGL -framework IOKit
  endif
  OTHER_INCLUDES := -I./lib
endif

CFLAGS := -std=$(STD) $(CWARNINGS) $(OPENGL_INCLUDE) $(OTHER_INCLUDES) -I./$(SRC_DIR)
LDFLAGS := $(OPENGL_LIBS)

BUILD_DIR = build
SRC_DIR = nsrc

# SRCS := $(wildcard $(SRC_DIR)/*.c)
# OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS) -lm

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -MT '$@' -c $< -o $@ -lm

$(BUILD_DIR):
	@mkdir -p $@

-include $(DEPS)

# BUILD

.PHONY: build
build: $(TARGET)

# DEBUG

.PHONY: debug
debug: CFLAGS += -g -O0
debug: $(TARGET)

# RELEASE

.PHONY: release
release: CFLAGS += -O2
release: $(TARGET)

# CLEAR

.PHONY: clear
clear:
	rm -rf $(BUILD_DIR)/*