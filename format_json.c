/*
 * format_json.c
 *
 *  Created on: 23 de fev de 2019
 *      Author: Rafhaini Veronezi
 */


#include "format_json.h"

void create_object_json() {
	jobj = json_object_new_object();
}

void print_data(st_client* client) {

	//printf("Headers: %s , %s , %d \n", uuid_of_the_meter, client->h.seconds, client->h.num_connections);
	//printf("Estatisticas: %s, %s, %f, %f, %f", client->hostname, client->ip, client->min, client->max, client->avg);

	json_object* uuid = json_object_new_object();
	json_object* last = json_object_new_object();
	json_object* conn = json_object_new_object();
	json_object* host = json_object_new_object();
	json_object* _ip = json_object_new_object();
	json_object* _min = json_object_new_object();
	json_object* _max = json_object_new_object();
	json_object* _avg = json_object_new_object();

	array_probes = json_object_new_array();

	json_object* str_uuid = json_object_new_string(uuid_of_the_meter);
	json_object_object_add(uuid, "uuid", str_uuid);
	json_object_array_add(array_probes, uuid);

	json_object* last_second = json_object_new_string(client->h.seconds);
	json_object_object_add(last, "last-contact-seconds", last_second);
	json_object_array_add(array_probes, last);

	json_object* connections = json_object_new_int(client->h.num_connections);
	json_object_object_add(conn, "connections", connections);
	json_object_array_add(array_probes, conn);

	json_object_object_add(jobj, "probes", array_probes);

	array_targets = json_object_new_array();

	json_object* host_name = json_object_new_string(client->hostname);
	json_object_object_add(host, "hostname", host_name);
	json_object_array_add(array_targets, host);

	json_object* ip = json_object_new_string(client->ip);
	json_object_object_add(_ip, "ip", ip);
	json_object_array_add(array_targets, _ip);

	json_object* min = json_object_new_double(client->min);
	json_object_object_add(_min, "ip", min);
	json_object_array_add(array_targets, _min);

	json_object* max = json_object_new_double(client->max);
	json_object_object_add(_max, "ip", max);
	json_object_array_add(array_targets, _max);

	json_object* avg = json_object_new_double(client->avg);
	json_object_object_add(_avg, "ip", avg);
	json_object_array_add(array_targets, _avg);

	json_object_object_add(jobj, "probes", array_targets);

	print_json();
}

void set_header(char* name_colector, char* seconds) {
	colector = json_object_new_string(name_colector);
	time_seconds = json_object_new_string(seconds);

	json_object_object_add(jobj, "colector", colector);
	json_object_object_add(jobj, "timestamp-seconds", time_seconds);
}

void mount_json() {

}

void print_json() {
	printf("Json: %s\n", json_object_to_json_string(jobj));
}

char* get_uuid_Meter() {
	return uuid_of_the_meter;
}

char* getData() {
	return data;
}

void zerarData() {
	position = 0;
	memset(data, 0, sizeof(data));
}

void setData(char* buf) {
	size_t size = strlen(buf);
	strncpy(&data[position], buf , size);
	position += size;
}

size_t sizeData() {
	return position;
}



