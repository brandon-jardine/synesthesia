#ifndef Samples_h
#define Samples_h

#include "Queue.h"

class Samples : public Queue<int>
{
    public:
        Samples(int size) : Queue<int>(size) { }

        int mean() {
            int s = size();
            int acc = 0;

            for (int i = 0; i < s; ++i) {
                acc += _arr[i];
            }

            return acc / s;
        }
};

#endif
