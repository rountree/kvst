CPPFLAGS=-I../install/include
LDFLAGS=-L../install/lib
CFLAGS=-Wall -std=c11

all: kvst.so

kvst.so: kvst.c
	$(CC) $(CPPFLAGS) $(LDFLAGS) $(CFLAGS) -shared -D BUILD_MODULE -fPIC -o kvst.so kvst.c -lflux-core

clean:
	rm kvst kvst.so
