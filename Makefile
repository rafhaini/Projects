GCC=gcc -g -Wall 

JSON_C_DIR=/usr/local
CFLAGS += -I$(JSON_C_DIR)/include/json-c
LIBS +=-pthread 

BIN=socket.o format_json.o

all: server

server: socket format_json
	$(GCC) server.c -o server $(BIN) $(CFLAGS) -ljson-c $(libs)  

socket:
	$(GCC) -c socket.c 	

format_json:
	$(GCC) -c format_json.c 
	
clean:
	rm -f *.o
	rm -f server