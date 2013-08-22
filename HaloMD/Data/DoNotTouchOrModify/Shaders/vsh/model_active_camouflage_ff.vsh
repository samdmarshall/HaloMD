!!ARBvp1.0
TEMP r0, r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11;
PARAM c[96] = { program.env[0..95] };
ALIAS oPos = result.position;
ALIAS oD0 = result.color.primary;
ALIAS oT0 = result.texcoord[0];
ALIAS oT1 = result.texcoord[1];
ALIAS oT2 = result.texcoord[2];

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
MOV oT2, c[95];

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

# (6) transform normal ------------------------------------------------------------------
DP3 r1.x, v1, r4;
DP3 r1.y, v1, r5;
DP3 r1.z ,v1, r6;
DP3 r1.w, r1, r1;
RSQ r1.w, r1.w;
MUL r1.xyz, r1, r1.w;

# (8) eye vector (incidence) ------------------------------------------------------------
ADD r5.xyz, -r0, c[4];
DP3 r5.w, r5, c[5];
MUL r5.w, r5.w, r5.w;        # squared distance
RCP r5.w, r5.w;
MUL r5.w, r5.w, c[10].y; # refraction scale
MIN r5.w, r5.w, v4.w;                 # clamp to one
MAX r5.w, r5.w, v4.z;                # clamp to zero
MOV oD0.w, r5.w;                                 # distance also affects edge-density

# (4) output homogeneous point ----------------------------------------------------------
DP4 oPos.x, r0, c[0];
DP4 oPos.y, r0, c[1];
DP4 oPos.z, r0, c[2];
DP4 oPos.w, r0, c[3];

# output texcoords ----------------------------------------------------------------------
# (8) projection ------------------------------------------------------------------------
DP4 r10.x, r0, c[0];
DP4 r10.y, -r0, c[1];
DP4 r10.zw, r0, c[3];
ADD r10, r0.xyww, r0.wwww; # range-compress texcoords
RCP r10.w, r10.w;
MUL r10.xy, r10, r10.w;      # do the divide here
MUL oT1.x, c[10].z, r10.x;  # base u-texcoord
MUL oT2.y, c[10].w, r10.y;  # base v-texcoord

# (1) output tint color -----------------------------------------------------------------
MOV oD0.xyz, c[12];

END