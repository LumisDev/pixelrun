#include <SDL3/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>

// Load binary shader function declaration
GLuint loadBinShader(const char* filename, GLenum shaderType);

typedef struct {
    GLuint vbo;
    GLuint vao;
    GLuint program;
} VBInt;

SDL_Window* window;
SDL_GLContext glContext;

float vertices[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f,
    -1.0f, 1.0f,
    -1.0f, -1.0f, // Repeat last vertex to close the shape
    1.0f, 1.0f
};

void getFPS() {
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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create SDL window
    window = SDL_CreateWindow("PixelRun", 800, 600, SDL_WINDOW_OPENGL);
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

    // Set up OpenGL viewport
    glViewport(0, 0, 800, 600);
}

void render(VBInt vb) {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render OpenGL content here
    glUseProgram(vb.program);
    glBindVertexArray(vb.vao);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (sizeof(float) * 2)); // Changed size calculation
    glBindVertexArray(0);
    glUseProgram(0);

    // Swap buffers
    SDL_GL_SwapWindow(window);
}

VBInt initVBData() {
    // Create a vertex buffer object
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0); // Corrected vertex attribute size
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create a shader program
    GLuint program;
    program = glCreateProgram();
    glAttachShader(program, loadBinShader("shaders/default.vert", GL_VERTEX_SHADER));
    glAttachShader(program, loadBinShader("shaders/default.frag", GL_FRAGMENT_SHADER));
    glLinkProgram(program);

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    VBInt vb = initVBData();

    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) { // Corrected event type name
                quit = true;
            }
        }
        getFPS();
        render(vb);
    }

    // Cleanup
    glDeleteBuffers(1, &vb.vbo);
    glDeleteVertexArrays(1, &vb.vao);
    glDeleteProgram(vb.program);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
