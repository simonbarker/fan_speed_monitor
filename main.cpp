#include "mbed.h"

AnalogIn input(p20);
DigitalOut led1(LED1);
Serial pc(USBTX, USBRX); // tx, rx
Timer t;

int main() {
    float sample;
    bool newDetection = true;   //need this as multiple reads happen across white spot
    int samples[120];           //higher number = greater accuracy but longer read time
    int sampleCount = 0;
        
    pc.printf("Starting:\n\r"); 

    //start initial timer
    t.start();

    while(1) {
        //poll analogue in
        sample = input.read();
        if(sample < 0.1 && newDetection == true){       //tune 0.1 to appropriate level
            //detected white spot so stop timer
            samples[sampleCount] = t.read_us();
            t.reset();
            //reset flag
            newDetection = false;
            sampleCount++;
            
            //change for shorter/longer read times - must be less than array length (set in ling 11)
            if(sampleCount == 100){
                //total up time and average across number of readings taken
                int total = 0;
                for(int i = 1; i < 99; i++){    //start at 1 as the first one will be a junk reading
                    total += samples[i];
                }
                float ave = total/98;
                float rpm = (1000000/ave)*60;   //convert from us to rpm
                pc.printf("RPM = %f\r\n",rpm);
                sampleCount = 0;
            }
        }
        else if(sample > 0.9){      //tune 0.9 to appropriate level
            newDetection = true;
        }
    }
}