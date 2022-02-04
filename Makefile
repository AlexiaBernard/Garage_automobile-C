CC = gcc -Wall 

all : main client chef_atelier mecanicien

client : client.c types.h
	$(CC) -o client client.c 

chef_atelier : chef_atelier.c types.h
	$(CC) -o chef_atelier chef_atelier.c

mecanicien : mecanicien.c types.h
	$(CC) -o mecanicien mecanicien.c 

main : main.c types.h
	$(CC) -o main main.c
	touch cle.chefs

clean :
	rm -f main chef_atelier client mecanicien