#include <shader_binding.hpp>
#include <filesystem>
#include <vector>
#include <fstream>
GLuint loadBinShader(const char* filePath, GLenum shaderType){
    // Load binary shader file into memory (You can use your own file loading mechanism)
    std::ifstream file(filePath, std::ios::binary);
    std::vector<char> shaderData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Load binary shader into OpenGL
    GLuint shaderID = glCreateShader(shaderType);
    glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, shaderData.data(), shaderData.size());
    glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);

    return shaderID;
}