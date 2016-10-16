/*--------------------------------------------------------------------------*
* Formatted serial output functions
*---------------------------------------------------------------------------*
* 14-Apr-2014 ShaneG
*
* Provides some simple formatted output functions for serial communications.
*--------------------------------------------------------------------------*/
#include <stdbool.h>
#include "softuart.h"

/** Print a string from RAM
 *
 * This function simply prints the nul terminated string from RAM to the
 * serial port.
 *
 * @param cszString pointer to a character array in RAM.
 */
void uartPrint(const char *cszString) {
  while(*cszString) {
    uartSend(*cszString);
    cszString++;
    }
  }

/** Print a string from PROGMEM
 *
 * This function simply prints the nul terminated string from PROGMEM to the
 * serial port.
 *
 * @param cszString pointer to a character array in PROGMEM.
 */
void uartPrintP(const char *cszString) {
  uint8_t ch;
  while((ch = pgm_read_byte_near(cszString))!='\0') {
    uartSend(ch);
    cszString++;
    }
  }

/** Print an unsigned 16 bit value in decimal
 *
 * Print the given value in decimal format to the UART.
 *
 * @param value the value to print.
 */
void uartInt(uint16_t value) {
  bool emit = false;
  uint16_t divisor;
  // Special case for 0
  if(value==0) {
    uartSend('0');
    return;
    }
  // Emit the value, skip leading zeros
  for(divisor = 10000; divisor > 0; divisor = divisor / 10) {
    uint8_t digit = value / divisor;
    value = value % divisor;
    if((digit>0)||emit) {
      uartSend('0' + digit);
      emit = true;
      }
    }
  }

/** Convert the low four bits of a byte value into an upper case hex digit.
 *
 * @param value the value to convert
 *
 * @return the ASCII code for the appropriate hex digit.
 */
char hexChar(uint8_t value) {
  value &= 0x0F;
  if(value<10)
    return '0' + value;
  return value + 'A' - 10;
  }

/** Print an unsigned 16 bit value in hexadecimal
 *
 * Print the given value in hexadecimal format to the UART.
 *
 * @param value the value to print.
 */
void uartHex(uint16_t value) {
  uartSend(hexChar(value >> 12));
  uartSend(hexChar(value >> 8));
  uartSend(hexChar(value >> 4));
  uartSend(hexChar(value));
  }

