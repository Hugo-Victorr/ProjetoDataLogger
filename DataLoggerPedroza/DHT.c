#include <LiquidCrystal_I2C.h>  
#include <RTClib.h> 
#include <Wire.h>  
#include <DHT.h>


DHT dht(7,DHT22);     
LiquidCrystal_I2C lcd(0x27,16,2); 
RTC_DS1307 RTC;
   

void setup() { 
  lcd.init();   
  lcd.backlight();  
  RTC.begin();   
  dht.begin();
}

void loop() {
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature(); 
  float sentermica = dht.computeHeatIndex(temperatura,umidade,false);//sensação térmica true=farenheit e false=graus celsius
  DateTime now=RTC.now();


  lcd.setCursor(0, 0);lcd.print(now.day()); lcd.print("/"); lcd.print(now.month()); lcd.print("/"); lcd.print(now.year() % 100);
  lcd.setCursor(7, 0); lcd.print("-");
  lcd.setCursor(8, 0);lcd.print(now.hour()); lcd.print(":"); lcd.print(now.minute()); lcd.print(":"); lcd.print(now.second());
  lcd.setCursor(0,1);lcd.print("Temp: ");lcd.setCursor(7,1);lcd.print(temperatura);lcd.setCursor(12,1);
  delay(10000);

}
