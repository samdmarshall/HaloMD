Texture Texture0;
Texture Texture1;
Texture Texture2;
Texture Texture3;

/* DAJ
sampler TexSampler0 = sampler_state
{
    Texture = (Texture0);
};

sampler TexSampler1 = sampler_state
{
    Texture = (Texture1);
};

sampler TexSampler2 = sampler_state
{
    Texture = (Texture2);
};

sampler TexSampler3 = sampler_state
{
    Texture = (Texture3);
};

half4 c_eye_forward;
half4 c_view_perpendicular_color;
half4 c_view_parallel_color;

half4 EnvironmentReflectionMirrorBumped(
    half4 Diff : COLOR0,
    half4 Tex0  : TEXCOORD0,
    half4 Tex1  : TEXCOORD1,
    half4 Tex2  : TEXCOORD2,
    half4 Tex3  : TEXCOORD3) : COLOR
{
	half4 T0 = tex2D(TexSampler0, Tex0);
	half4 T1 = texCUBE(TexSampler1, Tex1);
	half4 T2 = texCUBE(TexSampler2, Tex2);
	half4 T3 = tex2Dproj(TexSampler3, Tex3);
	
	half3 R0;
	half R0a;
	half3 R1;
	half R1a;
	half3 SRCCOLOR;
	half SRCALPHA;
	
	// combiner 0
	R0	= dot(((2*T2)-1),c_eye_forward);
	R1	= T3*T3;

	saturate(R0);
	
	// combiner 1
	R0	= R0*R0;
	R1	= R1*R1;
	
	// combiner 2
	R1	= R1*R1;
	
	// combiner 3
	R0a	= lerp(c_view_parallel_color.a, c_view_perpendicular_color.a, R0.b);
	R0	= lerp(c_view_parallel_color, c_view_perpendicular_color, R0.b);
	
	// combiner 4
	R0	= lerp( R1, T3, R0 );
	
	// final combiner
	SRCCOLOR = R0*R0a;
	SRCALPHA = 1.0;
	
	return half4( SRCCOLOR, SRCALPHA );
}

Technique ps_2_0
{
    Pass P0
	{
		ColorOp[0]	= Disable;
		AlphaOp[0]	= Disable;
		
		PixelShader = compile PS_2_0_TARGET EnvironmentReflectionMirrorBumped();
 	}
}

PixelShader PS_EnvironmentReflectionMirrorBumped_ps_1_4 = asm
{
    #define T0 r0
    #define T1 r1
    #define T2 r2
    #define T2_bx2 r2_bx2
    #define T3 r3
    #define R0 r4
    #define R1 r5
    #define c_eye_forward c0
    #define c_view_perpendicular_color c1
    #define c_view_parallel_color c2
    #define one c3

    ps_1_4

    def one, 1, 1, 1, 1

    texld T0, t0
    texld T1, t1
    texld T2, t2
    texld T3, t3_dw.xyw

    dp3_sat R0, T2_bx2, c_eye_forward
    mul R1, T3, T3
    mul_sat R0, R0, R0
    mul R1, R1, R1
    mul R1, R1, R1
    lrp_sat R0, R0.b, c_view_perpendicular_color, c_view_parallel_color
    lrp R0.rgb, R0, T3, R1
    mul r0.rgb, R0.a, R0
    + mov r0.a, one

//    #undef T0
//    #undef T1
//    #undef T2
//    #undef T2_bx2
//    #undef T3
//    #undef R0
//    #undef R1
//    #undef one
//    #undef c_eye_forward
//    #undef c_view_perpendicular_color
//    #undef c_view_parallel_color
};

*/

Technique ps_1_4
{
    Pass P0
	{
		PixelShaderConstant[0] = (c_eye_forward);
		PixelShaderConstant[1] = (c_view_perpendicular_color);
		PixelShaderConstant[2] = (c_view_parallel_color);
		// KLC - Viewport bounds for RECT scaling, actually
		PixelShaderConstant[3] = (c_primary_change_color);

        Texture[0]	= (Texture0);
        Texture[1]	= (Texture1);
        Texture[2]	= (Texture2);
        Texture[3]	= (Texture3);

		ColorOp[0]	= Disable;
		AlphaOp[0]	= Disable;
		
		PixelShader = asm
		{
			TEMP t0, t1, t2, t3, t4, t5, t6, t7, tmp0, tmp1, tmp2, r0, r1, r2, r3, r4, r5;
			PARAM const = {0.0, 0.5, 1.0, 2.0};
			PARAM mulConst = {2.0, 4.0, 8.0, 0.0};
			PARAM divConst = {0.5, 0.25, 0.125, 0.0};
			PARAM c0 = program.env[0];
			PARAM c1 = program.env[1];
			PARAM c2 = program.env[2];
			PARAM c3 = program.env[3];
			ATTRIB tc0 = fragment.texcoord[0];
			ATTRIB tc1 = fragment.texcoord[1];
			ATTRIB tc2 = fragment.texcoord[2];
			ATTRIB tc3 = fragment.texcoord[3];
			OUTPUT oC0 = result.color;
			TEX r0, tc0, texture[0], 2D;
			TEX r1, tc1, texture[1], CUBE;
			TEX r2, tc2, texture[2], CUBE;
			
			# convert to not projective texture coord (divide by w)
         	MOV tmp0, tc3;
         	RCP tmp0, tc3.w;
         	MUL tmp0, tc3, tmp0;
         	#MOV tmp0.w, 1;
         	        	
         	#scale to pixel coords and invert
         	MUL tmp0, tmp0, c3;
         	SUB tmp0.y, c3.y, tmp0.y;
          							
			TEX r3, tmp0, texture[3], RECT;
						
			MAD tmp0, r2, mulConst.r, -const.b;
			DP3_SAT r4, tmp0, c0;
			MUL r5, r3, r3;
			MUL_SAT r4, r4, r4;
			MUL r5, r5, r5;
			MUL r5, r5, r5;
			LRP_SAT r4, r4.b, c1, c2;
			LRP r4.rgb, r4, r3, r5;
			MUL r0.rgb, r4.a, r4;
			MOV r0.a, c3;
			MOV oC0, r0;
		};
		//PixelShader = (PS_EnvironmentReflectionMirrorBumped_ps_1_4);
 	}
}

Technique ps_1_1
{
    Pass P0
	{
		PixelShaderConstant[0] = (c_eye_forward);
		PixelShaderConstant[1] = (c_view_perpendicular_color);
		PixelShaderConstant[2] = (c_view_parallel_color);
		// KLC - Viewport bounds for RECT scaling, actually
		PixelShaderConstant[3] = (c_primary_change_color);

		Texture[0]	= (Texture0);
		Texture[1]	= (Texture1);
		Texture[2]	= (Texture2);
		Texture[3]	= (Texture3);
		
		ColorOp[0]	= Disable;
		AlphaOp[0]	= Disable;
		
		PixelShader	= asm
		{
			TEMP r0, r1;
			ATTRIB f0 = fragment.texcoord[0];
			ATTRIB f1 = fragment.texcoord[1];
			ATTRIB f2 = fragment.texcoord[2];
			ATTRIB f3 = fragment.texcoord[3];
			ATTRIB v0 = fragment.color.primary;
			
			PARAM half = { 0.5, 0.5, 0.5, 0.5 };
			PARAM two = { 2.0, 2.0, 2.0, 2.0 };
			PARAM one = { 1.0, 1.0, 1.0, 1.0 };
			PARAM c1 = program.env[1];
			PARAM c2 = program.env[2];
			PARAM c3 = program.env[3];

			TEMP t0, t1, t2, t3;
			TEX t0, f0, texture[0], 2D;
			TEX t1, f1, texture[1], 2D;
			TEX t2, f2, texture[2], CUBE;
			TEX t3, f3, texture[3], CUBE;
			OUTPUT oC0 = result.color; 

			TEMP t0_bx2, t1_bx2;
			SUB t0_bx2, t0, half;			
			MUL t0_bx2, t0_bx2, two;
			SUB t1_bx2, t1, half;			
			MUL t1_bx2, t1_bx2, two;
			
			# this doesn't actually render the mirror since we cant do the divide by w on the texture coords...			
			DP3_SAT	r0, t1_bx2, t0_bx2;
			MUL r0, r0, r0;
			MOV r0.a, r0.b;
			LRP r0, c1, c2, r0.a;

#DAJ trying the 1.4, didn't work	
#			TEMP t2_bx2;
#			SUB t2_bx2, t2, half;			
#			MUL t2_bx2, t2_bx2, two;
			
#			DP3_SAT r0, t2_bx2, c1;
#			MUL r1, t3, t3;
#			MUL_SAT r0, r0, r0;
#			MUL r1, r1, r1;
#			MUL r1, r1, r1;
#			LRP_SAT r0, r0.b, c1, c2;
#			LRP r0.rgb, r0, t3, r1;
#			MUL r0.rgb, r0.a, r0;
#			MOV r0.a, one;

			MOV     oC0, r0;	#DAJ save off
		};
 	}
}

Technique ps_0_0
{
	Pass P0
	{
	}
}

Technique fallback
{
    Pass P0
	{
		ColorOp[0]			= Modulate;
		ColorArg1[0]		= Texture;
		ColorArg2[0]		= Diffuse;
		AlphaOp[0]			= SelectArg1;
		AlphaArg1[0]		= Texture;

		ColorOp[1]			= Disable;
		AlphaOp[1]			= Disable;

		PixelShader			= Null;
	}
}
