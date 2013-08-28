CFLAGS =  -Wall 
LDFLAGS = -lpthread

CFLAGS += -g3 -O0
LDFLAGS += -g3 -O0

TARGETS = FSServer FSClient

all: $(TARGETS)


FSServer: FSServer.o FSCommon.o
FSClient: FSClient.o FSCommon.o
FSCommon.o: FSCommon.c FSCommon.h

clean:
	rm -rf FSServer FSClient *~ *.o
