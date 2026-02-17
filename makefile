CXX ?= g++
SRC_FILES = $(wildcard src/*.cpp)
EXECUTABLE = exec

# Basic flags (keep it simple; override from CLI if needed)
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra

# raylib via pkg-config (recommended for system/package-manager installs)
RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS := $(shell pkg-config --libs raylib 2>/dev/null)

CPPFLAGS += $(RAYLIB_CFLAGS)
LDLIBS += $(RAYLIB_LIBS)

ifeq ($(strip $(RAYLIB_LIBS)),)
$(warning raylib not found via pkg-config. Install raylib + its .pc file, or set RAYLIB_CFLAGS/RAYLIB_LIBS manually.)
endif

all: build

build:
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $(EXECUTABLE) $(SRC_FILES) $(LDFLAGS) $(LDLIBS)

run: build
	@./$(EXECUTABLE)

clean:
	@rm -f $(EXECUTABLE)
