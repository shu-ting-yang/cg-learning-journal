# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 4.0

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/build

# Include any dependencies generated for this target.
include CMakeFiles/shader_playground.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/shader_playground.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/shader_playground.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/shader_playground.dir/flags.make

CMakeFiles/shader_playground.dir/codegen:
.PHONY : CMakeFiles/shader_playground.dir/codegen

CMakeFiles/shader_playground.dir/src/main.cpp.o: CMakeFiles/shader_playground.dir/flags.make
CMakeFiles/shader_playground.dir/src/main.cpp.o: /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/src/main.cpp
CMakeFiles/shader_playground.dir/src/main.cpp.o: CMakeFiles/shader_playground.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/shader_playground.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/shader_playground.dir/src/main.cpp.o -MF CMakeFiles/shader_playground.dir/src/main.cpp.o.d -o CMakeFiles/shader_playground.dir/src/main.cpp.o -c /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/src/main.cpp

CMakeFiles/shader_playground.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/shader_playground.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/src/main.cpp > CMakeFiles/shader_playground.dir/src/main.cpp.i

CMakeFiles/shader_playground.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/shader_playground.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/src/main.cpp -o CMakeFiles/shader_playground.dir/src/main.cpp.s

CMakeFiles/shader_playground.dir/src/glad.c.o: CMakeFiles/shader_playground.dir/flags.make
CMakeFiles/shader_playground.dir/src/glad.c.o: /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/src/glad.c
CMakeFiles/shader_playground.dir/src/glad.c.o: CMakeFiles/shader_playground.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/shader_playground.dir/src/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/shader_playground.dir/src/glad.c.o -MF CMakeFiles/shader_playground.dir/src/glad.c.o.d -o CMakeFiles/shader_playground.dir/src/glad.c.o -c /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/src/glad.c

CMakeFiles/shader_playground.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/shader_playground.dir/src/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/src/glad.c > CMakeFiles/shader_playground.dir/src/glad.c.i

CMakeFiles/shader_playground.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/shader_playground.dir/src/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/src/glad.c -o CMakeFiles/shader_playground.dir/src/glad.c.s

# Object files for target shader_playground
shader_playground_OBJECTS = \
"CMakeFiles/shader_playground.dir/src/main.cpp.o" \
"CMakeFiles/shader_playground.dir/src/glad.c.o"

# External object files for target shader_playground
shader_playground_EXTERNAL_OBJECTS =

shader_playground: CMakeFiles/shader_playground.dir/src/main.cpp.o
shader_playground: CMakeFiles/shader_playground.dir/src/glad.c.o
shader_playground: CMakeFiles/shader_playground.dir/build.make
shader_playground: /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/OpenGL.framework
shader_playground: /opt/homebrew/lib/libglfw.3.4.dylib
shader_playground: CMakeFiles/shader_playground.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable shader_playground"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shader_playground.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/shader_playground.dir/build: shader_playground
.PHONY : CMakeFiles/shader_playground.dir/build

CMakeFiles/shader_playground.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/shader_playground.dir/cmake_clean.cmake
.PHONY : CMakeFiles/shader_playground.dir/clean

CMakeFiles/shader_playground.dir/depend:
	cd /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/build /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/build /Users/shutingyang/Documents/blog/my-cg-journal/projects/w2-shader-playground/build/CMakeFiles/shader_playground.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/shader_playground.dir/depend

