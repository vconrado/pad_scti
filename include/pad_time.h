#ifndef __PAD_TIME_H__
#define __PAD_TIME_H__

#include <stdint.h>

uint64_t micros_since_epoch();

int pad_usleep(long value);
void pad_sleep(int value);


#endif /* __PAD_TIME_H__ */
