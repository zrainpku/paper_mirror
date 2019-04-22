CPP = g++

TARGET = libhash.so

$(TARGET) : mytools.cpp frangi.cpp calhash.cpp
	$(CPP) $(OBJS) calhash.cpp mytools.cpp frangi.cpp  `pkg-config --cflags --libs opencv` -fpic -shared -o $(TARGET)

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
