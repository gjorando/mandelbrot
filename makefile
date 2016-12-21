LIBS=-lpng
CC=$(CROSS)g++ -c -Wall -std=c++11 $(DEFINES)
SRCS=src/main.cpp\
	 src/RGBPixel.cpp\
	 src/RGBBitmap.cpp\
	 src/Mandelbrot.cpp\
	 src/RGBGradient.cpp
OBJ=$(subst src/,output/, $(subst .cpp,.o, $(SRCS)))

all: createdirs mandelbrot
createdirs:
	@mkdir -p .d
	@mkdir -p output
mandelbrot: $(OBJ)
	g++ -o mandelbrot output/*.o $(LIBS)
output/%.o: src/%.cpp .d/%.d
	$(CC) $< -o $@
.d/%.d: src/%.cpp
	$(CC) -MM $< -MF $@
clean:
	rm -f output/*.o
	rm mandelbrot
re: clean mandelbrot
.PHONY: clean

-include $(OBJECTS:%.o=.d/%.d)
-include $(LOBJECTS:%.o=.d/%.d)
