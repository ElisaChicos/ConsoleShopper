all:
	gcc server.c -o server
	gcc client.c -o client
client: all 
	./client 127.0.0.1 2024