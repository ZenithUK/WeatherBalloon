CC=gcc
CFLAGS=-c -w -std=c99

all: zenith

zenith: main.o Utils.o DS18B20.o MCP3008.o gps.o geiger.o radio.o BMP085.o smbus.o
	$(CC) main.o Utils.o DS18B20.o MCP3008.o gps.o geiger.o radio.o BMP085.o smbus.o -lpthread -g -o zenith

main.o: main.c
	$(CC) $(CFLAGS) main.c

DS18B20.o: DS18B20.c
	$(CC) $(CFLAGS) DS18B20.c

MCP3008.o: MCP3008.c
	$(CC) $(CFLAGS) MCP3008.c

gps.o: gps.c
	$(CC) $(CFLAGS) gps.c

geiger.o: geiger.c
	$(CC) $(CFLAGS) geiger.c

radio.o: radio.c
	$(CC) $(CFLAGS) radio.c

BMP085.o: BMP085.c
	$(CC) $(CFLAGS) BMP085.c

smbus.o: smbus.c
	$(CC) $(CFLAGS) smbus.c

Utils.o: Utils.c
	$(CC) $(CFLAGS) Utils.c

clean:
	rm -rf *o zenith

