#ifndef __SLIP_H__
#define __SLIP_H__ 1
#include <arduino.h>
/** Slip **/
class Slip {
private:
    static const char END =0xC0;
    static const char ESC =0xdb;
    static const char ESC_END = 0xdc;
    static const char ESC_ESC = 0xdd;
private:
    Stream& stream;
public:
    Slip(Stream& stream)
    :stream(stream){
    }
    
    inline
    int write(char p){
        int bytesWritten = 0;
        switch(p) {
            case END:
                bytesWritten += stream.write(ESC);
                bytesWritten += stream.write(ESC_END);
                break;
            case ESC:
                bytesWritten += stream.write(ESC);
                bytesWritten += stream.write(ESC_ESC);
                break;
            default:
                bytesWritten += stream.write(p);
        }
        return bytesWritten;
    }
    
    inline
    int write( char* p, int len){
        int bytesWritten = 0;
        while(len--)
            bytesWritten += write(*p++);
        return bytesWritten;
    }
    
    inline
    void begin(){
        stream.write(END);
    }
    inline
    void end(){
        stream.write(END);
        stream.flush(); //prior to Arduino 1.0 this flushes = emptys the buffer.. now it commits the data
    }
    
    inline
    int available(){
        return stream.available();
    }
    
    inline
    int read(char* p, size_t len){
        int data=-1;
        int c;
        int received = 0;
        while(1) {
            do { data = stream.read(); } while (data==-1);
            c=data;
            switch(c) {
                case END:
                    if(received)
                        return received;
                    else
                        break; //retry
                case ESC:
                    do { data = stream.read(); } while (data==-1);
                    c=data;
                    switch(c) {
                        case ESC_END:
                            c = END;
                            break;
                        case ESC_ESC:
                            c = ESC;
                            break;
                    }
                default:
                    if(received < len)
                        p[received++] = c;
            }
            
        }
        return (-1);
    };
    

    
};
#endif