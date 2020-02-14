#include "Renderer.h"

Renderer::Renderer( Camera& camera )
	:
cam( camera )
{}

Renderer::~Renderer()
{
	glDeleteBuffers( 1, &quad );
	glDeleteBuffers( 1, &color );
	glDeleteBuffers( 1, &UV );
	glDeleteVertexArrays( 1, &vao );
}

void Renderer::load( bool OGL3, int w, int h )
{
	this->OGL3 = OGL3;
	
	this->winW = w;
	this->winH = h;

	if( OGL3 )
	{
		projectionMatrix = glm::mat4( 1.0f );
		projectionMatrix = glm::perspective<float>( 45.0f, (float) winW / (float) winH, 0.16f, 1600.0f );

		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );
	
		shader.loadAndCompile( "Res/Shaders/std.vs", "Res/Shaders/std.fs" );
		shaderNoTex.loadAndCompile( "Res/Shaders/NoTex.vs", "Res/Shaders/NoTex.fs" );

		glUseProgram( shader.getProgram() );
		isShaderSTD = true;
		isShaderNoTex = false;
		
		ground.loadTexture( "Res/Tex/ground.png" );

		texture.loadTexture( "Res/Tex/sheet.png" );
		texture.bind();

		static Vertex quadArray[] = {
			{ -0.5, -0.5, 0.0 },
			{ -0.5, 0.5, 0.0 },
			{ 0.5, -0.5, 0.0 },
			{ 0.5, -0.5, 0.0 },
			{ 0.5, 0.5, 0.0 },
			{ -0.5, 0.5, 0.0 }
		};
	
		static Vertex cubeArray[] = {
			{ -0.5, -0.5, 0.5 },
			{ -0.5, 0.5, 0.5 },
			{ 0.5, -0.5, 0.5 },
			{ 0.5, -0.5, 0.5 },
			{ 0.5, 0.5, 0.5 },
			{ -0.5, 0.5, 0.5 },

			{ 0.5, -0.5, 0.5 },
			{ 0.5, 0.5, 0.5 },
			{ 0.5, -0.5, -0.5 },
			{ 0.5, -0.5, -0.5 },
			{ 0.5, 0.5, -0.5 },
			{ 0.5, 0.5, 0.5 },

			{ 0.5, -0.5, -0.5 },
			{ 0.5, 0.5, -0.5 },
			{ -0.5, -0.5, -0.5 },
			{ -0.5, -0.5, -0.5 },
			{ -0.5, 0.5, -0.5 },
			{ 0.5, 0.5, -0.5 },

			{ -0.5, -0.5, -0.5 },
			{ -0.5, 0.5, -0.5 },
			{ -0.5, -0.5, 0.5 },
			{ -0.5, -0.5, 0.5 },
			{ -0.5, 0.5, 0.5 },
			{ -0.5, 0.5, -0.5 },

			{ -0.5, 0.5, 0.5 },
			{ -0.5, 0.5, -0.5 },
			{ 0.5, 0.5, 0.5 },
			{ 0.5, 0.5, 0.5 },
			{ 0.5, 0.5, -0.5 },
			{ -0.5, 0.5, -0.5 },

			{ -0.5, -0.5, 0.5 },
			{ -0.5, -0.5, -0.5 },
			{ 0.5, -0.5, 0.5 },
			{ 0.5, -0.5, 0.5 },
			{ 0.5, -0.5, -0.5 },
			{ -0.5, -0.5, -0.5 }
		};
		
		MVPLocation = glGetUniformLocation( shader.getProgram(), "MVP" );
		texMatLoc = glGetUniformLocation( shader.getProgram(), "textureMatrix" );
		MVPLocationNoTex = glGetUniformLocation( shaderNoTex.getProgram(), "MVP" );

		glGenBuffers( 1, &quad );
		glBindBuffer( GL_ARRAY_BUFFER, quad );
		glBufferData( GL_ARRAY_BUFFER, sizeof( quadArray ), quadArray, GL_STATIC_DRAW );

		glGenBuffers( 1, &cube );
		glBindBuffer( GL_ARRAY_BUFFER, cube );
		glBufferData( GL_ARRAY_BUFFER, sizeof( cubeArray ), cubeArray, GL_STATIC_DRAW );

		glGenBuffers( 1, &color );
		glBindBuffer( GL_ARRAY_BUFFER, color );
		glBufferData( GL_ARRAY_BUFFER, 48, NULL, GL_STREAM_DRAW );

		glGenBuffers( 1, &UV );
		glBindBuffer( GL_ARRAY_BUFFER, UV );
		glBufferData( GL_ARRAY_BUFFER, 48, NULL, GL_STREAM_DRAW );
	}
	else
	{
		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluPerspective( 45.0f, (float) winW / (float) winH, 0.01, 100.0f );
		glMatrixMode( GL_MODELVIEW );

		texture.loadTexture( "Res/Tex/sheet.png" );
		texture.bind();
	}
}

void Renderer::changePerspective( int w, int h )
{
	this->winW = w;
	this->winH = h;
	
	projectionMatrix = glm::mat4( 1.0f );
	projectionMatrix = glm::perspective<float>( 45.0f, (float) winW / (float) winH, 0.16f, 1600.0f );
}

void Renderer::translate( glm::vec3 pos )
{
	if( OGL3 )
		this->transMatrix = glm::translate( glm::mat4( 1.0f ), pos );
	else
		glTranslatef( pos.x, pos.y, pos.z );
}

void Renderer::translate( float x, float y, float z )
{
	translate( glm::vec3( x, y, z ) );
}

void Renderer::rotate( float angle, glm::vec3 axis )
{
	if( OGL3 )
		this->rotationMatrix = glm::rotate( glm::mat4( 1.0f ), angle, axis );
	else
		glRotatef( angle, axis.x, axis.y, axis.z );
}

void Renderer::scale( glm::vec3 scale )
{
	if( OGL3 )
		this->scaleMatrix = glm::scale( glm::mat4( 1.0f ), scale );
	else
		glScalef( scale.x, scale.y, scale.z );
}

void Renderer::scale( float x, float y, float z )
{
	scale( glm::vec3( x, y, z ) );
}

void Renderer::loadIdentity()
{
	this->transMatrix = glm::mat4( 1.0f );
	this->rotationMatrix = glm::mat4( 1.0f );
	this->scaleMatrix = glm::mat4( 1.0f );
}

void Renderer::setTexture( std::string tex )
{
	if( tex == "sheet" )
		texture.bind();
	else if( tex == "ground" )
		ground.bind();
}

void Renderer::drawSpriteBillboarded( glm::vec3 pos, glm::vec3 lookAtPos, int w, int h, int uP, int vP, glm::vec4 col )
{
	w *= 16;
	h *= 16;

	int texW = texture.getWidth(), texH = texture.getHeight();

	Color colArray[] = {
		{ col.r, col.g, col.b, col.a },
		{ col.r, col.g, col.b, col.a },
		{ col.r, col.g, col.b, col.a },
		{ col.r, col.g, col.b, col.a },
		{ col.r, col.g, col.b, col.a },
		{ col.r, col.g, col.b, col.a }
	};

	UVs uvArray[] = {
		{ 0, 1 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 }
	};

	translate( pos );
	
	glm::vec2 dist = glm::vec2( -lookAtPos.x, -lookAtPos.z ) - glm::vec2( pos.x, pos.z );

	rotate( RADTODEG( atan2f( dist.y, dist.x ) ) - 90, glm::vec3( 0, 1, 0 ) );

	scale( w / 16 * 1.0, h / 16 * 1.0, 1.0 );

	modelMatrix = transMatrix * rotationMatrix * scaleMatrix;

	camTransMatrix = glm::translate( glm::mat4( 1.0f ), glm::vec3( -cam.getPosX(), -cam.getPosY(), -cam.getPosZ() ) );
	camRotationXMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotX(), glm::vec3( 1, 0, 0 ) );
	camRotationYMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotY(), glm::vec3( 0, 1, 0 ) );
	camRotationZMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotZ(), glm::vec3( 0, 0, 1 ) );
	glm::mat4 camRot = camRotationXMatrix * camRotationYMatrix * camRotationZMatrix;

	viewMatrix = camTransMatrix * camRot;
	
	modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
	
	glUniformMatrix4fv( MVPLocation, 1, GL_FALSE, &modelViewProjectionMatrix[ 0 ][ 0 ] );

	glBindBuffer( GL_ARRAY_BUFFER, quad );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

	glBindBuffer( GL_ARRAY_BUFFER, color );
	glBufferData( GL_ARRAY_BUFFER, sizeof( colArray ), colArray, GL_STREAM_DRAW );

	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0 );
	
	glBindBuffer( GL_ARRAY_BUFFER, UV );
	glBufferData( GL_ARRAY_BUFFER, sizeof( uvArray ), uvArray, GL_STREAM_DRAW );

	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glDrawArrays( GL_TRIANGLES, 0, 6 );
}

void Renderer::drawSprite( int w, int h, int uP, int vP, glm::vec4 col )
{
	if( !isShaderSTD && isShaderNoTex )
	{
		glUseProgram( shader.getProgram() );

		isShaderSTD = true;
		isShaderNoTex = false;
	}

	int texW = texture.getWidth(), texH = texture.getHeight();

	float u, v, uEnd, vEnd;

	Color colArray[] = {
		{ col.r, col.g, col.b, col.a },
		{ col.r, col.g, col.b, col.a },
		{ col.r, col.g, col.b, col.a },
		{ col.r, col.g, col.b, col.a },
		{ col.r, col.g, col.b, col.a },
		{ col.r, col.g, col.b, col.a }
	};

	UVs uvArray[] = {
		{ 0, 1 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 }
	};

	glm::mat4 texMat( 1.0f );

	texMat = glm::scale( texMat, glm::vec3( 1.0/texW, 1.0/texH, 0.0 ) );
	texMat = glm::translate( texMat, glm::vec3( uP+0.25, vP+0.25, 0.0 ) );
	texMat = glm::scale( texMat, glm::vec3( w - 0.5, h - 0.5, 0.0 ) );

	glUniformMatrix4fv( texMatLoc, 1, GL_FALSE, &texMat[ 0 ][ 0 ] );

	scale( w * 1.0, h * 1.0, 1.0 );

	modelMatrix = transMatrix * rotationMatrix * scaleMatrix;

	camTransMatrix = glm::translate( glm::mat4( 1.0f ), glm::vec3( -cam.getPosX(), -cam.getPosY(), -cam.getPosZ() ) );
	camRotationXMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotX(), glm::vec3( 1, 0, 0 ) );
	camRotationYMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotY(), glm::vec3( 0, 1, 0 ) );
	camRotationZMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotZ(), glm::vec3( 0, 0, 1 ) );
	glm::mat4 camRot = camRotationXMatrix * camRotationYMatrix * camRotationZMatrix;

	viewMatrix = camTransMatrix * camRot;
	
	modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
	
	glUniformMatrix4fv( MVPLocation, 1, GL_FALSE, &modelViewProjectionMatrix[ 0 ][ 0 ] );

	glBindBuffer( GL_ARRAY_BUFFER, quad );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

	glBindBuffer( GL_ARRAY_BUFFER, color );
	glBufferData( GL_ARRAY_BUFFER, sizeof( colArray ), colArray, GL_STREAM_DRAW );

	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0 );
	
	glBindBuffer( GL_ARRAY_BUFFER, UV );
	glBufferData( GL_ARRAY_BUFFER, sizeof( uvArray ), uvArray, GL_STREAM_DRAW );

	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

	glDrawArrays( GL_TRIANGLES, 0, 6 );
}

void Renderer::drawQuad( glm::vec4 col )
{
	if( isShaderSTD && !isShaderNoTex )
	{
		glUseProgram( shaderNoTex.getProgram() );

		isShaderSTD = false;
		isShaderNoTex = true;
	}

	Color colArray[] = {
		{ col.x, col.y, col.z, col.w },
		{ col.x, col.y, col.z, col.w },
		{ col.x, col.y, col.z, col.w },
		{ col.x, col.y, col.z, col.w },
		{ col.x, col.y, col.z, col.w },
		{ col.x, col.y, col.z, col.w }
	};

	modelMatrix = transMatrix * rotationMatrix * scaleMatrix;

	camTransMatrix = glm::translate( glm::mat4( 1.0f ), glm::vec3( -cam.getPosX(), -cam.getPosY(), -cam.getPosZ() ) );
	camRotationXMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotX(), glm::vec3( 1, 0, 0 ) );
	camRotationYMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotY(), glm::vec3( 0, 1, 0 ) );
	camRotationZMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotZ(), glm::vec3( 0, 0, 1 ) );
	glm::mat4 camRot = camRotationXMatrix * camRotationYMatrix * camRotationZMatrix;

	viewMatrix = camTransMatrix * camRot;
	
	modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
	
	glUniformMatrix4fv( MVPLocationNoTex, 1, GL_FALSE, &modelViewProjectionMatrix[ 0 ][ 0 ] );

	glBindBuffer( GL_ARRAY_BUFFER, quad );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

	glBindBuffer( GL_ARRAY_BUFFER, color );
	glBufferData( GL_ARRAY_BUFFER, sizeof( colArray ), colArray, GL_STREAM_DRAW );

	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0 );
	
	glDrawArrays( GL_TRIANGLES, 0, 6 );
}

void Renderer::drawCube( int w, int h, int uP, int vP, glm::vec4 col )
{
	int texW = texture.getWidth(), texH = texture.getHeight();

	float u, v, uEnd, vEnd;

	if( OGL3 )
	{
		Color colArray[] = {
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },

			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },

			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },

			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },

			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },

			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a },
			{ col.r, col.g, col.b, col.a }
		};

		UVs uvArray[] = {
		{ 0, 1 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },

		{ 0, 1 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },

		{ 0, 1 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },

		{ 0, 1 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },

		{ 0, 1 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 },

		{ 0, 1 },
		{ 0, 0 },
		{ 1, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 0, 0 }
		};

		glm::mat4 texMat( 1.0f );
		
		texMat = glm::scale( texMat, glm::vec3( 1.0/texW, 1.0/texH, 0.0 ) );
		texMat = glm::translate( texMat, glm::vec3( uP+0.25, vP+0.25, 0.0 ) );
		texMat = glm::scale( texMat, glm::vec3( w - 0.5, h - 0.5, 0.0 ) );

		glUniformMatrix4fv( texMatLoc, 1, GL_FALSE, &texMat[ 0 ][ 0 ] );

		scale( w * 1.0, h * 1.0, 1.0 );

		modelMatrix = transMatrix * rotationMatrix * scaleMatrix;

		camTransMatrix = glm::translate( glm::mat4( 1.0f ), glm::vec3( -cam.getPosX(), -cam.getPosY(), -cam.getPosZ() ) );
		camRotationXMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotX(), glm::vec3( 1, 0, 0 ) );
		camRotationYMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotY(), glm::vec3( 0, 1, 0 ) );
		camRotationZMatrix = glm::rotate( glm::mat4( 1.0f ), cam.getRotZ(), glm::vec3( 0, 0, 1 ) );
		glm::mat4 camRot = camRotationXMatrix * camRotationYMatrix * camRotationZMatrix;

		viewMatrix = camTransMatrix * camRot;
	
		modelViewProjectionMatrix = projectionMatrix * viewMatrix * modelMatrix;
	
		glUniformMatrix4fv( MVPLocation, 1, GL_FALSE, &modelViewProjectionMatrix[ 0 ][ 0 ] );

		glBindBuffer( GL_ARRAY_BUFFER, cube );

		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0 );

		glBindBuffer( GL_ARRAY_BUFFER, color );
		glBufferData( GL_ARRAY_BUFFER, sizeof( colArray ), colArray, GL_STREAM_DRAW );

		glEnableVertexAttribArray( 1 );
		glVertexAttribPointer( 1, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0 );
	
		glBindBuffer( GL_ARRAY_BUFFER, UV );
		glBufferData( GL_ARRAY_BUFFER, sizeof( uvArray ), uvArray, GL_STREAM_DRAW );

		glEnableVertexAttribArray( 2 );
		glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

		glDrawArrays( GL_TRIANGLES, 0, 6*6 );
	}
	else
	{
		glm::mat4 trans( 1.0f ), rot( 1.0f );
		glm::mat4 view = glm::mat4( 1.0f );

		trans = glm::translate( glm::mat4( 1.0f ), -cam.getPos() );
		rot = glm::rotate( glm::mat4( 1.0f ), cam.getRotX(), glm::vec3( 1, 0, 0 ) );
		rot = glm::rotate( glm::mat4( 1.0f ), cam.getRotY(), glm::vec3( 0, 1, 0 ) );
		rot = glm::rotate( glm::mat4( 1.0f ), cam.getRotZ(), glm::vec3( 0, 0, 1 ) );

		view = trans * rot;

		glMultMatrixf( &view[ 0 ][ 0 ] );

		glColor4ub( col.r, col.g, col.b, col.a );
		glBegin( GL_TRIANGLES );
			glTexCoord2f( u, vEnd );
			glVertex3f( -0.5, -0.5, 0.5 );
			glTexCoord2f( u, v );
			glVertex3f( -0.5, 0.5, 0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( 0.5, -0.5, 0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( 0.5, -0.5, 0.5 );
			glTexCoord2f( uEnd, v );
			glVertex3f( 0.5, 0.5, 0.5 );
			glTexCoord2f( u, v );
			glVertex3f( -0.5, 0.5, 0.5 );

			glTexCoord2f( u, vEnd );
			glVertex3f( 0.5, -0.5, 0.5 );
			glTexCoord2f( u, v );
			glVertex3f( 0.5, 0.5, 0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( 0.5, -0.5, -0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( 0.5, -0.5, -0.5 );
			glTexCoord2f( uEnd, v );
			glVertex3f( 0.5, 0.5, -0.5 );
			glTexCoord2f( u, v );
			glVertex3f( 0.5, 0.5, 0.5 );

			glTexCoord2f( u, vEnd );
			glVertex3f( 0.5, -0.5, -0.5 );
			glTexCoord2f( u, v );
			glVertex3f( 0.5, 0.5, -0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( -0.5, -0.5, -0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( -0.5, -0.5, -0.5 );
			glTexCoord2f( uEnd, v );
			glVertex3f( -0.5, 0.5, -0.5 );
			glTexCoord2f( u, v );
			glVertex3f( 0.5, 0.5, -0.5 );

			glTexCoord2f( u, vEnd );
			glVertex3f( -0.5, -0.5, -0.5 );
			glTexCoord2f( u, v );
			glVertex3f( -0.5, 0.5, -0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( -0.5, -0.5, 0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( -0.5, -0.5, 0.5 );
			glTexCoord2f( uEnd, v );
			glVertex3f( -0.5, 0.5, 0.5 );
			glTexCoord2f( u, v );
			glVertex3f( -0.5, 0.5, -0.5 );

			glTexCoord2f( u, vEnd );
			glVertex3f( -0.5, 0.5, 0.5 );
			glTexCoord2f( u, v );
			glVertex3f( -0.5, 0.5, -0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( 0.5, 0.5, 0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( 0.5, 0.5, 0.5 );
			glTexCoord2f( uEnd, v );
			glVertex3f( 0.5, 0.5, -0.5 );
			glTexCoord2f( u, v );
			glVertex3f( -0.5, 0.5, -0.5 );

			glTexCoord2f( u, vEnd );
			glVertex3f( -0.5, -0.5, 0.5 );
			glTexCoord2f( u, v );
			glVertex3f( -0.5, -0.5, -0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( 0.5, -0.5, 0.5 );
			glTexCoord2f( uEnd, vEnd );
			glVertex3f( 0.5, -0.5, 0.5 );
			glTexCoord2f( uEnd, v );
			glVertex3f( 0.5, -0.5, -0.5 );
			glTexCoord2f( u, v );
			glVertex3f( -0.5, -0.5, -0.5 );
		glEnd();
	}
}

void Renderer::drawText( std::string text, glm::vec2 pos, glm::vec4 col )
{
	glDisable( GL_DEPTH_TEST );

	for( int i = 0; i < text.length(); i++ )
	{
		char c = text[ i ];

		int tP = ( (int)c - 32 );

		translate( pos.x, pos.y, 0.0 );
		
		if( tP >= 0 && tP < 32 * 3 )
		{
			drawSprite( 6, 8, ( tP % 16 * 6 ), ( tP / 16 * 8 + 64 ), col );
		}
		else
		{
			drawSprite( 6, 8, 90, 104, col );
		}

		pos.x += 6;
	}

	glEnable( GL_DEPTH_TEST );
}