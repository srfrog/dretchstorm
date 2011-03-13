/*
===========================================================================
Copyright (C) 2010 Robert Beckebans <trebor_7@users.sourceforge.net>

This file is part of XreaL source code.

XreaL source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

XreaL source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with XreaL source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
// gl_shader.cpp -- GLSL shader handling

#include "gl_shader.h"

// *INDENT-OFF*

void		 GLSL_InitGPUShader3(shaderProgram_t * program,
								const char *vertexMainShader,
								const char *fragmentMainShader,
								const char *vertexLibShaders,
								const char *fragmentLibShaders,
								const char *compileMacros,
								int attribs,
								qboolean optimize);

void		GLSL_ValidateProgram(GLhandleARB program);
void		GLSL_ShowProgramUniforms(GLhandleARB program);



GLShader_generic* gl_genericShader = NULL;
GLShader_lightMapping* gl_lightMappingShader = NULL;
GLShader_vertexLighting_DBS_entity* gl_vertexLightingShader_DBS_entity = NULL;




const char* GLShader::GetCompileMacrosString(int permutation)
{
	static char compileMacros[4096];

	Com_Memset(compileMacros, 0, sizeof(compileMacros));

	for(size_t j = 0; j < _compileMacros.size(); j++)
	{
		GLCompileMacro* macro = _compileMacros[j];

		if(permutation & macro->GetBit())
		{
			Q_strcat(compileMacros, sizeof(compileMacros), macro->GetName());
			Q_strcat(compileMacros, sizeof(compileMacros), " ");
		}
	}

	return compileMacros;
}






GLShader_generic::GLShader_generic():
		GLShader(),
		u_ColorTextureMatrix(this),
		u_ViewOrigin(this),
		u_ColorGen(this),
		u_AlphaGen(this),
		u_AlphaTest(this),
		u_Color(this),
		u_ModelMatrix(this),
		u_ModelViewProjectionMatrix(this),
		u_BoneMatrix(this),
		u_VertexInterpolation(this),
		u_PortalPlane(this),
		GLDeformStage(this),
		GLCompileMacro_USE_PORTAL_CLIPPING(this),
		GLCompileMacro_USE_ALPHA_TESTING(this),
		GLCompileMacro_USE_VERTEX_SKINNING(this),
		GLCompileMacro_USE_VERTEX_ANIMATION(this),
		GLCompileMacro_USE_DEFORM_VERTEXES(this),
		GLCompileMacro_USE_TCGEN_ENVIRONMENT(this)
{
	_vertexAttribs = ATTR_POSITION | ATTR_TEXCOORD | ATTR_NORMAL;

	_shaderPrograms = std::vector<shaderProgram_t>(1 << _compileMacros.size());
	
	//Com_Memset(_shaderPrograms, 0, sizeof(_shaderPrograms));

	size_t numPermutations = (1 << _compileMacros.size());	// same as 2^n, n = no. compile macros
	for(size_t i = 0; i < numPermutations; i++)
	{
		const char* compileMacros = GetCompileMacrosString(i);

		ri.Printf(PRINT_ALL, "Compile macros: '%s'\n", compileMacros);

		shaderProgram_t *shaderProgram = &_shaderPrograms[i];

		GLSL_InitGPUShader3(shaderProgram,
						"generic",
						"generic",
						"vertexAnimation deformVertexes",
						"",
						compileMacros,
						ATTR_POSITION | ATTR_TEXCOORD | ATTR_NORMAL | ATTR_COLOR |
						ATTR_POSITION2 | ATTR_NORMAL2,
						qtrue);

		shaderProgram->u_ColorMap = qglGetUniformLocationARB(shaderProgram->program, "u_ColorMap");
		shaderProgram->u_ColorTextureMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_ColorTextureMatrix");
		shaderProgram->u_ColorGen = qglGetUniformLocationARB(shaderProgram->program, "u_ColorGen");
		shaderProgram->u_AlphaGen = qglGetUniformLocationARB(shaderProgram->program, "u_AlphaGen");
		shaderProgram->u_Color = qglGetUniformLocationARB(shaderProgram->program, "u_Color");
		shaderProgram->u_AlphaTest = qglGetUniformLocationARB(shaderProgram->program, "u_AlphaTest");
		shaderProgram->u_ViewOrigin = qglGetUniformLocationARB(shaderProgram->program, "u_ViewOrigin");
		shaderProgram->u_TCGen_Environment = qglGetUniformLocationARB(shaderProgram->program, "u_TCGen_Environment");
		shaderProgram->u_DeformGen = qglGetUniformLocationARB(shaderProgram->program, "u_DeformGen");
		shaderProgram->u_DeformWave = qglGetUniformLocationARB(shaderProgram->program, "u_DeformWave");
		shaderProgram->u_DeformBulge = qglGetUniformLocationARB(shaderProgram->program, "u_DeformBulge");
		shaderProgram->u_DeformSpread = qglGetUniformLocationARB(shaderProgram->program, "u_DeformSpread");
		shaderProgram->u_Time = qglGetUniformLocationARB(shaderProgram->program, "u_Time");
		shaderProgram->u_PortalClipping = qglGetUniformLocationARB(shaderProgram->program, "u_PortalClipping");
		shaderProgram->u_PortalPlane = qglGetUniformLocationARB(shaderProgram->program, "u_PortalPlane");
		shaderProgram->u_ModelMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_ModelMatrix");
		/*
		   shaderProgram->u_ModelViewMatrix =
		   qglGetUniformLocationARB(shaderProgram->program, "u_ModelViewMatrix");
		   shaderProgram->u_ProjectionMatrix =
		   qglGetUniformLocationARB(shaderProgram->program, "u_ProjectionMatrix");
		 */
		shaderProgram->u_ModelViewProjectionMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_ModelViewProjectionMatrix");
		if(glConfig.vboVertexSkinningAvailable)
		{
			shaderProgram->u_VertexSkinning = qglGetUniformLocationARB(shaderProgram->program, "u_VertexSkinning");
			shaderProgram->u_BoneMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_BoneMatrix");
		}

		qglUseProgramObjectARB(shaderProgram->program);
		qglUniform1iARB(shaderProgram->u_ColorMap, 0);
		qglUseProgramObjectARB(0);

		GLSL_ValidateProgram(shaderProgram->program);
		GLSL_ShowProgramUniforms(shaderProgram->program);
		GL_CheckErrors();
	}

	SelectProgram();
}











GLShader_lightMapping::GLShader_lightMapping():
		GLShader(),
		u_DiffuseTextureMatrix(this),
		u_AlphaTest(this),
		u_ModelViewProjectionMatrix(this),
		u_PortalPlane(this),
		GLDeformStage(this),
		GLCompileMacro_USE_PORTAL_CLIPPING(this),
		GLCompileMacro_USE_ALPHA_TESTING(this),
		GLCompileMacro_USE_DEFORM_VERTEXES(this)
{
	_shaderPrograms = std::vector<shaderProgram_t>(1 << _compileMacros.size());
	
	//Com_Memset(_shaderPrograms, 0, sizeof(_shaderPrograms));

	size_t numPermutations = (1 << _compileMacros.size());	// same as 2^n, n = no. compile macros
	for(size_t i = 0; i < numPermutations; i++)
	{
		const char* compileMacros = GetCompileMacrosString(i);

		ri.Printf(PRINT_ALL, "Compile macros: '%s'\n", compileMacros);

		shaderProgram_t *shaderProgram = &_shaderPrograms[i];

		GLSL_InitGPUShader3(shaderProgram,
						"lightMapping",
						"lightMapping",
						"deformVertexes",
						"",
						compileMacros,
						ATTR_POSITION | ATTR_TEXCOORD | ATTR_LIGHTCOORD | ATTR_NORMAL,
						qtrue);

		shaderProgram->u_ModelViewProjectionMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_ModelViewProjectionMatrix");

		shaderProgram->u_DiffuseMap = qglGetUniformLocationARB(shaderProgram->program, "u_DiffuseMap");
		shaderProgram->u_LightMap = qglGetUniformLocationARB(shaderProgram->program, "u_LightMap");

		shaderProgram->u_DiffuseTextureMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_DiffuseTextureMatrix");
		shaderProgram->u_AlphaTest = qglGetUniformLocationARB(shaderProgram->program, "u_AlphaTest");
		shaderProgram->u_DeformGen = qglGetUniformLocationARB(shaderProgram->program, "u_DeformGen");
		shaderProgram->u_DeformWave = qglGetUniformLocationARB(shaderProgram->program, "u_DeformWave");
		shaderProgram->u_DeformBulge = qglGetUniformLocationARB(shaderProgram->program, "u_DeformBulge");
		shaderProgram->u_DeformSpread = qglGetUniformLocationARB(shaderProgram->program, "u_DeformSpread");
		shaderProgram->u_Time = qglGetUniformLocationARB(shaderProgram->program, "u_Time");

		qglUseProgramObjectARB(shaderProgram->program);
		qglUniform1iARB(shaderProgram->u_DiffuseMap, 0);
		qglUniform1iARB(shaderProgram->u_LightMap, 1);
		qglUseProgramObjectARB(0);

		GLSL_ValidateProgram(shaderProgram->program);
		GLSL_ShowProgramUniforms(shaderProgram->program);
		GL_CheckErrors();
	}

	SelectProgram();
}



GLShader_vertexLighting_DBS_entity::GLShader_vertexLighting_DBS_entity():
		GLShader(),
		u_AlphaTest(this),
		u_AmbientColor(this),
		u_ViewOrigin(this),
		u_LightDir(this),
		u_LightColor(this),
		u_ModelMatrix(this),
		u_ModelViewProjectionMatrix(this),
		u_BoneMatrix(this),
		u_VertexInterpolation(this),
		u_PortalPlane(this),
		u_DepthScale(this),
		GLDeformStage(this),
		GLCompileMacro_USE_PORTAL_CLIPPING(this),
		GLCompileMacro_USE_ALPHA_TESTING(this),
		GLCompileMacro_USE_VERTEX_SKINNING(this),
		GLCompileMacro_USE_VERTEX_ANIMATION(this),
		GLCompileMacro_USE_DEFORM_VERTEXES(this),
		GLCompileMacro_USE_PARALLAX_MAPPING(this)
{
	_shaderPrograms = std::vector<shaderProgram_t>(1 << _compileMacros.size());
	
	//Com_Memset(_shaderPrograms, 0, sizeof(_shaderPrograms));

	size_t numPermutations = (1 << _compileMacros.size());	// same as 2^n, n = no. compile macros
	for(size_t i = 0; i < numPermutations; i++)
	{
		const char* compileMacros = GetCompileMacrosString(i);

		ri.Printf(PRINT_ALL, "Compile macros: '%s'\n", compileMacros);

		shaderProgram_t *shaderProgram = &_shaderPrograms[i];

		GLSL_InitGPUShader3(shaderProgram,
						"vertexLighting_DBS_entity",
						"vertexLighting_DBS_entity",
						"vertexAnimation deformVertexes",
						"",
						compileMacros,
						ATTR_POSITION | ATTR_TEXCOORD | ATTR_TANGENT | ATTR_BINORMAL | ATTR_NORMAL |
						ATTR_POSITION2 | ATTR_TANGENT2 | ATTR_BINORMAL2 | ATTR_NORMAL2,
						qtrue);

		shaderProgram->u_DiffuseMap	= qglGetUniformLocationARB(shaderProgram->program, "u_DiffuseMap");
		shaderProgram->u_NormalMap = qglGetUniformLocationARB(shaderProgram->program, "u_NormalMap");
		shaderProgram->u_SpecularMap = qglGetUniformLocationARB(shaderProgram->program, "u_SpecularMap");
		shaderProgram->u_DiffuseTextureMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_DiffuseTextureMatrix");
		shaderProgram->u_NormalTextureMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_NormalTextureMatrix");
		shaderProgram->u_SpecularTextureMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_SpecularTextureMatrix");
		shaderProgram->u_AlphaTest = qglGetUniformLocationARB(shaderProgram->program, "u_AlphaTest");
		shaderProgram->u_DeformGen = qglGetUniformLocationARB(shaderProgram->program, "u_DeformGen");
		shaderProgram->u_DeformWave = qglGetUniformLocationARB(shaderProgram->program, "u_DeformWave");
		shaderProgram->u_DeformBulge = qglGetUniformLocationARB(shaderProgram->program, "u_DeformBulge");
		shaderProgram->u_DeformSpread = qglGetUniformLocationARB(shaderProgram->program, "u_DeformSpread");
		shaderProgram->u_ViewOrigin = qglGetUniformLocationARB(shaderProgram->program, "u_ViewOrigin");
		shaderProgram->u_AmbientColor = qglGetUniformLocationARB(shaderProgram->program, "u_AmbientColor");
		shaderProgram->u_LightDir = qglGetUniformLocationARB(shaderProgram->program, "u_LightDir");
		shaderProgram->u_LightColor = qglGetUniformLocationARB(shaderProgram->program, "u_LightColor");
		shaderProgram->u_ParallaxMapping = qglGetUniformLocationARB(shaderProgram->program, "u_ParallaxMapping");
		shaderProgram->u_DepthScale = qglGetUniformLocationARB(shaderProgram->program, "u_DepthScale");
		shaderProgram->u_PortalClipping = qglGetUniformLocationARB(shaderProgram->program, "u_PortalClipping");
		shaderProgram->u_PortalPlane = qglGetUniformLocationARB(shaderProgram->program, "u_PortalPlane");
		shaderProgram->u_ModelMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_ModelMatrix");
		shaderProgram->u_ModelViewProjectionMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_ModelViewProjectionMatrix");
		shaderProgram->u_Time = qglGetUniformLocationARB(shaderProgram->program, "u_Time");
		if(glConfig.vboVertexSkinningAvailable)
		{
			shaderProgram->u_VertexSkinning = qglGetUniformLocationARB(shaderProgram->program, "u_VertexSkinning");
			shaderProgram->u_BoneMatrix = qglGetUniformLocationARB(shaderProgram->program, "u_BoneMatrix");
		}
		shaderProgram->u_VertexInterpolation = qglGetUniformLocationARB(shaderProgram->program, "u_VertexInterpolation");

		qglUseProgramObjectARB(shaderProgram->program);
		qglUniform1iARB(shaderProgram->u_DiffuseMap, 0);
		qglUniform1iARB(shaderProgram->u_NormalMap, 1);
		qglUniform1iARB(shaderProgram->u_SpecularMap, 2);
		qglUseProgramObjectARB(0);

		GLSL_ValidateProgram(shaderProgram->program);
		GLSL_ShowProgramUniforms(shaderProgram->program);
		GL_CheckErrors();
	}

	SelectProgram();
}