#include "olOpenGL.h"
//#include "GUtils.h"

GLenum g_OpenGLError                = GL_NO_ERROR;
GLuint g_OpenGLInitExtensionsStatus = OPENGL_INIT_EXTENSIONS_SUCCESS;

#define OPENGL_INT_PRINT_DEBUG(name)			\
	GLint info_ ## name;						\
	glGetIntegerv(name, &info_ ## name);		\
	log(#name " = %d\n", info_ ## name);

void OpenGLPrintExtList()
{
	const char* extension = (const char*)glGetString (GL_EXTENSIONS);
	char name[1024];

	FILE *out = fopen("ogl_supported_extentions.txt", "w");

	int	i, j;
	fprintf(out, "Supported extensions:\n");
	for (i = 0, j = 0; extension[i] != '\0'; i++)
		if (!isspace(extension[i]))
			name[j++] = extension[i];
		else
		{
			name[j] = '\0';
			fprintf(out, "\t%s\n", name);
			j = 0;
		}
	if (j > 0)
	{
		name[j] = '\0';
		fprintf(out, "\t%s\n", name);
	}
	fclose(out);
}

void OpenGLPrintDebugInfo()
{
//	// выведем в лог немного информации о контексте OpenGL
//	GLint major, minor;
//	glGetIntegerv(GL_MAJOR_VERSION, &major);
//	glGetIntegerv(GL_MINOR_VERSION, &minor);
//	LOG_DEBUG("OpenGL render context information:\n"
//		"  Renderer       : %s\n"
//		"  Vendor         : %s\n"
//		"  Version        : %s\n"
//		"  GLSL version   : %s\n"
//		"  OpenGL version : %d.%d\n",
//		(const char*)glGetString(GL_RENDERER),
//		(const char*)glGetString(GL_VENDOR),
//		(const char*)glGetString(GL_VERSION),
//		(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION),
//		major, minor
//	);
//
//	// значения различных параметров OpenGL
//	OPENGL_INT_PRINT_DEBUG(GL_MAX_VERTEX_ATTRIBS);
//	OPENGL_INT_PRINT_DEBUG(GL_MAX_VERTEX_UNIFORM_COMPONENTS);
//	OPENGL_INT_PRINT_DEBUG(GL_MAX_TEXTURE_IMAGE_UNITS);
//	OPENGL_INT_PRINT_DEBUG(GL_MAX_COLOR_ATTACHMENTS);
//	OPENGL_INT_PRINT_DEBUG(GL_MIN_PROGRAM_TEXEL_OFFSET);
//	OPENGL_INT_PRINT_DEBUG(GL_MAX_PROGRAM_TEXEL_OFFSET);
//
//	OPENGL_CHECK_FOR_ERRORS();
}

bool OpenGLInitExtensions()
{
	g_OpenGLInitExtensionsStatus = OPENGL_INIT_EXTENSIONS_SUCCESS;

	// Texture
	//OPENGL_GET_PROC(PFNGLACTIVETEXTUREPROC,         glActiveTexture);
	OPENGL_GET_PROC(PFNGLCOMPRESSEDTEXIMAGE2DPROC, glCompressedTexImage2D);
	// VAO
	//OPENGL_GET_PROC(PFNGLGENVERTEXARRAYSPROC,    glGenVertexArrays);
	//OPENGL_GET_PROC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
	//OPENGL_GET_PROC(PFNGLBINDVERTEXARRAYPROC,    glBindVertexArray);	
	// VBO
	//OPENGL_GET_PROC(PFNGLGENBUFFERSPROC,    glGenBuffers);
	//OPENGL_GET_PROC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
	//OPENGL_GET_PROC(PFNGLBINDBUFFERPROC,    glBindBuffer);
	//OPENGL_GET_PROC(PFNGLBUFFERDATAPROC,    glBufferData);
	//OPENGL_GET_PROC(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
	//OPENGL_GET_PROC(PFNGLMAPBUFFERPROC,     glMapBuffer);
	//OPENGL_GET_PROC(PFNGLUNMAPBUFFERPROC,   glUnmapBuffer);
	// Shaders
	OPENGL_GET_PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
	OPENGL_GET_PROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
	OPENGL_GET_PROC(PFNGLLINKPROGRAMPROC, glLinkProgram);
	//OPENGL_GET_PROC(PFNGLVALIDATEPROGRAMPROC,   glValidateProgram);
	OPENGL_GET_PROC(PFNGLUSEPROGRAMPROC, glUseProgram);
	OPENGL_GET_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
	OPENGL_GET_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
	OPENGL_GET_PROC(PFNGLCREATESHADERPROC, glCreateShader);
	OPENGL_GET_PROC(PFNGLDELETESHADERPROC, glDeleteShader);
	OPENGL_GET_PROC(PFNGLSHADERSOURCEPROC, glShaderSource);
	OPENGL_GET_PROC(PFNGLCOMPILESHADERPROC, glCompileShader);
	OPENGL_GET_PROC(PFNGLATTACHSHADERPROC, glAttachShader);
	//OPENGL_GET_PROC(PFNGLDETACHSHADERPROC,      glDetachShader);
	OPENGL_GET_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv);
	OPENGL_GET_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
	OPENGL_GET_PROC(PFNGLISSHADERPROC, glIsShader);
	// Attributes
	//OPENGL_GET_PROC(PFNGLGETATTRIBLOCATIONPROC,        glGetAttribLocation);
	//OPENGL_GET_PROC(PFNGLVERTEXATTRIBPOINTERPROC,      glVertexAttribPointer);
	//OPENGL_GET_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC,  glEnableVertexAttribArray);
	//OPENGL_GET_PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
	// Uniforms
	OPENGL_GET_PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
	//OPENGL_GET_PROC(PFNGLUNIFORMMATRIX3FVPROC,   glUniformMatrix3fv);
	//OPENGL_GET_PROC(PFNGLUNIFORMMATRIX4FVPROC,   glUniformMatrix4fv);
	//OPENGL_GET_PROC(PFNGLUNIFORM1IPROC,          glUniform1i);
	OPENGL_GET_PROC(PFNGLUNIFORM1FVPROC, glUniform1fv);
	//OPENGL_GET_PROC(PFNGLUNIFORM3FVPROC,         glUniform3fv);
	//OPENGL_GET_PROC(PFNGLUNIFORM4FVPROC,         glUniform4fv);
	//  OPENGL_GET_PROC(PFNGLUNIFORM1FPROC,          glUniform1f);

	//OPENGL_GET_PROC1(PFNGLISRENDERBUFFEREXTPROC,						   glIsRenderbuffer,						   glIsRenderbufferEXT);
	//OPENGL_GET_PROC1(PFNGLBINDRENDERBUFFEREXTPROC,						glBindRenderbuffer,						glBindRenderbufferEXT);
	//OPENGL_GET_PROC1(PFNGLDELETERENDERBUFFERSEXTPROC,					glDeleteRenderbuffers,					glDeleteRenderbuffersEXT);
	//OPENGL_GET_PROC1(PFNGLGENRENDERBUFFERSEXTPROC,						glGenRenderbuffers,						glGenRenderbuffersEXT);
	//OPENGL_GET_PROC1(PFNGLRENDERBUFFERSTORAGEEXTPROC,					glRenderbufferStorage,					glRenderbufferStorageEXT);
	//OPENGL_GET_PROC1(PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC,			glGetRenderbufferParameteriv,			glGetRenderbufferParameterivEXT);

	//OPENGL_GET_PROC1(PFNGLBINDFRAMEBUFFEREXTPROC,						glBindFramebuffer,						glBindFramebufferEXT);
	//OPENGL_GET_PROC1(PFNGLDELETEFRAMEBUFFERSEXTPROC,					glDeleteFramebuffers,					glDeleteFramebuffersEXT);
	//OPENGL_GET_PROC1(PFNGLGENFRAMEBUFFERSEXTPROC,						glGenFramebuffers,						glGenFramebuffersEXT);
	//OPENGL_GET_PROC1(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC,				glCheckFramebufferStatus,				glCheckFramebufferStatusEXT);
	//OPENGL_GET_PROC1(PFNGLFRAMEBUFFERTEXTUREEXTPROC,					glFramebufferTexture,					glFramebufferTextureEXT);
	//OPENGL_GET_PROC1(PFNGLFRAMEBUFFERTEXTURE2DEXTPROC,					glFramebufferTexture2D,					glFramebufferTexture2DEXT);
	//OPENGL_GET_PROC1(PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC,				glFramebufferRenderbuffer,				glFramebufferRenderbufferEXT);
	//OPENGL_GET_PROC1(PFNGLGENERATEMIPMAPEXTPROC,						   glGenerateMipmap,						   glGenerateMipmapEXT);

	OPENGL_CHECK_FOR_ERRORS();

	return true;//(g_OpenGLInitExtensionsStatus == OPENGL_INIT_EXTENSIONS_SUCCESS);
}

// объявим расширения OpenGL
// Texture
//PFNGLACTIVETEXTUREPROC              glActiveTexture               = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC       glCompressedTexImage2D        = NULL;
// VAO
//PFNGLGENVERTEXARRAYSPROC            glGenVertexArrays             = NULL;
//PFNGLDELETEVERTEXARRAYSPROC         glDeleteVertexArrays          = NULL;
//PFNGLBINDVERTEXARRAYPROC            glBindVertexArray             = NULL;
// VBO
//PFNGLGENBUFFERSPROC                 glGenBuffers                  = NULL;
//PFNGLDELETEBUFFERSPROC              glDeleteBuffers               = NULL;
//PFNGLBINDBUFFERPROC                 glBindBuffer                  = NULL;
//PFNGLBUFFERDATAPROC                 glBufferData                  = NULL;
//PFNGLBUFFERSUBDATAPROC              glBufferSubData               = NULL;
//PFNGLMAPBUFFERPROC                  glMapBuffer                   = NULL;
//PFNGLUNMAPBUFFERPROC                glUnmapBuffer                 = NULL;
// Shaders
PFNGLCREATEPROGRAMPROC              glCreateProgram               = NULL;
PFNGLDELETEPROGRAMPROC              glDeleteProgram               = NULL;
PFNGLLINKPROGRAMPROC                glLinkProgram                 = NULL;
//PFNGLVALIDATEPROGRAMPROC            glValidateProgram             = NULL;
PFNGLUSEPROGRAMPROC                 glUseProgram                  = NULL;
PFNGLGETPROGRAMIVPROC               glGetProgramiv                = NULL;
PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog           = NULL;
PFNGLCREATESHADERPROC               glCreateShader                = NULL;
PFNGLDELETESHADERPROC               glDeleteShader                = NULL;
PFNGLSHADERSOURCEPROC               glShaderSource                = NULL;
PFNGLCOMPILESHADERPROC              glCompileShader               = NULL;
PFNGLATTACHSHADERPROC               glAttachShader                = NULL;
//PFNGLDETACHSHADERPROC               glDetachShader                = NULL;
PFNGLGETSHADERIVPROC                glGetShaderiv                 = NULL;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog            = NULL;
PFNGLISSHADERPROC                   glIsShader                    = NULL;
// Attributes
//PFNGLGETATTRIBLOCATIONPROC          glGetAttribLocation           = NULL;
//PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer         = NULL;
//PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray     = NULL;
//PFNGLDISABLEVERTEXATTRIBARRAYPROC   glDisableVertexAttribArray    = NULL;
// Uniforms
PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation          = NULL;
//PFNGLUNIFORMMATRIX3FVPROC           glUniformMatrix3fv            = NULL;
//PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv            = NULL;
//PFNGLUNIFORM1IPROC                  glUniform1i                   = NULL;
PFNGLUNIFORM1FVPROC                 glUniform1fv                  = NULL;
//PFNGLUNIFORM3FVPROC                 glUniform3fv                  = NULL;
//PFNGLUNIFORM4FVPROC                 glUniform4fv                  = NULL;
//PFNGLUNIFORM1FPROC					   glUniform1f                   = NULL;
// RBO
//PFNGLISRENDERBUFFERPROC				   glIsRenderbuffer				   = NULL;
//PFNGLBINDRENDERBUFFERPROC			   glBindRenderbuffer				= NULL;
//PFNGLDELETERENDERBUFFERSPROC		   glDeleteRenderbuffers			= NULL;
//PFNGLGENRENDERBUFFERSPROC			   glGenRenderbuffers				= NULL;
//PFNGLRENDERBUFFERSTORAGEPROC		   glRenderbufferStorage			= NULL;
//PFNGLGETRENDERBUFFERPARAMETERIVPROC	glGetRenderbufferParameteriv	= NULL;
// FBO
//PFNGLBINDFRAMEBUFFERPROC			   glBindFramebuffer				   = NULL;
//PFNGLDELETEFRAMEBUFFERSPROC			glDeleteFramebuffers			   = NULL;
//PFNGLGENFRAMEBUFFERSPROC			   glGenFramebuffers				   = NULL;
//PFNGLCHECKFRAMEBUFFERSTATUSPROC		glCheckFramebufferStatus		= NULL;
//PFNGLFRAMEBUFFERTEXTUREPROC			glFramebufferTexture			   = NULL;
//PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	   glFramebufferTexture2D			= NULL;
//PFNGLFRAMEBUFFERRENDERBUFFERPROC	   glFramebufferRenderbuffer		= NULL;
   
//PFNGLGENERATEMIPMAPPROC				   glGenerateMipmap				   = NULL;	
