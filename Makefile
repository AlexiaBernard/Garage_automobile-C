CC = gcc -Wall 

client : client.c
	$(CC) client.c -o client

chef_atelier : chef_atelier.c
	$(CC) chef_atelier.c -o chef_atelier

mecanicien : mecanicien.c
	$(CC) mecanicien.c -o mecanicien

main : main.c 
	$(CC) main.c -o main
	touch cle.chefs

clean :
	rm -f main chef_atelier client mecanicien