CC      = $(CROSS_COMPILE)gcc

CFLAGS += -Wall -I../../ta/include -I$(TEEC_EXPORT)/include
LDADD += -lteec -L$(TEEC_EXPORT)/TALL$(TEE_TA_LOG_LEVEL)/lib

LIBNAME = libsess
BINARY = $(LIBNAME).so.1 
SRCS = session.c
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
