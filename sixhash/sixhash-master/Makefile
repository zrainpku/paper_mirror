CPP = g++

TARGET = libhashsix.so

$(TARGET) : hashsix.cpp
	$(CPP) $(OBJS) hashsix.cpp  `pkg-config --cflags --libs opencv` -fpic -shared -o $(TARGET)

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
