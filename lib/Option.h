#ifndef Option_h
#define Option_h

template <typename T>
class Option
{
    public:
        const static Option<T> Empty;


        Option<T>(T value) : _value(&value) {

        }

        bool HasValue() {
            return _value != nullptr;
        }

        bool IsEmpty() {
            return _value == nullptr;
        }

        T Value() {
            if (_value == nullptr) {
                // fuck
                exit(69); // nice
            }

            return *_value;
        }

    private:
        Option<T>() : _value(nullptr) { }
        const T* _value;
};

template <typename T>
const Option<T> Option<T>::Empty = Option<T>();

#endif
