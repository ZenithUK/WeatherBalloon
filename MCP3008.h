#ifndef MCP3008_H
#define MCP3008_H

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define ARRAY_SIZE(array) sizeof(array)/sizeof(array[0])

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 500000;
static uint16_t delay;
uint8_t *txbuf, *rxbuf;

extern short ADCValue[8];

int mcp3008;

void pabort(const char *s);

int getADCValue(int channel);

int closeMCP3008();

int initialiseMCP3008() ;

#endif
