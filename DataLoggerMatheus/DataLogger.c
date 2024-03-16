#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <DHT.h>

#define LCD_ADDRESS 0x27
#define LCD_ROWS 2
#define LCD_COLUMNS 16
#define SCROLL_DELAY 150
#define BACKLIGHT 255

#define DHTPIN 7
#define DHTTYPE DHT22
#define LDRPIN A0

TwoWire wireObj = TwoWire();
LiquidCrystal_I2C lcdI2C(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtcDS;

char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
char daysOfTheWeekEnglish[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

unsigned long previousMillis = 0;
int humiditySum = 0;
int temperatureSum = 0;
int lightSum = 0;
int readingsCount = 0;
bool initialDisplayDone = false;
bool english = false;

void setupLcdAndRTC() {
    wireObj.begin();
    lcdI2C.init();
    dht.begin();
    if (!rtcDS.begin()) {
        lcdI2C.println("Couldn't find RTC");
    }
    if (rtcDS.lostPower()) {
        lcdI2C.println("RTC lost power, lets set the time!");
        rtcDS.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void setup() {
    pinMode(10,OUTPUT);
    pinMode(LDRPIN, INPUT);
    pinMode(7, OUTPUT);
    setupLcdAndRTC();
}

String getCurrentDateTimeString(bool translateToEnglish) {
    DateTime now = rtcDS.now();
    String dateTimeString = "";
    if (translateToEnglish) {
        dateTimeString += String(now.month()) + '/' + String(now.day()) + '/' + String(now.year()) + " ";
        dateTimeString += daysOfTheWeekEnglish[now.dayOfTheWeek()];
    } else {
        dateTimeString += String(now.day()) + '/' + String(now.month()) + '/' + String(now.year()) + " ";
        dateTimeString += daysOfTheWeek[now.dayOfTheWeek()];
    }
    dateTimeString += " " + String(now.hour()) + ':' + String(now.minute()) + ':' + String(now.second());
    return dateTimeString;
}

String getAverageSensorDataString(bool translateToEnglish) {
    int humidityAverage = humiditySum / readingsCount;
    int temperatureAverage = temperatureSum / readingsCount;
    int lightAverage = lightSum / readingsCount;

    String sensorDataString = "";
    sensorDataString += String(humidityAverage) + "% ";
    if (translateToEnglish) {
        sensorDataString += String(temperatureAverage * 1.8 + 32) + "F ";
    } else {
        sensorDataString += String(temperatureAverage) + "*C ";
    }
    sensorDataString += String(lightAverage) + " ";
    return sensorDataString;
}

void updateReadings() {
    humiditySum += dht.readHumidity();
    temperatureSum += dht.readTemperature();
    lightSum += analogRead(LDRPIN);
    readingsCount++;
}

void resetReadings() {
    humiditySum = 0;
    temperatureSum = 0;
    lightSum = 0;
    readingsCount = 0;
}

void loop() {
    unsigned long currentMillis = millis();
    String values = "";

    if (!initialDisplayDone) {
        // Atualize as leituras e exiba a média
        updateReadings();
        values = getAverageSensorDataString(english);
        
        lcdI2C.print(values);
        // Configuração inicial concluída
        initialDisplayDone = true;
    } else {
        // Verifica se passou um minuto desde a última atualização das leituras
        if (currentMillis % 60000 == 0) {
            // Atualiza a média das leituras
            values = getAverageSensorDataString(english);
            lcdI2C.clear();
            lcdI2C.print(values);
            resetReadings();
        }

        // Verifica se passaram 10 segundos dentro do minuto atual
        if (currentMillis % 10000 == 0) {
            // Atualiza as leituras
            updateReadings();
        } 
    }
}


