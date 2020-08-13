#ifndef Queue_h
#define Queue_h

#include "Option.h"

template <typename T>
class Queue
{
    public: 
        Queue(int size) {
            _arr = new T[size];
            _capacity = size;
            _front = 0;
            _rear = -1;
            _count = 0;
        }

        ~Queue() {
            delete _arr;
        }

        Option<T> push(T value) {
            if (isFull()) {
                return Option<T>::Empty;
            }

            _rear = (_rear + 1) % _capacity;
            _arr[_rear] = value;
            _count++;

            return Option<T>(value);
        }

        Option<T> pull() {
            if (isEmpty()) {
                return Option<T>::Empty;
            }

            T value = _arr[_front];
            _front = (_front + 1) % _capacity;
            _count--;

            return Option<T>(value);
        }

        Option<T> peak() {
            if (isEmpty()) {
                return Option<T>::Empty;
            }

            return Option<T>(_arr[_front]);
        }

        int capacity() {
            return _capacity;
        }

        int size() {
            return _count;
        }

        bool isEmpty() {
            return (size() == 0);
        }

        bool isFull() {
            return (size() == _capacity);
        }
    protected:
        T* _arr;
    private:
        int _capacity;
        int _front;
        int _rear;
        int _count;
};

#endif
