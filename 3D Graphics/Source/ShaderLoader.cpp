// (c) 2020 Alexander Jenkins
//
// File Name   : shaderLoader.cpp
// Description : shaderLoader implementation file
// Author      : alexander jenkins
// Mail        : alexander.jen8470@mediadesign.school.nz
//

#include "ShaderLoader.h" 

ShaderLoader::ShaderLoader(void){}
ShaderLoader::~ShaderLoader(void){}

GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	GLuint program = glCreateProgram();

	GLuint vertexshader =  CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
	GLuint fragshader =  CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

	glAttachShader(program, vertexshader);
	glAttachShader(program, fragshader);
	glLinkProgram(program);


	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = vertexShaderFilename + *fragmentShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}
	return program;
}

GLuint ShaderLoader::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* GeometryShaderFilename)
{
	GLuint program = glCreateProgram();

	GLuint vertexshader =  CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
	GLuint fragshader =  CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);
	GLuint geometryshader = CreateShader(GL_GEOMETRY_SHADER, GeometryShaderFilename);

	glAttachShader(program, vertexshader);
	glAttachShader(program, fragshader);
	glAttachShader(program, geometryshader);
	glLinkProgram(program);


	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = vertexShaderFilename + *fragmentShaderFilename + *GeometryShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}
	return program;
}


GLuint ShaderLoader::CreateComputeProgram(const char* ComputeShaderFilename)
{
	GLuint program = glCreateProgram();


	GLuint computeShader = CreateShader(GL_COMPUTE_SHADER, ComputeShaderFilename);

	glAttachShader(program, computeShader);
	glLinkProgram(program);


	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = ComputeShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}
	return program;
}

GLuint ShaderLoader::CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* TessControlShaderFilename, const char* TessEvalShaderFilename)
{
	GLuint program = glCreateProgram();
	GLuint vertexshader = CreateShader(GL_VERTEX_SHADER, VertexShaderFilename);
	GLuint fragshader = CreateShader(GL_FRAGMENT_SHADER, FragmentShaderFilename);
	GLuint tessControlShader = CreateShader(GL_TESS_CONTROL_SHADER, TessControlShaderFilename);
	GLuint TessEvalShader = CreateShader(GL_TESS_EVALUATION_SHADER, TessEvalShaderFilename);
	glAttachShader(program, vertexshader);
	glAttachShader(program, fragshader);
	glAttachShader(program, tessControlShader);
	glAttachShader(program, TessEvalShader);
	glLinkProgram(program);


	// Check for link errors
	int link_result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (link_result == GL_FALSE)
	{
		std::string programName = VertexShaderFilename + *FragmentShaderFilename + *TessControlShaderFilename + *TessEvalShaderFilename;
		PrintErrorDetails(false, program, programName.c_str());
		return 0;
	}
	return program;
}

GLuint ShaderLoader::CreateShader(GLenum shaderType, const char* shaderName)
{
	GLuint shaderID = glCreateShader(shaderType);

	std::string shadersource = ReadShaderFile(shaderName);

	const GLchar* source = shadersource.c_str();

	std::string sLength = shadersource.c_str();
	
	const GLint sourcelength = sLength.size();

	glShaderSource(shaderID, 1, &source, &sourcelength);
	glCompileShader(shaderID);


	// Check for errors
	int compile_result = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		PrintErrorDetails(true, shaderID, shaderName);
		return 0;
	}
	return shaderID;
}

std::string ShaderLoader::ReadShaderFile(const char *filename)
{
	// Open the file for reading
	std::ifstream file(filename, std::ios::in);
	std::string shaderCode;

	// Ensure the file is open and readable
	if (!file.good()) {
		std::cout << "Cannot read file:  " << filename << std::endl;
		return "";
	}

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	return shaderCode;
}

void ShaderLoader::PrintErrorDetails(bool isShader, GLuint id, const char* name)
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(isShader == true) ? glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength) : glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	(isShader == true) ? glGetShaderInfoLog(id, infoLogLength, NULL, &log[0]) : glGetProgramInfoLog(id, infoLogLength, NULL, &log[0]);		
	std::cout << "Error compiling " << ((isShader == true) ? "shader" : "program") << ": " << name << std::endl;
	std::cout << &log[0] << std::endl;
}