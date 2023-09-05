#include "Arduino_LED_Matrix.h"
ArduinoLEDMatrix matrix;

// Speaker module
bool isOn;
bool isTrippel;
int val;
int theFormel;
int screenTime;
int delayVarMin;
int delayVarMax;
int buzzPin = 8;
int randomSoundLevel;

// Temperature sensor
int V_input;
float R_ohms;
float temp_C;

const uint32_t HighRem[][4] = {
        { // 0 = !
                0x6006006,
                0x600600,
                0x60060,
                66
        },
        { // 1 = radioactive background warning
                0xccc99933,
                0x3666ccc9,
                0x99333666,
                66
        },
        { // 2 = "run"
                0x0,
                0xcaca,
                0xaacaaaea,
                66
        },
        { // 3 = warning sign
                0x6009016,
                0x8264462c,
                0x3861fff,
                66
        },
        { // 4 = REM
                0x0,
                0xed1a9bc,
                0xd5a91ad1,
                66
        },
        { // 5 = 1000
                0x0,
                0x492d,
                0x6d56d492,
                66
        }
};



void setup()
{
    pinMode(buzzPin, OUTPUT);
    Serial.begin(115200);
    matrix.begin();
    matrix.loadFrame(HighRem[4]);
    delay(1000);
}
void loop()
{

    V_input = analogRead(A0);

    R_ohms = 10000 * (1023.0 / (float)V_input - 1.0);

    temp_C = (1.0 / (0.001129148 + (0.000234125*log(R_ohms)) + 0.0000000876741*log(R_ohms)*log(R_ohms)*log(R_ohms)))-273.15;

    isOn = (random(1,2)) > 0;

    delayVarMin = 101;
    delayVarMax = 105;

    theFormel = -(pow(temp_C, 2)*2)+(124.5*temp_C)-1838;
    // If theFormel is negative, it will crash the code
    if (theFormel <= 0){
        theFormel = 1;
    }

    delayVarMin -= theFormel;
    delayVarMax -= theFormel;

    if ((int)temp_C > 25){
        isOn = (random(1, 2)) > 0;

        if (screenTime++ < 50) {
            matrix.loadFrame(HighRem[0]);
        } else if (screenTime++ < 100) {
            matrix.loadFrame(HighRem[2]);
        } else {
            screenTime = 0;
        }
    } else {
        matrix.loadFrame(HighRem[1]);
    }

    randomSoundLevel = random(37, 42);

    if (isOn); {
        analogWrite(buzzPin, randomSoundLevel);
        delayMicroseconds(random(100+(theFormel*10),500+(theFormel*10)));
    }

    if (!isOn); {
        analogWrite(buzzPin, 0);
        delay(random(1,5));
    }

    delay(random(delayVarMin, delayVarMax));
}