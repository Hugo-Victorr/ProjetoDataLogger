#include <LiquidCrystal_I2C.h>  
#include <RTClib.h> 
#include <Wire.h>  
#include <DHT.h>
#define LDR_PIN 2
#define botao_MAIS 2                  //define o botão no pino 2 (MAIS)
#define botao_ENTER 3                 //define o botão no pino 3 (ENTER/VOLTAR)
#define botao_MENOS 4                 //define o botão no pino 4 (MENOS)
bool L_botao_MAIS = 1;                //variável para fazer a leitura do botão              (MAIS)
bool LA_botao_MAIS = 1;               //variável para verificar a leitura anterior do botão (MAIS)
bool L_botao_ENTER = 1;               //variável para fazer a leitura do botão              (ENTER/VOLTAR)
bool LA_botao_ENTER = 1;              //variável para verificar a leitura anterior do botão (ENTER/VOLTAR)
bool L_botao_MENOS = 1;               //variável para fazer a leitura do botão              (MENOS)
bool LA_botao_MENOS = 1;              //variável para verificar a leitura anterior do botão (MENOS)

int tela = 1;                         //variável para controlar a mudança de tela no menu

const float GAMMA = 0.7;
const float RL10 = 50;
int alertaled = 5;
int Pinofalante = 6;
byte customChar[] = {
  B00000,
  B01110,
  B00100,
  B01111,
  B10110,
  B01010,
  B00010,
  B00000
};
DHT dht(7,DHT22);     
LiquidCrystal_I2C lcd(0x27,16,2); 
RTC_DS1307 RTC;   

void setup() {
  RTC.begin();   
  dht.begin();
  pinMode(LDR_PIN, INPUT);
  pinMode(alertaled, OUTPUT);
  pinMode(Pinofalante,OUTPUT);
  pinMode(botao_MAIS, INPUT_PULLUP);   //define o botao (MAIS) como uma entrada pull up
  pinMode(botao_ENTER, INPUT_PULLUP);  //define o botao (ENTER/VOLTAR) como uma entrada pull up
  pinMode(botao_MENOS, INPUT_PULLUP);  //define o botao (MENOS) como uma entrada pull up 
  lcd.init();   
  lcd.createChar(0, customChar);
  lcd.home();
  lcd.write(0);
  delay(500);
  lcd.backlight();
}

void loop() {
   static int telaAnterior = 0; // Variável para armazenar o valor da tela anterior
  if (tela != telaAnterior) { // Verifica se a tela mudou
    lcd.clear(); // Limpa o LCD apenas uma vez ao entrar em uma nova opção
    telaAnterior = tela; // Atualiza o valor da tela anterior
  }

  if (tela == 1) {
    lcd.setCursor(0, 0);
    lcd.print(F("   << MENU >>   "));
    lcd.setCursor(0, 1);
    lcd.print(F("> TEMPERATURA "));
  }
  else if (tela == 2) {
    lcd.setCursor(0, 0);
    lcd.print(F("   << MENU >>   "));
    lcd.setCursor(0, 1);
    lcd.print(F("> UMIDADE     "));
  }
  else if (tela == 3) {
    lcd.setCursor(0, 0);
    lcd.print(F("   << MENU >>   "));
    lcd.setCursor(0, 1);
    lcd.print(F("> LUMINOSIDADE  "));
  }

  else if (tela == 10) {
    MarcaHora();
    AcionaTemperatura();
  }
  else if (tela == 20) {
    MarcaHora();
    AcionaUmidade();
  }
  else if (tela == 30) {
    MarcaHora();
    AcionaLuminosidade();
  }

  // === Ações do Botão MAIS === //
  L_botao_MAIS = digitalRead(botao_MAIS);
  if (!L_botao_MAIS && LA_botao_MAIS) {
    if (tela == 1) tela = 2;
    else if (tela == 2) tela = 3;
    else if (tela == 3) tela = 1;
    delay(30);
  }
  LA_botao_MAIS = L_botao_MAIS;

  // === Ações do Botão MENOS === //
  L_botao_MENOS = digitalRead(botao_MENOS);
  if (!L_botao_MENOS && LA_botao_MENOS) {
    if (tela == 1) tela = 3;
    else if (tela == 3) tela = 2;
    else if (tela == 2) tela = 1;
    delay(30);
  }
  LA_botao_MENOS = L_botao_MENOS;

  // === Ações do Botão ENTER === //
  L_botao_ENTER = digitalRead(botao_ENTER);
  if (!L_botao_ENTER && LA_botao_ENTER) {
    if (tela == 1) tela = 10;
    else if (tela == 2) tela = 20;
    else if (tela == 3) tela = 30;

    else if (tela == 10) tela = 1;
    else if (tela == 20) tela = 2;
    else if (tela == 30) tela = 3;
    delay(30);}
  LA_botao_ENTER = L_botao_ENTER;
  
}

void AcionaTemperatura() {
  float temperatura = dht.readTemperature(); 
  lcd.setCursor(0,1); lcd.print("Temp: "); lcd.setCursor(7,1); lcd.print(temperatura); lcd.setCursor(12,1);
  
  // Verificar se a temperatura saiu da faixa desejada
  if (temperatura < 15 || temperatura > 25) {
    // Executar o loop enquanto a temperatura estiver fora do intervalo permitido
    while (temperatura < 15 || temperatura > 25) {
      tone(Pinofalante, 330, 300);
      digitalWrite(alertaled, HIGH);
      delay(500); // Pausa para o LED ficar ligado
      digitalWrite(alertaled, LOW);
      delay(500); // Pausa para o LED ficar desligado
      
      // Atualizar a leitura da temperatura após o atraso
      temperatura = dht.readTemperature();
    }
  } else {
    // Se a temperatura não estiver na faixa desejada, pare de tocar o sinal sonoro e acionar o LED
    noTone(Pinofalante);
    digitalWrite(alertaled, LOW);
  }
}

void AcionaUmidade() {
  float umidade = dht.readHumidity();
  lcd.setCursor(0,1); lcd.print("Umi: "); lcd.setCursor(7,1); lcd.print(umidade); lcd.setCursor(12,1);lcd.print("%");
  
  // Verificar se a umidade saiu da faixa permitida
  if(umidade < 30 || umidade > 40) {
    // Executar o loop enquanto a umidade estiver na faixa desejada
    while(umidade < 30 || umidade > 40) {
      tone(Pinofalante, 330, 300);
      digitalWrite(alertaled, HIGH);
      delay(500); // Pausa para o LED ficar ligado
      digitalWrite(alertaled, LOW);
      delay(500); // Pausa para o LED ficar desligado
      
      // Atualizar a leitura da umidade após o atraso
      umidade = dht.readHumidity();
    }
  } else {
    // Se a umidade não estiver na faixa desejada, pare de tocar o sinal sonoro e acionar o LED
    noTone(Pinofalante);
    digitalWrite(alertaled, LOW);
  }
}

void AcionaLuminosidade() {
  int analogValue = analogRead(A0);
  float voltage = analogValue / 1024. * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));

  lcd.setCursor(0,1); lcd.print("Luz: "); lcd.setCursor(7,1); lcd.print(lux);

  // Verificar se a luminosidade está na faixa desejada
  if (lux < 0 || lux > 30) {
    // Executar o loop enquanto a luminosidade estiver na faixa desejada
    while (lux < 0 || lux > 30) {
      digitalWrite(alertaled, HIGH);
      tone(Pinofalante, 330, 300);
      delay(500); 
      digitalWrite(alertaled, LOW);
      delay(500);
      
      // Atualizar a leitura da luminosidade após o atraso
      analogValue = analogRead(A0);
      voltage = analogValue / 1024. * 5;
      resistance = 2000 * voltage / (1 - voltage / 5);
      lux = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA));
    }
  } else {
    // Se a luminosidade não estiver na faixa desejada, pare de tocar o sinal sonoro e acionar o LED
    noTone(Pinofalante);
    digitalWrite(alertaled, LOW);
  }
}

void MarcaHora(){
  DateTime now = RTC.now();
  lcd.setCursor(0, 0); lcd.print(now.day()); lcd.print("/"); lcd.print(now.month()); lcd.print("/"); lcd.print(now.year() % 100);
  lcd.setCursor(7, 0); lcd.print("-");
  lcd.setCursor(8, 0); lcd.print(now.hour()); lcd.print(":"); lcd.print(now.minute()); lcd.print(":"); lcd.print(now.second());
}
