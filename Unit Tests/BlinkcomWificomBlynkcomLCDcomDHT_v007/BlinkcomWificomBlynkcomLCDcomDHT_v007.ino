#define LedPin D4 //Led de status
#define LDR A0 // Fototransistor
#define vpinLDR V2
#define DHTPIN D3     // 1-5V 2-Dados 4-GND
#define vpinUmi V3
#define vpinTemp V4
#define InfraVermelhoPin D6 //Presença

int loop100count; //Variáveis de Loop
unsigned long prevMillis = 0;

#include <ESP8266WiFi.h>
String essid = "";   //Put here your ssid
String epass = "";   //and your WiFi password

#include <BlynkSimpleEsp8266.h>
String blynkAuth = "1e300153262c41b99fb697cb39c17575"; //Ver no App
int blynkCnt = 100; // 10 segundos
WidgetLED led1(V1); //Keep Alive to virtual pin 1
WidgetLED led2(V5); //Keep Alive to virtual pin 5

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address

#include "DHT.h"
DHT dht(DHTPIN, DHT11);
float umidade, temperatura;
/* libraries:
   DHT-sensor-library-master
   Adafruit_Sensor-master
*/

byte contPresenca = 0;  //Contador do Sensor de Presença (Down)

void setup() {
  //Serial.begin(115200); //Debug
  //Serial.println();
  
  pinMode(LedPin, OUTPUT);  //Led ativo em 0
  pinMode(InfraVermelhoPin, INPUT); //Sensor de Presença

  WiFi.disconnect();
  WiFi.mode(WIFI_STA); //set mode to WIFI_AP , WIFI_STA , WIFI_AP_STA or WIFI_OFF
  WiFi.begin(essid.c_str(), epass.c_str()); //Conexão

  lcdsetup();
}

void loop100() {
  if ((millis() - prevMillis) >= 100){  //Executa a cada contagem de 100 millis = 0,1s
    //Serial.println(prevMillis); //Debug
    prevMillis = millis();  //Atualiza variável de contagem
    
    //Verificação de Placa operante
    if (loop100count % 10 == 0) digitalWrite(LedPin, LOW); //Executa no T=0, acende o Led
    if (loop100count % 10  == 1) digitalWrite(LedPin, HIGH); //Executa no T=1, desliga o Led
    
    //Verificação de conexão WiFi (se estiver desconectado, pisca mais uma vez)
    if ((loop100count % 10 == 2)and(WiFi.status() != WL_CONNECTED)) digitalWrite(LedPin, LOW); //Executa no T=2, acende o Led
    if (loop100count % 10  == 3) digitalWrite(LedPin, HIGH); //Executa no T=3, desliga o Led

    //Verificação da conexão ao Blynk 
    if ((loop100count % 10 == 4)and(!Blynk.connected())) digitalWrite(LedPin, LOW); //Executa no T=4, acende o Led
    if (loop100count % 10  == 5) digitalWrite(LedPin, HIGH); //Executa no T=5, desliga o Led

    //Verificação de conexão Blynk no App
    if (loop100count % 20 == 0) led1.on(); //Executa no T=0, acende o Led
    if (loop100count % 20  == 10) led1.off();; //Executa no T=10 (metade), desliga o Led

    //Atualização do LCD
    if (loop100count % 10  == 6) lcdMostrarStatus(); //Executa no T=4, atualiza o display LCD 16x2

    //Leitura do Sensor de Presença
    if (loop100count % 10  == 7){ //A cada 1 seg
      if (digitalRead(InfraVermelhoPin)) contPresenca = 60; //A cada 1 min
      else if (contPresenca > 0) contPresenca = contPresenca - 1; //Decrementa até 0
    }

    //Envio de dados para o server do Blynk
    if (loop100count % 100  == 0) BlynkSend();  //A cada 10s

    //Leitura de Umidade e Temperatura
    if (loop100count % 200 == 95) {readTemp(); readHumi();} //A cada 20s

    BlynkRun(); //Executa a cada 100 millis
    
    loop100count = loop100count + 1; //Incrementa contador
    if (loop100count == 1000) loop100count = 0; //Zera a cada contagem de 1000 millis = 100s
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  loop100();//Chamada de execução de procedimentos nas contagens de 100
}
