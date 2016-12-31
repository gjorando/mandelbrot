LIBS=-lpng -lm -lstdc++
CFLAGS=-c -Wall -std=c++11
CC=$(CROSS)g++

SRCS=src/main.cpp\
	 src/RGBPixel.cpp\
	 src/RGBBitmap.cpp\
	 src/Buddhabrot.cpp\
	 src/RGBGradient.cpp
OBJ=$(subst src/,output/, $(subst .cpp,.o, $(SRCS)))

all: createdirs buddhabrot

createdirs:
	@mkdir -p .d
	@mkdir -p output

buddhabrot: $(OBJ)
	$(CC) -o buddhabrot output/*.o $(LIBS)

output/%.o: src/%.cpp .d/%.d
	$(CC) $(CFLAGS) $< -o $@

.d/%.d: src/%.cpp
	$(CC) $(CFLAGS) -MM $< -MF $@

clean:
	rm -f output/*.o
	rm -f buddhabrot

re: clean mandelbrot

install:
	cp ./mandelbrot /usr/bin/buddhabrot
remove:
	rm -f /usr/bin/buddhabrot

.PHONY: clean

-include $(OBJECTS:%.o=.d/%.d)
-include $(LOBJECTS:%.o=.d/%.d)
