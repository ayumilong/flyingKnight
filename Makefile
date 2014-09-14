CFLAGS =`sdl-config --cflags` -g -W -Wall -Weffc++ -Wextra -pedantic -O0
VFLAGS = --leak-check=full --show-reachable=yes
#SUPP=--gen-suppressions=all --suppressions=yaolin_sdl.supp --log-file=val.log 
SUPP = --suppressions=yaolin_sdl.supp
LDFLAGS = `sdl-config --libs` -lSDL_ttf -lSDL_image -lexpat -lSDL_gfx -lSDL_mixer

SOURCES = $(shell find *.cpp) #All the cpp files of the current directory
#OBJS = $(SOURCES: .cpp = .o) #All the object files that should be generated, they have the same name with the corresponding cpp files, this one should work, I don't know the reason
OBJS = $(addsuffix .o, $(basename $(shell find *.cpp)))
EXEC = run


.PHONY: clang gcc setclang setgcc clean r vg

gcc: | setgcc $(EXEC)
clang: | setclang $(EXEC)

all : $(SOURCES) $(EXEC)
#Here must use $^, it represents all the OBJS but $< just represent the first one of OBJS
$(EXEC): $(OBJS)
	$(CXX) $(CFLAGS)  -o $@ $^ $(LDFLAGS)

#Here we can use ethier the $^ or $<, because for "%.**" the two behave the same
#$(OBJS): %.o : %.cpp  #This two sentences behave the same
.cpp.o:
	$(CXX) $(CFLAGS)  -c $^ -o $@ 

setclang:
	@echo "Setting clang"
	$(eval CXX = clang++)
	$(eval CXX_LINK = clang++)

setgcc:
	@echo "Setting gcc"
	$(eval CXX = g++)
	$(eval CXX_LINK = g++)

clean:
	rm -f $(EXEC) $(OBJS)

r:
	@./$(EXEC)

vg:
	@valgrind $(SUPP) $(VFLAGS) ./$(EXEC)
