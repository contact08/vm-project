CC = g++
CFLAGS = -c -Wall
LDFLAGS =
 
# List of sources:
SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
 
# Name of executable target:
EXECUTABLE = main
 
# Here we invoke "pkg-config" passing it as argument the list of the
# MRPT libraries needed by our program (see available libs
# with "pkg-config --list-all | grep mrpt").
#
CFLAGS += `pkg-config --cflags opencv4` -I../inc/
LDFLAGS += `pkg-config --libs opencv4` -L../lib
LDFLAGS += -lpthread -lISCSDKLibvm200 -lISCLibvm -lISCUSBLibvm -lftdi1
LDFLAGS += -lbasic -lx3
all: $(SOURCES) $(EXECUTABLE)
 
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

main: main.o

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
 
clean:
	rm -f $(OBJECTS) *.bak
depend:
	makedepend -- $(CFLAGS) -- $(SOURCES)

# DO NOT DELETE
