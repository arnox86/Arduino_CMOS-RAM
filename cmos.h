
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
*   At top of the program where the library is used in the pins have to be defined like
*   this:

#define Ad0 ...     // Put in your adress pins
#define Ad1 
#define Ad2 
#define Ad3 
#define Ad4 
#define Ad5 
#define Ad6 
#define Ad7 
#define Ad8 
#define Ad9 
#define Ad10 

#define IO1 ...     // Put in your data pins
#define IO2 
#define IO3 
#define IO4 
#define IO5 
#define IO6 
#define IO7 
#define IO8 

#define OE ...     // And here your output enable / write enable pin assignment
#define WE 

#define maxSize ...     // Put in the maximum size of your memory in bytes (max. 2048)

*
*/

#ifndef cmos_h
#define cmos_h

#include "Arduino.h"


#ifndef l__output
    #define l__output "Serial"      // Standard output
#endif


#if !defined Ad0 || !defined Ad1 || !defined Ad2 || !defined Ad3 || !defined Ad4 || !defined Ad5 || !defined Ad6 || !defined Ad7 || !defined Ad8 || !defined Ad9 || !defined Ad10
    #define adError 1
#else
    #define adError 0
#endif

#if !defined IO1 || !defined IO2 || !defined IO3 || !defined IO4 || !defined IO6 || !defined IO7 || !defined IO8
    #define ioError 1
#else
    #define ioError 0
#endif

#if !defined OE || !defined WE
    #define owError 1
#else
    #define owError 0
#endif


#ifndef maxSize
    #define siError 1
#else
    #define siError 0
    #ifndef maxSizeInv
        #define maxSizeInv 0xFFFF-maxSize       // Number of free bits in adress
    #endif
#endif


class cmos {

    public:
        cmos ();
        boolean adressValidation (uint16_t __adress, String __process);
        void setAdress (uint16_t __adress);
        void writeByteToRAM (uint16_t __adress, char __data);
        char readByteFromRAM (uint16_t __adress);

    private:
        
        int _adress_bit[11];        // Including numbers of adress pins
        int _io_bit[9];             // Including numbers of data pins

};

#endif
