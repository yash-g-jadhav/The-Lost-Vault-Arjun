/*
    OpenGL 3.3 Core Profile Loader (GLAD 1.x style generated header)
*/
#ifndef __glad_h_
#define __glad_h_

#ifdef __gl_h_
#error OpenGL header already included, remove this include, glad already provides it
#endif
#define __gl_h_

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include <KHR/khrplatform.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (*GLADloadproc)(const char *name);

#ifndef GLAD_PLATFORM_API_ENTRY
#define GLAD_PLATFORM_API_ENTRY KHRONOS_APIENTRY
#endif

#ifndef GLAD_API_CALL
#define GLAD_API_CALL
#endif

GLAD_API_CALL int gladLoadGLLoader(GLADloadproc load);
GLAD_API_CALL int gladLoadGL(void);

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
typedef khronos_int8_t GLbyte;
typedef khronos_uint8_t GLubyte;
typedef khronos_int16_t GLshort;
typedef khronos_uint16_t GLushort;
typedef int GLint;
typedef unsigned int GLuint;
typedef khronos_int32_t GLclampx;
typedef int GLsizei;
typedef khronos_float_t GLfloat;
typedef khronos_float_t GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef char GLchar;
typedef char GLcharARB;
#ifdef __APPLE__
typedef void *GLhandleARB;
#else
typedef unsigned int GLhandleARB;
#endif
typedef khronos_intptr_t GLintptr;
typedef khronos_ssize_t GLsizeiptr;
typedef khronos_int64_t GLint64;
typedef khronos_uint64_t GLuint64;
typedef struct __GLsync *GLsync;

#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_POINTS 0x0000
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_LINE_STRIP 0x0003
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_NEVER 0x0200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_FRONT_AND_BACK 0x0408
#define GL_CULL_FACE 0x0B44
#define GL_DEPTH_TEST 0x0B71
#define GL_BLEND 0x0BE2
#define GL_TEXTURE_2D 0x0DE1
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_BOOL 0x8B56
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C
#define GL_SAMPLER_2D 0x8B5E
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STREAM_DRAW 0x88E0
#define GL_STATIC_DRAW 0x88E4
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_RGBA 0x1908
#define GL_RGB 0x1907
#define GL_RED 0x1903
#define GL_ALPHA 0x1906
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03

typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLCLEARPROC)(GLbitfield mask);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLCLEARCOLORPROC)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLVIEWPORTPROC)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLENABLEPROC)(GLenum cap);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLDISABLEPROC)(GLenum cap);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLBLENDFUNCPROC)(GLenum sfactor, GLenum dfactor);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint *arrays);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint *arrays);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLGENBUFFERSPROC)(GLsizei n, GLuint *buffers);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLBUFFERSUBDATAPROC)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint *buffers);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLDISABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef GLuint (GLAD_PLATFORM_API_ENTRY *PFNGLCREATESHADERPROC)(GLenum type);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint *params);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef GLuint (GLAD_PLATFORM_API_ENTRY *PFNGLCREATEPROGRAMPROC)(void);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint *params);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLUSEPROGRAMPROC)(GLuint program);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLDELETESHADERPROC)(GLuint shader);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef GLint (GLAD_PLATFORM_API_ENTRY *PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar *name);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLUNIFORM4FVPROC)(GLint location, GLsizei count, const GLfloat *value);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLUNIFORMMATRIX4FVPROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLGENTEXTURESPROC)(GLsizei n, GLuint *textures);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLBINDTEXTUREPROC)(GLenum target, GLuint texture);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLTEXIMAGE2DPROC)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLTEXPARAMETERIPROC)(GLenum target, GLenum pname, GLint param);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLACTIVETEXTUREPROC)(GLenum texture);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLDELETETEXTURESPROC)(GLsizei n, const GLuint *textures);
typedef GLenum (GLAD_PLATFORM_API_ENTRY *PFNGLGETERRORPROC)(void);
typedef const GLubyte* (GLAD_PLATFORM_API_ENTRY *PFNGLGETSTRINGPROC)(GLenum name);

GLAD_API_CALL extern PFNGLCLEARPROC glad_glClear;
GLAD_API_CALL extern PFNGLCLEARCOLORPROC glad_glClearColor;
GLAD_API_CALL extern PFNGLVIEWPORTPROC glad_glViewport;
GLAD_API_CALL extern PFNGLENABLEPROC glad_glEnable;
GLAD_API_CALL extern PFNGLDISABLEPROC glad_glDisable;
GLAD_API_CALL extern PFNGLBLENDFUNCPROC glad_glBlendFunc;
GLAD_API_CALL extern PFNGLGENVERTEXARRAYSPROC glad_glGenVertexArrays;
GLAD_API_CALL extern PFNGLBINDVERTEXARRAYPROC glad_glBindVertexArray;
GLAD_API_CALL extern PFNGLDELETEVERTEXARRAYSPROC glad_glDeleteVertexArrays;
GLAD_API_CALL extern PFNGLGENBUFFERSPROC glad_glGenBuffers;
GLAD_API_CALL extern PFNGLBINDBUFFERPROC glad_glBindBuffer;
GLAD_API_CALL extern PFNGLBUFFERDATAPROC glad_glBufferData;
GLAD_API_CALL extern PFNGLBUFFERSUBDATAPROC glad_glBufferSubData;
GLAD_API_CALL extern PFNGLDELETEBUFFERSPROC glad_glDeleteBuffers;
GLAD_API_CALL extern PFNGLENABLEVERTEXATTRIBARRAYPROC glad_glEnableVertexAttribArray;
GLAD_API_CALL extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glad_glDisableVertexAttribArray;
GLAD_API_CALL extern PFNGLVERTEXATTRIBPOINTERPROC glad_glVertexAttribPointer;
GLAD_API_CALL extern PFNGLCREATESHADERPROC glad_glCreateShader;
GLAD_API_CALL extern PFNGLSHADERSOURCEPROC glad_glShaderSource;
GLAD_API_CALL extern PFNGLCOMPILESHADERPROC glad_glCompileShader;
GLAD_API_CALL extern PFNGLGETSHADERIVPROC glad_glGetShaderiv;
GLAD_API_CALL extern PFNGLGETSHADERINFOLOGPROC glad_glGetShaderInfoLog;
GLAD_API_CALL extern PFNGLCREATEPROGRAMPROC glad_glCreateProgram;
GLAD_API_CALL extern PFNGLATTACHSHADERPROC glad_glAttachShader;
GLAD_API_CALL extern PFNGLLINKPROGRAMPROC glad_glLinkProgram;
GLAD_API_CALL extern PFNGLGETPROGRAMIVPROC glad_glGetProgramiv;
GLAD_API_CALL extern PFNGLGETPROGRAMINFOLOGPROC glad_glGetProgramInfoLog;
GLAD_API_CALL extern PFNGLUSEPROGRAMPROC glad_glUseProgram;
GLAD_API_CALL extern PFNGLDELETESHADERPROC glad_glDeleteShader;
GLAD_API_CALL extern PFNGLDELETEPROGRAMPROC glad_glDeleteProgram;
GLAD_API_CALL extern PFNGLGETUNIFORMLOCATIONPROC glad_glGetUniformLocation;
GLAD_API_CALL extern PFNGLUNIFORM1IPROC glad_glUniform1i;
GLAD_API_CALL extern PFNGLUNIFORM1FPROC glad_glUniform1f;
GLAD_API_CALL extern PFNGLUNIFORM4FVPROC glad_glUniform4fv;
GLAD_API_CALL extern PFNGLUNIFORMMATRIX4FVPROC glad_glUniformMatrix4fv;
GLAD_API_CALL extern PFNGLGENTEXTURESPROC glad_glGenTextures;
GLAD_API_CALL extern PFNGLBINDTEXTUREPROC glad_glBindTexture;
GLAD_API_CALL extern PFNGLTEXIMAGE2DPROC glad_glTexImage2D;
GLAD_API_CALL extern PFNGLTEXPARAMETERIPROC glad_glTexParameteri;
GLAD_API_CALL extern PFNGLACTIVETEXTUREPROC glad_glActiveTexture;
GLAD_API_CALL extern PFNGLDELETETEXTURESPROC glad_glDeleteTextures;
GLAD_API_CALL extern PFNGLGETERRORPROC glad_glGetError;
GLAD_API_CALL extern PFNGLGETSTRINGPROC glad_glGetString;

typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLDRAWELEMENTSPROC)(GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void (GLAD_PLATFORM_API_ENTRY *PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);

GLAD_API_CALL extern PFNGLDRAWELEMENTSPROC glad_glDrawElements;
GLAD_API_CALL extern PFNGLDRAWARRAYSPROC glad_glDrawArrays;

#define glClear glad_glClear
#define glClearColor glad_glClearColor
#define glViewport glad_glViewport
#define glEnable glad_glEnable
#define glDisable glad_glDisable
#define glBlendFunc glad_glBlendFunc
#define glGenVertexArrays glad_glGenVertexArrays
#define glBindVertexArray glad_glBindVertexArray
#define glDeleteVertexArrays glad_glDeleteVertexArrays
#define glGenBuffers glad_glGenBuffers
#define glBindBuffer glad_glBindBuffer
#define glBufferData glad_glBufferData
#define glBufferSubData glad_glBufferSubData
#define glDeleteBuffers glad_glDeleteBuffers
#define glEnableVertexAttribArray glad_glEnableVertexAttribArray
#define glDisableVertexAttribArray glad_glDisableVertexAttribArray
#define glVertexAttribPointer glad_glVertexAttribPointer
#define glCreateShader glad_glCreateShader
#define glShaderSource glad_glShaderSource
#define glCompileShader glad_glCompileShader
#define glGetShaderiv glad_glGetShaderiv
#define glGetShaderInfoLog glad_glGetShaderInfoLog
#define glCreateProgram glad_glCreateProgram
#define glAttachShader glad_glAttachShader
#define glLinkProgram glad_glLinkProgram
#define glGetProgramiv glad_glGetProgramiv
#define glGetProgramInfoLog glad_glGetProgramInfoLog
#define glUseProgram glad_glUseProgram
#define glDeleteShader glad_glDeleteShader
#define glDeleteProgram glad_glDeleteProgram
#define glGetUniformLocation glad_glGetUniformLocation
#define glUniform1i glad_glUniform1i
#define glUniform1f glad_glUniform1f
#define glUniform4fv glad_glUniform4fv
#define glUniformMatrix4fv glad_glUniformMatrix4fv
#define glGenTextures glad_glGenTextures
#define glBindTexture glad_glBindTexture
#define glTexImage2D glad_glTexImage2D
#define glTexParameteri glad_glTexParameteri
#define glActiveTexture glad_glActiveTexture
#define glDeleteTextures glad_glDeleteTextures
#define glGetError glad_glGetError
#define glGetString glad_glGetString
#define glDrawElements glad_glDrawElements
#define glDrawArrays glad_glDrawArrays

#define GLAD_GL_VERSION_3_3 1

#ifdef __cplusplus
}
#endif

#endif
