#ifndef __crc32_h
#define __crc32_h

/* crc32.h */

/* wrapper for gall library */

typedef unsigned long crc32_t;
#define Z_NULL  0

#define byte(a) ((unsigned char)(a))

extern crc32_t __crc32_table[256];

static crc32_t updCrc32(unsigned char ch, crc32_t crc)
{
  return (crc32_t)(__crc32_table[byte(crc) ^ byte(ch)] ^ (crc >> 8));
}

static crc32_t crc32(crc32_t crc, const unsigned char *buf, unsigned int len)
{
  if (buf == Z_NULL) return 0L;
  crc = crc ^ 0xffffffffL;
  while (len--)
  {
    crc = updCrc32(*buf++, crc);
  }
  return crc ^ 0xffffffffL;
}

#endif /* __crc32_h */
