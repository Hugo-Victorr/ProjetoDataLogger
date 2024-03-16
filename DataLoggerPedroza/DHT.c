#include <LiquidCrystal_I2C.h>  
#include <RTClib.h> 
#include <Wire.h>  
#include <DHT.h>
int alertaled = 5;
unsigned long startTime = millis();

DHT dht(7,DHT22);     
LiquidCrystal_I2C lcd(0x27,16,2); 
RTC_DS1307 RTC;
   

void setup() { 
  lcd.init();   
  lcd.backlight();  
  RTC.begin();   
  dht.begin();
  pinMode(alertaled, OUTPUT);
}

void loop() {
  AcionaTemperatura();
  delay(5000);
}

void AcionaTemperatura(){
  float temperatura = dht.readTemperature(); 
  //float sentermica = dht.computeHeatIndex(temperatura, umidade, false); //sensação térmica true=farenheit e false=graus celsius
  DateTime now = RTC.now();
  lcd.setCursor(0, 0); lcd.print(now.day()); lcd.print("/"); lcd.print(now.month()); lcd.print("/"); lcd.print(now.year() % 100);
  lcd.setCursor(7, 0); lcd.print("-");
  lcd.setCursor(8, 0); lcd.print(now.hour()); lcd.print(":"); lcd.print(now.minute()); lcd.print(":"); lcd.print(now.second());
  lcd.setCursor(0,1); lcd.print("Temp: "); lcd.setCursor(7,1); lcd.print(temperatura); lcd.setCursor(12,1);
  
  // Pisca o LED enquanto não se passaram 10 segundos
  // Obtém o tempo atual em milissegundos
  while (temperatura > 15 && temperatura < 25) { // Executa enquanto não se passaram 10 segundos
    digitalWrite(alertaled, HIGH);
    delay(500); // Pausa para o LED ficar ligado
    digitalWrite(alertaled, LOW);
    delay(500); // Pausa para o LED ficar desligado
    
    temperatura = dht.readTemperature();
  }
}

void AcionaUmidade(){
  DateTime now = RTC.now();
  float umidade = dht.readHumidity();

  lcd.setCursor(0, 0); lcd.print(now.day()); lcd.print("/"); lcd.print(now.month()); lcd.print("/"); lcd.print(now.year() % 100);
  lcd.setCursor(7, 0); lcd.print("-");
  lcd.setCursor(8, 0); lcd.print(now.hour()); lcd.print(":"); lcd.print(now.minute()); lcd.print(":"); lcd.print(now.second());
  lcd.setCursor(0,1); lcd.print("Umi: "); lcd.setCursor(7,1); lcd.print(umidade); lcd.setCursor(12,1);lcd.print("%");
  
  while(umidade > 30 && umidade < 40) {
    digitalWrite(alertaled, HIGH);
    delay(500); 
    digitalWrite(alertaled, LOW);
    delay(500); 
    
    umidade = dht.readHumidity();
  }
}

