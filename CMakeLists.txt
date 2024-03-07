cmake_minimum_required(VERSION 3.7)
project(PixelRun)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

# Add a custom command to compile all shaders at once
add_subdirectory(extern/sdl)
add_subdirectory(extern/box2d)


add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/glad.c src/shader_binding.cpp)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include extern/sdl/include extern/box2d/include)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE SDL3-shared box2d)


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

add_dependencies(${CMAKE_PROJECT_NAME} CopyShaders SDL3::SDL3-shared)

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/shaders/"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/shaders"
)