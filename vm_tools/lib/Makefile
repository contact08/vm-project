CC = g++
LIB = ar 
CPPFLAGS = -fPIC `pkg-config --cflags opencv4` -I../inc/
LDFLAGS = `pkg-config --libs opencv4` -L./ -lISCSDKLibvm200 -lISCLibvm -lISCUSBLibvm -lftdi1

all: libbasic.a

OBJS = libbasic.o
SRCS = $(OBJS:.o=.cpp)

libbasic.a:  $(OBJS)
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

libbasic.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
libbasic.o: /usr/include/unistd.h /usr/include/features.h
libbasic.o: /usr/include/stdc-predef.h /usr/include/linux/types.h
libbasic.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
