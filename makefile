CXX ?= g++
EXECUTABLE ?= exec

BUILDDIR ?= build
OBJDIR ?= $(BUILDDIR)/obj
LIBECS := $(BUILDDIR)/libecs.a

CORE_SRC := $(wildcard src/*.cpp)
DEMO_SRC := $(wildcard demo/*.cpp)

CORE_OBJ := $(patsubst src/%.cpp,$(OBJDIR)/core_%.o,$(CORE_SRC))
DEMO_OBJ := $(patsubst demo/%.cpp,$(OBJDIR)/demo_%.o,$(DEMO_SRC))

CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra

CPPFLAGS += -Iinclude

RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib 2>/dev/null)
RAYLIB_LIBS := $(shell pkg-config --libs raylib 2>/dev/null)

CPPFLAGS += $(RAYLIB_CFLAGS)
LDLIBS += $(RAYLIB_LIBS)

ifeq ($(strip $(RAYLIB_LIBS)),)
$(warning raylib not found via pkg-config. Install raylib + its .pc file, or set RAYLIB_CFLAGS/RAYLIB_LIBS manually.)
endif

all: build

build: $(EXECUTABLE)

lib: $(LIBECS)

$(EXECUTABLE): $(LIBECS) $(DEMO_OBJ)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ $(DEMO_OBJ) $(LIBECS) $(LDFLAGS) $(LDLIBS)

$(LIBECS): $(CORE_OBJ)
	@ar rcs $@ $(CORE_OBJ)

$(OBJDIR)/core_%.o: src/%.cpp | $(OBJDIR)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJDIR)/demo_%.o: demo/%.cpp | $(OBJDIR)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJDIR):
	@mkdir -p $(OBJDIR)

run: build
	@./$(EXECUTABLE)

clean:
	@rm -rf $(BUILDDIR) $(EXECUTABLE)

.PHONY: all build lib run clean
