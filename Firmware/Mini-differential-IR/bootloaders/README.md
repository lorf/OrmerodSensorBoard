These are precompiled optiboot modified specifically for my slightly modified sensor.

Modifications are here: XXX

### optiboot_attiny85_tuned.hex

ATTiny85, 8MHz internal OSC, TX=PB2, RX=PB1, LED=PB3 (inverted, connect to
VCC). Tuner sketch builtin, see
https://github.com/TCWORLD/ATTinyCore/blob/master/tiny/bootloaders/optiboot/Readme%20for%20Tuned%20optiboot.txt
for tuning OSCCAL.

### optiboot_attiny85_tuned.hex

ATTiny85, 8MHz internal OSC, TX=PB2, RX=PB1, LED=PB3 (inverted, connect to VCC).

### Fuses

Changes from defaults: internal 8MHz OSC (this is a default), disable CLKDIV8,
enable self-programming, enable 2.7V BOD, enable preserving EEPROM during
erase.

Resulting fuses: LFUSE = 0xE2, HFUSE = 0xD5, EFUSE = 0xFE

Avrdude command line: -U lfuse:w:0xe2:m -U hfuse:w:0xd5:m -U efuse:w:0xfe:m 

### Problems compiling optiboot from ATTinyCore

* -mshort-calls removed in avr-gcc 4.6, removing this option results in larger
  binary (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=54854). Use older 4.3.2
  compiler supplied with arduino 1.0.6.
* Segfault in avr-ld linker with "--relax" in avr-gcc 4.3.2
  (https://sourceware.org/bugzilla/show_bug.cgi?id=9841). Use linker from
  avr-gcc 4.8 supplied in arduino-1.8.0.

### How to compile

* Download arduino-1.0.6 and extract to `$HOME/apps`
* Download arduino-1.8.0 and extract to `$HOME/apps`
* Remove `arduino-1.0.6/hardware/tools/avr/bin/avr-ld` and copy
  `arduino-1.8.0/hardware/tools/avr/bin/avr-ld` to
  `arduino-1.0.6/hardware/tools/avr/bin/avr-ld`
* Clone https://github.com/lorf/ATTinyCore
* Compile:
  ```
  cd tiny/bootloaders/optiboot
  make ENV=my attiny85_tuned
  ```
* Optionaly upload:
  ```
  make ENV=my attiny85_tuned_isp
  ```
