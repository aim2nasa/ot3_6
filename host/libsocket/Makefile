CC      = $(CROSS_COMPILE)gcc

CFLAGS += -Wall -I../../ta/socket/include -I$(TEEC_EXPORT)/include
CFLAGS += -I../libsess
CFLAGS += -I$(TA_DEV_KIT_DIR)/host_include
LDADD += -lteec -L$(TEEC_EXPORT)/lib

LIBNAME = libsocket
BINARY = $(LIBNAME).so.1 
SRCS = socket.c
OBJS = ${SRCS:.c=.o} 

all: $(BINARY)

$(BINARY): $(OBJS)
	@echo $@:$(OBJS)...
	$(CC) -shared -Wl,-soname,$@ -o $@ $< $(LDADD)
	ln -s $(BINARY) $(LIBNAME).so

%.o : %.c
	@echo $@...
	$(CC) -fPIC -c $^ $(CFLAGS)

clean:
	rm -f $(OBJS) $(BINARY) $(LIBNAME).*
