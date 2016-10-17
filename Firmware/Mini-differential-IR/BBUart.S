/*
 * UART TX implementation from this excellent lib:
 * https://github.com/nerdralph/nerdralph/tree/master/avr/libs/bbuart
 */

/* optimized half-duplex serial uart implementation - 27 instructions
 * @author: Ralph Doncaster 2014
 * @version: $Id$
 */

/* needed for <avr/io.h> to give io constant addresses */
#define __SFR_OFFSET 0
#include <avr/io.h>

#define UART_Port PORTB
#define UART_Tx 2

#define delayArg r22

.extern TXDELAY

.section .text.transmit,"ax",@progbits
; transmit byte contained in r24
; AVR305 has 1 cycle of jitter per bit, this has none
.global TxByte
TxByte:
	sbi UART_Port-1, UART_Tx		; set Tx line to output
	cbi UART_Port, UART_Tx			; start bit
	in r0, UART_Port
	ldi r25, 7						; stop bit & idle state
TxLoop:
	; 8 cycle loop + delay = 7 + 3*DelayArg
	ldi delayArg, TXDELAY
TxDelay:
    dec delayArg
	brne TxDelay
	bst r24, 0						; store lsb in T
	bld r0, UART_Tx
	lsr r25
	ror r24							; 2-byte shift register
	out UART_Port, r0
	brne TxLoop
	ret
