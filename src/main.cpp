#include <Arduino.h>
#include <EEPROM.h>
#include "SliderLEDControl.h"

// DEEJ SETUP
const int NUM_SLIDERS = 5;
int analogSliderValues[NUM_SLIDERS];
unsigned long lastSent = 0;
// END DEEJ SETUP

int currentTheme = 0;
int brightness = 10;
int maxBrightness = 50;

// Change the sensorPins (first argument, analog slider inputs) and dataPins (2nd argument, LED strip data output) to your configuration
SliderLEDControl sliders[5] = {
    SliderLEDControl(A4, 6, false),
    SliderLEDControl(A3, 5, false),
    SliderLEDControl(A2, 4, false),
    SliderLEDControl(A1, 3, false),
    SliderLEDControl(A0, 2, false)
};

// Configure your themes here
const char* themes[7][5][3] {
    // Theme 1
    {
        {"#0095ff", "#9900ff", "#ff0000"},  // slider 1 sections
        {"#0095ff", "#9900ff", "#ff0000"},  // slider 2 sections
        {"#0095ff", "#9900ff", "#ff0000"},  // slider 3 sections
        {"#0095ff", "#9900ff", "#ff0000"},  // slider 4 sections
        {"#0095ff", "#9900ff", "#ff0000"}   // slider 5 sections
    },
    // Theme 2
    {
        {"#0095ff", "#9900ff", "#ff0000"},  // slider 1 sections
        {"#0095ff", "#9900ff", "#ff0000"},  // slider 2 sections
        {"#ffffff", "#5865f2", "#2839f9"},  // slider 3 sections
        {"#ffffff", "#1ed760", "#007229"},  // slider 4 sections
        {"#009600", "#ffff00", "#ff0000"}   // slider 5 sections
    },
    // Theme 3: all red
    {
        {"#FF0000", "#FF0000", "#FF0000"},  // slider 1 sections
        {"#FF0000", "#FF0000", "#FF0000"},  // slider 2 sections
        {"#FF0000", "#FF0000", "#FF0000"},  // slider 3 sections
        {"#FF0000", "#FF0000", "#FF0000"},  // slider 4 sections
        {"#FF0000", "#FF0000", "#FF0000"}   // slider 5 sections
    },
    // Theme 4: all white
    {
        {"#FFFFFF", "#FFFFFF", "#FFFFFF"},  // slider 1 sections
        {"#FFFFFF", "#FFFFFF", "#FFFFFF"},  // slider 2 sections
        {"#FFFFFF", "#FFFFFF", "#FFFFFF"},  // slider 3 sections
        {"#FFFFFF", "#FFFFFF", "#FFFFFF"},  // slider 4 sections
        {"#FFFFFF", "#FFFFFF", "#FFFFFF"}   // slider 5 sections
    },
    // Theme 5: all blue
    {
        {"#0066ff", "#0066ff", "#0066ff"},  // slider 1 sections
        {"#0066ff", "#0066ff", "#0066ff"},  // slider 2 sections
        {"#0066ff", "#0066ff", "#0066ff"},  // slider 3 sections
        {"#0066ff", "#0066ff", "#0066ff"},  // slider 4 sections
        {"#0066ff", "#0066ff", "#0066ff"}   // slider 5 sections
    },
    // Theme 6: traditional
    {
        {"#00ff0d", "#ffe600", "#ff0000"},  // slider 1 sections
        {"#00ff0d", "#ffe600", "#ff0000"},  // slider 2 sections
        {"#00ff0d", "#ffe600", "#ff0000"},  // slider 3 sections
        {"#00ff0d", "#ffe600", "#ff0000"},  // slider 4 sections
        {"#00ff0d", "#ffe600", "#ff0000"}   // slider 5 sections
    },
    // Theme 7: pink
    {
        {"#00ffff", "#ff54f9", "#ffa6fc"},  // slider 1 sections
        {"#00ffff", "#ff54f9", "#ffa6fc"},  // slider 2 sections
        {"#00ffff", "#ff54f9", "#ffa6fc"},  // slider 3 sections
        {"#00ffff", "#ff54f9", "#ffa6fc"},  // slider 4 sections
        {"#00ffff", "#ff54f9", "#ffa6fc"}   // slider 5 sections
    }
};

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

int getRgbComponent(const char* hex, char component) {
    if (hex[0] == '#' && strlen(hex) == 7) {
        char compStr[3] = {0}; // Temporary string for the component
        int value = 0;

        switch (component) {
            case 'R':
            case 'r':
                strncpy(compStr, hex + 1, 2); // Copy the red component
                value = strtol(compStr, NULL, 16);
                break;
            case 'G':
            case 'g':
                strncpy(compStr, hex + 3, 2); // Copy the green component
                value = strtol(compStr, NULL, 16);
                break;
            case 'B':
            case 'b':
                strncpy(compStr, hex + 5, 2); // Copy the blue component
                value = strtol(compStr, NULL, 16);
                break;
            default:
                // If component is not recognized, return an error code
                return -1;
        }
        return value; // Return the parsed integer value of the component
    } else {
        // If the HEX format is incorrect, return an error code
        return -1;
    }
}

void writeIntIntoEEPROM(int address, int number)
{
    byte byte1 = number >> 8;
    byte byte2 = number & 0xFF;
    EEPROM.write(address, byte1);
    EEPROM.write(address + 1, byte2);
}

int readIntFromEEPROM(int address)
{
    byte byte1 = EEPROM.read(address);
    byte byte2 = EEPROM.read(address + 1);
    return (byte1 << 8) + byte2;
}

void setBrightness(int newValue) {
    if (newValue > maxBrightness) {
        newValue = maxBrightness;
    } else if (newValue < 0) {
        newValue = 0;
    }

    writeIntIntoEEPROM(2, newValue);
    for(int i = 0; i < 5; i++) {
        sliders[i].setBrightness(newValue);
    }
}

void setTheme(int theme) {
    int noOfThemes = sizeof(themes) / sizeof(themes[0]);
    if (theme < 0) {
        theme = noOfThemes - 1;
    } else if (theme > noOfThemes - 1) {
        theme = 0;
    }

    currentTheme = theme;

    for(int i = 0; i < 5; i++) {
        sliders[i].setColors(
            getRgbComponent(themes[theme][i][0], 'R'),
            getRgbComponent(themes[theme][i][0], 'G'),
            getRgbComponent(themes[theme][i][0], 'B'),
            getRgbComponent(themes[theme][i][1], 'R'),
            getRgbComponent(themes[theme][i][1], 'G'),
            getRgbComponent(themes[theme][i][1], 'B'),
            getRgbComponent(themes[theme][i][2], 'R'),
            getRgbComponent(themes[theme][i][2], 'G'),
            getRgbComponent(themes[theme][i][2], 'B')
        );
    }

    writeIntIntoEEPROM(0, currentTheme);
}

void setup() {
    Serial.begin(9600);

    // Configure datapins below (for LED strips output)
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    brightness = readIntFromEEPROM(2);
    if (brightness < 0 || brightness > maxBrightness) {
        brightness = 20;
        writeIntIntoEEPROM(2, brightness);
    }

    currentTheme = readIntFromEEPROM(0);
    if (currentTheme < 0 || currentTheme > sizeof(themes) / sizeof(themes[0]) - 1) {
        currentTheme = 0;
        writeIntIntoEEPROM(0, currentTheme);
    }

    setBrightness(brightness);
    setTheme(currentTheme);
}

void loop() {
    // Always update sliders
    for(int i = 0; i < 5; i++) {
        sliders[i].update();
    }

    if (sliders[1].checkForDoubleZero()) {
        brightness -= 10;
        setBrightness(brightness);
    }
    if (sliders[2].checkForDoubleZero()) {
        brightness += 10;
        setBrightness(brightness);
    }

    if (sliders[3].checkForDoubleZero()) {
        currentTheme--;
        setTheme(currentTheme);
    }
    if (sliders[4].checkForDoubleZero()) {
        currentTheme++;
        setTheme(currentTheme);
    }

    // Only send every 10ms
    if (millis() - lastSent > 10) {
        analogSliderValues[0] = sliders[0].getAverageValue();
        analogSliderValues[1] = sliders[1].getAverageValue();
        analogSliderValues[2] = sliders[2].getAverageValue();
        analogSliderValues[3] = sliders[3].getAverageValue();
        analogSliderValues[4] = sliders[4].getAverageValue();

        sendSliderValues();
        lastSent = millis();
    }
}