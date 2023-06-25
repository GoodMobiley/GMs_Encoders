#include <Arduino.h>

#ifndef GM_ENCODERS_H
#define GM_ENCODERS_H
namespace GM
{
    class Encoder{
        public:
                Encoder(uint16_t pin1, uint16_t pin2, int32_t dotsPerRev, float countsPerRev=360, bool inverted=false);
         void   tick();

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

        private:
         bool           inverted;
         uint8_t        state;
         const uint16_t pin1, pin2;
         int32_t        angle=0, prevAngle=0, wrapNum=0;
         const int32_t dotsPerRev, fourDPR;
         int64_t        position=0;
         float          countsPerRev=360, 
                        velocity=0, prevVelocity=0, acceleration=0;
         const float    samplePeriod;
         double         prevTime=0;

         void setAngle();
         void stepForwards();
         void stepBackwards();
    };
} // namespace GM
#endif
