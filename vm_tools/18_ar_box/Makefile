CC = g++
CFLAGS = -c -Wall
LDFLAGS =
LIBS = ../lib/libx3.a ../lib/libbasic.a

# List of sources:
SOURCES = main.cpp cmdproc.cpp commands.cpp
OBJECTS = $(SOURCES:.cpp=.o)
 
# Name of executable target:
EXECUTABLE = main
 
# Here we invoke "pkg-config" passing it as argument the list of the
# MRPT libraries needed by our program (see available libs
# with "pkg-config --list-all | grep mrpt").
#
CFLAGS += `pkg-config --cflags opencv4` -I../inc/
LDFLAGS += `pkg-config --libs opencv4` -L ../lib
LDFLAGS += -lpthread -lISCSDKLibvm200 -lISCLibvm -lISCUSBLibvm -lbasic -lx3 -lftdi1
all: $(SOURCES) $(EXECUTABLE)
 
$(EXECUTABLE): $(OBJECTS) $(LIBS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
 
clean:
	rm -f $(OBJECTS) *.bak
depend:
	makedepend -- $(CFLAGS) -- $(SOURCES)

# DO NOT DELETE

main.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
main.o: /usr/include/unistd.h /usr/include/features.h
main.o: /usr/include/stdc-predef.h
main.o: /usr/local/include/opencv4/opencv2/core/core.hpp
main.o: /usr/local/include/opencv4/opencv2/core.hpp
main.o: /usr/local/include/opencv4/opencv2/core/cvdef.h
main.o: /usr/local/include/opencv4/opencv2/core/version.hpp
main.o: /usr/include/limits.h
main.o: /usr/local/include/opencv4/opencv2/core/hal/interface.h
main.o: /usr/include/stdint.h
main.o: /usr/local/include/opencv4/opencv2/core/cv_cpu_dispatch.h
main.o: /usr/local/include/opencv4/opencv2/core/fast_math.hpp
main.o: /usr/include/math.h /usr/local/include/opencv4/opencv2/core/base.hpp
main.o: /usr/local/include/opencv4/opencv2/opencv_modules.hpp
main.o: /usr/local/include/opencv4/opencv2/core/cvstd.hpp
main.o: /usr/local/include/opencv4/opencv2/core/cvstd_wrapper.hpp
main.o: /usr/local/include/opencv4/opencv2/core/neon_utils.hpp
main.o: /usr/local/include/opencv4/opencv2/core/vsx_utils.hpp
main.o: /usr/include/assert.h
main.o: /usr/local/include/opencv4/opencv2/core/check.hpp
main.o: /usr/local/include/opencv4/opencv2/core/traits.hpp
main.o: /usr/local/include/opencv4/opencv2/core/matx.hpp
main.o: /usr/local/include/opencv4/opencv2/core/saturate.hpp
main.o: /usr/local/include/opencv4/opencv2/core/types.hpp
main.o: /usr/local/include/opencv4/opencv2/core/mat.hpp
main.o: /usr/local/include/opencv4/opencv2/core/bufferpool.hpp
main.o: /usr/local/include/opencv4/opencv2/core/mat.inl.hpp
main.o: /usr/local/include/opencv4/opencv2/core/persistence.hpp
main.o: /usr/local/include/opencv4/opencv2/core/operations.hpp
main.o: /usr/local/include/opencv4/opencv2/core/cvstd.inl.hpp
main.o: /usr/local/include/opencv4/opencv2/core/utility.hpp
main.o: /usr/local/include/opencv4/opencv2/core/optim.hpp
main.o: /usr/local/include/opencv4/opencv2/core/ovx.hpp
main.o: /usr/local/include/opencv4/opencv2/core/cvdef.h
main.o: /usr/local/include/opencv4/opencv2/highgui/highgui.hpp
main.o: /usr/local/include/opencv4/opencv2/highgui.hpp
main.o: /usr/local/include/opencv4/opencv2/imgcodecs.hpp
main.o: /usr/local/include/opencv4/opencv2/videoio.hpp
main.o: /usr/local/include/opencv4/opencv2/imgproc.hpp
main.o: /usr/local/include/opencv4/opencv2/./imgproc/segmentation.hpp
main.o: ../inc/ISCSDKLib.h ../inc/libbasic.h /usr/include/linux/types.h
main.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
main.o: ../inc/libx3.h commands.h
cmdproc.o: /usr/include/stdio.h /usr/include/string.h commands.h cmdtbl.h
commands.o: ../inc/libbasic.h /usr/include/stdio.h /usr/include/stdlib.h
commands.o: /usr/include/string.h /usr/include/unistd.h
commands.o: /usr/include/features.h /usr/include/stdc-predef.h
commands.o: /usr/include/linux/types.h /usr/include/linux/posix_types.h
commands.o: /usr/include/linux/stddef.h
commands.o: /usr/local/include/opencv4/opencv2/core/core.hpp
commands.o: /usr/local/include/opencv4/opencv2/core.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/cvdef.h
commands.o: /usr/local/include/opencv4/opencv2/core/version.hpp
commands.o: /usr/include/limits.h
commands.o: /usr/local/include/opencv4/opencv2/core/hal/interface.h
commands.o: /usr/include/stdint.h
commands.o: /usr/local/include/opencv4/opencv2/core/cv_cpu_dispatch.h
commands.o: /usr/local/include/opencv4/opencv2/core/fast_math.hpp
commands.o: /usr/include/math.h
commands.o: /usr/local/include/opencv4/opencv2/core/base.hpp
commands.o: /usr/local/include/opencv4/opencv2/opencv_modules.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/cvstd.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/cvstd_wrapper.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/neon_utils.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/vsx_utils.hpp
commands.o: /usr/include/assert.h
commands.o: /usr/local/include/opencv4/opencv2/core/check.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/traits.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/matx.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/saturate.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/types.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/mat.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/bufferpool.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/mat.inl.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/persistence.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/operations.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/cvstd.inl.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/utility.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/optim.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/ovx.hpp
commands.o: /usr/local/include/opencv4/opencv2/core/cvdef.h
commands.o: /usr/local/include/opencv4/opencv2/highgui/highgui.hpp
commands.o: /usr/local/include/opencv4/opencv2/highgui.hpp
commands.o: /usr/local/include/opencv4/opencv2/imgcodecs.hpp
commands.o: /usr/local/include/opencv4/opencv2/videoio.hpp
commands.o: /usr/local/include/opencv4/opencv2/imgproc.hpp
commands.o: /usr/local/include/opencv4/opencv2/./imgproc/segmentation.hpp
commands.o: ../inc/ISCSDKLib.h
