CC = gcc
CFLAGS = -I/home/stone/source/serv/php-5.4.0/ \
               -I/home/stone/source/serv/php-5.4.0/main \
              -I/home/stone/source/serv/php-5.4.0/Zend \
              -I/home/stone/source/serv/php-5.4.0/TSRM \
              -Wall -g
LDFLAGS = -lstdc++ -L/home/stone/source/serv/php-5.4.0/libs -lphp5 -lreadline 
#LDFLAGS = -lstdc++ -L/stone/local/php/include/php/libs  
phps:php.o main.c
	$(CC) -o phps main.c php.o $(CFLAGS) $(LDFLAGS)
php.o:php.c
	$(CC) -o php.o -c php.c $(CFLAGS) $(LDFLAGS)
clean:
	rm *.o 
install:
	cp phps /home/stone/bin/
