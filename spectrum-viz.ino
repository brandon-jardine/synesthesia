#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define DEBUG

#define LED_PIN 6
#define LED_COUNT 60

#define STROBE 4
#define RESET 5
#define DATA_LEFT A0
#define DATA_RIGHT A1

#define COLOR_START 54613
#define SAMPLE_DELAY 50

int frequenciesLeft[7];
int frequenciesRight[7];

const int bandwidth = LED_COUNT / 15;
const int ch_offset = (bandwidth / 2) + (LED_COUNT / 2);

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    #ifdef DEBUG
        Serial.begin(9600);
    #endif

    strip.begin();
    strip.setBrightness(50);
    strip.show();

    pinMode(STROBE, OUTPUT);
    pinMode(RESET, OUTPUT);
    pinMode(DATA_LEFT, INPUT);
    pinMode(DATA_RIGHT, INPUT);

    digitalWrite(STROBE, HIGH);
    digitalWrite(RESET, HIGH);

    digitalWrite(STROBE, LOW);
    delay(1);
    digitalWrite(RESET, HIGH);
    delay(1);
    digitalWrite(STROBE, HIGH);
    delay(1);
    digitalWrite(STROBE, LOW);
    delay(1);
    digitalWrite(RESET, LOW);
}

void loop() {
    frequencyRead();
    frequencyGraph();
    delay(SAMPLE_DELAY);
}

void frequencyRead() {
    for (int i = 0; i < 7; ++i) {
        frequenciesLeft[i] = (analogRead(DATA_LEFT) + analogRead(DATA_RIGHT)) >> 1;
        frequenciesRight[i] = (analogRead(DATA_LEFT) + analogRead(DATA_RIGHT)) >> 1;
        digitalWrite(STROBE, HIGH);
        digitalWrite(STROBE, LOW);
        delayMicroseconds(40);
    }
}

void frequencyGraph() {
    strip.clear();

    for (int i = 0; i < 7; ++i) {
        int avg = frequenciesLeft[6 - 1];
        avg *= 64;
        uint32_t color = strip.gamma32(strip.ColorHSV(0 - avg));

        for (int n = 0; n < bandwidth; ++n) {
            strip.setPixelColor((i * bandwidth) + n, color);
        }
    }

    for (int i = 0; i < 7; ++i) {
        int avg = frequenciesRight[i];
        avg *= 64;
        uint32_t color = strip.gamma32(strip.ColorHSV(0 - avg));

        for (int n = 0; n < bandwidth; ++n) {
            strip.setPixelColor((i * bandwidth) + n + ch_offset, color);
        }
    }

    strip.show();
}
