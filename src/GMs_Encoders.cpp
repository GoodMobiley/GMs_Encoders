#include "GMs_Encoders.h"

GM::Encoder::Encoder(uint16_t pin1, uint16_t pin2, int32_t dotsPerRev, float countsPerRev, bool inverted){
    this->dotsPerRev = dotsPerRev;      //Gimme dem vals
    this->fourDPR = 4*dotsPerRev;
    this->countsPerRev = countsPerRev;
    this->inverted = inverted;

    samplePeriod = 36000.0/dotsPerRev;  //period of reading rate of 2.5 (10/4) deg/s

    pinMode(pin1, INPUT_PULLUP);    //set pin modes
    pinMode(pin2, INPUT_PULLUP); 

    this->pin1 = pin1;  this->pin2 = pin2;

    state = (digitalRead(pin2)<<1) | digitalRead(pin1); //initializes the state var by punching pin 2's val over and inserting pin 1's val into first bit
    prevState = state;
}

void GM::Encoder::Tick(){
    state = (digitalRead(pin2)<<1) | digitalRead(pin1); //punches pin 2's val over and inserts pin 1's val into first bit

    if ((state&1) != (prevState&1)) //if first bit (Far right) changed
    if ((state&1) == (state>>1)){   //and the two bits in state are the same i.e. 00000011
        stepBackwards(); //rotates 1/4 dot backwards
    } else {                        //if two bits are different i.e. 00000010
        stepForwards(); //rotates 1/4 dot forwards
    }
    else if ((state>>1) != (prevState>>1)) //if first bit (Second to right) changed
    if((state&1) == (state>>1)){    //and the two bits in state are the same i.e. 00000011
        stepForwards(); //rotates 1/4 dot forwards
    } else {                        //if two bits are different i.e. 00000010
        stepBackwards(); //rotates 1/4 dot backwards
    }
    prevState = state;
    setAngle();

    uint32_t time = millis();

    if (time-prevTime > samplePeriod){  //If time since last sample has exceeded the sample time
        prevTime = 2*time - prevTime - samplePeriod;    //sets previous time with overhang taken into account
        velocity = float(angle-prevAngle)*1000.0/samplePeriod;    //solves for velocity (dp/dt)
        prevAngle = angle;
        acceleration = (velocity-prevVelocity)*1000.0/samplePeriod;
        prevVelocity = velocity;
    }
}

double GM::Encoder::getPosition(){return position*countsPerRev/fourDPR;}

float GM::Encoder::getAngle(){return angle*countsPerRev/fourDPR;}   //returns the angle in the given countsPerRevs

float GM::Encoder::getVelocity(){return velocity*countsPerRev/fourDPR;}   //returns velocity...

float GM::Encoder::getAcceleration(){return acceleration*countsPerRev/fourDPR;}    //returns acceleration...

int8_t GM::Encoder::getDirection(){return (velocity?(velocity>0?1:-1):0);} //if velocity is 0, return 0. Otherwise return 1 if velocity is positive and -1 if negative

uint8_t GM::Encoder::getState(){return state;}    //returns the raw state var

void GM::Encoder::setPosition(double newPosition){   //Sets a new angle for the encoder to read from
    this->position = newPosition*fourDPR/countsPerRev;    //convert from countsPerRevs to integer angle value
    setAngle();   //make sure angle is in bounds
    this->prevAngle = this->angle;    //reset previous angle so velocity doesn't flip out
}

void GM::Encoder::setCountsPerRev(float countsPerRev){this->countsPerRev = countsPerRev;}   //Sets the countsPerRev value to user's choice in counts per rev. EX: setcountsPerRev(360) would give 360 deg. per rev.

void GM::Encoder::setAngle(){ //keeps values in bounds. aligned ref used for variables ment to change with the wrapped variable
    while (position + wrapNum*fourDPR>fourDPR){     //keep subtracting from the wrap number until the angle is less than upper bounds
        --wrapNum;
        prevAngle -= fourDPR;   //keeps wrapping from impacting velocity
    } 
    while (position + wrapNum*fourDPR<0){        //keep adding to the wrap number until the angle is greater than 0
        ++wrapNum;
        prevAngle += fourDPR;   //keeps wrapping from impacting velocity
    }
    angle = position + wrapNum*fourDPR;    //sets angle to the position - offset (wrap*points per rev)
}
void GM::Encoder::stepForwards(){inverted?--position:++position;} //increment angle in forward direction
void GM::Encoder::stepBackwards(){inverted?++position:--position;} //decrement angle in backward direction