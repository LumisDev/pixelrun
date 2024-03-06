#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <shader_binding.hpp>
typedef struct {
    GLuint vbo;
    GLuint vao;
    GLuint program;
} VBInt;
SDL_Window* window;
SDL_GLContext glContext;
float vertices[][2] = {
        {-1.0f, -1.0f},
        {1.0f, -1.0f},
        {1.0f, 1.0f},
        {-1.0f, 1.0f},
        {-1.0f, -1.0f}, // Repeat last vertex to close the shape
        {1.0f, 1.0f}
    };

void getFPS(){
    const int DESIRED_FPS = 60;
    const int FRAME_DELAY = 1000 / DESIRED_FPS;

    // Inside the rendering loop
    uint32_t frameStart = (uint32_t)SDL_GetTicks();
    // Render function call
    uint32_t frameTime = (uint32_t)SDL_GetTicks() - frameStart;
    if (frameTime < FRAME_DELAY) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }

}
void setupSDL() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Set OpenGL version to 4.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


    // Create SDL window
    window = SDL_CreateWindow("SDL OpenGL Example", 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

void setupOpenGL() {
    // Create OpenGL context
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }
    SDL_GL_MakeCurrent(window, glContext);
    SDL_GL_SetSwapInterval(1); // 1 for Vsync on, 0 for Vsync off


    // Set up OpenGL viewport
    glViewport(0, 0, 800, 600);
}

void render(VBInt vb) {
    // Clear the screen
    glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render OpenGL content here
    glad_glUseProgram(vb.program);
    glad_glBindVertexArray(vb.vao);
    glad_glDrawArrays(GL_TRIANGLES, 0, 3);
    glad_glBindVertexArray(0);
    glad_glUseProgram(0);
    // Swap buffers
    SDL_GL_SwapWindow(window);
}
int arrSize(float arr[][2], int rows) {
    // Divide the total size of the array by the size of one row
    return rows;
}
VBInt initVBData() {
    // Create a vertex buffer object
    GLuint vbo;
    glad_glGenBuffers(1, &vbo);
    glad_glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Create a vertex array object
    GLuint vao;
    glad_glGenVertexArrays(1, &vao);
    glad_glBindVertexArray(vao);
    glad_glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glad_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, arrSize(vertices, 6) * sizeof(float), (void*)0);
    glad_glEnableVertexAttribArray(0);
    glad_glBindBuffer(GL_ARRAY_BUFFER, 0);
    glad_glBindVertexArray(0);
    // Create a shader program
    GLuint program;
    program = glad_glCreateProgram();
    glad_glAttachShader(program, loadBinShader("shaders/default.vert.spv", GL_VERTEX_SHADER));
    glad_glAttachShader(program, loadBinShader("shaders/default.frag.spv", GL_FRAGMENT_SHADER));
    glad_glLinkProgram(program);
    VBInt vb;
    vb.vbo = vbo;
    vb.vao = vao;
    vb.program = program;
    return vb;
}
int main(int argc, char* argv[]) {
    setupSDL();
    setupOpenGL();

    // Main loop
    glad_glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    VBInt vb = initVBData();
    
    
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) { // Corrected event type check
                quit = true;
            }
        }
        getFPS();
        render(vb);
    }
    glad_glDeleteBuffers(1, &vb.vbo);
    glad_glDeleteVertexArrays(1, &vb.vao);
    glad_glDeleteProgram(vb.program);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
