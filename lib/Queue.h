#ifndef Queue_h
#define Queue_h

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

        void push(T value) {
            if (isFull()) {
                exit(2);
            }

            _rear = (_rear + 1) % _capacity;
            _arr[_rear] = value;
            _count++;
        }

        T pull() {
            if (isEmpty()) {
                exit(1);
            }

            T value = _arr[_front];
            _front = (_front + 1) % _capacity;
            _count--;

            return value;
        }

        T peak() {
            if (isEmpty()) {
                exit(3);
            }

            return _arr[_front];
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
        T *_arr;
    private:
        int _capacity;
        int _front;
        int _rear;
        int _count;
};

#endif
