CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lstdc++ -lphp5 -lreadline 
#LDFLAGS = -lstdc++ -L/stone/local/php/include/php/libs  
phps:main.c
	$(CC) -o phps main.c $(CFLAGS) $(LDFLAGS)
install:
	cp phps /home/stone/bin/
