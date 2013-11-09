
#ifndef __PETPL086_H__
#define __PETPL086_H__ 1


class PeTpl086
{
public:
    //Message<uint64_t, uint64_t, uint16_t, uint16_t, uint16_t, Buffer<uint16_t,8> >
    struct Sample{ //one sample returned by read
        uint64_t ts;
        uint64_t id;
        uint16_t vamb, vref, vpix[8];
    };
    
private:
    uint32_t id ;
    uint8_t analogInputPin, sampleChiPin, resetPin;
public:
    
    inline void Init( int analogInputPin = 0, //the analogInput Pin
                     int sampleChiPin = 2, //
                     int resetPin = 11    //reset pin in order to control the thermopile
    ){
        id = 0;
        /** Configure TPL **/
        pinMode(resetPin, OUTPUT);
        digitalWrite(resetPin, LOW);
        
        pinMode(sampleChiPin, INPUT);
    }
    
    /* Routine that should be called if an interrupt occured
     uint8_t _sampleCnt; //should be called on interrupt
     inline void OnSamplePinStateChanged(){
     if (_sampleCnt >0)
     sample->vpix[_sampleCnt-1] = ReadAnalogInVoltage(SamplePin);
     _sampleCnt++;
     }
     **/
    
    //we could map the input signal via voltage divider to the range of 0 to 1.1 Volts
    inline
    uint16_t ReadAnalogInVoltage(uint8_t pin){
        return map( (analogRead(pin)+analogRead(pin))>>1 , 0, 1024, 0, 5000); //sampling takes 100us and we have >200 so sample twice and average
    };
    
    
    inline
    void WaitForRisingEdge(uint8_t pin){
        uint8_t pinState;
        pinState = HIGH; //assume high, since want it to low and need to read at least onexs
        while(pinState != LOW) pinState = digitalRead(pin); //wait for it to be or stay low
        while(pinState != HIGH) pinState = digitalRead(pin);
    }
    
    inline
    int Read(struct Sample *sample){ //Read sample using polling ... we could call it poll
        analogReference(DEFAULT); //set reference to default
        uint64_t curSampleTimeInMillis = millis();
        sample->ts = curSampleTimeInMillis; //the sampling time... it may overflow
        sample->id = id++; //sample number
        
        digitalWrite(resetPin, LOW); //pull low such that the thermopile outputs data
        delayMicroseconds(20); //delay 2 µs
        sample->vref = ReadAnalogInVoltage( analogInputPin); //read analog voltage
        digitalWrite(resetPin, HIGH); // now pull up and read vamb
        
        delayMicroseconds(20); //delay 2 ms //wait for stable output
        sample->vamb = ReadAnalogInVoltage( analogInputPin);
        WaitForRisingEdge(sampleChiPin);
        delayMicroseconds(20);
        //read dummy pixellastSampleTimeInMillis
        for (int i=0;i<8;i++){
            WaitForRisingEdge(sampleChiPin); //
            delayMicroseconds(20); //delay <20 µs
            sample->vpix[i] = ReadAnalogInVoltage(analogInputPin); //signal should stay activ for atleast 270-400 my and analogRead takes 100 us... so we can sample twice?!?!
        }
        digitalWrite(resetPin, LOW); //pull reset low to stop sampling
        return (0); //EXIT_SUCCESS
        
        /** old interrupt code
         
         SampleBufferPtr = sample->vpix;
         SampleCnt = 0;
         SamplePin = analogInputPin; //set
         attachInterrupt( sampleChiPin - 2, onInterrupt, RISING); //attach the interrupt routine
         while (SampleCnt <=8) (void)0; //wait until n samples a read
         digitalWrite(resetPin, LOW);
         **/
        
    }
    
};


#endif
