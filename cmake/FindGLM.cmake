find_path(GLM_INCLUDE_DIR glm/glm.hpp
    PATHS
    /usr/include
    /usr/local/include
    NO_DEFAULT_PATH
)

if (GLM_INCLUDE_DIR)
    set(GLM_FOUND TRUE)
    set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
else()
    set(GLM_FOUND FALSE)
endif()

mark_as_advanced(GLM_INCLUDE_DIR)

