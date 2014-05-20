OBJS = tnetstring.o tnetstring_tests.o

TARGET = tnetstring_tests

CXXFLAGS = -std=c++11 -Wall -g -fmessage-length=0 

$(TARGET): $(OBJS) 
	$(CXX) -o $(TARGET) $(OBJS) 

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

