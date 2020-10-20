// (c) 2020 Alexander Jenkins
//
// File Name   : ShaderLoader.h
// Description : shader loader declaration file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#pragma once

#include "Utils.h"

class ShaderLoader
{
	
public:
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* GeometryShaderFilename);
	static GLuint CreateComputeProgram(const char* ComputeShaderFilename);
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename);

private:
	ShaderLoader(void);
	~ShaderLoader(void);
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char *filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);
};