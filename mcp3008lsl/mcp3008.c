#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <assert.h>

#include "mcp3008.h"

#define RX_LEN 3

int spi_init(spi* s, const char* filename)
{
  assert(s);

  s->dev = -1;
  s->mode = SPI_MODE_0;
  s->bits = 8;
  s->speed = 1000000;


  if((s->dev = open(filename, O_RDWR)) < 0)
  {
    perror("Failed to open bus.\n");
    exit(1);
  }

  if(ioctl(s->dev, SPI_IOC_WR_MODE, &s->mode) < 0)
  {
    perror("Failed to set spi WR_MODE");
    return -1;
  }

  if(ioctl(s->dev, SPI_IOC_RD_MODE, &s->mode) < 0)
  {
    perror("Failed to set spi RD_MODE");
    return -1;
  }

  if(ioctl(s->dev, SPI_IOC_WR_BITS_PER_WORD, &s->bits) < 0)
  {
    perror("Failed to get spi WR_BITS_PER_WORD");
    return -1;
  }

  if(ioctl(s->dev, SPI_IOC_RD_BITS_PER_WORD, &s->bits) < 0)
  {
    perror("Failed to get spi RD_BITS_PER_WORD");
    return -1;
  }

  if(ioctl(s->dev, SPI_IOC_WR_MAX_SPEED_HZ, &s->speed) < 0)
  {
    perror("Failed to set spi WR_MAX_SPEED_HZ");
    return -1;
  }

  if(ioctl(s->dev, SPI_IOC_RD_MAX_SPEED_HZ, &s->speed) < 0)
  {
    perror("Failed to set spi RD_MAX_SPEED_HZ");
    return -1;
  }

  return 0;
}

void spi_close(spi* s)
{
  assert(s);
  assert(s->dev >= 0);
  close(s->dev);
  s->dev = -1;
}

int spi_getadc(spi* s, int channel)
{
  assert(s);
  assert(s->dev >= 0);

  if(channel < 0 && channel > 7)
  {
    return -1;
  }
  int ret = -1;

  struct spi_ioc_transfer xfer[3];
  /*
   * To get a query we send a startbit, followed by a single or diff bit, followed by 3 selection
   * bits. To align this within our bytes we center this data within the middle of zeros using 3
   * bytes in the following layout.
   * 0 0 0 0 0 0 0 1
   * S D D D 0 0 0 0
   * 0 0 0 0 0 0 0 0
   * The end padding gives the chip a chance to collect, buffer, and respond.
   */
  uint8_t buf[RX_LEN] = {0b00000001,
                         0b10000000 | (channel << 4),
                         0b00000000};
  memset(xfer, 0, sizeof(struct spi_ioc_transfer)*3);

  for(int i = 0; i < 3; i++) {
    xfer[i].tx_buf        = (unsigned long) &buf[i];
    xfer[i].rx_buf        = (unsigned long) &buf[i];
    xfer[i].len	          = 1;
    xfer[i].speed_hz      = s->speed;
    xfer[i].bits_per_word = s->bits;
  }

  ret = ioctl(s->dev, SPI_IOC_MESSAGE(3), xfer);
  if(ret < 0){
    printf("SPI_IOC_MESSAGE failed... %d\n", ret);
    return -1;
  }

  ret = (((uint32_t)buf[1])<<8) & 0b1100000000; // Taking the top 2 bits read from the chip
  ret |= (buf[2] & 0xff);
  return ret;
}
