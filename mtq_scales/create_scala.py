import os
import scale_src

def nameToFn(n):
    fn=n.replace(" ","_")
    fn = fn + ".scl"
    return fn

def createScalaFile(n,intervals):
    fn = nameToFn(n)
    f=open("tb_scales/"+fn,"w")
    f.write("! "+ fn + "\n")
    f.write("! created by TheTechnobear" + "\n")
    f.write("!" + "\n")
    f.write(n + "\n")
    f.write(" " + str(len(intervals)-1) + "\n")
    f.write("!" + "\n")
    for interval in intervals :
        if interval > 0:
            f.write(" " + str(interval*100)+".\n")

    f.close()

for i in scale_src.scales :
    if not os.path.exists('./tb_scales'):
        os.mkdir("./tb_scales")
    sc = i[0]
    ival = i[1]
    if len(ival) > 1:
        createScalaFile(sc,ival)
