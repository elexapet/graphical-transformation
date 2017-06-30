#Makefile
CCP		= 	g++
CFLAGS	=	-Wall -pedantic -std=c++11 -g
LIBS	=	-lX11
BIN		=	out/draw-app
SRC		=	src/main.cpp src/shape.cpp src/triangle.cpp src/matrix.cpp \
			src/image.cpp src/x11context.cpp src/mydrawing.cpp src/gcontext.cpp \
			src/viewcontext.cpp
OBJ		=	$(SRC:.cpp=.o)
HEAD 	=   

#to avoid a conflict with a file of the same name
.PHONY: all clean compile run

all: compile

compile: $(BIN)

run: $(BIN)
	./$(BIN)

clean: 
	rm -f $(BIN) $(OBJ)

#links objects to binary	
$(BIN) : $(OBJ)
	$(CCP) $(OBJ) -o $(BIN) $(LIBS)

#all dependencies from header files
$(OBJ) : $(HEAD)

#makes any xxx.o from xxx.cpp
%.o : %.cpp
	$(CCP) -c $(CFLAGS)  $< -o $@