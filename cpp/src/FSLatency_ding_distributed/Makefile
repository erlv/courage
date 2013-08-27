CFLAGS = -D_REENTRANT -Wall 
LDFLAGS = -lpthread

CFLAGS += -g3 -O0
LDFLAGS += -g3 -O0

TARGETS = FSServer FSClient

all: $(TARGETS)


FSServer: FSServer.o threadpool.o
FSClient: FSClient.o threadpool.o
threadpool.o: threadpool.c threadpool.h

clean:
	rm -rf FSServer FSClient *~ *.o
