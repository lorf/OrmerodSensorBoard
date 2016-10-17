/* optimized half-duplex high-speed serial uart implementation
 * @author: Ralph Doncaster 2014
 * @version: $Id$
 * soft UART has only 0.8% timing error at default 115200 baud rate @8Mhz
 * and 2.1% timing error at 230400.
 * To use UART, include BBUart.h in your C program, compile BBUart.S,
 * and link BBUart.o with your program.
 *
 * define BAUD_RATE before including BBUart.h to change default baud rate 
 */

#ifndef F_CPU
#error F_CPU undefined
#endif

#ifdef __cplusplus
extern "C" {
#endif
    void TxByte(unsigned char);
#ifdef __cplusplus
}
#endif

#define STR1(x) #x
#define STR(x) STR1(x)

#ifndef BAUD_RATE
// default baud rate
#define BAUD_RATE 115200 
#endif

#define DIVIDE_ROUNDED(NUMERATOR, DIVISOR) ((((2*(NUMERATOR))/(DIVISOR))+1)/2)

// txbit takes 3*RXDELAY + 15 cycles
#define BIT_CYCLES DIVIDE_ROUNDED(F_CPU,BAUD_RATE) 
#define TXDELAYCOUNT DIVIDE_ROUNDED(BIT_CYCLES - 7, 3)

asm(".global TXDELAY" );

// dummy function defines no code
// hack to define absolute linker symbols using C macro calculations
static void dummy() __attribute__ ((naked));
static void dummy() __attribute__ ((used));
static void dummy(){
asm (
    ".equ TXDELAY, %[txdcount]\n"
    ::[txdcount] "M" (TXDELAYCOUNT)
    );
}
