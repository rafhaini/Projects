/*
 ============================================================================
 Name        : server.c
 Author      : Rafhaini Veronezi
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

#include "socket.h"
#include "format_json.h"

#define MINUTES 60
#define NUMBER_MAX_CLIENT 10000;
#define SIZE_UUID 37

#define log(args...) { printf("%d: %s - ", __LINE__, __FILE__); printf(args); printf("\n"); }

void format(char* buffer) {
	int i = 0, x = 0;

	st_client lt;
	memset(&lt, 0, sizeof(st_client));

	printf("\n \nformat: %s \n", buffer);

	for (; i < strlen(buffer); i++) {
		if (buffer[i] == '(')
			break;
	}
	strncpy(lt.hostname, buffer, i);

	x = i;
	for (; i < strlen(buffer); i++) {
		if (buffer[i] == ')')
			break;
	}
	strncpy(lt.ip, &buffer[x++], (i - 1));

	char* data = strstr((const char*) buffer, "min/avg/max = ");
	if (data != NULL) {
		data += 14;
		//printf("Buffer: %s \n", data);
		sscanf(data, "%02f/%02f/%02f", &lt.min, &lt.avg, &lt.max);
	}

	print_data(&lt);
}

void format_header() {
	time_t timer = time(NULL);
	struct tm* data = localtime(&timer);
	set_header("Servidor-Coletor", asctime(data));
}

bool format_header_client(char* uuid) {
	if (strlen(uuid) > 0) {
		log("UUID: %s ", uuid);
	}
	return true;
}

int main(void) {

	int desc_ready, new_sd, max_sd, end_server = false;
	int i = 0, close_conn;
	int res, rc;

	fd_set master_set, working_set;
	struct timeval timeout;

	log("Initializing Server.");
	set_nonBloking(true);
	if (!create_socket(PORT)) {
		return EXIT_FAILURE;
	}

	log("Create socket.");
	if (!listen_socket(MAX_CONECTIONS)) {
		return EXIT_FAILURE;
	}

	log("Listen socket.");

	FD_ZERO(&master_set);
	max_sd = get_descriptor();
	FD_SET(get_descriptor(), &master_set);

	timeout.tv_sec = 3 * MINUTES;
	timeout.tv_usec = 0;

	create_object_json(); /* Create object json */
	format_header(); /* insert header in json */

	zerarData(); // Initialize buffer

	do {
		memcpy(&working_set, &master_set, sizeof(master_set));
		rc = select_socket(max_sd + 1, &working_set, &timeout);

		desc_ready = rc;
		for (; i <= max_sd && desc_ready > 0; ++i) {

			if (FD_ISSET(i, &working_set)) {
				desc_ready -= 1;

				if (i == get_descriptor()) {

					do {
						new_sd = accept_socket();
						if (new_sd < 0) {
							if (errno != EWOULDBLOCK) {
								perror ("Error: failed accept.");
								end_server = true;
							}
							break;
						}

						FD_SET(new_sd, &master_set);
						if (new_sd > max_sd)
							max_sd = new_sd;

					} while (new_sd != -1);

				} else {
					close_conn = false;

					do {
						res = read_socket(i);
						if (res < 0) {
							if (errno != EWOULDBLOCK) {
								perror ("Error: failed recv.");
								end_server = true;
							}
							break;
						} else if (res == 0) {
							close_conn = true;

							if (sizeData() > 0)
								format(getData());
							break;
						}

						send_socket(i, (byte*) "OK"); // send an "OK" to the client

						if (res == SIZE_UUID) {
							strncpy(get_uuid_Meter(),(char*) getBuffer(), strlen((char*) getBuffer()) - 1);
						} else {
							setData((char*) getBuffer());
						}

					} while (true);

					if (close_conn) {
						close_socket(i);
						FD_CLR(i, &master_set);

						if (i == max_sd) {
							while(FD_ISSET(i, &master_set)) {
								max_sd -= i;
							}
						}
					}

				}
			} // end if FD_ISSET
		} // end for


	} while (end_server == false);

	return EXIT_SUCCESS;
}
