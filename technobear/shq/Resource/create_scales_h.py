import os
import scale_src


def intToString(n):
#   mask=list("c#d#ef#g#a#b")
    mask=list("000000000000")
    for i in n :
        if i<12 : 
            mask[11-i]='1'
    return "".join(mask)

def createScale(f,n,intervals):
    f.write("    {")
    f.write("\""+n+"\", ")
    f.write(str(len(intervals)-1)+",")
    f.write("0b"+intToString(intervals));
    f.write("}")

fn = "Scales.h"
f=open(fn,"w")
f.write("#pragma once\n")
f.write("\n")
f.write("\n")
f.write("// Generated automatically by TheTechnobear\n")
f.write("// derived from EigenD scale manager\n")
f.write("// open source : https://github.com/EigenLabs/EigenD\n")
f.write("\n")
f.write("// scale is dim(bit11) to root (bit 0), e.g. b#a#g#fe#d#c\n")
f.write("\n")
f.write("\n")
f.write("static constexpr unsigned MAX_SCALES = "+str(len(scale_src.scales))+";\n")
f.write("\n")
f.write("static const struct Scales {\n")
f.write("    char name[30];\n")
f.write("    uint16_t numnotes;\n")
f.write("    uint16_t scale;\n")
f.write("} scales[MAX_SCALES] = {\n")


idx=0
for i in scale_src.scales :
    idx=idx+1
    sc = i[0]
    ival = i[1]
    createScale(f, sc,ival)
    if idx!=len(scale_src.scales) : 
        f.write(",")
    f.write("\n")


f.write("};\n")
f.write("\n")

f.close()
