/*
 * format_json.h
 *
 *  Created on: 23 de fev de 2019
 *      Author: Rafhaini Veronezi
 */

#ifndef FORMAT_JSON_H_
#define FORMAT_JSON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <json-c/json.h>

#define byte unsigned char

struct header {
	char uuid[128];
	char seconds[128];
	int num_connections;
};
typedef struct header st_header;

struct list_client {
	st_header h;
	char hostname[256];
	char ip[128];
	float min, max, avg;
};
typedef struct list_client st_client;

char uuid_of_the_meter[128];

json_object* jobj; // main object

// header
json_object* colector;
json_object* time_seconds;

// PROBES
json_object* array_probes;

// TARGETS
json_object* array_targets;

st_client list[10000];

char* get_uuid_Meter();

void create_object_json();
void print_data(st_client* client);
void print_json();

void set_header(char* name_colector, char* seconds);

/*
 * Tratativas de buffer
 */
uint position;
char data[100000];

char* getData();
void setData(char* buf);
void zerarData();
size_t sizeData();

#endif /* FORMAT_JSON_H_ */
