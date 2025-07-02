
# Find GLFW on Windows:
if(WIN32)
find_path(GLFW3_INCLUDE_DIR GLFW/glfw3.h
PATHS
${CMAKE_SOURCE_DIR}/inc
NO_DEFAULT_PATH
)

find_library(GLFW3_LIBRARY NAMES glfw3 glfw
PATHS
${CMAKE_SOURCE_DIR}/ext
NO_DEFAULT_PATH
)

else()

# Find GLFW on Linux: Prefer to find the library and include on the system rather than the one provided by this project.
find_path(GLFW3_INCLUDE_DIR GLFW/glfw3.h
PATHS
/usr/include
/usr/local/include
NO_DEFAULT_PATH
)

find_library(GLFW3_LIBRARY NAMES glfw3 glfw
PATHS
/usr/lib
/usr/local/lib
NO_DEFAULT_PATH
)
endif()

if (GLFW3_INCLUDE_DIR AND GLFW3_LIBRARY)
set(GLFW3_FOUND TRUE)
set(GLFW3_LIBRARIES ${GLFW3_LIBRARY})
set(GLFW3_INCLUDE_DIRS ${GLFW3_INCLUDE_DIR})
else()
set(GLFW3_FOUND FALSE)
endif()

mark_as_advanced(GLFW3_INCLUDE_DIR GLFW3_LIBRARY)

