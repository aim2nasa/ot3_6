CC      = $(CROSS_COMPILE)gcc

OBJS = main.o

CFLAGS += -Wall -I../../../ta/include -I$(TEEC_EXPORT)/include
CFLAGS += -I$(TA_DEV_KIT_DIR)/host_include
CFLAGS += -I../../libfs
LDADD += -lteec -L$(TEEC_EXPORT)/lib
LDADD += -lfs -L../../libfs

BINARY = create

all: $(BINARY)

$(BINARY): $(OBJS)
	$(CC) -o $@ $< $(LDADD)

clean:
	rm -f $(OBJS) $(BINARY)
