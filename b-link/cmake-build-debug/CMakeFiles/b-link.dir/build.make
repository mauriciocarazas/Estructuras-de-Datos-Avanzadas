# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.24

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2022.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2022.3\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\mauri\Escritorio\b-link

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\mauri\Escritorio\b-link\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/b-link.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/b-link.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/b-link.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/b-link.dir/flags.make

CMakeFiles/b-link.dir/source/main.cpp.obj: CMakeFiles/b-link.dir/flags.make
CMakeFiles/b-link.dir/source/main.cpp.obj: CMakeFiles/b-link.dir/includes_CXX.rsp
CMakeFiles/b-link.dir/source/main.cpp.obj: C:/Users/mauri/Escritorio/b-link/source/main.cpp
CMakeFiles/b-link.dir/source/main.cpp.obj: CMakeFiles/b-link.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\mauri\Escritorio\b-link\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/b-link.dir/source/main.cpp.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/b-link.dir/source/main.cpp.obj -MF CMakeFiles\b-link.dir\source\main.cpp.obj.d -o CMakeFiles\b-link.dir\source\main.cpp.obj -c C:\Users\mauri\Escritorio\b-link\source\main.cpp

CMakeFiles/b-link.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/b-link.dir/source/main.cpp.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\mauri\Escritorio\b-link\source\main.cpp > CMakeFiles\b-link.dir\source\main.cpp.i

CMakeFiles/b-link.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/b-link.dir/source/main.cpp.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.3\bin\mingw\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\mauri\Escritorio\b-link\source\main.cpp -o CMakeFiles\b-link.dir\source\main.cpp.s

# Object files for target b-link
b__link_OBJECTS = \
"CMakeFiles/b-link.dir/source/main.cpp.obj"

# External object files for target b-link
b__link_EXTERNAL_OBJECTS =

b-link.exe: CMakeFiles/b-link.dir/source/main.cpp.obj
b-link.exe: CMakeFiles/b-link.dir/build.make
b-link.exe: CMakeFiles/b-link.dir/linklibs.rsp
b-link.exe: CMakeFiles/b-link.dir/objects1.rsp
b-link.exe: CMakeFiles/b-link.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\mauri\Escritorio\b-link\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable b-link.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\b-link.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/b-link.dir/build: b-link.exe
.PHONY : CMakeFiles/b-link.dir/build

CMakeFiles/b-link.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\b-link.dir\cmake_clean.cmake
.PHONY : CMakeFiles/b-link.dir/clean

CMakeFiles/b-link.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\mauri\Escritorio\b-link C:\Users\mauri\Escritorio\b-link C:\Users\mauri\Escritorio\b-link\cmake-build-debug C:\Users\mauri\Escritorio\b-link\cmake-build-debug C:\Users\mauri\Escritorio\b-link\cmake-build-debug\CMakeFiles\b-link.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/b-link.dir/depend

