CC = g++
CFLAGS = -c -Wall
LDFLAGS =
 
# List of sources:
SOURCES = main.cpp libbasic.cpp
OBJECTS = $(SOURCES:.cpp=.o)
 
# Name of executable target:
EXECUTABLE = main
 
# Here we invoke "pkg-config" passing it as argument the list of the
# MRPT libraries needed by our program (see available libs
# with "pkg-config --list-all | grep mrpt").
#
CFLAGS += `pkg-config --cflags opencv4` -I../../inc/
LDFLAGS += `pkg-config --libs opencv4` -L ../../lib
LDFLAGS += -lpthread -lISCSDKLibvm200 -lISCLibvm -lISCUSBLibvm -lftdi1
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

main.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
main.o: /usr/include/unistd.h /usr/include/features.h
main.o: /usr/include/stdc-predef.h /usr/include/linux/types.h
main.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
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
main.o: ../../inc/ISCSDKLib.h libbasic.h
libbasic.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/string.h
libbasic.o: /usr/include/unistd.h /usr/include/features.h
libbasic.o: /usr/include/stdc-predef.h /usr/include/linux/types.h
libbasic.o: /usr/include/linux/posix_types.h /usr/include/linux/stddef.h
libbasic.o: /usr/local/include/opencv4/opencv2/core/core.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/cvdef.h
libbasic.o: /usr/local/include/opencv4/opencv2/core/version.hpp
libbasic.o: /usr/include/limits.h
libbasic.o: /usr/local/include/opencv4/opencv2/core/hal/interface.h
libbasic.o: /usr/include/stdint.h
libbasic.o: /usr/local/include/opencv4/opencv2/core/cv_cpu_dispatch.h
libbasic.o: /usr/local/include/opencv4/opencv2/core/fast_math.hpp
libbasic.o: /usr/include/math.h
libbasic.o: /usr/local/include/opencv4/opencv2/core/base.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/opencv_modules.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/cvstd.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/cvstd_wrapper.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/neon_utils.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/vsx_utils.hpp
libbasic.o: /usr/include/assert.h
libbasic.o: /usr/local/include/opencv4/opencv2/core/check.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/traits.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/matx.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/saturate.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/types.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/mat.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/bufferpool.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/mat.inl.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/persistence.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/operations.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/cvstd.inl.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/utility.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/optim.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/ovx.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/core/cvdef.h
libbasic.o: /usr/local/include/opencv4/opencv2/highgui/highgui.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/highgui.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/imgcodecs.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/videoio.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/imgproc.hpp
libbasic.o: /usr/local/include/opencv4/opencv2/./imgproc/segmentation.hpp
libbasic.o: ../../inc/ISCSDKLib.h
