#include "stdafx.h"
#include "ShaderProgram.h"

uint buildShader(std::string const& filename, GLenum shaderType) {
    uint shader = glCreateShader(shaderType);
    std::ifstream fs(filename);
    if (!fs.is_open()) {
        throw "Failed to open shader file" + filename;
    }
    std::ostringstream ss;
    ss << fs.rdbuf();
    fs.close();
    std::string source = ss.str();
    const char* c_str = source.c_str();
    glShaderSource(shader, 1, &c_str, NULL);

    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << infoLog << std::endl;
        throw "Failed to compile shader " + filename;
    }
    return shader;
}

uint Shader::buildVertexShader(std::string const& filename) {
    return buildShader(filename, GL_VERTEX_SHADER);
}

uint Shader::buildFragShader(std::string const& filename) {
    return buildShader(filename, GL_FRAGMENT_SHADER);
}

void Shader::deleteShader(uint shader) {
    glDeleteShader(shader);
}

ShaderProgram::ShaderProgram() : ID(glCreateProgram()) {}

ShaderProgram::ShaderProgram(std::string const& vsFn, std::string const& fsFn) : ShaderProgram() {
    setVertexShader(vsFn);
    setFragShader(fsFn);
    link();
}

ShaderProgram::~ShaderProgram() {
    for (uint shader : exclusiveShaders) {
        glDeleteShader(shader);
    }
    glDeleteProgram(ID);
}

void ShaderProgram::setVertexShader(uint shader) {
    glAttachShader(ID, shader);
}

void ShaderProgram::setVertexShader(std::string const& filename) {
    uint shader = Shader::buildVertexShader(filename);
    exclusiveShaders.insert(shader);
    glAttachShader(ID, shader);
}

void ShaderProgram::setFragShader(uint shader) {
    glAttachShader(ID, shader);
}

void ShaderProgram::setFragShader(std::string const& filename) {
    uint shader = Shader::buildFragShader(filename);
    exclusiveShaders.insert(shader);
    glAttachShader(ID, shader);
}

void ShaderProgram::link() {
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cerr << infoLog << std::endl;
        throw "Failed to link shader program " + std::to_string(ID);
    }

    for (uint shader : exclusiveShaders) {
        glDeleteShader(shader);
    }
}


