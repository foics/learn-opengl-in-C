# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/foics/code/C/learn-opengl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/foics/code/C/learn-opengl/build

# Include any dependencies generated for this target.
include CMakeFiles/main.exe.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/main.exe.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/main.exe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/main.exe.dir/flags.make

CMakeFiles/main.exe.dir/src/glad.c.o: CMakeFiles/main.exe.dir/flags.make
CMakeFiles/main.exe.dir/src/glad.c.o: /home/foics/code/C/learn-opengl/src/glad.c
CMakeFiles/main.exe.dir/src/glad.c.o: CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/foics/code/C/learn-opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/main.exe.dir/src/glad.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.exe.dir/src/glad.c.o -MF CMakeFiles/main.exe.dir/src/glad.c.o.d -o CMakeFiles/main.exe.dir/src/glad.c.o -c /home/foics/code/C/learn-opengl/src/glad.c

CMakeFiles/main.exe.dir/src/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/main.exe.dir/src/glad.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/foics/code/C/learn-opengl/src/glad.c > CMakeFiles/main.exe.dir/src/glad.c.i

CMakeFiles/main.exe.dir/src/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/main.exe.dir/src/glad.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/foics/code/C/learn-opengl/src/glad.c -o CMakeFiles/main.exe.dir/src/glad.c.s

CMakeFiles/main.exe.dir/src/io.c.o: CMakeFiles/main.exe.dir/flags.make
CMakeFiles/main.exe.dir/src/io.c.o: /home/foics/code/C/learn-opengl/src/io.c
CMakeFiles/main.exe.dir/src/io.c.o: CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/foics/code/C/learn-opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/main.exe.dir/src/io.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.exe.dir/src/io.c.o -MF CMakeFiles/main.exe.dir/src/io.c.o.d -o CMakeFiles/main.exe.dir/src/io.c.o -c /home/foics/code/C/learn-opengl/src/io.c

CMakeFiles/main.exe.dir/src/io.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/main.exe.dir/src/io.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/foics/code/C/learn-opengl/src/io.c > CMakeFiles/main.exe.dir/src/io.c.i

CMakeFiles/main.exe.dir/src/io.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/main.exe.dir/src/io.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/foics/code/C/learn-opengl/src/io.c -o CMakeFiles/main.exe.dir/src/io.c.s

CMakeFiles/main.exe.dir/src/main.c.o: CMakeFiles/main.exe.dir/flags.make
CMakeFiles/main.exe.dir/src/main.c.o: /home/foics/code/C/learn-opengl/src/main.c
CMakeFiles/main.exe.dir/src/main.c.o: CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/foics/code/C/learn-opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/main.exe.dir/src/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.exe.dir/src/main.c.o -MF CMakeFiles/main.exe.dir/src/main.c.o.d -o CMakeFiles/main.exe.dir/src/main.c.o -c /home/foics/code/C/learn-opengl/src/main.c

CMakeFiles/main.exe.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/main.exe.dir/src/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/foics/code/C/learn-opengl/src/main.c > CMakeFiles/main.exe.dir/src/main.c.i

CMakeFiles/main.exe.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/main.exe.dir/src/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/foics/code/C/learn-opengl/src/main.c -o CMakeFiles/main.exe.dir/src/main.c.s

CMakeFiles/main.exe.dir/src/shader.c.o: CMakeFiles/main.exe.dir/flags.make
CMakeFiles/main.exe.dir/src/shader.c.o: /home/foics/code/C/learn-opengl/src/shader.c
CMakeFiles/main.exe.dir/src/shader.c.o: CMakeFiles/main.exe.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/foics/code/C/learn-opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/main.exe.dir/src/shader.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/main.exe.dir/src/shader.c.o -MF CMakeFiles/main.exe.dir/src/shader.c.o.d -o CMakeFiles/main.exe.dir/src/shader.c.o -c /home/foics/code/C/learn-opengl/src/shader.c

CMakeFiles/main.exe.dir/src/shader.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/main.exe.dir/src/shader.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/foics/code/C/learn-opengl/src/shader.c > CMakeFiles/main.exe.dir/src/shader.c.i

CMakeFiles/main.exe.dir/src/shader.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/main.exe.dir/src/shader.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/foics/code/C/learn-opengl/src/shader.c -o CMakeFiles/main.exe.dir/src/shader.c.s

# Object files for target main.exe
main_exe_OBJECTS = \
"CMakeFiles/main.exe.dir/src/glad.c.o" \
"CMakeFiles/main.exe.dir/src/io.c.o" \
"CMakeFiles/main.exe.dir/src/main.c.o" \
"CMakeFiles/main.exe.dir/src/shader.c.o"

# External object files for target main.exe
main_exe_EXTERNAL_OBJECTS =

/home/foics/code/C/learn-opengl/main.exe: CMakeFiles/main.exe.dir/src/glad.c.o
/home/foics/code/C/learn-opengl/main.exe: CMakeFiles/main.exe.dir/src/io.c.o
/home/foics/code/C/learn-opengl/main.exe: CMakeFiles/main.exe.dir/src/main.c.o
/home/foics/code/C/learn-opengl/main.exe: CMakeFiles/main.exe.dir/src/shader.c.o
/home/foics/code/C/learn-opengl/main.exe: CMakeFiles/main.exe.dir/build.make
/home/foics/code/C/learn-opengl/main.exe: CMakeFiles/main.exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/foics/code/C/learn-opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable /home/foics/code/C/learn-opengl/main.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/main.exe.dir/build: /home/foics/code/C/learn-opengl/main.exe
.PHONY : CMakeFiles/main.exe.dir/build

CMakeFiles/main.exe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/main.exe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/main.exe.dir/clean

CMakeFiles/main.exe.dir/depend:
	cd /home/foics/code/C/learn-opengl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/foics/code/C/learn-opengl /home/foics/code/C/learn-opengl /home/foics/code/C/learn-opengl/build /home/foics/code/C/learn-opengl/build /home/foics/code/C/learn-opengl/build/CMakeFiles/main.exe.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/main.exe.dir/depend

