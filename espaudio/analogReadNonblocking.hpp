#ifndef ANALOGREADNONBLOCKING_H
#define ANALOGREADNONBLOCKING_H
#include <stdbool.h>
#include <stdint.h>
void analogReadStart();
bool analogReadNonblocking(uint16_t *result, bool rearm);
#endif
