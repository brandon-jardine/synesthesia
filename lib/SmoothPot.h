/*
    SmoothPot.h - Library for smoothing potentiometer input

    Created by Brandon T. Jardine, 2020
*/

#ifndef SmoothPot_h
#define SmoothPot_h

#include <Arduino.h>

#include "Samples.h"

class SmoothPot
{
    public:
        SmoothPot(int pin) {
            pinMode(pin, INPUT);
            _pin = pin;

            for (int i = 0; i < _samples.size(); ++i) {
                _samples.push(digitalRead(_pin));
            }
        }

        int Read() {
            int value = digitalRead(_pin);
            _samples.pull();
            _samples.push(value);

            return _samples.mean();
        }
    private:
        int _pin;
        Samples _samples = Samples(6);
};

#endif