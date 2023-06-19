#include <Arduino.h>
#include <math.h>

#ifndef GM_ENCODERS_H
#define GM_ENCODERS_H
namespace GM
{
    class Encoder{
        private:
         bool       inverted;
         uint8_t    state, prevState;
         uint16_t   pin1, pin2;
         int32_t    dotsPerRev, fourDPR, 
                    angle=0, prevAngle=0, wrapNum=0;
         int64_t    position=0;
         float      samplePeriod, countsPerRev=360, 
                    velocity=0, prevVelocity=0, acceleration=0;
         double     prevTime=0;

         void setAngle();
         void stepForwards();
         void stepBackwards();
         
        public:
                Encoder(uint16_t pin1, uint16_t pin2, int32_t dotsPerRev, float countsPerRev=360, bool inverted=false);
         void   Tick();

         double     getPosition();
         int8_t     getDirection();
         float      getAngle();
         float      getVelocity();
         float      getAcceleration();

         uint8_t    getState();
         int32_t    getDotsPerRev();
         float      getCountsPerRev();
         bool       getInverted();

         void   setPosition(double newPosition);
         void   setCountsPerRev(float countsPerRev);
         void   setInverted(bool inverted);
    };
} // namespace GM
#endif
