!!ARBvp1.0
TEMP r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
PARAM c[96] = { program.env[0..95] };
ALIAS oPos = result.position;
ALIAS oD0 = result.color.primary;
ALIAS oT0 = result.texcoord[0];
ALIAS oT1 = result.texcoord[1];
ALIAS oFog = result.fogcoord;

ADDRESS a0;

ATTRIB v0 = vertex.attrib[0];
ATTRIB v1 = vertex.attrib[1];
ATTRIB v2 = vertex.attrib[2];
ATTRIB v3 = vertex.attrib[3];
ATTRIB v4 = vertex.attrib[4];
ATTRIB v5 = vertex.attrib[5];
ATTRIB v6 = vertex.attrib[6];

#DAJ defaults
MOV oD0, c[95].w;
MOV oT0, c[95];
MOV oT1, c[95];
MOV oFog, c[95];

# (10) build blended transform (2 nodes) ------------------------------------------------
MUL r0.xy, v5, c[9].w;
ADD r0.xy, r0, c[5].w;
ARL a0.x, r0.x;      
MUL r4, v6.x, c[a0.x + 29];
MUL r5, v6.x, c[a0.x + 30];
MUL r6, v6.x, c[a0.x + 31];
ARL a0.x, r0.y;      
MAD r4, v6.y, c[a0.x + 29], r4;
MAD r5, v6.y, c[a0.x + 30], r5;
MAD r6, v6.y, c[a0.x + 31], r6;

# (4) transform position ----------------------------------------------------------------
DP4 r0.x, v0, r4;
DP4 r0.y, v0, r5;
DP4 r0.z, v0, r6;
MOV r0.w, v4.w; # necessary because we can't use DPH

# KLC - commenting out most of this for now.

# (6) transform normal ------------------------------------------------------------------
DP3 r1.x, v1, r4;
DP3 r1.y, v1, r5;
DP3 r1.z ,v1, r6;
DP3 r1.w, r1, r1;
RSQ r1.w, r1.w;
MUL r1.xyz, r1, r1.w;

# (6) transform binormal ----------------------------------------------------------------
DP3 r2.x, v2, r4;
DP3 r2.y, v2, r5;
DP3 r2.z ,v2, r6;
DP3 r2.w, r2, r2;
RSQ r2.w, r2.w;
MUL r2.xyz, r2, r2.w;

# (6) transform tangent -----------------------------------------------------------------
DP3 r3.x, v3, r4;
DP3 r3.y, v3, r5;
DP3 r3.z ,v3, r6;
DP3 r3.w, r3, r3;
RSQ r3.w, r3.w;
MUL r3.xyz, r3, r3.w;

# (1) eye vector ------------------------------------------------------------------------
ADD r5.xyz, -r0, c[4];

# (4) output homogeneous point ----------------------------------------------------------
DP4 oPos.x, r0, c[0];
DP4 oPos.y, r0, c[1];
DP4 oPos.z, r0, c[2];
DP4 oPos.w, r0, c[3];

# (4) output texcoords ------------------------------------------------------------------
MOV oT0.xy, v4; # opacity map
# KLC - commenting out for now.
DP3 oT1.x, r5, r3; # eye vector in bump map pace
DP3 oT1.y, r5, r2;
DP3 oT1.z, r5, r1;


# (2) atmospheric fog ----------------------------------------------------------------
DP4 oFog.x, v0, c[6];
END
