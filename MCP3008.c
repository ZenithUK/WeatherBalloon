#include "MCP3008.h"

void pabort(const char *s)
{
	perror(s);
	abort();
}

int getADCValue(int channel)
{
	int ret;
	int status;

	long retData[3];	

	uint8_t tx[3] = {0x01, (8+channel)<<4, 0x00};

	uint8_t rx[ARRAY_SIZE(tx)] = {0, };
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = ARRAY_SIZE(tx),
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
 
	ret = ioctl(mcp3008, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("Can't send spi message");

	status = read(mcp3008, &rxbuf[0], 0);

	for (int i = 0; i < ARRAY_SIZE(tx); i++) {
		retData[i] = (long)rx[i];
	}

	ADCValue[channel] = (short)(((retData[1]&3) << 8) + retData[2]);

	free(txbuf);
	free(rxbuf);

	return 1;
}

int closeMCP3008(){
	if (close(mcp3008) == 0){
		return 1;
	}else{
		return 0;
	}
}

int initialiseMCP3008() 
{
	int ret = 0;
        int fd;

	mcp3008 = open(device, O_RDWR);

	if (mcp3008 < 0)
		pabort("can't open device");
 
	/*
	 * spi mode
	 */
	ret = ioctl(mcp3008, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");
 
	ret = ioctl(mcp3008, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");
 
	/*
	 * bits per word
	 */
	ret = ioctl(mcp3008, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");
 
	ret = ioctl(mcp3008, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");
 
	/*
	 * max speed hz
	 */
	ret = ioctl(mcp3008, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");
 
	ret = ioctl(mcp3008, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");
 
	printf("Spi mode: %d\n", mode);
	printf("Bits per word: %d\n", bits);
	printf("Max speed: %d Hz (%d KHz)\n", speed, speed/1000);

	printf("MCP3008 Initilised.\n");

        return 1;
}
