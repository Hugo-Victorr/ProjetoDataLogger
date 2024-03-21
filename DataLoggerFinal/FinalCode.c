#include "Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include <DHT.h>
#include <EEPROM.h>

#define LCD_ADDRESS 0x27
#define LCD_ROWS 2
#define LCD_COLUMNS 16
#define SCROLL_DELAY 150
#define BACKLIGHT 255
#define botao_T 2                  

#define DHTPIN 7
#define DHTTYPE DHT11 // ----------------------------------Lembrar de trocar 22 por 11, quando executar em sala \o/
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
int humidityAverage = 0;
int temperatureAverage = 0;
int lightAverage = 0;
const int timeOffset = -1; //Registra o fuso-horário BR - EUA
const int NUM_VALUES = 3; // Número de valores a serem armazenados na EEPROM

unsigned long ultimoTempoPressionado = 0;
unsigned long intervaloDebounce = 50; // Tempo de debounce em milissegundos

// Estado do botão
bool estadoBotao;
bool ultimoEstadoBotao;
bool botaoPressionado = false;

void ConstroiLogo() {
  byte p1[] = {
     B00111,
  B01000,
  B10001,
  B10000,
  B10000,
  B10000,
  B01000,
  B00111
  };
  byte p2[] = {
   B00010,
  B00100,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000,
  B11111
  };
  byte p3[] = {
  B00001,
  B00001,
  B00010,
  B11100,
  B00000,
  B00000,
  B00000,
  B11111
  };
  byte p4[] = {
     B10000,
  B01000,
  B00100,
  B01010,
  B01010,
  B01010,
  B11010,
  B00100
  };
  byte p5[] = {
 B11100,
  B00010,
  B00001,
  B01001,
  B01000,
  B00000,
  B00001,
  B11111
  };
  byte p6[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B10000,
  B10000,
  B00000,
  B00000
  };
  byte p7[] = {
  B11111,
  B00000,
  B00000,
  B10000,
  B10000,
  B00000,
  B00000,
  B11111
};
byte p8[] = {
  B00000,
  B00001,
  B00001,
  B00010,
  B00010,
  B00010,
  B00001,
  B00000
};


  lcdI2C.setCursor(1, 0);
  lcdI2C.createChar(0, p1);
  lcdI2C.createChar(1, p2);
  lcdI2C.createChar(2, p3);
  lcdI2C.createChar(3, p4);
  lcdI2C.createChar(4, p5);
  lcdI2C.createChar(5, p6);
  lcdI2C.createChar(6, p7);
  lcdI2C.createChar(7, p8);

  lcdI2C.setCursor(1, 0); // Definindo a posição para escrever p5
  lcdI2C.write(byte(7));  // Escrevendo p5
  lcdI2C.write(byte(6));  // Escrevendo p5
  lcdI2C.write(byte(4));  // Escrevendo p5
  lcdI2C.write(byte(5));  // Escrevendo p5

  lcdI2C.setCursor(1, 1); // Definindo a posição para escrever p1
  lcdI2C.write(byte(0));  // Escrevendo p1
  lcdI2C.write(byte(1));  // Escrevendo p2
  lcdI2C.write(byte(2));  // Escrevendo p3
  lcdI2C.write(byte(3));  // Escrevendo p4

  lcdI2C.setCursor(6,0);
  lcdI2C.print("WELCOME TO");
  lcdI2C.setCursor(6,1);
  lcdI2C.print("THE FUTURE");
}

void setupLcdAndRTC() {
  wireObj.begin();
  lcdI2C.init();
  lcdI2C.backlight();
  ConstroiLogo();
  delay(2000);
  lcdI2C.clear();

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
  pinMode(10, OUTPUT);
  pinMode(LDRPIN, INPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(2, INPUT_PULLUP);
  setupLcdAndRTC();
  Serial.begin(9600);
}

String getCurrentDateTimeString(bool translateToEnglish) {
  DateTime now = rtcDS.now();
  String dateTimeString = "";

  //Faz a tradução do formato de data
  if (translateToEnglish) {
    dateTimeString += String(now.month()) + '/' + String(now.day()) + '/' + String((now.year() % 100)) + " ";
    //dateTimeString += daysOfTheWeekEnglish[now.dayOfTheWeek()];
  } else {
    dateTimeString += String(now.day()) + '/' + String(now.month()) + '/' + String((now.year() % 100)) + " ";
    //dateTimeString += daysOfTheWeek[now.dayOfTheWeek()];
  }

  //Faz a tradução do formato de data e hora
  if(translateToEnglish){
    
    //Subtrai o valor do fuso-horário
    now = now + TimeSpan(0, timeOffset, 0, 0);

    char formattedTime[12]; // Espaço para armazenar a hora formatada
          sprintf(formattedTime, "%02d:%02d%s", 
          (now.hour() % 12 ? now.hour() % 12 : 12), // Converte horas para o formato 12 horas
          now.minute(),
          (now.hour() < 12 ? "am" : "pm"));

    dateTimeString += " " + String(formattedTime);
  } else{
    char brazilianTime[6];
    sprintf(brazilianTime, "%02d:%02d", now.hour(), now.minute());
    dateTimeString += " " + String(brazilianTime);
  }
  
  return dateTimeString;
}

//Calcula a média dos valores obtidos através dos sensores
String getAverageSensorDataString(bool translateToEnglish) {
  humidityAverage = humiditySum / readingsCount;
  temperatureAverage = temperatureSum / readingsCount;
  lightAverage = lightSum / readingsCount;

  lightAverage = map(lightAverage, 0, 1023, 0, 50); 

  String sensorDataString = "";
  
  if (translateToEnglish) {
    sensorDataString += String(humidityAverage) + "%RH ";
    sensorDataString += String(round(temperatureAverage * 1.8 + 32)) + (char)0xDF + "F ";
  } else {
    sensorDataString += String(humidityAverage) + "%UR ";
    sensorDataString += String(temperatureAverage) + (char)0xDF + "C ";
  }
  sensorDataString += String(lightAverage) + "%E ";
  
  return sensorDataString;
}

//Atualiza o valor das variaveis com o sinal obtido dos sensores
void updateReadings() {
  humiditySum += dht.readHumidity();
  temperatureSum += dht.readTemperature();
  lightSum += analogRead(LDRPIN);
  readingsCount++;
}

//Reseta o valor das variaveis
void resetReadings() {
  humiditySum = 0;
  temperatureSum = 0;
  lightSum = 0;
  readingsCount = 0;
}

void averageValidation() {
  if ((temperatureAverage < 15 || temperatureAverage > 25) ||
        (humidityAverage < 30 || humidityAverage > 50) ||
        (lightAverage < 0 || lightAverage > 30))
 {
    tone(10,440, 5000);
    digitalWrite(8, HIGH);
    delay(2000);
    digitalWrite(8, LOW);

    //Salva valores e timeStamp quando média fora do intervalo seguro 
    controllerEEPROM(temperatureAverage, humidityAverage, lightAverage);
 } 
}

void controllerEEPROM(int temp, int hum, int lum){
  //Variaveis do RTC
  uint8_t dia, mes, ano, hora, minuto, segundo; //tipo de dado usa somente 1 byte
  DateTime now = rtcDS.now();
  dia = now.day(); 
  mes = now.month();
  ano = now.year() % 100;
  hora = now.hour();
  minuto = now.minute();
  segundo = now.second();

  saveEEPROM(temp, hum, lum, dia, mes, ano, hora, minuto, segundo);
  readAndPrintEEPROM();
  
}

//funcao para salvar os dados no EEPROM
void saveEEPROM(int temp, int hum, int lum, uint8_t dia, uint8_t mes, uint8_t ano, uint8_t hora, uint8_t minuto, uint8_t segundo) {
  const int SIZE_OF_RECORD = 12; // Tamanho de cada registro na EEPROM (3 ints + 6 uint8_t)
  const int EEPROM_ADDR = 0; // Endereço inicial na EEPROM
  

  // Move os valores antigos para frente
  for (int i = NUM_VALUES - 1; i > 0 ; i--) {
    int addrAtual = EEPROM_ADDR + (i - 1) * SIZE_OF_RECORD;

    // Move os valores para a posição seguinte
    for (int j = 0; j < SIZE_OF_RECORD; j++) {
      uint8_t valueByte;

      //Sobrescrever byte a byte 
      EEPROM.get(addrAtual + j, valueByte);
      EEPROM.put(addrAtual + SIZE_OF_RECORD + j, valueByte);
    }
  }

  // Salva o novo registro na primeira posição: addr(0)
  EEPROM.put(EEPROM_ADDR, temp);
  EEPROM.put(EEPROM_ADDR + sizeof(int), hum);
  EEPROM.put(EEPROM_ADDR + 2 * sizeof(int), lum);
  EEPROM.put(EEPROM_ADDR + 3 * sizeof(int), dia);
  EEPROM.put(EEPROM_ADDR + 3 * sizeof(int) + sizeof(uint8_t), mes);
  EEPROM.put(EEPROM_ADDR + 3 * sizeof(int) + 2 * sizeof(uint8_t), ano); //coletar somente os ultimos valores do ano
  EEPROM.put(EEPROM_ADDR + 3 * sizeof(int) + 3 * sizeof(uint8_t), hora);
  EEPROM.put(EEPROM_ADDR + 3 * sizeof(int) + 4 * sizeof(uint8_t), minuto);
  EEPROM.put(EEPROM_ADDR + 3 * sizeof(int) + 5 * sizeof(uint8_t), segundo);
}

void readAndPrintEEPROM() {
  const int SIZE_OF_RECORD = 12; // Tamanho de cada registro na EEPROM (3 ints + 6 uint8_t)
  const int EEPROM_ADDR = 0; // Endereço inicial na EEPROM

  // Itera sobre os registros salvos na EEPROM
  for (int i = 0; i < NUM_VALUES; i++) {
    int addrAtual = EEPROM_ADDR + i * SIZE_OF_RECORD;

    int temp, hum, lum;
    uint8_t dia, mes, ano, hora, minuto, segundo;

    // Lê os valores do registro atual
    EEPROM.get(addrAtual, temp);
    EEPROM.get(addrAtual + sizeof(int), hum);
    EEPROM.get(addrAtual + 2 * sizeof(int), lum);
    EEPROM.get(addrAtual + 3 * sizeof(int), dia);
    EEPROM.get(addrAtual + 3 * sizeof(int) + sizeof(uint8_t), mes);
    EEPROM.get(addrAtual + 3 * sizeof(int) + 2 * sizeof(uint8_t), ano);
    EEPROM.get(addrAtual + 3 * sizeof(int) + 3 * sizeof(uint8_t), hora);
    EEPROM.get(addrAtual + 3 * sizeof(int) + 4 * sizeof(uint8_t), minuto);
    EEPROM.get(addrAtual + 3 * sizeof(int) + 5 * sizeof(uint8_t), segundo);

    // Imprime os valores lidos
    Serial.print("Registro ");
    Serial.println(i + 1);
    Serial.print("Temperatura: ");
    Serial.println(temp);
    Serial.print("Umidade: ");
    Serial.println(hum);
    Serial.print("Luminosidade: ");
    Serial.println(lum);
    Serial.print("Data: ");
    Serial.print(dia);
    Serial.print("/");
    Serial.print(mes);
    Serial.print("/");
    Serial.println(ano);
    Serial.print("Horário: ");
    Serial.print(hora);
    Serial.print(":");
    Serial.print(minuto);
    Serial.print(":");
    Serial.println(segundo);
    Serial.println();
  }
}

void loop() { 
  unsigned long currentMillis = millis();
  String values = "";
  //Armazena o sinal atual do botao
  estadoBotao = digitalRead(2);

  // Verifica se houve mudança no estado do botão
  if (estadoBotao != ultimoEstadoBotao) {
    // Reinicia o temporizador de debounce
    ultimoTempoPressionado = millis();
  }

  // Verifica se o botão está estabilizado após o debounce
  if ((millis() - ultimoTempoPressionado) > intervaloDebounce) {
    // Se o estado atual do botão for diferente do estado anterior e estiver estabilizado, consideramos que o botão foi pressionado
    if (estadoBotao != botaoPressionado) {
      botaoPressionado = estadoBotao;

      // Faça o que desejar quando o botão é pressionado aqui...

      // Exemplo: 
      if (botaoPressionado == LOW) {
        // Botão foi pressionado
        english = !english;
        Serial.println("Botão Clicado");
      }
    }
  }

  //2Registra ultimo estado do botão
  ultimoEstadoBotao = estadoBotao;

  if (!initialDisplayDone) {
    // Atualize as leituras e exiba a média
    updateReadings();
    values = getAverageSensorDataString(english);
    lcdI2C.setCursor(0, 0);
    lcdI2C.print(values);

    values = getCurrentDateTimeString(english);
    lcdI2C.setCursor(0, 1);
    lcdI2C.print(values);
    averageValidation();
    // Configuração inicial concluída
    initialDisplayDone = true;
  } else {
    // Verifica se passou um minuto desde a última atualização das leituras
    if (currentMillis % 30000 == 0) {
      // Atualiza a média das leituras
      values = getAverageSensorDataString(english);
      lcdI2C.clear();
      lcdI2C.setCursor(0, 0);
      lcdI2C.print(values);

      values = getCurrentDateTimeString(english);
      lcdI2C.setCursor(0, 1);
      lcdI2C.print(values);
      averageValidation(); //Os dados são salvos na EEPROM a partir dessa chamada
      resetReadings();
    }

    // Verifica se passaram 10 segundos dentro do minuto atual
    if (currentMillis % 1000 == 0) {
      // Atualiza as leituras
      updateReadings();
    }
  }
}
