

#ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs)


CFLAGS=-std=c++11
#ROOTSYS=/usr/local/root
INCLUDE=-I ${ROOTSYS}/include
#LIBS=-L ${ROOTSYS}/lib -lCore -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lTMVA -lMinuit -lXMLIO -lMLP -lTreePlayer -lz -lGui -pthread -lm -ldl 
LIBS=${ROOTLIBS}

OBJ=ObjClass.o HistoDef.o ShowTimeDep.o

default: zamt_hf5.exe zamt_timeDep.exe

zamt_hf.exe: zamt_hf.cc
	c++ ${CFLAGS} $^ -o $@ ${INCLUDE} ${LIBS}

zamt_hf2.exe: zamt_hf2.cc ObjClass.o
	c++ ${CFLAGS} $^ -o $@ ${INCLUDE} ${LIBS}

zamt_hf3.exe: zamt_hf3.cc ObjClass.o HistoDef.o
	c++ ${CFLAGS} $^ -o $@ ${INCLUDE} ${LIBS}

zamt_hf4.exe: zamt_hf4.cc ObjClass.o HistoDef.o ShowTimeDep.o
	c++ ${CFLAGS} $^ -o $@ ${INCLUDE} ${LIBS}

zamt_hf5.exe: zamt_hf5.cc ObjClass.o HistoDef.o ShowTimeDep.o
	c++ ${CFLAGS} $^ -o $@ ${INCLUDE} ${LIBS}

zamt_timeDep.exe: zamt_timeDep.cc ObjClass.o HistoDef.o ShowTimeDep.o
	c++ ${CFLAGS} $^ -o $@ ${INCLUDE} ${LIBS}


ShowTimeDep.o: ShowTimeDep.cc ShowTimeDep.h helpWebPage.h HistoDef.o
	c++ ${CFLAGS} $< -c -o $@ ${INCLUDE}

HistoDef.o: HistoDef.cc HistoDef.h ObjClass.o
	c++ ${CFLAGS} $< -c -o $@ ${INCLUDE}

ObjClass.o: ObjClass.cc ObjClass.h
	c++ ${CFLAGS} $< -c -o $@ ${INCLUDE}
