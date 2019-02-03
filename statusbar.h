#ifndef _STATUSBAR_H
#define _STATUSBAR_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define ARRAY_SIZE(array) ( sizeof(array) / sizeof(array[0]) )

typedef size_t (* handler_t)(char * output, size_t length);

size_t volume_update(char * output, size_t length);
size_t mpd_update(char * output, size_t length);
size_t time_update(char * output, size_t length);


#endif /* _STATUSBAR_H */
