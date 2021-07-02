delgroup:client.o server.o
				gcc -o delgroup server.o client.o
client.o:server.c interface.h
				gcc -c client.c interface.h
server.o:interface.h server.c
				gcc -c interface.h server.c
