all: myhttpd 

myhttpd: myhttpd.o
	gcc myhttpd.o -lpthread -lrt -o myhttpd

myhttpd.o: myhttpd.c
	gcc -c myhttpd.c -lpthread -lrt

clean :
	rm -rf *o myhttpd
