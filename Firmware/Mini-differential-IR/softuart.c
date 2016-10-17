/*
 * This is combined version of the UART helper functions from this nice library:
 * https://github.com/thegaragelab/tinytemplate 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdarg.h>
#include <stdbool.h>
#include "softuart.h"
#include "BBUart.h"

/** Write a single character
 *
 * Send a single character on the UART.
 *
 * @param ch the character to send.
 */
void uartSend(char ch) {
  cli();
  TxByte(ch);
  sei();
}

/*--------------------------------------------------------------------------*
* Formatted serial output functions
*---------------------------------------------------------------------------*
* 14-Apr-2014 ShaneG
*
* Provides some simple formatted output functions for serial communications.
*--------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------*
* Formatted serial output functions
*---------------------------------------------------------------------------*
* 14-Apr-2014 ShaneG
*
* Provides some simple formatted output functions for serial communications.
*--------------------------------------------------------------------------*/

//---------------------------------------------------------------------------
// Helper functions
//---------------------------------------------------------------------------

/** Do the actual formatting
 *
 * This function uses the current two characters of the input string to
 * determine what to send to the serial port.
 *
 * @param ch1 the current character of the format string
 * @param ch2 the next character of the format string
 * @param args the argument list containing the embedded items
 *
 * @return true if both characters should be skipped, false if we only need
 *              to move ahead by one.
 */
static bool printFormat(char ch1, char ch2, va_list *args) {
  bool skip = true;
  // Fail fast
  if(ch1=='%') {
    // Use the second character to determine what is requested
    if((ch2=='%')||(ch2=='\0'))
      uartSend('%');
    else if(ch2=='c')
      uartSend(va_arg(*args, int));
    else if(ch2=='u')
      uartInt(va_arg(*args, unsigned int));
    else if(ch2=='x')
      uartHex(va_arg(*args, unsigned int));
    else if(ch2=='s')
      uartPrint(va_arg(*args, char *));
    else if(ch2=='S')
      uartPrintP(va_arg(*args, char *));
    }
  else {
    uartSend(ch1);
    skip = false;
    }
  return skip;
  }

//---------------------------------------------------------------------------
// Public API
//---------------------------------------------------------------------------

/** Print a formatted string from RAM
 *
 * Prints a formatted string from RAM to the serial port. This function
 * supports a subset of the 'printf' string formatting syntax. Allowable
 * insertion types are:
 *
 *  %% - Display a % character. There should be no entry in the variable
 *       argument list for this entry.
 *  %u - Display an unsigned integer in decimal. The matching argument may
 *       be any 16 bit value.
 *  %x - Display an unsigned integer in hexadecimal. The matching argument may
 *       be any 16 bit value.
 *  %c - Display a single ASCII character. The matching argument may be any 8
 *       bit value.
 *  %s - Display a NUL terminated string from RAM. The matching argument must
 *       be a pointer to a RAM location.
 *  %S - Display a NUL terminated string from PROGMEM. The matching argument
 *       must be a pointer to a PROGMEM location.
 *
 * @param cszString pointer to a nul terminated format string in RAM.
 */
void uartFormat(const char *cszString, ...) {
  int index;
  va_list args;
  va_start(args, cszString);
  char ch1, ch2 = *cszString;
  for(index=1; ch2!='\0'; index++) {
    ch1 = ch2;
    ch2 = cszString[index];
    if(printFormat(ch1, ch2, &args)) {
      // Move ahead an extra character so we wind up jumping by two
      ch1 = ch2;
      ch2 = cszString[++index];
      }
    }
  va_end(args);
  }

/** Print a formatted string from PROGMEM
 *
 * Prints a formatted string from PROGMEM to the serial port. This function
 * supports a subset of the 'printf' string formatting syntax. Allowable
 * insertion types are:
 *
 *  %% - Display a % character. There should be no entry in the variable
 *       argument list for this entry.
 *  %u - Display an unsigned integer in decimal. The matching argument may
 *       be any 16 bit value.
 *  %x - Display an unsigned integer in hexadecimal. The matching argument may
 *       be any 16 bit value.
 *  %c - Display a single ASCII character. The matching argument may be any 8
 *       bit value.
 *  %s - Display a NUL terminated string from RAM. The matching argument must
 *       be a pointer to a RAM location.
 *  %S - Display a NUL terminated string from PROGMEM. The matching argument
 *       must be a pointer to a PROGMEM location.
 *
 * @param cszString pointer to a nul terminated format string in PROGMEM.
 */
void uartFormatP(const char *cszString, ...) {
  int index;
  va_list args;
  va_start(args, cszString);
  char ch2 = pgm_read_byte_near(cszString), ch1 = ch2;
  for(index=1; ch2!='\0'; index++) {
    ch1 = ch2;
    ch2 = pgm_read_byte_near(cszString + index);
    if(printFormat(ch1, ch2, &args)) {
      // Move ahead an extra character so we wind up jumping by two
      index++;
      ch1 = ch2;
      ch2 = pgm_read_byte_near(cszString + index);
      }
    }
  va_end(args);
  }
