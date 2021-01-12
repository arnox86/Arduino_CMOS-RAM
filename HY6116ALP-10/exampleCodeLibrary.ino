
#include <stdio.h>
#include <cmos.h>

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

#define maxSize 0x07FF    // Maximum Size of memory
//#define maxSizeInv 0xFFFF-maxSize

cmos cmosRam ();

void setup () {
  
  Serial.begin (9600);
  
}

void loop () {
  
  cmosRam.writeByteToRAM (0x001F, 'a');
  Serial.println (cmosRam.readByteFromRAM (0x001F));
  
  delay (100);
  exit (0);
  
}
