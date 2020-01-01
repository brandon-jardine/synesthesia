#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// uncomment to bring debug output to serial
// #define DEBUG true

// neopixel data line & led count
#define LED_PIN 6
#define LED_COUNT 60

// spectrum shield pin definitions
#define STROBE 4
#define RESET 5
#define DATA_LEFT A0
#define DATA_RIGHT A1

// initial base color
#define COLOR_START 54613

// sample once every x milliseconds
#define SAMPLE_DELAY 50

const int bandwidth = LED_COUNT / 15;
const int ch_offset = (bandwidth / 2) + (LED_COUNT / 2);

int frequenciesLeft[7];
int frequenciesRight[7];
int colorStart = COLOR_START;

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
    int count = 0;

    frequencyRead();
    frequencyGraph();
    delay(SAMPLE_DELAY);

    if (++count > 1200) {
        #ifdef DEBUG
            Serial.write("Randomizing base color value.");
        #endif

        count = 0;
        colorStart = random(65535);
    }
}

void frequencyRead() {
    for (int i = 0; i < 7; ++i) {
        frequenciesLeft[i] = (analogRead(DATA_LEFT) + analogRead(DATA_LEFT)) >> 1;
        frequenciesRight[i] = (analogRead(DATA_RIGHT) + analogRead(DATA_RIGHT)) >> 1;
        digitalWrite(STROBE, HIGH);
        digitalWrite(STROBE, LOW);
        delayMicroseconds(40);
    }

    #ifdef DEBUG
        Serial.print("left: ");
        for (int i = 0; i < 7; ++i) {
            Serial.print(frequenciesLeft[i]);
            Serial.print(" ");
        }
        Serial.print("\n");
        Serial.print("right: ");
        for (int i = 0; i < 7; ++i) {
            Serial.print(frequenciesRight[i]);
            Serial.print(" ");
        }
        Serial.print("\n");
    #endif
}

void frequencyGraph() {
    strip.clear();

    for (int i = 0; i < 7; ++i) {
        int amplitude = frequenciesLeft[6 - i];
        amplitude *= 64;
        uint32_t color = strip.gamma32(strip.ColorHSV(colorStart - amplitude));

        for (int n = 0; n < bandwidth; ++n) {
            strip.setPixelColor((i * bandwidth) + n, color);
        }
    }

    for (int i = 0; i < 7; ++i) {
        int amplitude = frequenciesRight[i];
        amplitude *= 64;
        uint32_t color = strip.gamma32(strip.ColorHSV(colorStart - amplitude));

        for (int n = 0; n < bandwidth; ++n) {
            strip.setPixelColor((i * bandwidth) + n + ch_offset, color);
        }
    }

    strip.show();
}
