.PHONY: all
all: filheader.so send_stuff.o
swap_bytes.o:swap_bytes.c
	gcc -shared -fPIC -c swap_bytes.c -lpython2.7
send_stuff.o:send_stuff.c swap_bytes.o
	gcc -shared -c -fPIC send_stuff.c swap_bytes.o -lpython2.7
filheader.so:filheader.c send_stuff.o
	gcc -shared -I/usr/include/python2.7 -o filheader.so -fPIC filheader.c send_stuff.o swap_bytes.o  -lpython2.7
