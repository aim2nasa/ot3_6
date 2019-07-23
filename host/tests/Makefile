CXX = $(CROSS_COMPILE)g++
CXXFLAGS = -std=c++11

ifeq ($(shell echo $(TEST_DEBUG)|tr [a-z] [A-Z]),YES)
	CXXFLAGS += -D DEBUG 
endif
INCLUDES = -I../../googletest/googletest/include

TARGET = runTests
SRCS = main.cpp base.cpp
OBJS = ${SRCS:.cpp=.o}

GTEST_LIBRARIES = -L../../googletest/googlemock/gtest -lgtest -lpthread
LIBS = $(GTEST_LIBRARIES)

all : $(TARGET)
	@echo $@:$(TARGET)...

$(TARGET) : $(OBJS)
	@echo $@:$^...
	$(CXX) -o $@ $^ $(LIBS)

%.o : %.cpp
	@echo $@:$<...
	$(CXX) -c $< $(CXXFLAGS) $(INCLUDES)

clean:
	@echo $@...
	rm -rf *.o $(TARGET)
