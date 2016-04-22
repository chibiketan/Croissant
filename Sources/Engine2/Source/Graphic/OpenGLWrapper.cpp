/*
 * OpenGLWrapper.cpp
 *
 *  Created on: 6 févr. 2015
 *      Author: Nadege
 */

#include "Debug/MemoryManager.hpp"
#include "Graphic/OpenGLWrapper.hpp"
#include "Exception/CroissantException.hpp"
#include "Core/LogManager.hpp"
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

#include <windows.h>

#define DUMMY_WINDOW_CLASS_NAME             "oglversionchecksample"
#define LOGGER_NAME "OpenGL"

#define GLFUNCINDEX_GEN_BUFFERS					 0
#define GLFUNCINDEX_CLEAR_INDEX					 1
#define GLFUNCINDEX_BIND_BUFFER					 2
#define GLFUNCINDEX_BUFFER_DATA					 3
#define GLFUNCINDEX_CREATE_PROGRAM				 4
#define GLFUNCINDEX_CREATE_SHADER				 5
#define GLFUNCINDEX_CLEAR_DEPTH					 6
#define GLFUNCINDEX_ENABLE						 7
#define GLFUNCINDEX_MATRIX_MODEL				 8
#define GLFUNCINDEX_SWAP_INTERVAL				 9
#define GLFUNCINDEX_DEBUG_MESSAGE_CALLBACK		10
#define GLFUNCINDEX_ENABLE_CLIENT_STATE			11
#define GLFUNCINDEX_VERTEX_POINTER				12
#define GLFUNCINDEX_COLOR_POINTER				13
#define GLFUNCINDEX_POLYGON_MODE				14
#define GLFUNCINDEX_DRAW_ELEMENTS				15
#define GLFUNCINDEX_DELETE_BUFFERS				16
#define GLFUNCINDEX_DELETE_PROGRAM				17
#define GLFUNCINDEX_DELETE_SHADER				18
#define GLFUNCINDEX_MATRIX_MODE					19
#define GLFUNCINDEX_CLEAR						20
#define GLFUNCINDEX_PUSH_MATRIX					21
#define GLFUNCINDEX_POP_MATRIX					22
#define GLFUNCINDEX_DISABLE_CLIENT_STATE		23
#define GLFUNCINDEX_MAP_BUFFER					24
#define GLFUNCINDEX_UNMAP_BUFFER				25
#define GLFUNCINDEX_GET_INTEGERV				26
#define GLFUNCINDEX_SHADER_SOURCE				27
#define GLFUNCINDEX_ATTACH_SHADER				28
#define GLFUNCINDEX_COMPILE_SHADER				29
#define	GLFUNCINDEX_GET_SHADER_IV				30
#define GLFUNCINDEX_BIND_ATTRIB_LOCATION		31
#define GLFUNCINDEX_LINK_PROGRAM				32
#define GLFUNCINDEX_GET_PROGRAM_IV				33
#define GLFUNCINDEX_USE_PROGRAM					34
#define GLFUNCINDEX_ENABLE_VERTEX_ATTRIB_ARRAY	35
#define GLFUNCINDEX_DISABLE_VERTEX_ATTRIB_ARRAY	36
#define	GLFUNCINDEX_VERTEX_ATTRIB_POINTER		37
#define GLFUNCINDEX_GET_UNIFORM_LOCATION		38
#define GLFUNCINDEX_UNIFORM_MATRIX_4FV			39

using OpenGLRendererException = Croissant::Exception::CroissantException;

namespace Croissant
{
	namespace Graphic
	{
		namespace
		{

			template<unsigned int funcIndex>
			void glThrowOnError(GLint)
			{
				static_assert(funcIndex < 0, "La fonction \"glThrowOnError\" doit être spécialisée");
			};

			template<>
			void glThrowOnError<GLFUNCINDEX_GEN_BUFFERS>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de la génération du buffer : Valeur du nombre de buffer à générer invalide.");
				default:
					throw OpenGLRendererException("Erreur lors de la génération du buffer : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_BIND_BUFFER>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glBindBuffer : Valeur pour le paramètre target incorrecte.");
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glBindBuffer : Le buffer fourni n'a pas été créée avec un appel à glGenBuffer.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glBindBuffer : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_BUFFER_DATA>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glBufferData : Buffer invalide ou valeur du paramètre usage incorrecte.");
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glBufferData : Le paramètre size est négatif.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glBufferData : Le buffer associé est 0 ou alors le buffer n'est pas modifiable.");
				case GL_OUT_OF_MEMORY:
					throw OpenGLRendererException("Erreur lors de l'appel à glBufferData : Impossible de créer un buffer de la taille demandée.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glBufferData : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_CLEAR_INDEX>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glClearIndex : appel entre 2 appel de glBegin et glEnd.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glClearIndex : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_MATRIX_MODEL>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glMatrixModel : appel entre 2 appel de glBegin et glEnd.");
				case GLU_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glMatrixModel : Le mode fourni n'est pas una valeur valide.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glMatrixModel : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_SWAP_INTERVAL>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à wglSwapIntervalExt : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_DRAW_ELEMENTS>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glDrawElements : Valeur de mode incorrecte.");
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glDrawElements : La valeur pour count est invalide.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glDrawElements : La commande ne peut pas être effectuée dans l'état actuel du contexte.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glDrawElements : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_CREATE_PROGRAM>(GLint)
			{
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_CREATE_SHADER>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glCreateShader : Valeur de shaderType incorrecte.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glCreateShader : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_CLEAR_DEPTH>(GLint)
			{
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_ENABLE>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glEnable : Valeur de cap incorrecte.");
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glEnable : La valeur pour index est plus grande ou égale à la limite maximum pour la capacité de cap.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glEnable : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_ENABLE_CLIENT_STATE>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glEnableClientState : Valeur de cap incorrecte.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glEnableClientState : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_DISABLE_CLIENT_STATE>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glDisableClientState : Valeur de cap incorrecte.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glDisableClientState : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_POLYGON_MODE>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glPolygonMode : Valeur de face ou mode incorrecte.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glPolygonMode : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_COLOR_POINTER>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glColorPointer  : Valeur de type incorrecte.");
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glColorPointer  : Valeur de size différente de 3 ou 4 OU la valeur de stride est négative.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glColorPointer  : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_VERTEX_POINTER>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glVertexPointer : Valeur de type incorrecte.");
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glVertexPointer : Valeur de size différente de 2, 3 ou 4 OU la valeur de stride est négative.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glVertexPointer : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_DEBUG_MESSAGE_CALLBACK>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glDebugMessageCallback : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_DELETE_BUFFERS>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glDeleteBuffers : Valeur de n négative.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glDeleteBuffers : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_DELETE_PROGRAM>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glDeleteProgram : Valeur de program non générée par OpenGL.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glDeleteProgram : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_DELETE_SHADER>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glDeleteShader : Valeur de shader non générée par OpenGL.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glDeleteShader : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_CLEAR>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glClear : Valeur de mask contient des bits inconnus.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glClear : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_MATRIX_MODE>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glMatrixMode : Valeur de mode inconnue.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glMatrixMode : Appel de la méthode entre glBegin et glEnd.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glMatrixMode : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_PUSH_MATRIX>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				case GL_STACK_OVERFLOW:
					throw OpenGLRendererException("Erreur lors de l'appel à glPushMatrix : La pile de matrices est pleine.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glPushMatrix : Appel de la méthode entre glBegin et glEnd.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glPushMatrix : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_POP_MATRIX>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				case GL_STACK_UNDERFLOW:
					throw OpenGLRendererException("Erreur lors de l'appel à glPopMatrix : La pile de matrices est vide.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glPopMatrix : Appel de la méthode entre glBegin et glEnd.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glPopMatrix : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_MAP_BUFFER>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glMapBuffer : La valeur de target ou de access n'est pas valide.");
				case GL_OUT_OF_MEMORY:
					throw OpenGLRendererException("Erreur lors de l'appel à glMapBuffer : Impossible d'allouer la tailler mémoire nécessaire.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glMapBuffer : Le buffer a une taille de 0 ou est déjà mappé.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glMapBuffer : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_UNMAP_BUFFER>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glUnmapBuffer : La valeur de target ou de access n'est pas valide.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glUnmapBuffer : Le buffer n'est pas mappé.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glUnmapBuffer : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_GET_INTEGERV>(GLint err)
			{
				switch(err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetIntegerv : La valeur de pname n'est pas valide.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetIntegerv : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_SHADER_SOURCE>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glShaderSource : identifiant de shader n'existe pas, ou count est inférieur à 0.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glShaderSource : l'identifiant n'est pas un shader.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glShaderSource : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_ATTACH_SHADER>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glAttachShader : l'identifiant de programme ou de shader n'existe pas.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glAttachShader : program n'est pas un program ou shader n'est pas un shader ou shader est déjà attaché à program.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glAttachShader : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_COMPILE_SHADER>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glCompileShader : l'identifiant de shader n'existe pas.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glCompileShader : L'identifiant n'est pas celui d'un shader.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glCompileShader : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_GET_SHADER_IV>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetShaderiv : l'identifiant de shader n'existe pas.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetShaderiv : L'identifiant n'est pas celui d'un shader.");
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetShaderiv : La valeur du paramète pname n'existe pas.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetShaderiv : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_BIND_ATTRIB_LOCATION>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glBindAttribLocation : l'identifiant de programme n'existe pas ou l'index est plus grand que la valeur de GL_MAX_VERTEX_ATTRIBS.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glBindAttribLocation : Le nom commence part 'gl_' ce qui est interdit ou l'identifiant de programme n'est pas celui d'un programme.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glBindAttribLocation : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_LINK_PROGRAM>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glLinkProgram : l'identifiant de programme n'existe pas.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glLinkProgram : l'identifiant de programme n'est pas celui d'un programme ou le programme est actuellement le programme actif et le mode de retour sur transformation est activé.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glLinkProgram : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_GET_PROGRAM_IV>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetProgramiv : l'identifiant de programme n'existe pas.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetProgramiv : l'identifiant de programme n'est pas celui d'un programme ou le programme ne contient pas de geomtry shader pour les names GL_GEOMETRY_VERTICES_OUT, GL_GEOMETRY_INPUT_TYPE, or GL_GEOMETRY_OUTPUT_TYPE.");
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetProgramiv : La valeur du paramète name n'existe pas.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetProgramiv : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_USE_PROGRAM>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glUseProgram : l'identifiant de programme n'existe pas et ne vaut pas 0.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glUseProgram : l'identifiant de programme n'est pas celui d'un programme OU le programme ne peut pas être mis dans l'état courant OU le retour de transformation est activé.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glUseProgram : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_ENABLE_VERTEX_ATTRIB_ARRAY>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glEnableVertexAttribArray : l'index est plus grand ou égal à la valeur de GL_MAX_VERTEX_ATTRIBS.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glEnableVertexAttribArray : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_DISABLE_VERTEX_ATTRIB_ARRAY>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glDisableVertexAttribArray : l'index est plus grand ou égal à la valeur de GL_MAX_VERTEX_ATTRIBS.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glDisableVertexAttribArray : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_VERTEX_ATTRIB_POINTER>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glVertexAttribPointer : l'index est plus grand ou égal à la valeur de GL_MAX_VERTEX_ATTRIBS OU la valeur de size n'est pas compris entre 1 et 4 OU la valeur de stride est négative.");
				case GL_INVALID_ENUM:
					throw OpenGLRendererException("Erreur lors de l'appel à glVertexAttribPointer : la valeur de type n'est pas reconnue.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glVertexAttribPointer : Pas de vertex array lié au context.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glVertexAttribPointer : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_GET_UNIFORM_LOCATION>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetUniformLocation : La valeur de programId n'a pas été généré par OpenGL.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetUniformLocation : La valeur de programId n'est pas un programme OU le prgramme n'a pas été link.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glGetUniformLocation : Erreur non identifiée.");
				}
			}

			template<>
			void glThrowOnError<GLFUNCINDEX_UNIFORM_MATRIX_4FV>(GLint err)
			{
				switch (err)
				{
				case GL_NO_ERROR:
					break;
				case GL_INVALID_VALUE:
					throw OpenGLRendererException("Erreur lors de l'appel à glUniformMatrix4fv : La valeur de count est inférieure à 0.");
				case GL_INVALID_OPERATION:
					throw OpenGLRendererException("Erreur lors de l'appel à glUniformMatrix4fv : Il n'y a pas de programme actif OU location n'est pas l'emplacement d'un uniform valide OU la variable n'est mas une matrice 4x4 de float. OU la valeur de count > 1 et la variable uniforme n'est pas un tableau.");
				default:
					throw OpenGLRendererException("Erreur lors de l'appel à glUniformMatrix4fv : Erreur non identifiée.");
				}
			}

			using LogManager = Core::LogManager;
			using string = std::string;

			template <typename Func>
			Func LoadGLSymbol(string const& ext, bool throwIfNotFound = true)
			{
				auto proc = wglGetProcAddress(ext.c_str());

				if (nullptr == proc)
				{
					HMODULE module = LoadLibrary("opengl32.dll");
					proc = GetProcAddress(module, ext.c_str());
					FreeLibrary(module);
				}

				if (nullptr == proc && throwIfNotFound)
				{
					throw OpenGLRendererException(string("Impossible de trouver l'extension : ") + ext);
				}

				return reinterpret_cast<Func>(proc);
			}

			LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}

			void createDummyContext(HDC& ourWindowHandleToDeviceContext, HGLRC& ourOpenGLRenderingContext, HWND& windowHandle)
			{
				PIXELFORMATDESCRIPTOR pfd =
				{
					sizeof(PIXELFORMATDESCRIPTOR),
					1,
					PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
					PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
					32,                        //Colordepth of the framebuffer.
					0, 0, 0, 0, 0, 0,
					0,
					0,
					0,
					0, 0, 0, 0,
					24,                        //Number of bits for the depthbuffer
					8,                        //Number of bits for the stencilbuffer
					0,                        //Number of Aux buffers in the framebuffer.
					PFD_MAIN_PLANE,
					0,
					0, 0, 0
				};

				WNDCLASS wc      = {
					/* style */ 0,
					/* lpfnWndProc */ nullptr,
					/* cbClsExtra */ 0,
					/* cbWndExtra */ 0,
					/* hInstance */ nullptr,
					/* hIcon */ nullptr,
					/* hCursor */ nullptr,
					/* hbrBackground */ nullptr,
					/* lpszMenuName */ nullptr,
					/* lpszClassName */ nullptr
				};

				wc.lpfnWndProc   = WndProc;
				wc.hInstance     = nullptr;
				wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BACKGROUND);
				wc.lpszClassName = DUMMY_WINDOW_CLASS_NAME;
				wc.style = CS_OWNDC;
				if( !RegisterClass(&wc) )
					return;
				windowHandle = CreateWindow(wc.lpszClassName,"openglversioncheck",WS_OVERLAPPEDWINDOW,0,0,640,480,nullptr,nullptr,nullptr,nullptr);

				ourWindowHandleToDeviceContext = GetDC(windowHandle);

				int  letWindowsChooseThisPixelFormat;
				letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
				SetPixelFormat(ourWindowHandleToDeviceContext,letWindowsChooseThisPixelFormat, &pfd);

				ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
				wglMakeCurrent (ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);
			}

			void deleteDummyContext(HDC& ourWindowHandleToDeviceContext, HGLRC& ourOpenGLRenderingContext, HWND& windowHandle)
			{
				MSG msg = {
					/* HWND*/ nullptr,
					/* UINT */ 0,
					/* WPARAM */ 0L,
					/* LPARAM */ 0L,
					/* DWORD */ 0,
					/* POINT */ {
						/* x */ 0,
						/* y */ 0
					}
				};

				//PostQuitMessage(0);
				DestroyWindow(windowHandle);
				while( PeekMessage( &msg, windowHandle, 0, 0, PM_REMOVE) != 0 )
				{
					DispatchMessage( &msg );
				}

				wglMakeCurrent (nullptr, nullptr);
				wglDeleteContext(ourOpenGLRenderingContext);
				DeleteDC(ourWindowHandleToDeviceContext);
				UnregisterClass(DUMMY_WINDOW_CLASS_NAME, nullptr);
			}

		} // !namespace anonyme

		template<unsigned int funcIndex>
		inline bool glCheckForError(const OpenGLWrapper& wrapper)
		{
			auto err = glGetError();

			wrapper.m_lastGLError = err;

#if defined(CROISSANT_OPENGL_ERROR_THROW)
			glThrowOnError<funcIndex>(err);
#endif
			return GL_NO_ERROR != err;
		}

		OpenGLWrapper::OpenGLWrapper()
			: m_logManager{ CROISSANT_GET_LOG(OpenGLWrapper) }
		{
			HDC ourWindowHandleToDeviceContext = { nullptr };
			HGLRC ourOpenGLRenderingContext = { nullptr };
			HWND windowHandle = { nullptr };

			createDummyContext(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext, windowHandle);

			m_logManager->Write("Entrée dans OpenGLWrapper constructeur");
			ext_glDebugMessageCallbackARB = LoadGLSymbol<glDebugMessageCallbackARB_t>("glDebugMessageCallbackARB", false);
			ext_glBindBuffer = LoadGLSymbol<glBindBuffer_t>("glBindBuffer");
			ext_glGenBuffers = LoadGLSymbol<glGenBuffers_t>("glGenBuffers");
			ext_glBufferData = LoadGLSymbol<glBufferData_t>("glBufferData");
			ext_glDeleteBuffers = LoadGLSymbol<glDeleteBuffers_t>("glDeleteBuffers");
			ext_wglSwapIntervalEXT = LoadGLSymbol<wglSwapIntervalEXT_t>("wglSwapIntervalEXT");
			ext_glCreateShader = LoadGLSymbol<glCreateShader_t>("glCreateShader");
			ext_glCreateProgram = LoadGLSymbol<glCreateProgram_t>("glCreateProgram");
			ext_glLinkProgram = LoadGLSymbol<glLinkProgram_t>("glLinkProgram");
			ext_glAttachShader = LoadGLSymbol<glAttachShader_t>("glAttachShader");
			ext_glShaderSource = LoadGLSymbol<glShaderSource_t>("glShaderSource");
			ext_glDeleteProgram = LoadGLSymbol<glDeleteProgram_t>("glDeleteProgram");;
			ext_glDeleteShader = LoadGLSymbol<glDeleteShader_t>("glDeleteShader");;
			ext_glDetachShader = LoadGLSymbol<glDetachShader_t>("glDetachShader");;
			ext_glGetStringi = LoadGLSymbol<glGetStringi_t>("glGetStringi");
			ext_glClearIndex = LoadGLSymbol<glClearIndex_t>("glClearIndex");
			ext_glClearDepth = LoadGLSymbol<glClearDepth_t>("glClearDepth");
			ext_glMatrixMode = LoadGLSymbol<glMatrixMode_t>("glMatrixMode");
			ext_glClear = LoadGLSymbol<glClear_t>("glClear");
			ext_glPushMatrix = LoadGLSymbol<glPushMatrix_t>("glPushMatrix");
			ext_glPopMatrix = LoadGLSymbol<glPopMatrix_t>("glPopMatrix");
			ext_glEnableClientState = LoadGLSymbol<glEnableClientState_t>("glEnableClientState");
			ext_glDisableClientState = LoadGLSymbol<glDisableClientState_t>("glDisableClientState");
			ext_glVertexPointer = LoadGLSymbol<glVertexPointer_t>("glVertexPointer");
			ext_glColorPointer = LoadGLSymbol<glColorPointer_t>("glColorPointer");
			ext_glPolygonMode = LoadGLSymbol<glPolygonMode_t>("glPolygonMode");
			ext_glDrawElements = LoadGLSymbol<glDrawElements_t>("glDrawElements");
			ext_glMapBuffer = LoadGLSymbol<glMapBuffer_t>("glMapBuffer");
			ext_glUnmapBuffer = LoadGLSymbol<glUnmapBuffer_t>("glUnmapBuffer");
			ext_glCompileShader = LoadGLSymbol<glCompileShader_t>("glCompileShader");
			ext_glGetShaderiv = LoadGLSymbol<glGetShaderiv_t>("glGetShaderiv");
			ext_glBindAttribLocation = LoadGLSymbol<glBindAttribLocation_t>("glBindAttribLocation");
			ext_glGetProgramiv = LoadGLSymbol<glGetProgramiv_t>("glGetProgramiv");
			ext_glUseProgram = LoadGLSymbol<glUseProgram_t>("glUseProgram");
			ext_glEnableVertexAttribArray = LoadGLSymbol<glEnableVertexAttribArray_t>("glEnableVertexAttribArray");
			ext_glDisableVertexAttribArray = LoadGLSymbol<glDisableVertexAttribArray_t>("glDisableVertexAttribArray");
			ext_glVertexAttribPointer = LoadGLSymbol<glVertexAttribPointer_t>("glVertexAttribPointer");
			ext_glGetProgramInfoLog = LoadGLSymbol<glGetProgramInfoLog_t>("glGetProgramInfoLog");
			ext_glGetUniformLocation = LoadGLSymbol<glGetUniformLocation_t>("glGetUniformLocation");
			ext_glUniformMatrix4fv = LoadGLSymbol<glUniformMatrix4fv_t>("glUniformMatrix4fv");

			int NumberOfExtensions;
			glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
			m_logManager->Write("openGL", "Liste des extensions openGL supportées :");

			for(int i=0; i<NumberOfExtensions; i++)
			{
				const GLubyte *ccc = ext_glGetStringi(GL_EXTENSIONS, i);

				m_logManager->Write("openGL", string("extension OpenGL : ") + reinterpret_cast<const char*>(ccc));
			}



			//deleteDummyContext(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext, windowHandle);

			m_logManager->Write("Sortie de OpenGLWrapper constructeur");
		}

		void OpenGLWrapper::Enable(GLenum capacity) const
		{
			glEnable(capacity);
			glCheckForError<GLFUNCINDEX_ENABLE>(*this);
		}

		void OpenGLWrapper::RegisterDebugMessageCallback(DebugMessageCallback_t callback) const
		{
			if (nullptr == ext_glDebugMessageCallbackARB)
			{
				return;
			}

			Enable(GL_DEBUG_OUTPUT);
			if (GL_NO_ERROR == m_lastGLError)
			{
				// si l'extension est disponible, on enregistre le callback en cas d'erreur openGL au plus tôt
				ext_glDebugMessageCallbackARB(callback, &m_logManager);
				glCheckForError<GLFUNCINDEX_DEBUG_MESSAGE_CALLBACK>(*this);
//				auto error = glGetError();
//
//				if (GL_NO_ERROR != error)
//				{
//					m_logManager.Write("Impossible de définir le context en mode DEBUG");
//				}
//
//				m_lastGLError = error;
			}
			else
			{
				m_logManager->Write("Impossible de définir le context en mode DEBUG");
			}
		}

		void OpenGLWrapper::BindBuffer(GLenum type, uint32_t shaderId) const
		{
			ext_glBindBuffer(type, shaderId);
			glCheckForError<GLFUNCINDEX_BIND_BUFFER>(*this);
		}

		void OpenGLWrapper::GenBuffers(size_t size, uint32_t* tab) const
		{
			ext_glGenBuffers(static_cast<GLsizei>(size), tab);
			glCheckForError<GLFUNCINDEX_GEN_BUFFERS>(*this);
		}

		void OpenGLWrapper::BufferData(GLenum target, size_t size, void const* buffer, GLenum usageType) const
		{
			ext_glBufferData(target, static_cast<GLsizei>(size), buffer, usageType);
			glCheckForError<GLFUNCINDEX_BUFFER_DATA>(*this);
		}

		uint32_t OpenGLWrapper::CreateProgram(void) const
		{
			auto programId = ext_glCreateProgram();

			glCheckForError<GLFUNCINDEX_CREATE_PROGRAM>(*this);
			return programId;
		}

		void OpenGLWrapper::LinkProgram(uint32_t programId) const
		{
			ext_glLinkProgram(programId);
			glCheckForError<GLFUNCINDEX_LINK_PROGRAM>(*this);
		}

		void OpenGLWrapper::AttachShader(uint32_t programId, uint32_t shaderId) const
		{
			ext_glAttachShader(programId, shaderId);
			glCheckForError<GLFUNCINDEX_ATTACH_SHADER>(*this);
		}

		void OpenGLWrapper::ShaderSource(uint32_t shaderId, std::string const & source) const
		{
			auto cStr = source.c_str();
			auto length = static_cast<int>(source.length());

			ext_glShaderSource(shaderId, 1, &cStr, &length);
			glCheckForError<GLFUNCINDEX_SHADER_SOURCE>(*this);
		}

		uint32_t OpenGLWrapper::CreateShader(GLenum shaderType) const
		{
			auto shaderId = ext_glCreateShader(shaderType);

			glCheckForError<GLFUNCINDEX_CREATE_SHADER>(*this);
			return shaderId;
		}

		void OpenGLWrapper::DeleteBuffers(size_t size, uint32_t const* tab) const
		{
			ext_glDeleteBuffers(static_cast<GLsizei>(size), tab);
			glCheckForError<GLFUNCINDEX_DELETE_BUFFERS>(*this);
		}

		void OpenGLWrapper::DeleteProgram(uint32_t programId) const
		{
			ext_glDeleteProgram(programId);
			glCheckForError<GLFUNCINDEX_DELETE_PROGRAM>(*this);
		}

		void OpenGLWrapper::DeleteShader(uint32_t shaderId) const
		{
			ext_glDeleteShader(shaderId);
			glCheckForError<GLFUNCINDEX_DELETE_SHADER>(*this);
		}

		bool OpenGLWrapper::EnableVSync(bool enable) const
		{
			auto val = 0;

			if (enable)
			{
				val = 1;
			}

			auto ret = ext_wglSwapIntervalEXT(val);

			glCheckForError<GLFUNCINDEX_SWAP_INTERVAL>(*this);
			return ret == TRUE;
		}

		void OpenGLWrapper::ClearIndex(GLfloat colorIndex) const
		{
			ext_glClearIndex(colorIndex);
			glCheckForError<GLFUNCINDEX_CLEAR_INDEX>(*this);
		}

		void OpenGLWrapper::ClearDepth(GLclampd depth) const
		{
			ext_glClearDepth(depth);
			glCheckForError<GLFUNCINDEX_CLEAR_DEPTH>(*this);
		}

		void OpenGLWrapper::MatrixMode(GLenum mode) const
		{
			ext_glMatrixMode(mode);
			glCheckForError<GLFUNCINDEX_MATRIX_MODE>(*this);
		}

		void OpenGLWrapper::Clear(GLbitfield mask) const
		{
			ext_glClear(mask);
			glCheckForError<GLFUNCINDEX_CLEAR>(*this);
		}

		void OpenGLWrapper::PushMatrix(void) const
		{
			ext_glPushMatrix();
			glCheckForError<GLFUNCINDEX_PUSH_MATRIX>(*this);
		}

		void OpenGLWrapper::PopMatrix(void) const
		{
			ext_glPopMatrix();
			glCheckForError<GLFUNCINDEX_POP_MATRIX>(*this);
		}

		void OpenGLWrapper::EnableClientState(GLenum cap) const
		{
			ext_glEnableClientState(cap);
			glCheckForError<GLFUNCINDEX_ENABLE_CLIENT_STATE>(*this);
		}

		void OpenGLWrapper::DisableClientState(GLenum cap) const
		{
			ext_glDisableClientState(cap);
			glCheckForError<GLFUNCINDEX_DISABLE_CLIENT_STATE>(*this);
		}

		void OpenGLWrapper::VertexPointer(GLint size, GLenum type, GLsizei stride, GLvoid const* pointer) const
		{
			ext_glVertexPointer(size, type, stride, pointer);
			glCheckForError<GLFUNCINDEX_VERTEX_POINTER>(*this);
		}

		void OpenGLWrapper::ColorPointer(GLint size, GLenum type, GLsizei stride, GLvoid const* pointer) const
		{
			ext_glColorPointer(size, type, stride, pointer);
			glCheckForError<GLFUNCINDEX_COLOR_POINTER>(*this);
		}

		void OpenGLWrapper::PolygonMode(GLenum face, GLenum mode) const
		{
			ext_glPolygonMode(face, mode);
			glCheckForError<GLFUNCINDEX_POLYGON_MODE>(*this);
		}

		void OpenGLWrapper::DrawElements(GLenum mode, GLsizei count, GLenum type, GLvoid const* indices) const
		{
			ext_glDrawElements(mode, count, type, indices);
			glCheckForError<GLFUNCINDEX_DRAW_ELEMENTS>(*this);
		}

		void* OpenGLWrapper::MapBuffer(GLenum target, GLenum access) const
		{
			auto ret = ext_glMapBuffer(target, access);

			if (nullptr == ret)
			{
				glCheckForError<GLFUNCINDEX_MAP_BUFFER>(*this);
			}

			return ret;
		}

		GLboolean OpenGLWrapper::UnmapBuffer(GLenum target) const
		{
			auto ret = ext_glUnmapBuffer(target);

			if (GL_FALSE == ret)
			{
				glCheckForError<GLFUNCINDEX_UNMAP_BUFFER>(*this);
			}

			return ret;
		}

		int32_t		OpenGLWrapper::GetInteger(OpenGLValueNameEnum valueName) const
		{
			GLint val;
			auto enumValue = s_valueNames[static_cast<unsigned int>(valueName)];

			glGetIntegerv(enumValue, &val);
			glCheckForError<GLFUNCINDEX_GET_INTEGERV>(*this);
			return val;
		}

		void OpenGLWrapper::CompileShader(uint32_t shaderId) const
		{
			ext_glCompileShader(shaderId);
			glCheckForError<GLFUNCINDEX_COMPILE_SHADER>(*this);
		}

		int32_t OpenGLWrapper::GetShaderInteger(uint32_t shaderId, OpenGLShaderIntegerNameEnum name) const
		{
			int32_t val;

			ext_glGetShaderiv(shaderId, s_shaderIntegerNames[static_cast<size_t>(name)], &val);
			glCheckForError<GLFUNCINDEX_GET_SHADER_IV>(*this);
			return val;
		}

		void OpenGLWrapper::BindAttribLocation(uint32_t programId, uint32_t index, std::string const & name) const
		{
			ext_glBindAttribLocation(programId, index, name.c_str());
			glCheckForError<GLFUNCINDEX_BIND_ATTRIB_LOCATION>(*this);
		}

		int32_t OpenGLWrapper::GetProgramInteger(uint32_t programId, OpenGLProgramIntegerNameEnum name) const
		{
			int32_t result;

			ext_glGetProgramiv(programId, s_programIntegerNames[static_cast<size_t>(name)], &result);
			glCheckForError<GLFUNCINDEX_GET_PROGRAM_IV>(*this);
			return result;
		}

		void OpenGLWrapper::UseProgram(uint32_t programId) const
		{
			ext_glUseProgram(programId);
			glCheckForError<GLFUNCINDEX_USE_PROGRAM>(*this);
		}

		void OpenGLWrapper::EnableVertexAttribArray(uint32_t index) const
		{
			ext_glEnableVertexAttribArray(index);
			glCheckForError<GLFUNCINDEX_ENABLE_VERTEX_ATTRIB_ARRAY>(*this);
		}

		void OpenGLWrapper::DisableVertexAttribArray(uint32_t index) const
		{
			ext_glDisableVertexAttribArray(index);
			glCheckForError<GLFUNCINDEX_DISABLE_VERTEX_ATTRIB_ARRAY>(*this);
		}

		void OpenGLWrapper::VertexAttribPointer(uint32_t index, int32_t size, GLenum type, bool normalzed, size_t stride, void const * data) const
		{
			ext_glVertexAttribPointer(index, size, type, normalzed ? GL_TRUE : GL_FALSE, stride, data);
			glCheckForError<GLFUNCINDEX_VERTEX_ATTRIB_POINTER>(*this);
		}

		std::string OpenGLWrapper::GetProgramInfoLog(uint32_t programId) const
		{
			std::string result;
			auto logLength = this->GetProgramInteger(programId, OpenGLProgramIntegerNameEnum::InfoLogLength);

			if (0 == logLength)
			{
				return result;
			}

			GLsizei len;

			result.resize(logLength);
			ext_glGetProgramInfoLog(programId, logLength, &len, &result[0]);
			return result;
		}

		int32_t OpenGLWrapper::GetUniformLocation(uint32_t programId, std::string const name) const
		{
			int32_t result = ext_glGetUniformLocation(programId, name.c_str());

			glCheckForError<GLFUNCINDEX_GET_UNIFORM_LOCATION>(*this);
			return result;
		}

		void OpenGLWrapper::SetUniformMatrix4f(int32_t location, int32_t count, bool transpose, Math::Matrix4f const& matrix) const
		{
			ext_glUniformMatrix4fv(location, count, (transpose ? GL_TRUE : GL_FALSE), matrix.Data().data());
			glCheckForError<GLFUNCINDEX_UNIFORM_MATRIX_4FV>(*this);
		}

		GLenum OpenGLWrapper::s_valueNames[] {
				GL_ACTIVE_TEXTURE,					// ActiveTexture
				GL_ALIASED_LINE_WIDTH_RANGE,		// AliasedLineWidthRange
				GL_SMOOTH_LINE_WIDTH_RANGE,			// SmoothLineWidthRange
				GL_SMOOTH_LINE_WIDTH_GRANULARITY,	// SmoothLineWidthGranularity
				GL_ARRAY_BUFFER_BINDING,			// ArrayBufferBinding
				GL_BLEND,							// Blend
				GL_BLEND_COLOR,						// BlendColor
				GL_BLEND_DST_ALPHA,					// BlendDstAlpha
				GL_BLEND_DST_RGB,					// BlendDstRgb
				GL_BLEND_EQUATION_RGB,				// BlendEquationRgb
				GL_BLEND_EQUATION_ALPHA,			// BlendEquationAlpha
				GL_BLEND_SRC_ALPHA,					// BlendSrcAlpha
				GL_BLEND_SRC_RGB,					// BlendSrcRgb
				GL_COLOR_CLEAR_VALUE,				// ColorClearValue
				GL_COLOR_LOGIC_OP,					// ColorLogicOp
				GL_COLOR_WRITEMASK,					// ColorWritemask
				GL_COMPRESSED_TEXTURE_FORMATS,		// CompressedTextureFormats
				GL_CULL_FACE,						// CullFace
				GL_CURRENT_PROGRAM,					// CurrentProgram
				GL_DEPTH_CLEAR_VALUE,				// DepthClearValue
				GL_DEPTH_FUNC,						// DepthFunc
				GL_DEPTH_RANGE,						// DepthRange,
				GL_DEPTH_TEST,						// DepthTest,
				GL_DEPTH_WRITEMASK,					// DepthWritemask,
				GL_DITHER,							// Dither,
				GL_DOUBLEBUFFER,					// Doublebuffer,
				GL_DRAW_BUFFER,						// DrawBuffer,
				GL_DRAW_BUFFER0,					// DrawBuffer0,
				GL_DRAW_BUFFER1,					// DrawBuffer1,
				GL_DRAW_BUFFER2,					// DrawBuffer2,
				GL_DRAW_BUFFER3,					// DrawBuffer3,
				GL_DRAW_BUFFER4,					// DrawBuffer4,
				GL_DRAW_BUFFER5,					// DrawBuffer5,
				GL_DRAW_BUFFER6,					// DrawBuffer6,
				GL_DRAW_BUFFER7,					// DrawBuffer7,
				GL_DRAW_BUFFER8,					// DrawBuffer8,
				GL_DRAW_BUFFER9,					// DrawBuffer9,
				GL_DRAW_BUFFER10,					// DrawBuffer10,
				GL_DRAW_BUFFER11,					// DrawBuffer11,
				GL_DRAW_BUFFER12,					// DrawBuffer12,
				GL_DRAW_BUFFER13,					// DrawBuffer13,
				GL_DRAW_BUFFER14,					// DrawBuffer14,
				GL_DRAW_BUFFER15,					// DrawBuffer15,
				GL_DRAW_FRAMEBUFFER,				// DrawFramebufferBinding,
				GL_READ_FRAMEBUFFER_BINDING,		// ReadFramebufferBinding,
				GL_ELEMENT_ARRAY_BUFFER_BINDING,	// ElementArrayBufferBinding,
				GL_RENDERBUFFER_BINDING,			// RenderbufferBinding,
				GL_FRAGMENT_SHADER_DERIVATIVE_HINT,	// FragmentShaderDerivativeHint,
				GL_LINE_SMOOTH,						// LineSmooth,
				GL_LINE_SMOOTH_HINT,				// LineSmoothHint,
				GL_LINE_WIDTH,						// LineWidth,
				GL_LOGIC_OP_MODE,					// LogicOpMode,
				GL_MAX_3D_TEXTURE_SIZE,				// Max3DTextureSize,
				GL_MAX_CLIP_DISTANCES,				// MaxClipDistances,
				GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS,	// MaxCombinedFragmentUniformComponents,
				GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,			// MaxCombinedTextureImageUnits,
				GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS,		// MaxCombinedVertexUniformComponents,
				GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS,	// MaxCombinedGeometryUniformComponents,
				GL_MAX_VARYING_COMPONENTS,						// MaxVaryingComponents,
				GL_MAX_COMBINED_UNIFORM_BLOCKS,					// MaxCombinedUniformBlocks,
				GL_MAX_CUBE_MAP_TEXTURE_SIZE,					// MaxCubeMapTextureSize,
				GL_MAX_DRAW_BUFFERS,							// MaxDrawBuffers,
				GL_MAX_DUAL_SOURCE_DRAW_BUFFERS,				// MaxDualSourceDrawBuffers,
				GL_MAX_ELEMENTS_INDICES,						// MaxElementsIndices,
				GL_MAX_ELEMENTS_VERTICES,						// MaxElementsVertices,
				GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,				// MaxFragmentUniformComponents,
				GL_MAX_FRAGMENT_UNIFORM_BLOCKS,					// MaxFragmentUniformBlocks,
				GL_MAX_FRAGMENT_INPUT_COMPONENTS,				// MaxFragmentInputComponents,
				GL_MIN_PROGRAM_TEXEL_OFFSET,					// MinProgramTexelOffset,
				GL_MAX_PROGRAM_TEXEL_OFFSET,					// MaxProgramTexelOffset,
				GL_MAX_RECTANGLE_TEXTURE_SIZE,					// MaxRectangleTextureSize,
				GL_MAX_TEXTURE_IMAGE_UNITS,						// MaxTextureImageUnits,
				GL_MAX_TEXTURE_LOD_BIAS,						// MaxTextureLodBias,
				GL_MAX_TEXTURE_SIZE,							// MaxTextureSize,
				GL_MAX_RENDERBUFFER_SIZE,						// MaxRenderbufferSize,
				GL_MAX_ARRAY_TEXTURE_LAYERS,					// MaxArrayTextureLayers,
				GL_MAX_TEXTURE_BUFFER_SIZE,						// MaxTextureBufferSize,
				GL_MAX_UNIFORM_BLOCK_SIZE,						// MaxUniformBlockSize,
				GL_MAX_VARYING_FLOATS,							// MaxVaryingFloats,
				GL_MAX_VERTEX_ATTRIBS,							// MaxVertexAttribs,
				GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,				// MaxVertexTextureImageUnits,
				GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS,			// MaxGeometryTextureImageUnits,
				GL_MAX_VERTEX_UNIFORM_COMPONENTS,				// MaxVertexUniformComponents,
				GL_MAX_VERTEX_OUTPUT_COMPONENTS,				// MaxVertexOutputComponents,
				GL_MAX_GEOMETRY_UNIFORM_COMPONENTS,				// MaxGeometryUniformComponents,
				GL_MAX_SAMPLE_MASK_WORDS,						// MaxSamplMaskWords,
				GL_MAX_COLOR_TEXTURE_SAMPLES,					// MaxColorTextureSamples,
				GL_MAX_DEPTH_TEXTURE_SAMPLES,					// MaxDepthTextureSamples,
				GL_MAX_INTEGER_SAMPLES,							// MaxIntegerSamples,
				GL_MAX_SERVER_WAIT_TIMEOUT,						// MawServerWaitTimeout,
				GL_MAX_UNIFORM_BUFFER_BINDINGS,					// MaxUniformBufferBindings,
				GL_MAX_VERTEX_UNIFORM_BLOCKS,					// MaxVertexBufferUniformBlocks,
				GL_MAX_GEOMETRY_UNIFORM_BLOCKS,					// MaxGeometryUniformBlocks,
				GL_MAX_GEOMETRY_INPUT_COMPONENTS,				// MaxGeometryInputComponents,
				GL_MAX_GEOMETRY_OUTPUT_COMPONENTS,				// MaxGeometryOutputComponents,
				GL_MAX_VIEWPORT_DIMS,							// MaxViewportDims,
				GL_NUM_COMPRESSED_TEXTURE_FORMATS,				// NumCompressedTextureFormats,
				GL_PACK_ALIGNMENT,								// PackAlignment,
				GL_PACK_IMAGE_HEIGHT,							// PackImageHeight,
				GL_PACK_LSB_FIRST,								// PackLsbFirst,
				GL_PACK_ROW_LENGTH,								// PackRowLength,
				GL_PACK_SKIP_IMAGES,							// PackSkipImages,
				GL_PACK_SKIP_PIXELS,							// PackSkipPixels,
				GL_PACK_SKIP_ROWS,								// PackSkipRows,
				GL_PACK_SWAP_BYTES,								// PackSwapBytes,
				GL_PIXEL_PACK_BUFFER_BINDING,					// PixelPackBufferBinding,
				GL_PIXEL_UNPACK_BUFFER_BINDING,					// PixelUnpackBufferBinding,
				GL_POINT_FADE_THRESHOLD_SIZE,					// PointFadeThresholdSize,
				GL_PRIMITIVE_RESTART_INDEX,						// PrimitiveRestartIndex,
				GL_PROGRAM_POINT_SIZE,							// ProgramPointSize,
				GL_PROVOKING_VERTEX,							// ProvokingVertex,
				GL_POINT_SIZE,									// PointSize,
				GL_POINT_SIZE_GRANULARITY,						// PointSizeGranularity,
				GL_POINT_SIZE_RANGE,							// PointSizeRange,
				GL_POLYGON_OFFSET_FACTOR,						// PolygonOffsetFactor,
				GL_POLYGON_OFFSET_UNITS,						// PolygonOffsetUnits,
				GL_POLYGON_OFFSET_FILL,							// PolygonOffsetFill,
				GL_POLYGON_OFFSET_LINE,							// PolygonOffsetLine,
				GL_POLYGON_OFFSET_POINT,						// PolygonOffsetPoint,
				GL_POLYGON_SMOOTH,								// PolygonSmooth,
				GL_POLYGON_SMOOTH_HINT,							// PolygonSmoothHint,
				GL_READ_BUFFER,									// ReadBuffer,
				GL_SAMPLE_BUFFERS,								// SampleBuffers,
				GL_SAMPLE_COVERAGE_VALUE,						// SampleCoverageValue,
				GL_SAMPLE_COVERAGE_INVERT,						// SampleCoverageInvert,
				GL_SAMPLER_BINDING,								// SamplerBinding,
				GL_SAMPLES,										// Samples,
				GL_SCISSOR_BOX,									// ScissorBox,
				GL_SCISSOR_TEST,								// ScissorTest,
				GL_STENCIL_BACK_FAIL,							// StencilBackFail,
				GL_STENCIL_BACK_FUNC,							// StencilBackFunc,
				GL_STENCIL_BACK_PASS_DEPTH_FAIL,				// StencilBackPassDepthFail,
				GL_STENCIL_BACK_PASS_DEPTH_PASS,				// StencilBackPassDepthPass,
				GL_STENCIL_BACK_REF,							// StencilBackRef,
				GL_STENCIL_BACK_VALUE_MASK,						// StencilBackValueMask,
				GL_STENCIL_BACK_WRITEMASK,						// StencilBackWritemask,
				GL_STENCIL_CLEAR_VALUE,							// StencilClearValue,
				GL_STENCIL_FAIL,								// StencilFail,
				GL_STENCIL_FUNC,								// StencilFunc,
				GL_STENCIL_PASS_DEPTH_FAIL,						// StencilPassDepthFail,
				GL_STENCIL_PASS_DEPTH_PASS,						// StencilPassDepthPass,
				GL_STENCIL_REF,									// StencilRef,
				GL_STENCIL_TEST,								// StencilTest,
				GL_STENCIL_VALUE_MASK,							// StencilValueMask,
				GL_STENCIL_WRITEMASK,							// StencilWritemask,
				GL_STEREO,										// Stereo,
				GL_SUBPIXEL_BITS,								// SubpixelBits,
				GL_TEXTURE_BINDING_1D,							// TextureBinding1D,
				GL_TEXTURE_BINDING_1D_ARRAY,					// TextureBinding1DArray,
				GL_TEXTURE_BINDING_2D,							// TextureBinding2D,
				GL_TEXTURE_BINDING_2D_ARRAY,					// TextureBinding2DArray,
				GL_TEXTURE_BINDING_2D_MULTISAMPLE,				// TextureBinding2DMultisample,
				GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY,		// TextureBinding2DMultisampleArray,
				GL_TEXTURE_BINDING_3D,							// TextureBinding3D,
				GL_TEXTURE_BINDING_BUFFER,						// TextureBindingBuffer,
				GL_TEXTURE_BINDING_CUBE_MAP,					// TextureBindingCubeMap,
				GL_TEXTURE_BINDING_RECTANGLE,					// TextureBindingRectangle,
				GL_TEXTURE_COMPRESSION_HINT,					// TextureCompressionHint,
				GL_TIMESTAMP,									// Timestamp,
				GL_TRANSFORM_FEEDBACK_BUFFER_BINDING,			// TransformFeedbackBufferBinding,
				GL_TRANSFORM_FEEDBACK_BUFFER_START,				// TransformFeedbackBufferStart,
				GL_TRANSFORM_FEEDBACK_BUFFER_SIZE,				// TransformFeedbackBufferSize,
				GL_UNIFORM_BUFFER_BINDING,						// UniformBufferBinding,
				GL_UNIFORM_BUFFER_START,						// UniformBufferStart,
				GL_UNIFORM_BUFFER_SIZE,							// UniformBufferSize,
				GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,				// UniformBufferOffsetAlignment,
				GL_UNPACK_ALIGNMENT,							// UnpackAlignment,
				GL_UNPACK_IMAGE_HEIGHT,							// UnpackImageHeight,
				GL_UNPACK_LSB_FIRST,							// UnpackLsbFirst,
				GL_UNPACK_ROW_LENGTH,							// UnpackRowLength,
				GL_UNPACK_SKIP_IMAGES,							// UnpackSkipImages,
				GL_UNPACK_SKIP_PIXELS,							// UnpackSkipPixels,
				GL_UNPACK_SKIP_ROWS,							// UnpackSkipRows,
				GL_UNPACK_SWAP_BYTES,							// UnpackSwapBytes,
				GL_NUM_EXTENSIONS,								// NumExtentions,
				GL_MAJOR_VERSION,								// MajorVersion,
				GL_MINOR_VERSION,								// MinorVersion,
				GL_CONTEXT_FLAGS,								// ContextFlags,
				GL_VIEWPORT										// Viewport
		};

		GLenum OpenGLWrapper::s_shaderIntegerNames[]{
			GL_SHADER_TYPE,				// ShaderType
			GL_DELETE_STATUS,			// DeleteStatus
			GL_COMPILE_STATUS,			// CompileStatus
			GL_INFO_LOG_LENGTH,			// InfoLogLength
			GL_SHADER_SOURCE_LENGTH		// ShaderSourceLength
		};

		GLenum OpenGLWrapper::s_programIntegerNames[]{
			GL_DELETE_STATUS,							//
			GL_LINK_STATUS,								//
			GL_VALIDATE_STATUS,							//
			GL_INFO_LOG_LENGTH,							//
			GL_ATTACHED_SHADERS,						//
			GL_ACTIVE_ATTRIBUTES,						//
			GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,				//
			GL_ACTIVE_UNIFORMS,							//
			GL_ACTIVE_UNIFORM_BLOCKS,					//
			GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH,	//
			GL_ACTIVE_UNIFORM_MAX_LENGTH,				//
			GL_TRANSFORM_FEEDBACK_BUFFER_MODE,			//
			GL_TRANSFORM_FEEDBACK_VARYINGS,				//
			GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH,	//
			GL_GEOMETRY_VERTICES_OUT,					//
			GL_GEOMETRY_INPUT_TYPE,						//
			GL_GEOMETRY_OUTPUT_TYPE						//
		};

	} // !namespace Graphic
} // !namespace Croissant
