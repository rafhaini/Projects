/*
 * socket.h
 *
 *  Created on: 16 de fev de 2019
 *      Author: Rafhaini Veronezi
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#define PORT 43000
#define MAX_CONECTIONS 10000
#define byte unsigned char

int fd_server, fd_client;

struct sockaddr_in6 server, client;

byte buffer[1024];
bool non_blocking;

bool create_socket(unsigned short);
bool bind_socket();
bool listen_socket(unsigned int conections);
int select_socket(int nfds, fd_set* read_fds, struct timeval* timeout);
int accept_socket();
void close_socket(int descriptor);

int get_descriptor();
void set_nonBloking(bool active);
int read_socket(int descriptor);
int send_socket(int descriptor, byte* msg);

byte* getBuffer();

#endif /* SOCKET_H_ */
