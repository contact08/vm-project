CC = g++
LIB = ar 
CPPFLAGS = -fPIC `pkg-config --cflags opencv4` -I../inc/
LDFLAGS = `pkg-config --libs opencv4` -L./ -lISCSDKLibvm200 -lISCLibvm -lISCUSBLibvm -lftdi1

all: libbasic.a libx3.a

#OBJS = libbasic.o
#SRCS = $(OBJS:.o=.cpp)

libbasic.a: libbasic.o 
	$(LIB) rcs $@ $^

libx3.a:  libx3.o
	$(LIB) rcs $@ $^

%.o: %.c
	$(CC) $(CPPFLAG) $(INC) -c $<

%.o: %.cpp
	$(CC) $(CPPFLAGS) $(INC) -c $< 

clean:
	rm -f *.o *.*~ *~ *.un~

depend:
	makedepend -- -I.\ -- $(SRCS)

# DO NOT DELETE
