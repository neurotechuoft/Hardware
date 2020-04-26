/*
example include file
*/
#include <stdint.h>

#define MCP_NUM_CHANNELS 8

typedef struct _spi
{
  int dev;
  uint8_t mode;
  uint8_t bits;
  uint32_t speed;
}
spi;

extern int spi_init(spi* s, const char* filename);

extern int spi_getadc(spi* s, int input);

extern void spi_close(spi* s);
