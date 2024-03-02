cmake_minimum_required(VERSION 3.7)
project(PixelRun)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(SHADER_FILES
    src/shader/default.vert
    src/shader/default.frag
)

# Add a custom command to compile all shaders at once
add_subdirectory(extern/sdl)


add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/glad.c)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include extern/sdl/include)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3-shared)


file(GLOB_RECURSE GLSL_SOURCE_FILES
    "src/shaders/*.frag"
    "src/shaders/*.vert"
    )

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/${FILE_NAME}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders/"
    COMMAND glslangValidator -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
    )

add_dependencies(${CMAKE_PROJECT_NAME} Shaders SDL3::SDL3-shared)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/shaders"
        )

