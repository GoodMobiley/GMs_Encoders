#include <GMs_Encoders.h>

GM::Encoder myEncoder{3, 4, 600, 2*PI, false}; //(Pin 1, Pin 2, Dots Per Revolution, Counts Per Revloution, Inverted)

void setup()
{
    Serial.begin(115200);
    myEncoder.setInverted(true);    //flip the positive direction of rotation
    myEncoder.setPosition(50);      //sets current position of encoder *Note, based off of counts per revolution
    myEncoder.setCountsPerRev(360); //i.e. setCountsPerRev(360) ==> 360 degrees per revolution
}

int64_t prevTime = 0;   //to find total time passed
String numGap = " | ";  //separates values printed

void loop()
{
	myEncoder.tick();   //function placed in a loop to allow the arduino to check for encoder state changes

    uint32_t time = millis();   //gets the time in milliseconds since arduino was turned on
    uint16_t delay = 100;   //time between each serial print
                            //added a delay between prints because sending data tends to take some time.
                            //This can get in the way of the encoder and cause it to skip states.

    if (time - prevTime > delay){   //if time passed since last print is greater than delay variable
        prevTime = 2*time - prevTime - delay;   //resets previous time with overhang

        Serial.println(String(myEncoder.getPosition())   +numGap+      //print out information
                       String(myEncoder.getAngle())      +numGap+
                       String(myEncoder.getDirection())  +numGap+
                       String(myEncoder.getVelocity())   +numGap+
                       String(myEncoder.getAcceleration()));
    }
}
