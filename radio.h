#ifndef GPS_H
#define GPS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include <sys/time.h>
#include "Utils.h"

int radio;

long packetID = 0;

int sendString(uint32_t Time, double latitude, double longitude, double altitude, double intTemp, double exTemp, int satalites, int pressure, int geigerCount, short UV);

//uint16_t getCRC16Checksum(char *string)

void getGPS();   

int closeGPS();

int openRadio();

#endif
