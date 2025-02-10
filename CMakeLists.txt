cmake_minimum_required(VERSION 3.6)

# Set the project name
project(OpenGL_Build)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(OpenGL_GL_PREFERENCE GLVND)

# Add the cmake directory to the module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Include GLFW3
include_directories(${GLFW3_INCLUDE_DIRS})

# Find OpenGL and supporting packages
find_package(GLFW3 REQUIRED)

# Include header files
include_directories("${CMAKE_SOURCE_DIR}/inc")

# Gather all cpp and c files from the source directory
file(GLOB_RECURSE SOURCES 
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/*.c"
)

# Gather the header files
file(GLOB_RECURSE HEADERS
 	"${CMAKE_SOURCE_DIR}/inc/*.h"
 	"${CMAKE_SOURCE_DIR}/src/*.h"
)

if(WIN32)

# Add the shader files to the project on Windows.
file(GLOB_RECURSE SHADERS "${CMAKE_SOURCE_DIR}/assets/shaders*")
file(GLOB_RECURSE ASSETS "${CMAKE_SOURCE_DIR}/assets/*")

# Make Shaders visible in the editor.
source_group("Shaders" FILES ${SHADERS})

# Set default project to this one in visual studio.
set_property( DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME} )

# Add source files to the executable so that the linker knows where to look for them
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${SHADERS})

# Link together the external libraries and make it accessible to the compiler
target_link_libraries(
	${PROJECT_NAME} 
	${GLFW3_LIBRARIES}
)

# Define output directory
#set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/assets/shaders")
set(ASSET_OUTPUT_DIR "${CMAKE_BINARY_DIR}/assets")

# Copy shader files to the build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${ASSET_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets" ${ASSET_OUTPUT_DIR}
)

else()

# Add source files to the executable so that the linker knows where to look for them
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link together the external libraries and make it accessible to the compiler
target_link_libraries(
	${PROJECT_NAME} 
	${GLFW3_LIBRARIES}
)

endif()

