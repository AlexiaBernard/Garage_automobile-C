CC = gcc -Wall 

all : main client chef_atelier mecanicien

client : client.c types.h
	$(CC) client.c -o client

chef_atelier : chef_atelier.c types.h
	$(CC) chef_atelier.c -o chef_atelier

mecanicien : mecanicien.c types.h
	$(CC) mecanicien.c -o mecanicien

main : main.c types.h
	$(CC) main.c -o main
	touch cle.chefs

clean :
	rm -f main chef_atelier client mecanicien