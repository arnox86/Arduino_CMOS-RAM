
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



#ifndef cmos_h
#define cmos_h

#include "Arduino.h"


#define SERIAL_BAUD 9600        // Standard serial baud rate


#ifndef OUT_TYPE
    #define OUT_TYPE "Serial"      // Standard output
#endif

#ifndef SERIAL
    #define S_DEF 0
#else
    #define S_DEF 1
#endif


class cmos {

    public:
        cmos (int adressArray[], int ioArray[], int oePin, int wePin, int size);

        void writeByteToRAM (uint16_t __adress, char __data);
        char readByteFromRAM (uint16_t __adress);

    private:

        boolean adressValidation (uint16_t __adress, String __process);
        void setAdress (uint16_t __adress);
        
        int _adress_bit[11];        // Including numbers of adress pins
        int _io_bit[9];             // Including numbers of data pins

        int __OE;
        int __WE;

        uint16_t __maxSize;
        uint16_t __maxSizeInv;

};

#endif
