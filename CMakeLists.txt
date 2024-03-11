cmake_minimum_required(VERSION 3.7)
project(PixelRun)
cmake_policy(SET CMP0077 NEW)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
# Add a custom command to compile all shaders at once
add_subdirectory(extern/sdl)
add_subdirectory(extern/box2d)
set(GLM_ENABLE_CXX_17 ON)
set(GLM_ENABLE_LANG_EXTENSIONS ON)
set(GLM_ENABLE_FAST_MATH ON)
add_subdirectory(extern/lua)
add_subdirectory(extern/glm)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/glad.c src/shader_binding.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include extern/sdl/include extern/box2d/include extern/lua/include extern/glm)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3-shared box2d lualib glm)


file(GLOB_RECURSE GLSL_SOURCE_FILES
    "src/shaders/*.frag"
    "src/shaders/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME}")
    add_custom_command(
        OUTPUT ${DESTINATION}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/"
        COMMAND ${CMAKE_COMMAND} -E copy ${GLSL} ${DESTINATION}
        DEPENDS ${GLSL})
    list(APPEND SHADER_FILES ${DESTINATION})
endforeach(GLSL)

add_custom_target(
    CopyShaders 
    DEPENDS ${SHADER_FILES}
)

add_dependencies(${CMAKE_PROJECT_NAME} CopyShaders SDL3::SDL3-shared box2d unit_test testbed lualib glm)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/shaders"
)
