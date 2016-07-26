#!/bin/csh
c++ -std=c++11 ${1} -I ${ROOTSYS}/include -L ${ROOTSYS}/lib -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lTMVA -lMinuit -lXMLIO -lMLP -lTreePlayer -lz -lGui -pthread -lm -ldl 
mv a.out ${1}.exe
