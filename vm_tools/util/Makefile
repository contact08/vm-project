CC = gcc
CFLAGS = -c -Wall
LDFLAGS =
 
# List of sources:
SOURCES = gencmdp.c csv.c grep.c util.o
OBJECTS = $(SOURCES:.c=.o)
 
# Name of executable target:
EXECUTABLE = gencmdp
 
CFLAGS += -I./
#LDFLAGS += `pkg-config --libs opencv4` -L ../lib
#LDFLAGS += -lpthread -lISCSDKLibvm200 -lISCLibvm -lISCUSBLibvm -lbasic -lftdi1
all: $(SOURCES) $(EXECUTABLE)
 
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
 
clean:
	rm -f $(OBJECTS) *.bak
depend:
	makedepend -- $(CFLAGS) -- $(SOURCES)

# DO NOT DELETE

gencmdp.o: /usr/include/stdio.h /usr/include/stdlib.h csv.h grep.h util.h
csv.o: /usr/include/stdio.h /usr/include/string.h /usr/include/stdlib.h
csv.o: /usr/include/assert.h /usr/include/features.h
csv.o: /usr/include/stdc-predef.h csv.h
grep.o: /usr/include/stdio.h /usr/include/string.h /usr/include/stdlib.h
grep.o: grep.h
