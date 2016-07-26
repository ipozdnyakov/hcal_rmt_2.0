#!/bin/bash
# ./compile.sh code.cc

files=""
if [ "${1}" == "all" ] ; then
    echo "compile all"
    files=`ls *.cc`
else
    files=${1}
fi

if [ ${#files} -eq 0 ] ; then
    echo "syntax:"
    echo "  compile.csh code.cc"
    echo "  compile.csh all"
    if [ "${1}" == "all" ] ; then
	echo "Error: no *.cc files detected"
    fi
    exit
fi

ROOTINC="-I `root-config --incdir`"
ROOTLIBS="`root-config --libs`"
CFLAGS="-std=c++11"
#CFLAGS=""

#echo "ROOTINC=${ROOTINC}"
#echo "ROOTLIBS=${ROOTLIBS}"

for code in ${files} ; do
    echo "code=${code}"
    comm="c++ ${code} ${ROOTINC} ${CFLAGS} -o ${code}.exe ${ROOTLIBS}"
    #echo "${comm}"
    ${comm}
done

#c++ ${1} -std=c++11 -I ${ROOTSYS}/include -L ${ROOTSYS}/lib -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lTMVA -lMinuit -lXMLIO -lMLP -lTreePlayer -lz -lGui -pthread -lm -ldl 
#mv a.out ${1}.exe
