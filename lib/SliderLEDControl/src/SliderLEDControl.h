#ifndef VOLUMEMIXER_SLIDERLEDCONTROL_H
#define VOLUMEMIXER_SLIDERLEDCONTROL_H

#include <Adafruit_NeoPixel.h>

#define NUM_LEDS_PER_SLIDER 12 // Number of LEDs each slider controls
#define NUM_READINGS 3 // Number of readings for smoothing

class SliderLEDControl {
private:
    bool debug;
    int sensorPin; // Analog pin to read the slider
    int readings[NUM_READINGS]; // Circular buffer for storing sensor readings
    int readIndex = 0; // Current position in the buffer
    int total = 0; // Sum of the readings for averaging
    int averageValue = 0;
    int brightness = 35;
    int section1R = 0;
    int section1G = 149;
    int section1B = 255;
    int section2R = 153;
    int section2G = 0;
    int section2B = 255;
    int section3R = 255;
    int section3G = 0;
    int section3B = 0;
    int animationFrame = 0;
    int direction = false;
    Adafruit_NeoPixel strip;

public:
    SliderLEDControl(int sensorPin, int dataPin, bool debug = false);

    void setColors(
            int r1, int g1, int b1,
            int r2, int g2, int b2,
            int r3, int g3, int b3
    );

    int getAverageValue();

    void animate();

    void update();

    void showLights(int value);
};


#endif //VOLUMEMIXER_SLIDERLEDCONTROL_H
