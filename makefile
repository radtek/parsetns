main:main.o tns_parse.o
	gcc main.o -o main

main.o:main.c test_buffer.h
	gcc -c main.c -o main.o

tns_parse.o:tns_parse.c tns_parse.h
	gcc -c tns_parse.c -o tns_parse.o
	
clean:
	rm -rf *.o main