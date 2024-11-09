/**
 * author: Brando
 * date: 8/1/22
 */

#ifndef FREE_H
#define FREE_H

#include <stdlib.h>

#define BFFree(x) if (x) { free(x); x = 0; }

#endif // FREE_H

