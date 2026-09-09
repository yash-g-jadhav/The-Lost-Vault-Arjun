cmake_minimum_required(VERSION 3.16)
project(TheLostVault C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

# GLFW Dependency
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# GLM Dependency
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# Sources for core library
set(CORE_SOURCES
    src/glad.c
    src/core/Log.cpp
    src/core/Clock.cpp
    src/core/Game.cpp
    src/core/Application.cpp
    src/graphics/Shader.cpp
    src/graphics/PrimitiveFactory.cpp
    src/graphics/Renderer2D.cpp
    src/graphics/Camera2D.cpp
    src/input/InputManager.cpp
    src/entities/TransformComponent.cpp
    src/entities/GameObject.cpp
    src/entities/Player.cpp
    src/entities/ColliderComponent.cpp
    src/gameplay/CollisionSystem.cpp
    src/levels/LevelData.cpp
    src/gameplay/GemSystem.cpp
)

set(CORE_HEADERS
    include/glad/glad.h
    include/KHR/khrplatform.h
    include/stb_image.h
    include/stb_truetype.h
    src/core/Log.h
    src/core/Clock.h
    src/core/Game.h
    src/core/Application.h
    src/graphics/Shader.h
    src/graphics/PrimitiveFactory.h
    src/graphics/Renderer2D.h
    src/graphics/Camera2D.h
    src/input/InputManager.h
    src/entities/Component.h
    src/entities/TransformComponent.h
    src/entities/GameObject.h
    src/entities/Player.h
    src/entities/ColliderComponent.h
    src/gameplay/CollisionSystem.h
    src/levels/LevelData.h
    src/utils/AABB.h
)

add_library(TheLostVaultCore STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_compile_definitions(TheLostVaultCore PUBLIC GLM_FORCE_CTOR_INIT)

target_include_directories(TheLostVaultCore PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${glfw_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
)

target_link_libraries(TheLostVaultCore PUBLIC glfw glm)

if(WIN32)
    target_link_libraries(TheLostVaultCore PUBLIC opengl32)
elseif(APPLE)
    find_package(OpenGL REQUIRED)
    target_link_libraries(TheLostVaultCore PUBLIC OpenGL::GL)
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(TheLostVaultCore PUBLIC OpenGL::GL)
endif()

# Main executable
add_executable(TheLostVault src/main.cpp)
target_link_libraries(TheLostVault PRIVATE TheLostVaultCore)

# Asset directory copy post-build step
add_custom_command(
    TARGET TheLostVault POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/assets
            $<TARGET_FILE_DIR:TheLostVault>/assets
    COMMENT "Copying assets directory to executable output folder..."
)

# Testing
enable_testing()
add_executable(TheLostVaultTests tests/test_collision.cpp)
target_link_libraries(TheLostVaultTests PRIVATE TheLostVaultCore)
add_test(NAME CollisionTests COMMAND TheLostVaultTests)
