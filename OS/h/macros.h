#ifndef H_MACROS_H_
#define H_MACROS_H_

#ifndef BCC_BLOCK_IGNORE
#define LOCK asm pushf; asm cli;
#define UNLOCK asm popf;
#else
#define LOCK ;
#define UNLOCK ;
#endif

#ifndef BCC_BLOCK_IGNORE
#include <dos.h>

#define GETVECT(num) getvect(num)
#define SETVECT(num, val) setvect(num, val)

#else

#define GETVECT(num) 0
#define SETVECT(num, val) 0
// eclipse ne moze da ih nadje, ovako me ne smara kako mi ne valja kod
#define FP_SEG(val) 0
#define FP_OFF(val) 0
#endif

#endif /* H_MACROS_H_ */
