NAME=Elysium

IDIR =../include
CC=g++ 
DEBUG_FLAG=-g
PROF_FLAG=-pg
CFLAGS= -std=c++17 $(DEBUG_FLAG) $(PROF_FLAG)  -I$(IDIR)

ODIR=obj
SUBDIR = Game Rendering Physics Carina

#Look in current directory for internal libraries
LIBFLAG=$(wildcard *.dll)

#Add the external libraries
LIBFLAG += -L/usr/lib
LIBFLAG += -lm -lCirce -lGLEW -lGLU -lGL -lSDL2 -lassimp

DEPS = $(wildcard *.h $(IDIR)/*.h)\
       $(wildcard *.h $(foreach fd, $(SUBDIR), $(IDIR)/$(fd)/*.h))

SRC=$(wildcard *.cpp $(foreach fd, $(SUBDIR), $(fd)/*.cpp))

_OBJ = $(SRC:.cpp=.o)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $< 

	#$(CC) -c -fpic -o $@ $< $(CFLAGS)
#1-Clean console
#2-Compile and link
#3-Run the exe
#	$(CC) -o $@ $^ $(CFLAGS) $(LIBFLAG)
$(NAME).exe: $(OBJ)
#	clear
	$(CC) -o $@ $^ $(LIBFLAG)
	./$(NAME).exe

#Clean and make
over:
	$(MAKE) clean
	$(MAKE)

#Creates a static library	
statlib: $(OBJ)
	ar rcs lib$(NAME).a $(OBJ)

#Creates a dynamic library
#It will create a file name "ProjectName".dll (no lib in front), which is internal (only works with this compiler)
#Copy/paste "ProjectName".dll into "DestinationProject"/lib/internal
#Call with g++ -o (...).exe (...).o "ProjectName".dll ...
dynlib: $(OBJ)
	$(CC) -shared -o $(NAME).dll $^ $(CFLAGS) $(LIBFLAG)

#make NAME=... newproject
newproject:
	@echo off
	mkdir $(NAME)
	mkdir $(NAME)/include
	mkdir $(NAME)/lib
	mkdir $(NAME)/src
	mkdir $(NAME)/src/obj
	echo NAME=$(NAME)>temp
	type makefile>>temp
	move temp $(NAME)/src
	ren $(NAME)/src/temp makefile

#Create a new class (h & cpp files) make CLASS=... newfile
newclass:
	@echo off
	@echo #pragma once> $(CLASS).h
	@echo.>> $(CLASS).h
	@echo class $(CLASS)>> $(CLASS).h
	@echo {>> $(CLASS).h
	@echo public:>> $(CLASS).h
	@echo 	$(CLASS)();>> $(CLASS).h
	@echo.>> $(CLASS).h
	@echo private:>> $(CLASS).h
	@echo };>> $(CLASS).h
	move $(CLASS).h ../include
	@echo #include ^"$(CLASS).h^"> $(CLASS).cpp
	@echo.>> $(CLASS).cpp
	@echo $(CLASS)::$(CLASS)()>> $(CLASS).cpp	
	@echo {>> $(CLASS).cpp
	@echo }>> $(CLASS).cpp
	
#make DEST=... install
install:
	@echo off
	rm $(NAME).dll
	$(MAKE) dynlib
	if not exist ../../$(DEST)/include/$(NAME) md ../../$(DEST)/include/$(NAME)
	cp ../include/*.h ../../$(DEST)/include/$(NAME)
	cp $(NAME).dll ../../$(DEST)/src
	
#Push files to github (once all set up)
#make COMMIT=... git
git:
	@echo off
	cd .. && \
	git add include/*.h && \
	git add src/*.cpp && \
	git commit -m $(COMMIT) && \
	git push origin master

.PHONY: clean

#delete all the Object files and the executable.
clean:
	clear
	for fd in $(SUBDIR); do \
		rm $(ODIR)/$$fd/*.o; \
	done
	rm $(ODIR)/*.o *.exe

#Print all the files that will be compiled
echoes:
	@echo "Include files: $(DEPS)"
	@echo "Source files: $(SRC)"

#Create a new subdirectory. Remember to add it to "subdir" variable!
#make DIR=Rendering directory
directory:
	mkdir $(IDIR)/$(DIR)
	mkdir $(DIR)
	mkdir $(ODIR)/$(DIR)

#http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
#http://nuclear.mutantstargoat.com/articles/make/
