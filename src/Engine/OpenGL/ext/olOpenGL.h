#ifndef OL_OPENGL_H
#define OL_OPENGL_H

#define WIN32_LEAN_AND_MEAN 1

#include <windows.h>
#include <GL/gl.h>
#include "glext.h"
#include "wglext.h"

#include <stdio.h>

#pragma comment(lib, "OpenGL32.Lib")
#pragma comment(lib, "GlU32.Lib")

#ifdef WIN32
	#define GL_GETPROCADDRESS wglGetProcAddress
#else
	#define GL_GETPROCADDRESS glGetProcAddress
#endif

//#define _LOGO_OPENGL_
#ifdef _LOGO_OPENGL_
#include "olLogo.h"
#endif

// глобальная перменная для хранения ошибки OpenGL
extern GLenum g_OpenGLError;

#define OPENGL_INIT_EXTENSIONS_FAIL    0
#define OPENGL_INIT_EXTENSIONS_SUCCESS 1
extern GLuint g_OpenGLInitExtensionsStatus;

// получить адрес функции из драйвера
#define OPENGL_GET_PROC(p, n)														                     \
	n = (p)GL_GETPROCADDRESS(#n);													                     \
	//if (n == NULL) {															                           \
    //  AbsTrace(L_INFO,GLog,"Loading extension '%s' fail (%d)\n", #n, GetLastError());				\
		g_OpenGLInitExtensionsStatus = OPENGL_INIT_EXTENSIONS_FAIL;					            \
	}
#define OPENGL_GET_PROC1(type, ptr, name)											                  \
	ptr = (type)GL_GETPROCADDRESS(#name);											                  \
	if (ptr == NULL) {															                        \
	//	AbsTrace(L_INFO,GLog,"Loading extension '%s' fail (%d)\n", #name, GetLastError());			\
		g_OpenGLInitExtensionsStatus = OPENGL_INIT_EXTENSIONS_FAIL;					            \
	}

// проверка на ошибки OpenGL
#define OPENGL_CHECK_FOR_ERRORS()                                                         \
	if ((g_OpenGLError = glGetError()) != GL_NO_ERROR)                                     \
	//	AbsTrace(L_INFO,GLog,"OpenGL error 0x%X\n", (unsigned)g_OpenGLError);

void OpenGLPrintExtList();

// инициализация необходимых расширений OpenGL
bool OpenGLInitExtensions();

// проверка поддержки расширения
bool OpenGLIsSupportedExtention();

// вывод в лог различной информации по OpenGL, версия, константы и т.п.
void OpenGLPrintDebugInfo();

// расширения OpenGL
// Texture
//extern PFNGLACTIVETEXTUREPROC				   glActiveTexture;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC      glCompressedTexImage2D;
// VAO
//extern PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays;
//extern PFNGLDELETEVERTEXARRAYSPROC			glDeleteVertexArrays;
//extern PFNGLBINDVERTEXARRAYPROC				glBindVertexArray;
// VBO
//extern PFNGLGENBUFFERSPROC					   glGenBuffers;
//extern PFNGLDELETEBUFFERSPROC				   glDeleteBuffers;
//extern PFNGLBINDBUFFERPROC					   glBindBuffer;
//extern PFNGLBUFFERDATAPROC					   glBufferData;
//extern PFNGLBUFFERSUBDATAPROC				   glBufferSubData;
//extern PFNGLMAPBUFFERPROC					   glMapBuffer;
//extern PFNGLUNMAPBUFFERPROC					glUnmapBuffer;
// Shaders
extern PFNGLCREATEPROGRAMPROC				   glCreateProgram;
extern PFNGLDELETEPROGRAMPROC				   glDeleteProgram;
extern PFNGLLINKPROGRAMPROC					glLinkProgram;
//extern PFNGLVALIDATEPROGRAMPROC				glValidateProgram;
extern PFNGLUSEPROGRAMPROC					   glUseProgram;
extern PFNGLGETPROGRAMIVPROC				   glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog;
extern PFNGLCREATESHADERPROC				   glCreateShader;
extern PFNGLDELETESHADERPROC				   glDeleteShader;
extern PFNGLSHADERSOURCEPROC				   glShaderSource;
extern PFNGLCOMPILESHADERPROC				   glCompileShader;
extern PFNGLATTACHSHADERPROC				   glAttachShader;
//extern PFNGLDETACHSHADERPROC				   glDetachShader;
extern PFNGLGETSHADERIVPROC					glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC			   glGetShaderInfoLog;
extern PFNGLISSHADERPROC                  glIsShader;

// Attributes
//extern PFNGLGETATTRIBLOCATIONPROC			glGetAttribLocation;
//extern PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
//extern PFNGLENABLEVERTEXATTRIBARRAYPROC   glEnableVertexAttribArray;
//extern PFNGLDISABLEVERTEXATTRIBARRAYPROC	glDisableVertexAttribArray;

// Uniforms
extern PFNGLGETUNIFORMLOCATIONPROC			glGetUniformLocation;
//extern PFNGLUNIFORMMATRIX3FVPROC			   glUniformMatrix3fv;
//extern PFNGLUNIFORMMATRIX4FVPROC			   glUniformMatrix4fv;
//extern PFNGLUNIFORM1IPROC					   glUniform1i;
extern PFNGLUNIFORM1FVPROC					   glUniform1fv;
//extern PFNGLUNIFORM3FVPROC					   glUniform3fv;
//extern PFNGLUNIFORM4FVPROC					   glUniform4fv;
//extern PFNGLUNIFORM1FPROC					   glUniform1f;

// RBO
//extern PFNGLISRENDERBUFFERPROC				   glIsRenderbuffer;
//extern PFNGLBINDRENDERBUFFERPROC			      glBindRenderbuffer;
//extern PFNGLDELETERENDERBUFFERSPROC			   glDeleteRenderbuffers;
//extern PFNGLGENRENDERBUFFERSPROC			      glGenRenderbuffers;
//extern PFNGLRENDERBUFFERSTORAGEPROC			   glRenderbufferStorage;
//extern PFNGLGETRENDERBUFFERPARAMETERIVPROC	glGetRenderbufferParameteriv;

// FBO
//extern PFNGLBINDFRAMEBUFFERPROC				glBindFramebuffer;
//extern PFNGLDELETEFRAMEBUFFERSPROC			glDeleteFramebuffers;
//extern PFNGLGENFRAMEBUFFERSPROC				glGenFramebuffers;
//extern PFNGLCHECKFRAMEBUFFERSTATUSPROC		glCheckFramebufferStatus;
//extern PFNGLFRAMEBUFFERTEXTUREPROC			glFramebufferTexture;
//extern PFNGLFRAMEBUFFERTEXTURE2DPROC		glFramebufferTexture2D;
//extern PFNGLFRAMEBUFFERRENDERBUFFERPROC	glFramebufferRenderbuffer;
//
//extern PFNGLGENERATEMIPMAPPROC				glGenerateMipmap;

#endif /* OPENGL_H */
