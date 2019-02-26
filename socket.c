/*
 * socket.c
 *
 *  Created on: 16 de fev de 2019
 *      Author: Rafhaini Veronezi
 */

#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool create_socket(unsigned short port) {
	fd_server = socket(AF_INET6, SOCK_STREAM, 0);
	if (fd_server < 0) {
		perror("Error: Create socket failed.");
		return false;
	}

	int on =1;
	if (setsockopt(fd_server, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) < 0) {
		perror("Erro: setsockopt: SO_REUSEADDR failed");
		close(fd_server);
		return false;
	}

	if (non_blocking) {
		if (ioctl(fd_server, FIONBIO, (char*)&on)) {
			perror("Error: ioctl failed");
			close(fd_server);
			return false;
		}
	}

	/* Configure socket */
	memset(&server, 0, sizeof(server));
	server.sin6_family = AF_INET6;
	server.sin6_port = htons(port);
	server.sin6_addr = in6addr_any;

	return bind_socket();
}

bool bind_socket() {
	if (bind(fd_server, (struct sockaddr*)&server, sizeof(server)) == -1) {
		perror("Error: Bind failed");
		close(fd_server);
		return false;
	}
	return true;
}

bool listen_socket(unsigned int conections) {
	if (listen(fd_server, conections) == -1) {
		perror("Error: listen failed.");
		close(fd_server);
		return false;
	}
	return true;
}

int accept_socket() {
    socklen_t size_client = sizeof(client);
    fd_client =  accept(fd_server, (struct sockaddr*) &client, &size_client);

    /*char ipv6[128];
    if (inet_ntop(AF_INET6, &client.sin6_addr ,ipv6, sizeof(ipv6)) != NULL) {
    	printf("Servidor recebeu a conexao: %s\n", ipv6);
    } */
    return fd_client;
}

void close_socket(int descriptor) {
	close(descriptor);
}

int select_socket(int nfds, fd_set* read_fds, struct timeval* timeout) {
	int res = select(nfds, read_fds, NULL, NULL, timeout);
	if (res < 0){
		perror("Error: failed select.");
		close(fd_server);
		exit(1);
	} else if (res == 0) {
		perror("Error: failed select timeout.");
		close(fd_server);
		exit(1);
	}
	return res;
}

int get_descriptor() {
	return fd_server;
}

int read_socket(int descriptor) {
	int res = recv(descriptor, (void*) buffer, sizeof(buffer), 0);
	return res;
}

int send_socket(int descriptor, byte* msg) {
	int res = send(descriptor, (void*) msg, sizeof(msg), 0);
	return res;
}

byte* getBuffer() {
	return buffer;
}

void set_nonBloking(bool active) {
	non_blocking = active;
}
