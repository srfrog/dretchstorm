/*
===========================================================================
Copyright (C) 2006-2011 Robert Beckebans <trebor_7@users.sourceforge.net>

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

/* shadowFill_fp.glsl */

uniform sampler2D	u_ColorMap;
uniform int			u_AlphaTest;
uniform vec4		u_PortalPlane;
uniform vec3		u_LightOrigin;
uniform float       u_LightRadius;

varying vec3		var_Position;
varying vec2		var_Tex;
varying vec4		var_Color;

void	main()
{
#if defined(USE_PORTAL_CLIPPING)
	{
		float dist = dot(var_Position.xyz, u_PortalPlane.xyz) - u_PortalPlane.w;
		if(dist < 0.0)
		{
			discard;
			return;
		}
	}
#endif

	vec4 color = texture2D(u_ColorMap, var_Tex);

#if defined(USE_ALPHA_TESTING)
	if(u_AlphaTest == ATEST_GT_0 && color.a <= 0.0)
	{
		discard;
		return;
	}
	else if(u_AlphaTest == ATEST_LT_128 && color.a >= 0.5)
	{
		discard;
		return;
	}
	else if(u_AlphaTest == ATEST_GE_128 && color.a < 0.5)
	{
		discard;
		return;
	}
#endif

	
#if defined(VSM)

	float distance;

#if defined(LIGHT_DIRECTIONAL)
	distance = gl_FragCoord.z;
#else
	distance = length(var_Position - u_LightOrigin) / u_LightRadius;
#endif
	
	float distanceSquared = distance * distance;

	// shadowmap can be float RGBA or luminance alpha so store distanceSquared into alpha
	
#if defined(VSM_CLAMP)
	// convert to [0,1] color space
	gl_FragColor = vec4(distance, 0.0 , 0.0, distanceSquared) * 0.5 + 0.5;
#else
	gl_FragColor = vec4(distance, 0.0, 0.0, distanceSquared);
#endif

#elif defined(EVSM)
	
	float distance;
#if defined(LIGHT_DIRECTIONAL)
	{
		distance = gl_FragCoord.z;// * r_ShadowMapDepthScale;
		//distance /= gl_FragCoord.w;
		//distance = var_Position.z / var_Position.w;
		//distance = var_Position.z;
	}
#else
	{
		distance = (length(var_Position - u_LightOrigin) / u_LightRadius); // * r_ShadowMapDepthScale;
	}
#endif
	
	gl_FragColor = vec4(0.0, 0.0, 0.0, distance);
#else
	gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
#endif
}
