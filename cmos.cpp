
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



#include "Arduino.h"
#include "cmos.h"



// Constructor:
cmos::cmos () {

    if (adError != 0) {

        l__output.println ("Error: adress pin(s) not defined");
        return;

    }
    if (ioError != 0) {

        l__output.println ("Error: io pin(s) not defined");
        return;

    }
    if (siError != 0) {

        l__output.println ("Error: ");

    }



    _adress_bit = {Ad0, Ad1, Ad2, Ad3, Ad4, Ad5, Ad6, Ad7, Ad8, Ad9, Ad10};
    _io_bit = {0, IO1, IO2, IO3, IO4, IO5, IO6, IO7, IO8};

    for (int __adout_cnt = 0; __adou_cnt < 11; __adout_cnt++) {

        pinMode (_adress_bit[__adout_cnt], OUTPUT);

    }

    pinMode (OE, OUTPUT);
    pinMode (WE, OUTPUT);

}

// private:
boolean cmos::adressValidation (uint16_t _adress, String _process) {

    // Checking length of the adress and ending process, if its to long to avoid overwrite

    uint16_t _b_adress = _adress;

    _b_adress &= 0xF800;    // Setting not needed bits high, checking if adress is valid

    if (_b_adress > 0) {
    
        l__output.print ("Error: adress is invalid (too long): 0x");
        l__output.print (_adress, HEX);
        l__output.print ("  ::");
        l__output.print (_process);        // Entering sort of executed process
        l__output.println (" progress interrupted");
    
        return (1);
    
    }
    _b_adress = _adress;
    return (0);

}

// private:
void cmos::setAdress (uint16_t __adress) {

    uint16_t __b_adress = __adress;      // Adress buffer

    for (int __adr_cnt = 0; __adr_cnt < 11; __adr_cnt++) {

        digitalWrite (_adress_bit[__adr_cnt], __b_adress & 0x1);

        __b_adress = __b_adress >> 1;       // Shifting to next bit

    }

}

void cmos::writeByteToRAM (uint16_t __adress, char __data) {

    // Checking adress:

    if (adressValidation (__adress, "writing") return;
    
    // Setting io pins as output:
    
    for (int __ioout_cnt  = 0; __ioout_cnt < 8; __ioout_cnt++) {

        pinMode (_io_bit[__ioout_cnt+1], OUTPUT);
        digitalWrite (_io_bit[__ioout_cnt+1], LOW);

    }

    // Setting adress pins:

    setAdress (__adress);

    // Setting OE and WE pins:

    digitalWrite (OE, HIGH);
    digitalWrite (WE, LOW);

    // Setting data pins:

    char __b_data = __data;     // Data buffer

    for (int __data_cnt = 0; __data_cnt < 8; __data_cnt++) {

        digitalWrite (_io_bit[__data_cnt+1], __b_data & 0x1);

        __b_data = __b_data >> 1;

    }

    digitalWrite (WE, HIGH)     // Ending write cycle by resetting WE

    // Setting data pins to LOW:

    for (int __dlow_cnt = 0; __dlow_cnt < 8; __dlow_cnt++) {

        digitalWrite (_io_pin[__dlow_cnt], LOW);
    
    }

    // Time buffer (exper.):
    //delayMicroseconds (1);

}

char cmos::readByteFromRAM (uint16_t __adress) {

    byte __output_char = 0;     // Buffer for final return value
    uint16_t __b_adress = __adress;     // Adress buffer

    if (adressValidation (__adress, "reading") return (0);

    // Set data pins to input:

    for (int __ioin_cnt = 0; __ioin_cnt < 8; __ioin_cnt++) {

        pinMode (_io_bit[__ioin_cnt+1], INPUT);
        digitalWrite (_io_bit[__ioin_cnt], LOW);        // No pullup

    }

    // Setting adress:

    setAdress (__adress);

    // Setting OE, WE not critical
    
    digitalWrite (OE, LOW);

    // Reading data bits:

    boolean __bbuffer;
    byte __andbuffer;

    for (int __drd_cnt = 0; __drd_cnt < 8; __drd_cnt++) {

        __bbuffer = digitalRead (_io_bit[__drd_cnt+1]);     // Reading single pins

        if (__bbufer == 1) bitSet (__andbuffer, __drd_cnt);     // Setting bits in bufffer
        __output_char += __andbuffer;       // Adding values to output buffer
        __andbuffer = 0;

    }

    return (__output_char);

}
