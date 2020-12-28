
/*
*
*    Communicating with a CMOS-RAM, 2048 x 8 bits (Hyundai HY6116A) including
*    read-, search- and write-cycles.
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
#define maxSizeInv 0xFFF-maxSize


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
  
  delayMicroseconds (0.1);    // Reaction time of RAM
  
  digitalWrite (WE, HIGH);    // Resetting WE
  
    
  // Setting all data pins to low:
  
  for (int dst_cnt = 0; dst_cnt < 8; dst_cnt++) {
    
    digitalWrite (io_bit[dst_cnt+1], LOW);
    
  }
    
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
  
  delayMicroseconds (1);
  
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

//----------------------------------------------------------------------

// Function for entering start adress, if nothing was entered, it start's at 0 or
//  the last end adress:

uint16_t _startAdress;

void setStartAdress (uint16_t _adress) {
  
  uint16_t _b_adress = _adress;
  
  _b_adress &= 0xF800;    // Setting lower 10 bits zero, checking if adress is valid
   
  if (_b_adress > 0) {
    
    Serial.print ("Error: adress is invalid (too long): 0x");
    Serial.print (_adress, HEX);
    Serial.println ("  ::writing(movs) progress interrupted");
    
    return;
    
  }
  
  _startAdress = _adress;
  
}

//----------------------------------------------------------------------

// Function to write more than one byte to the RAM. The start adress has to be entered,
//  than its always added by 1. A char array has to be entered. Returnd you get the
//  las written adress+1:

int writeArrayToRAM (char _data[]) {
  
  size_t __array_size = sizeof (_data);    // Getting size of the array for loop
  uint16_t __current_adress = _startAdress;
  uint16_t __data_counter = 0;
  
  for ( ;__data_counter < __array_size;) {    // For size of array
  
    uint16_t _b_adress = __current_adress;
  
    _b_adress &= 0xF800;    // Setting lower 10 bits zero, checking if adress is valid
   
    if (_b_adress > 0) {
    
      Serial.print ("Error: adress is invalid (too long): 0x");
      Serial.print (__current_adress, HEX);
      Serial.println ("  ::array writing(movs) progress interrupted");
    
      return (0xFFFF);
    
    }
  
    writeByteToRAM (_startAdress, _data[__data_counter]);
  
    __current_adress++;    // increment adress
    __data_counter++;
  
    if (_startAdress > 0x07FF) {
    
      Serial.print ("Warning: last adress was filled: 0x");
      Serial.print (__current_adress-1, HEX);
      Serial.println ("  ::array writing(movs) will be interrupted");
    
    }
    
  }
  
  return (_startAdress);
  
}

//----------------------------------------------------------------------

// Function to read larger parts of the 

//int readArrayFromRAM (char _data


//----------------------------------------------------------------------

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
  
  
    
  
  delay (0.0001);
  exit (0);
  
}
