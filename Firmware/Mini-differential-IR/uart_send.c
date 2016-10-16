#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart_defs.h"
#include "softuart.h"
#include "BBUart.h"

/** Initialise the UART
 */
#if 0
void uartInit() {
  // Set up TX pin
  DDRB |= (1 << UART_TX);
  PORTB |= (1 << UART_TX);
  }
#endif

/** Write a single character
 *
 * Send a single character on the UART.
 *
 * @param ch the character to send.
 */
void uartSend(char ch) {
#if 0
  // Set to output state and bring high
  PORTB |= (1 << UART_TX);
  cli();
  asm volatile(
    "  cbi %[uart_port], %[uart_pin]    \n\t"  // start bit
    "  in r0, %[uart_port]              \n\t"
    "  ldi r30, 3                       \n\t"  // stop bit + idle state
    "  ldi r28, %[txdelay]              \n\t"
    "TxLoop:                            \n\t"
    // 8 cycle loop + delay - total = 7 + 3*r22
    "  mov r29, r28                     \n\t"
    "TxDelay:                           \n\t"
    // delay (3 cycle * delayCount) - 1
    "  dec r29                          \n\t"
    "  brne TxDelay                     \n\t"
    "  bst %[ch], 0                     \n\t"
    "  bld r0, %[uart_pin]              \n\t"
    "  lsr r30                          \n\t"
    "  ror %[ch]                        \n\t"
    "  out %[uart_port], r0             \n\t"
    "  brne TxLoop                      \n\t"
    :
    : [uart_port] "M" (_SFR_IO_ADDR(PORTB)),
      [uart_pin] "M" (UART_TX),
      [txdelay] "Mrm" (TXDELAY),
      [ch] "r" (ch)
    : "r0","r28","r29","r30");
  sei();
#endif
  cli();
  TxByte(ch);
  sei();

  }
