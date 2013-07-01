#include "radio.h"

uint16_t crc_xmodem_update(uint16_t crc, uint8_t data)
{
        crc = crc ^ ((uint16_t)data << 8);
        for (int i=0; i<8; i++)
        {
            if (crc & 0x8000)
                crc = (crc << 1) ^ 0x1021;
            else
                crc <<= 1;
        }

        return crc;
}

uint16_t getCRC16Checksum(char *string){
	size_t i;
	uint16_t crc;
	uint8_t c;
 
	crc = 0xFFFF;
 
	// Calculate checksum ignoring the first two $s
	for (i = 0; i < strlen(string); i++)
	{
		c = string[i];
		crc = crc_xmodem_update (crc, c);
	}
 
	return crc;
}

int sendString(uint32_t Time, double latitude, double longitude, double altitude, double intTemp, double exTemp, int satalites, int pressure, int geigerCount, short UV){

	openRadio();

	char* string;
	int len;
	uint16_t checksum;
	char* packet;
	int packetSize = 0;

	packetID++;

	const char* format = "ZN1,%i,%i,%F,%F,%.1f,%i,%.1f,%.3f,%i,%i,%i";

       len = snprintf(NULL, 0, format, packetID, Time, latitude, longitude, altitude, satalites, intTemp, exTemp, pressure, geigerCount, UV) + 1;

	if (!(string = malloc((len + 1) * sizeof(char))))
            return 0;

	snprintf(string, len, format, packetID, Time, latitude, longitude, altitude, satalites, intTemp, exTemp, pressure, geigerCount, UV);

	checksum = getCRC16Checksum(string);

	packetSize = len+8;

	if (!(packet = malloc((packetSize + 1) * sizeof(char))))
            return 0;

	len = snprintf(packet, packetSize, "$$%s*%04X\n", string, checksum);

	printf(packet);

	write(radio, packet, len);

	free(string);
	free(packet);

	//printf("L: %i", strlen(string));
}

int closeRadio(){
	if (close(radio ) == 0){
		return 1;
	}else{
		return 0;
	}
}

int openRadio() {

	if ((radio = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 0;
	}

	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(radio, &options);
	options.c_cflag = B50 | CS8 | CLOCAL | CREAD | CSTOPB;		//<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(radio, TCIFLUSH);
	tcsetattr(radio, TCSANOW, &options);

	//loop();

	return 1;
}
