#include "Shader.h"

Shader::Shader()
{}

Shader::Shader( const char* vertPath, const char* fragPath )
{
	loadAndCompile( vertPath, fragPath );
}

Shader::~Shader()
{
	glDeleteProgram( this->programID );
}

void Shader::loadAndCompile( const char* vertPath, const char* fragPath )
{
	std::string vsCode = "";
	std::ifstream vsStream( vertPath, std::ios::in );

	fprintf( stdout, "loading vertex shader %s\n", vertPath );

	if( vsStream.is_open() )
	{
		std::string line;
		while( std::getline( vsStream, line ) )
			vsCode += line + '\n';
		vsStream.close();
	}
	else
	{
		fprintf( stderr, "Error: could not find vertex shader %s\n", vertPath );
		return;
	}

	std::string fsCode = "";
	std::ifstream fsStream( fragPath, std::ios::in );

	fprintf( stdout, "loading fragment shader %s\n", fragPath );

	if( fsStream.is_open() )
	{
		std::string line;
		while( std::getline( fsStream, line ) )
			fsCode += line + '\n';
		fsStream.close();
	}
	else
	{
		fprintf( stderr, "Error: could not find fragment shader %s\n", fragPath );
		return;
	}

	fprintf( stdout, "Compiling vertexShader %s\n", vertPath );

	const char* vertCode = vsCode.c_str();
	GLuint vertShader = glCreateShader( GL_VERTEX_SHADER );
	
	glShaderSource( vertShader, 1, &vertCode, NULL );
	glCompileShader( vertShader );
	
	GLint result = GL_FALSE;
	int infoLogLength;

	glGetShaderiv( vertShader, GL_COMPILE_STATUS, &result );
	glGetShaderiv( vertShader, GL_INFO_LOG_LENGTH, &infoLogLength );

	if( infoLogLength > 0 )
	{
		std::vector< char > errorMsg( infoLogLength );
		glGetShaderInfoLog( vertShader, infoLogLength, NULL, &errorMsg[ 0 ] );
		fprintf( stderr, "%s\n", &errorMsg[ 0 ] );
	}

	fprintf( stdout, "Compiling fragmentShader %s\n", fragPath );

	const char* fragCode = fsCode.c_str();
	GLuint fragShader = glCreateShader( GL_FRAGMENT_SHADER );
	
	glShaderSource( fragShader, 1, &fragCode, NULL );
	glCompileShader( fragShader );
	
	glGetShaderiv( fragShader, GL_COMPILE_STATUS, &result );
	glGetShaderiv( fragShader, GL_INFO_LOG_LENGTH, &infoLogLength );

	if( infoLogLength > 0 )
	{
		std::vector< char > errorMsg( infoLogLength );
		glGetShaderInfoLog( fragShader, infoLogLength, NULL, &errorMsg[ 0 ] );
		fprintf( stderr, "%s\n", &errorMsg[ 0 ] );
	}

	fprintf( stdout, "Linking program...\n" );

	GLuint tmpProgID = glCreateProgram();

	glAttachShader( tmpProgID, vertShader );
	glAttachShader( tmpProgID, fragShader );
	glLinkProgram( tmpProgID );

	glGetProgramiv( tmpProgID, GL_LINK_STATUS, &result );
	glGetProgramiv( tmpProgID, GL_INFO_LOG_LENGTH, &infoLogLength );
	if( infoLogLength > 0 )
	{
		std::vector< char > errorMsg( infoLogLength );
		glGetProgramInfoLog( tmpProgID, infoLogLength, NULL, &errorMsg[ 0 ] );
		fprintf( stderr, "%s\n", &errorMsg[ 0 ] );
	}

	glDeleteShader( vertShader );
	glDeleteShader( fragShader );

	this->programID = tmpProgID;
}

GLuint Shader::getProgram()
{
	return this->programID;
}