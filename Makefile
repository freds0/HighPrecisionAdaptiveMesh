#------------------------------------------------------------------------------#
#  Make file of Project of Adaptive Mesh.
#  \author Frederico S. Oliveira.
#  \copyright GNU Public License.
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
# CFLAGS =  -Wall -I/usr/include/eigen3 -I./External/mpfr_real_v0.0.9-alpha
CFLAGS =  -Wall -I./External/mpfr_real_v0.0.9-alpha
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -lGLU -lGL -lglut -lm -lmpfr -lpthread

INC_DEBUG =  $(INC)
CFLAGS_DEBUG =  $(CFLAGS) -g -O0
RESINC_DEBUG =  $(RESINC)
RCFLAGS_DEBUG =  $(RCFLAGS)
LIBDIR_DEBUG =  $(LIBDIR)
LIB_DEBUG = $(LIB)
LDFLAGS_DEBUG =  $(LDFLAGS) -pg
OBJDIR_DEBUG = obj/Debug
DEP_DEBUG = 
OUT_DEBUG = bin/Debug/AdaptiveMesh

INC_RELEASE =  $(INC)
CFLAGS_RELEASE =  $(CFLAGS) -O3
RESINC_RELEASE =  $(RESINC)
RCFLAGS_RELEASE =  $(RCFLAGS)
LIBDIR_RELEASE =  $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE =  $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = bin/Release/AdaptiveMesh

OBJ_DEBUG = $(OBJDIR_DEBUG)/OpenGL/openGL.o  $(OBJDIR_DEBUG)/main.o $(OBJDIR_DEBUG)/Vertex/vertex.o $(OBJDIR_DEBUG)/Utils/utils.o $(OBJDIR_DEBUG)/Triangle/triangle.o $(OBJDIR_DEBUG)/Adjacency/adjacency.o $(OBJDIR_DEBUG)/Mesh/mesh.o $(OBJDIR_DEBUG)/FiniteVolumeMethod/FiniteVolume.o $(OBJDIR_DEBUG)/ConjugateGradient/ConjugateGradient.o

OBJ_RELEASE = $(OBJDIR_RELEASE)/OpenGL/openGL.o  $(OBJDIR_RELEASE)/main.o $(OBJDIR_RELEASE)/Vertex/vertex.o $(OBJDIR_RELEASE)/Utils/utils.o $(OBJDIR_RELEASE)/Triangle/triangle.o $(OBJDIR_RELEASE)/Adjacency/adjacency.o $(OBJDIR_RELEASE)/Mesh/mesh.o $(OBJDIR_RELEASE)/FiniteVolumeMethod/FiniteVolume.o $(OBJDIR_RELEASE)/ConjugateGradient/ConjugateGradient.o

all: debug release

clean: clean_debug clean_release

before_debug: 
	test -d bin/Debug || mkdir -p bin/Debug
	test -d $(OBJDIR_DEBUG)/OpenGL || mkdir -p $(OBJDIR_DEBUG)/OpenGL
	test -d $(OBJDIR_DEBUG) || mkdir -p $(OBJDIR_DEBUG)
	test -d $(OBJDIR_DEBUG)/Vertex || mkdir -p $(OBJDIR_DEBUG)/Vertex
	test -d $(OBJDIR_DEBUG)/Utils || mkdir -p $(OBJDIR_DEBUG)/Utils
	test -d $(OBJDIR_DEBUG)/Triangle || mkdir -p $(OBJDIR_DEBUG)/Triangle
	test -d $(OBJDIR_DEBUG)/Adjacency || mkdir -p $(OBJDIR_DEBUG)/Adjacency
	test -d $(OBJDIR_DEBUG)/Mesh || mkdir -p $(OBJDIR_DEBUG)/Mesh
	test -d $(OBJDIR_DEBUG)/FiniteVolumeMethod || mkdir -p $(OBJDIR_DEBUG)/FiniteVolumeMethod
	test -d $(OBJDIR_DEBUG)/ConjugateGradient || mkdir -p $(OBJDIR_DEBUG)/ConjugateGradient

after_debug: 

debug: before_debug out_debug after_debug

out_debug: $(OBJ_DEBUG) $(DEP_DEBUG)
	$(LD) $(LIBDIR_DEBUG) $(OBJ_DEBUG) $(LIB_DEBUG) -o $(OUT_DEBUG) $(LDFLAGS_DEBUG)

$(OBJDIR_DEBUG)/OpenGL/openGL.o: OpenGL/openGL.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c OpenGL/openGL.cpp -o $(OBJDIR_DEBUG)/OpenGL/openGL.o
	
$(OBJDIR_DEBUG)/main.o: main.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c main.cpp -o $(OBJDIR_DEBUG)/main.o

$(OBJDIR_DEBUG)/Vertex/vertex.o: Vertex/vertex.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c Vertex/vertex.cpp -o $(OBJDIR_DEBUG)/Vertex/vertex.o

$(OBJDIR_DEBUG)/Utils/utils.o: Utils/utils.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c Utils/utils.cpp -o $(OBJDIR_DEBUG)/Utils/utils.o

$(OBJDIR_DEBUG)/Triangle/triangle.o: Triangle/triangle.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c Triangle/triangle.cpp -o $(OBJDIR_DEBUG)/Triangle/triangle.o
	
$(OBJDIR_DEBUG)/Adjacency/adjacency.o: Adjacency/adjacency.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c Adjacency/adjacency.cpp -o $(OBJDIR_DEBUG)/Adjacency/adjacency.o

$(OBJDIR_DEBUG)/Mesh/mesh.o: Mesh/mesh.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c Mesh/mesh.cpp -o $(OBJDIR_DEBUG)/Mesh/mesh.o

$(OBJDIR_DEBUG)/FiniteVolumeMethod/FiniteVolume.o: FiniteVolumeMethod/FiniteVolume.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c FiniteVolumeMethod/FiniteVolume.cpp -o $(OBJDIR_DEBUG)/FiniteVolumeMethod/FiniteVolume.o

$(OBJDIR_DEBUG)/ConjugateGradient/ConjugateGradient.o: ConjugateGradient/ConjugateGradient.cpp
	$(CXX) $(CFLAGS_DEBUG) $(INC_DEBUG) -c ConjugateGradient/ConjugateGradient.cpp -o $(OBJDIR_DEBUG)/ConjugateGradient/ConjugateGradient.o

clean_debug: 
	rm -f $(OBJ_DEBUG) $(OUT_DEBUG)
	rm -rf bin/Debug
	rm -rf $(OBJDIR_DEBUG)/OpenGL
	rm -rf $(OBJDIR_DEBUG)
	rm -rf $(OBJDIR_DEBUG)/Vertex
	rm -rf $(OBJDIR_DEBUG)/Utils
	rm -rf $(OBJDIR_DEBUG)/Triangle
	rm -rf $(OBJDIR_DEBUG)/Adjacency
	rm -rf $(OBJDIR_DEBUG)/Mesh
	rm -rf $(OBJDIR_DEBUG)/FiniteVolumeMethod
	rm -rf $(OBJDIR_DEBUG)/ConjugateGradient
	rm -rf output/*

before_release: 
	test -d bin/Release || mkdir -p bin/Release
	test -d $(OBJDIR_RELEASE)/OpenGL || mkdir -p $(OBJDIR_RELEASE)/OpenGL
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)
	test -d $(OBJDIR_RELEASE)/Vertex || mkdir -p $(OBJDIR_RELEASE)/Vertex
	test -d $(OBJDIR_RELEASE)/Utils || mkdir -p $(OBJDIR_RELEASE)/Utils
	test -d $(OBJDIR_RELEASE)/Triangle || mkdir -p $(OBJDIR_RELEASE)/Triangle
	test -d $(OBJDIR_RELEASE)/Adjacency || mkdir -p $(OBJDIR_RELEASE)/Adjacency
	test -d $(OBJDIR_RELEASE)/Mesh || mkdir -p $(OBJDIR_RELEASE)/Mesh
	test -d $(OBJDIR_RELEASE)/FiniteVolumeMethod || mkdir -p $(OBJDIR_RELEASE)/FiniteVolumeMethod
	test -d $(OBJDIR_RELEASE)/ConjugateGradient || mkdir -p $(OBJDIR_RELEASE)/ConjugateGradient

after_release: 

release: before_release out_release after_release

out_release: $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) $(OBJ_RELEASE) $(LIB_RELEASE) -o $(OUT_RELEASE) $(LDFLAGS_RELEASE)

$(OBJDIR_RELEASE)/OpenGL/openGL.o: OpenGL/openGL.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c OpenGL/openGL.cpp -o $(OBJDIR_RELEASE)/OpenGL/openGL.o
	
$(OBJDIR_RELEASE)/main.o: main.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.cpp -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/Vertex/vertex.o: Vertex/vertex.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c Vertex/vertex.cpp -o $(OBJDIR_RELEASE)/Vertex/vertex.o

$(OBJDIR_RELEASE)/Utils/utils.o: Utils/utils.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c Utils/utils.cpp -o $(OBJDIR_RELEASE)/Utils/utils.o

$(OBJDIR_RELEASE)/Triangle/triangle.o: Triangle/triangle.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c Triangle/triangle.cpp -o $(OBJDIR_RELEASE)/Triangle/triangle.o

$(OBJDIR_RELEASE)/Adjacency/adjacency.o: Adjacency/adjacency.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c Adjacency/adjacency.cpp -o $(OBJDIR_RELEASE)/Adjacency/adjacency.o

$(OBJDIR_RELEASE)/Mesh/mesh.o: Mesh/mesh.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c Mesh/mesh.cpp -o $(OBJDIR_RELEASE)/Mesh/mesh.o

$(OBJDIR_RELEASE)/FiniteVolumeMethod/FiniteVolume.o: FiniteVolumeMethod/FiniteVolume.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c FiniteVolumeMethod/FiniteVolume.cpp -o $(OBJDIR_RELEASE)/FiniteVolumeMethod/FiniteVolume.o

$(OBJDIR_RELEASE)/ConjugateGradient/ConjugateGradient.o: ConjugateGradient/ConjugateGradient.cpp
	$(CXX) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ConjugateGradient/ConjugateGradient.cpp -o $(OBJDIR_RELEASE)/ConjugateGradient/ConjugateGradient.o
	
clean_release: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf bin/Release
	rm -rf $(OBJDIR_RELEASE)/OpenGL
	rm -rf $(OBJDIR_RELEASE)
	rm -rf $(OBJDIR_RELEASE)/Vertex
	rm -rf $(OBJDIR_RELEASE)/Utils
	rm -rf $(OBJDIR_RELEASE)/Triangle
	rm -rf $(OBJDIR_RELEASE)/Adjacency
	rm -rf $(OBJDIR_RELEASE)/Mesh
	rm -rf $(OBJDIR_RELEASE)/FiniteVolumeMethod
	rm -rf $(OBJDIR_RELEASE)/ConjugateGradient
	rm -rf output/*

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

