
#include <stdio.h>
#include <cmos.h>

// Example: pin assignments for Arduino Mega 2560 R3:

#define Ad0 47    // Adress pins
#define Ad1 45
#define Ad2 43
#define Ad3 41
#define Ad4 39
#define Ad5 37
#define Ad6 35
#define Ad7 33
#define Ad8 32
#define Ad9 34
#define Ad10 40



#define IO1 49    // Input- / Output- pins
#define IO2 51
#define IO3 53
#define IO4 52
#define IO5 50
#define IO6 48
#define IO7 46
#define IO8 44



#define OE 38    // Output Enable / Write Enable
#define WE 36


// Putting all pin assignments into the arrays:
int _adress_bit[11] = {Ad0, Ad1, Ad2, Ad3, Ad4, Ad5, Ad6, Ad7, Ad8, Ad9, Ad10};
int _io_bit[8] = {IO1, IO2, IO3, IO4, IO5, IO6, IO7, IO8};

#define maxSize 0x07FF    // Maximum Size of memory


void setup () {
  
  Serial.begin (9600);
  
}

// Calling constructor:
cmos cmosRam (_adress_bit, _io_bit, OE, WE, maxSize);

void loop () {
  
  // Writeing character 'a' to position 0x014F at RAM:
  cmosRam.writeByteToRAM (0x014F, 'a');
  
  // Reading and giving out the written character:
  Serial.println (cmosRam.readByteFromRAM (0x014F));
  
  delay (100);    // Let time to clear serial buffer
  exit (0);
  
}
