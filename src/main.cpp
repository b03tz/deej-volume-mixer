#include <Arduino.h>
#include "SliderLEDControl.h"

// DEEJ SETUP
const int NUM_SLIDERS = 5;
int analogSliderValues[NUM_SLIDERS];
unsigned long lastSent = 0;
// END DEEJ SETUP

// Change the sensorPins (first argument, analog slider inputs) and dataPins (2nd argument, LED strip data output) to your configuration
SliderLEDControl slider1(A4, 6, false);
SliderLEDControl slider2(A3, 5, false);
SliderLEDControl slider3(A2, 4, false);
SliderLEDControl slider4(A1, 3, false);
SliderLEDControl slider5(A0, 2, false);

void sendSliderValues() {
    String builtString = String("");

    for (int i = 0; i < NUM_SLIDERS; i++) {
        builtString += String((int)analogSliderValues[i]);

        if (i < NUM_SLIDERS - 1) {
            builtString += String("|");
        }
    }

    Serial.println(builtString);
}

void setup() {
    Serial.begin(9600);

    // Configure datapins below (for LED strips output)
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    slider3.setColors(
            255, 255, 255,
            88, 101, 242,
            40, 57, 249
    );

    slider4.setColors(
            255, 255, 255,
            30, 215, 96,
            0, 114, 41
    );

    slider5.setColors(
            0, 150, 0,
            255, 255, 0,
            255, 0, 0
    );
}

void loop() {
    // Always update sliders
    slider1.update();
    slider2.update();
    slider3.update();
    slider4.update();
    slider5.update();

    // Only send every 10ms
    if (millis() - lastSent > 10) {
        analogSliderValues[0] = slider1.getAverageValue();
        analogSliderValues[1] = slider2.getAverageValue();
        analogSliderValues[2] = slider3.getAverageValue();
        analogSliderValues[3] = slider4.getAverageValue();
        analogSliderValues[4] = slider5.getAverageValue();
        sendSliderValues();
        lastSent = millis();
    }
}