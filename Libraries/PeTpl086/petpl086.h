#ifndef __PETPL086_H__
#define __PETPL086_H__ 1
struct TplSample{
    uint32_t ts;
    uint32_t id;
    uint16_t vamb, vref, vpix[8];
};


//we could map the input signal via voltage divider to the range of 0 to 1.1 Volts
inline
uint16_t ReadAnalogInVoltage(uint8_t pin){ return map( analogRead(pin), 0, 1024, 0, 5000); };

class PeTpl086
{
    
    volatile static uint8_t SampleCnt; //this is only the definition
    volatile static uint16_t* SampleBufferPtr;
    volatile static uint8_t SamplePin;
    static void onInterrupt(){ //data can be passed by intterupt_params = void* for each interrupt
                               //first sample actually samples something different
        if (SampleCnt >0)
            SampleBufferPtr[SampleCnt-1] = ReadAnalogInVoltage(SamplePin);
        SampleCnt++;
    };
private:
    uint32_t id ;
    uint8_t analogInputPin, sampleChiPin, resetPin, powerPin, gndPin;
public:
    
    inline void Init( int analogInputPin = 0, //the analogInput Pin
                      int sampleChiPin = 2, //should be connected to an interrupt pin!!
                      int resetPin = 11,    //reset pin in order to control the thermopile
                      int powerPin = 15,
                      int gndPin   = 12){
        id = 0;
        pinMode(powerPin,OUTPUT);
        pinMode(gndPin,OUTPUT);
        digitalWrite(gndPin,LOW);    //put to low
        digitalWrite(powerPin,HIGH); //enable powerpin
        /** Configure TPL **/
        pinMode(resetPin, OUTPUT);
        pinMode(sampleChiPin, INPUT);
        /** Set TPL in RESET **/
        digitalWrite(resetPin, LOW);
    }
    
    inline void Read(struct TplSample *sample){
        analogReference(DEFAULT); //set reference to default
        sample->ts = millis();
        sample->id = id++;
        
        digitalWrite(resetPin, LOW); //pull low such that the thermopile outputs data
        attachInterrupt( sampleChiPin - 2, onInterrupt, RISING); //attach the interrupt routine
        delayMicroseconds(2); //delay 2 ms
        sample->vref = ReadAnalogInVoltage( analogInputPin); //read analog voltage
        digitalWrite(resetPin, HIGH);
        sample->vamb = ReadAnalogInVoltage( analogInputPin);
        
        SampleBufferPtr = sample->vpix;
        SampleCnt = 0;
        SamplePin = analogInputPin; //set
        attachInterrupt( sampleChiPin - 2, onInterrupt, RISING); //attach the interrupt routine
        while (SampleCnt <=8) (void)0; //wait until n samples a read
        digitalWrite(resetPin, LOW);
        detachInterrupt( sampleChiPin - 2); //attach the interrupt routine
        
    }
    
};

/** Init the corresponding static values **/

volatile uint8_t
PeTpl086::SampleCnt=0;
volatile uint16_t*
PeTpl086::SampleBufferPtr;
volatile uint8_t
PeTpl086::SamplePin=0;



#endif