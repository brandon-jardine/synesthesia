/*
    Synesthesia

    Simple Arduino audio visualization designed for the Sparkfun spectrum
    shield: <https://www.sparkfun.com/products/13116> and a NeoPixel strip.

    © Brandon T. Jardine 2020

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <Syph_SmoothPot.h>

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

#define COLOR A2

#define VOLUME_ENABLE true
#define VOLUME A3

// initial base color
#define COLOR_START 54613

// sample once every x milliseconds
#define SAMPLE_DELAY 50

const int bandwidth = LED_COUNT / 15;
const int ch_offset = (bandwidth / 2) + (LED_COUNT / 2);

int frequenciesLeft[7];
int frequenciesRight[7];
int amplitudeReading[6];
int amplitude = 32;
int colorStart = COLOR_START;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Syph::SmoothPot colorPot(COLOR, 6);
#if VOLUME_ENABLE
    Syph::SmoothPot volumePot(VOLUME, 6);
#endif

void setup() {
    #ifdef DEBUG
        Serial.begin(9600);
    #endif

    strip.begin();
    strip.setBrightness(128);
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
    colorStart = colorPot.Read() * 64;

    #if VOLUME_ENABLE
        amplitude = volumePot.Read() >> 3;
    #endif

    frequencyGraph();
    delay(SAMPLE_DELAY);
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
        int lAmplitude = frequenciesLeft[6 - i];
        int rAmplitude = frequenciesRight[i];
        lAmplitude *= amplitude;
        rAmplitude *= amplitude;

        uint16_t l_hue = colorStart - lAmplitude;
        uint16_t r_hue = colorStart - rAmplitude;

        uint32_t lColor = strip.gamma32(strip.ColorHSV(l_hue));
        uint32_t rColor = strip.gamma32(strip.ColorHSV(r_hue));

        for (int n = 0; n < bandwidth; ++n) {
            strip.setPixelColor((i * bandwidth) + n, lColor);
            strip.setPixelColor((i * bandwidth) + n + ch_offset, rColor);
        }

        if (i == 6) {
            for (int n = 0; n < (bandwidth / 2); ++n) {
                strip.setPixelColor(6 * bandwidth + (bandwidth + n), lColor);
            }
        }

        if (i == 0) {
            for (int n = 1; n <= (bandwidth / 2); ++n) {
                strip.setPixelColor(ch_offset - n, rColor);
            }
        }
    }

    strip.show();
}
