CXX = $(CROSS_COMPILE)g++
CXXFLAGS = -std=c++11

ifeq ($(shell echo $(TEST_DEBUG)|tr [a-z] [A-Z]),YES)
	CXXFLAGS += -D DEBUG 
endif
INCLUDES = -I../../googletest/googletest/include
INCLUDES += -I../libsess -I$(TEEC_EXPORT)/include
INCLUDES += -I../../ta/crypt/include \
			-I../libcrypt \
			-I../../ta/key/include \
			-I../libkey \
			-I$(TA_DEV_KIT_DIR)/host_include

TARGET = runTests
SRCS = main.cpp base.cpp crypt.cpp key.cpp aes-block.cpp aes-ae-ccm.cpp aes-ae-gcm.cpp
OBJS = ${SRCS:.cpp=.o}

GTEST_LIBRARIES = -L../../googletest/googlemock/gtest -lgtest -lpthread
LIB_SESS = -L../libsess -lsess
LIB_CRYPT = -L../libcrypt -lcrypt
LIB_TEEC = -lteec -L$(TEEC_EXPORT)/lib
LIB_KEY = -L../libkey -lkey
LIBS = $(GTEST_LIBRARIES) $(LIB_SESS) $(LIB_TEEC) $(LIB_CRYPT) $(LIB_KEY)

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
