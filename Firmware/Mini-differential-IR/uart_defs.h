/*--------------------------------------------------------------------------*
* Internal configuration for software UART implementation
*---------------------------------------------------------------------------*
* 14-Apr-2014 ShaneG
*
* Provides additional configuration for the software UART implementation
* based on the values set in 'hardware.h'.
*--------------------------------------------------------------------------*/
#ifndef __UART_DEFS_H
#define __UART_DEFS_H

// Calculate delays for the bit bashing functions
#ifdef F_CPU
/* account for integer truncation by adding 3/2 = 1.5 */
#  define TXDELAY   (int)(((F_CPU/BAUD_RATE)-7 +1.5)/3)
#  define RXDELAY   (int)(((F_CPU/BAUD_RATE)-5 +1.5)/3)
#else
#  error CPU frequency F_CPU undefined
#endif

#endif /* __UART_DEFS_H */
