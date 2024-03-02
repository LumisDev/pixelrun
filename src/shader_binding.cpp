#include <shader_binding.hpp>
#include <vector>
#include <fstream>
#include <sstream>

GLuint compileShader(GLenum shaderType, const char* shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    glad_glShaderSource(shader, 1, &shaderSource, NULL);
    glad_glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glad_glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    return shader;
}

GLuint loadShaderFromFile(const char* filePath, GLenum shaderType) {
    // Read shader source code from file
    std::ifstream file(filePath);
    if (!file.is_open()) {
        printf("Failed to open file: %s\n", filePath);
        return 0;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shaderSource = buffer.str();

    return compileShader(shaderType, shaderSource.c_str());
}

GLuint loadBinShader(const char* filePath, GLenum shaderType) {
    return loadShaderFromFile(filePath, shaderType);
}
