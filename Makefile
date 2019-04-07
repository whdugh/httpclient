COBJS:=$(patsubst %.c,%.o,$(wildcard source/*.c))
CPPOBJS:=$(patsubst %.cpp,%.o,$(wildcard source/*.cpp))
OBJS:=$(COBJS) $(CPPOBJS)
DOWNLOADEXE=Download.out

#CC = icpc
CPPFLAGS = g++
C11FLAGS = -std=c++11


all: $(DOWNLOADEXE)

INC = -I./lib/include
LIB = -L./lib/lib -lcurl 

$(DOWNLOADEXE): $(OBJS)
	$(CPPFLAGS) $(OBJS) -o $@  $(LIB) $(INC)

$(CPPOBJS): %.o : %.cpp 
	#-c $< -o $@ $(INC)
	$(CPPFLAGS) -c $< -o $@ $(INC) $(C11FLAGS)

clean:
		rm -rf $(OBJS) $(DOWNLOADEXE)
