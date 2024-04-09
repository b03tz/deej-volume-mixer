#include "SliderLEDControl.h"

SliderLEDControl::SliderLEDControl(int sensorPin, int dataPin, bool debug) :
    sensorPin(sensorPin),
    debug(debug),
    strip(12, dataPin, NEO_GRB + NEO_KHZ800)
{
    pinMode(sensorPin, INPUT);
    memset(readings, 0, sizeof(readings)); // Initialize all readings to 0
    strip.setBrightness(brightness);
    strip.clear();
}

void SliderLEDControl::setColors(
        int r1, int g1, int b1,
        int r2, int g2, int b2,
        int r3, int g3, int b3
) {
    section1R = r1;
    section1G = g1;
    section1B = b1;
    section2R = r2;
    section2G = g2;
    section2B = b2;
    section3R = r3;
    section3G = g3;
    section3B = b3;
}

int SliderLEDControl::getAverageValue() {
    return averageValue;
}

void SliderLEDControl::animate() {
    showLights(animationFrame);

    if (!direction) {
        animationFrame += 25;
        if (animationFrame > 1023) {
            direction = !direction;
        }
    } else {
        animationFrame -= 25;
        if (animationFrame <= 0) {
            direction = !direction;
        }
    }
}

void SliderLEDControl::update() {
    int sensorValue = analogRead(sensorPin);

    // Subtract the oldest reading, add the new one
    total = total - readings[readIndex];
    readings[readIndex] = sensorValue;
    total = total + readings[readIndex];
    readIndex = (readIndex + 1) % NUM_READINGS;

    averageValue = total / NUM_READINGS;

    showLights(averageValue);
}

void SliderLEDControl::showLights(int value) {
    // Determine how many LEDs should be on based on finalValue
    int lightsOn = map(value, 0, 1020, 0, NUM_LEDS_PER_SLIDER);

    // Determine brightness of next LED
    float oneSection = 1023.0 / NUM_LEDS_PER_SLIDER;
    int nextLedBrightness = map(value - (lightsOn * oneSection), 0, oneSection, 0, 100);

    if (debug) {
        Serial.print("Pin: ");
        Serial.print(sensorPin);
        Serial.print(", Average: ");
        Serial.print(value);
        Serial.print(", Next: ");
        Serial.print(nextLedBrightness);
        Serial.print(", Lights on: ");
        Serial.println(lightsOn);
    }

    // Set the LEDs for this segment
    for(int i = 0; i < NUM_LEDS_PER_SLIDER; i++) {
        if (i < 0 + lightsOn) {
            if (i < 3) {
                strip.setPixelColor(i, section1R, section1G, section1B);
            } else if (i < 8) {
                strip.setPixelColor(i, section2R, section2G, section2B);
            } else {
                strip.setPixelColor(i, section3R, section3G, section3B);
            }
        } else {
            strip.setPixelColor(i, 0, 0, 0);
        }
    }

    // Fade the next LED based on slider position
    if (lightsOn != 12) {
        if (lightsOn < 3) {
            strip.setPixelColor(lightsOn, map(nextLedBrightness, 0, 100, 0, section1R), map(nextLedBrightness, 0, 100, 0, section1G), map(nextLedBrightness, 0, 100, 0, section1B));
        } else if (lightsOn < 8) {
            strip.setPixelColor(lightsOn, map(nextLedBrightness, 0, 100, 0, section2R), map(nextLedBrightness, 0, 100, 0, section2G), map(nextLedBrightness, 0, 100, 0, section2B));
        } else {
            strip.setPixelColor(lightsOn, map(nextLedBrightness, 0, 100, 0, section3R), map(nextLedBrightness, 0, 100, 0, section3G), map(nextLedBrightness, 0, 100, 0, section3B));
        }
    }

    strip.show();
}

void SliderLEDControl::updateZeroVolumeTimes() {
    zeroVolumeTimes[0] = zeroVolumeTimes[1]; // Move the older timestamp back
    zeroVolumeTimes[1] = millis(); // Update the latest timestamp to now
}

bool SliderLEDControl::checkForDoubleZero() {
    int sensorValue = averageValue;

    // Check if the current reading is zero
    if (sensorValue == 0) {
        if (!wasLastZero) { // If the last reading wasn't zero, consider this a new "zero volume" event
            updateZeroVolumeTimes(); // Update the zeroVolumeTimes array
            wasLastZero = true; // Set the flag since we've now seen a zero
        }
    } else if (sensorValue > 10) {
        wasLastZero = false; // Reset the flag since we have a non-zero reading
    }

    // Check if two "zero volume" events occurred within the last 1 second
    if (zeroVolumeTimes[0] > 0 && // Ensure a zero event has been recorded
        zeroVolumeTimes[1] - zeroVolumeTimes[0] <= 1000) { // 1000 milliseconds = 1 second

        // Reset it
        zeroVolumeTimes[0] = 0;
        zeroVolumeTimes[1] = 0;

        return true; // Detected two zero volume events within 1 second
    }

    return false; // No double zero volume detected
}

void SliderLEDControl::setBrightness(int newValue) {
    this->brightness = newValue;
    strip.setBrightness(this->brightness);
}
