CC      = $(CROSS_COMPILE)gcc

CFLAGS += -Wall -I../../ta/include -I$(TEEC_EXPORT)/include
LDADD += -lteec -L$(TEEC_EXPORT)/lib

LIBNAME = libfs
BINARY = $(LIBNAME).so.1 
OBJS = fs.o

all: $(BINARY)

$(BINARY): $(OBJS)
	@echo $@:$(OBJS)...
	$(CC) -shared -Wl,-soname,$@ -o $@ $< $(LDADD)
	ln -s $(BINARY) $(LIBNAME).so

fs.o : fs.c
	@echo $@...
	$(CC) -fPIC -c $^ $(CFLAGS)

clean:
	rm -f $(OBJS) $(BINARY) $(LIBNAME).*
