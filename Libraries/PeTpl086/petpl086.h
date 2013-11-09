
#ifndef __PETPL086_H__
#define __PETPL086_H__ 1
#include <arduino.h>

class PeTpl086
{
public:
    //Message<uint64_t, uint64_t, uint16_t, uint16_t, uint16_t, Buffer<uint16_t,8> >
    struct Sample{ //one sample returned by read
                   //uint64_t ts; /* Those should belong to the timeseries */
                   //        uint64_t id; /* sequence id **/
        uint16_t vamb, vref, vpix[8];
    };
    
private:
    uint8_t analogInputPin, sampleChiPin, resetPin;
    
private:
    
    //we could map the input signal via voltage divider to the range of 0 to 1.1 Volts
    inline
    static
    uint16_t ReadAnalogInVoltage(uint8_t pin){
        //signal should stay activ for atleast 270-400 my and analogRead takes 100 us... so we can sample twice?!?!
        return map( (analogRead(pin)+analogRead(pin))>>1 , 0, 1024, 0, 5000);
    };

    inline
    static
    void WaitForRisingEdge(uint8_t pin){
        uint8_t pinState;
        pinState = -1; //assume high, since want it to low and need to read at least onexs
                       //we expect the pin wait for rising edge
        while(pinState != LOW) pinState = digitalRead(pin); //wait for it to be or stay low
        while(pinState != HIGH) pinState = digitalRead(pin);
    }
public:
    
    inline void Init( int AnalogInputPin = 0, //the analogInput Pin
                     int SampleChiPin = 2, //
                     int ResetPin = 11    //reset pin in order to control the thermopile
    ){
        sampleChiPin = SampleChiPin;
        analogInputPin = AnalogInputPin;
        resetPin = ResetPin;
        /** Configure TPL **/
        pinMode(resetPin, OUTPUT);
        digitalWrite(resetPin, LOW);
        pinMode(sampleChiPin, INPUT);
    }
    
    
    inline
    int Read(struct Sample& sample){ //Read sample using polling ... we could call it poll
        analogReference(DEFAULT); //set reference to default
        digitalWrite(resetPin, LOW); //pull low such that the thermopile outputs data
        delayMicroseconds(20); //delay 2 µs
        sample.vref = ReadAnalogInVoltage( analogInputPin); //read analog voltage
        digitalWrite(resetPin, HIGH); // now pull up and read vamb
        delayMicroseconds(20); //delay 2 ms //wait for stable output
        sample.vamb = ReadAnalogInVoltage( analogInputPin);
        WaitForRisingEdge(sampleChiPin);
        delayMicroseconds(20);
        //read dummy pixellastSampleTimeInMillis
        for (int i=0;i<8;i++){
            WaitForRisingEdge(sampleChiPin); //
            delayMicroseconds(20); //delay <20 µs
            sample.vpix[i] = ReadAnalogInVoltage(analogInputPin);
        }
        digitalWrite(resetPin, LOW); //pull reset low to stop sampling
        return (0); //EXIT_SUCCESS
    }
    
};


#endif