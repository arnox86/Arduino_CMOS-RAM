/*
MIT License

Copyright (c) 2020 Benedikt Bursian

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



/*
*
*    Communicating with a CMOS-RAM, 2048 x 8 bits (Hyundai HY6116A or other with same or less bytes) including
*    read- and write- function.
*
*    Adress an data pins are examples for Arduino Mega2560 R3 and have to be changed manually.
*
*/


#include <stdio.h>


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

int adress_bit [11] = {Ad0, Ad1, Ad2, Ad3, Ad4, Ad5, Ad6, Ad7, Ad8, Ad9, Ad10};
int *p_adress = adress_bit;



#define IO1 49    // Input- / Output- pins
#define IO2 51
#define IO3 53
#define IO4 52
#define IO5 50
#define IO6 48
#define IO7 46
#define IO8 44

int io_bit [9] = {0, IO1, IO2, IO3, IO4, IO5, IO6, IO7, IO8};
              //  ^~~ to have IO1 at position &io_bit+1
int *p_io = io_bit;



#define OE 38    // Output Enable / Write Enable
#define WE 36

#define maxSize 0x07FF    // Maximum Size of memory
#define maxSizeInv 0xFFFF-maxSize


uint16_t adress_counter;    // Counts current adress
uint16_t adress_buffer;     // Saves specific adresses


//----------------------------------------------------------------------

// Function for writing single 8-bit character to specific adress

void writeByteToRAM (uint16_t _adress, char _data) {
  
  uint16_t _b_adress = _adress;
  
  _b_adress &= 0xF800;    // Setting lower 10 bits zero, checking if adress is valid
   
  if (_b_adress > 0) {
    
    Serial.print ("Error: adress is invalid (too long): 0x");
    Serial.print (_adress, HEX);
    Serial.println ("  ::writing progress interrupted");
    
    return;
    
  }
  _b_adress = _adress;
  
  
  // Setting data pins as output:
  for (int set_cnt = 0; set_cnt < 8; set_cnt++) {
    
    pinMode (io_bit[set_cnt+1], OUTPUT);
    digitalWrite (io_bit[set_cnt+1], LOW);
    
  }
  
  
  // Set Adress:
  // The single bits of the adress are divided by anding the lowest bit by 1 and then
  // shifting the complete varible right by 1. Data pins get set the same way.
  
  for (int adr_cnt = 0; adr_cnt < 11; adr_cnt++) {
    
    digitalWrite (adress_bit[adr_cnt], _b_adress & 0x1);  // Anding adress with 1, to get value of the lowest bit
    
    _b_adress = _b_adress >> 1;    // Shifting adress buffer right by one
    
  }
  
  digitalWrite (OE, HIGH);    // Setting OE to high, so output is disabled
  digitalWrite (WE, LOW);     // Setting WE low, so writing is enabled
  
  
  // Set data:
  
  char _b_data = _data;    // Buffer for _data value
  
  for (int data_cnt = 0; data_cnt < 8; data_cnt++) {
    
    digitalWrite (io_bit[data_cnt+1], _b_data & 0x1);
    
    _b_data = _b_data >> 1;
    
  }
  
  
  digitalWrite (WE, HIGH);    // Resetting WE
  
    
  // Setting all data pins to low:
  
  for (int dst_cnt = 0; dst_cnt < 8; dst_cnt++) {
    
    digitalWrite (io_bit[dst_cnt+1], LOW);
    
  }
  
  delayMicroseconds (2);
    
}

//----------------------------------------------------------------------

// Function for reading single byte of data at specific adress:

char readByteFromRAM (uint16_t _adress) {
  
  byte __output_char = 0;
  
  uint16_t _b_adress = _adress;
  
  _b_adress &= 0xF800;    // Setting lower 10 bits zero, checking if adress is valid
   
  if (_b_adress > 0) {
    
    Serial.print ("Error: adress is invalid (too long): 0x");
    Serial.print (_adress, HEX);
    Serial.println ("  ::reading progress interrupted");
    
    return (0xFFFF);
    
  }
  _b_adress = _adress;
  
  
  // Set data pins to input:
  
  for (int dis_cnt = 0; dis_cnt < 8; dis_cnt++) {
    
    pinMode (io_bit[dis_cnt+1], INPUT);
    digitalWrite (io_bit[dis_cnt+1], LOW);
    
  }
  
  
  // Set Adress (same way as in write process):
  
  for (int adr_cnt = 0; adr_cnt < 11; adr_cnt++) {
    
    digitalWrite (adress_bit[adr_cnt], _b_adress & 0x1);  // Anding adress with 1, to get value of the lowest bit
    
    _b_adress = _b_adress >> 1;    // Shifting adress buffer right by one
    
  }
  
  digitalWrite (OE, LOW);    // Enabling output
  
  // Reading data pins:
  
  boolean __bbuffer;
  byte __andbuffer;
  
  for (int rcy_cnt = 0; rcy_cnt < 8; rcy_cnt++) {
    
    __bbuffer = digitalRead (io_bit[rcy_cnt+1]);    // Reading single pins
    
    if (__bbuffer == 1) bitSet (__andbuffer, rcy_cnt);  // Setting bits in buffer var
    __output_char += __andbuffer;    // Adding each value to the output variable
    __andbuffer = 0;
    
  }

  return (__output_char);
  
}
void setup () {
  
  Serial.begin (9600);    // Bus transfer rate
  
  // Setting Adress pins to output:
  for (int out_cnt = 0; out_cnt < 11; out_cnt++) {
    
    pinMode (adress_bit[out_cnt], OUTPUT);
    
  }
  
  pinMode (OE, OUTPUT);    // Output enable and write enable set to output
  pinMode (WE, OUTPUT);
  
}


//----------------------------------------------------------------------


void loop () {
  
  int adress = 0x015F;    // Random adress
  char data = 'A';        // Random data
  
  writeByteToRAM (adress, data);
  
  Serial.println (readByteFromRAM (adress));    // Output of character at adress, in this case 'A'
  
  delay (100);    // Arduino has to transmit serial buffer
  exit (0);
  
}
