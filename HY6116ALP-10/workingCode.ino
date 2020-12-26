
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


uint16_t adress_counter;    // Counts current adress
uint16_t adress_buffer;     // Saves specific adresses


//----------------------------------------------------------------------

// Function for writing single 8-bit character to specific adress

void writeToRAM (uint16_t _adress, char _data) {
  
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
  
  delayMicroseconds (1);    // Reaction time of RAM
  
  digitalWrite (WE, HIGH);    // Resetting WE
  
  
  
  // Setting all adress pins to low:
  
  for (int ast_cnt = 0; ast_cnt < 11; ast_cnt++) {
    
    digitalWrite (adress_bit[ast_cnt], LOW);
    
  }
    
  // Setting all data pins to low:
  
  for (int dst_cnt = 0; dst_cnt < 8; dst_cnt++) {
    
    digitalWrite (io_bit[dst_cnt+1], LOW);
    
  }
  
  
    
}


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
 
  writeToRAM (0x07FE, 'c');
  
  delay (100000);
  exit (0);
  
}
