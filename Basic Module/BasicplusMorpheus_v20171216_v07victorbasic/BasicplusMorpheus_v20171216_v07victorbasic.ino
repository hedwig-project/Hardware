//#define Mini

//#define Jarinu // endereçamento do LCD

//#define debug Serial
//#define debugRF

//#define Acesso
//#define Hugo
#define Victor
//#define Dani
//#define Gabi

#define LDRPin A0
#define BtnRstPin A0
#define pinRF_RX D0
#define DHTPin D3
#define LedPin D4
#define InfraVermelhoPin D6 //#define pinRF_TX D6  //usar 4K7
#define buzzerPin D8

#ifdef Mini // Módulo MINI com sensor de presença
  #define Entrada1Pin D1
  #define Rele02Pin D2
  #define Rele01Pin D5
  #define Entrada2Pin D7
#else
  // ----- Módulo com Display e fonte
  #define LCD_SCL D1
  #define LCD_SDA S2
  #define Rele01Pin D7
  #define Rele02Pin D5
  #define Btn1Pin A0
  #define Btn2Pin D4
#endif

String eepromId;
String essid;
String epass;
byte ipMode;
String DeviceID;
String apPass;
bool APmode, prevAPmode; // 0=always 1=auto
String users;

byte Reles;
bool Rele[8], prevRele[8];
unsigned int ReleCnt[8];
unsigned int ReleTempoDesl[2]; // em decimos de segundos: (0)=nunca desliga, (5 a 50)=pulso, (3000 a 288000)=desligaAuto
byte ReleAuto[2]; // (0)=não, (1)=sensor1, (2)=sensor2
byte ReleAutoWait[2];
byte ReleHoraIni[2], ReleHoraFim[2];
String ReleSenha[2];
String ReleSenhaCod[2];
String teclas[10];

byte Sensores; bool Sensor[8], prevSensor[8];
unsigned int SensorCnt[8];
unsigned int PresencaCnt; // retenção do estado de presenca (600 = 1 min)
bool Btn1, prevBtn1;
byte Btn1_wait;
bool Btn2, prevBtn2;
byte Btn2_wait;
String BtnLabel[2]; // used in handleHome

String Board;

String Versao;

#include <EEPROM.h>

#include <ESP8266WiFi.h>
byte prevWiFistatus;
unsigned long wifiMillis;
byte wifiErrCnt;
#include <ESP8266mDNS.h>
//
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
//
#include <ESP8266Ping.h>
//

// MQTT
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient
const char* BROKER_MQTT = "138.197.83.143"; //URL do broker MQTT que se deseja utilizar (138.197.83.143 na cloud)

#ifdef Hugo
int BROKER_PORT = 1883; // Porta do Broker MQTT (1883 para conexão aberta, apenas com controle de acesso)
String SENHA_MQTT = "359985"; //senha MQTT (vem de fábrica) kitchen:449087 victor:374028
#endif
#ifdef Victor
int BROKER_PORT = 1884; // Porta do Broker MQTT (1883 para conexão aberta, apenas com controle de acesso)
String SENHA_MQTT = "374028"; //senha MQTT (vem de fábrica) kitchen:449087 victor:374028
#endif
#ifdef Dani
int BROKER_PORT = 1885; // Porta do Broker MQTT (1883 para conexão aberta, apenas com controle de acesso)
String SENHA_MQTT = "958668"; //senha MQTT (vem de fábrica) kitchen:449087 victor:374028
#endif
#ifdef Gabi
int BROKER_PORT = 1886; // Porta do Broker MQTT (1883 para conexão aberta, apenas com controle de acesso)
String SENHA_MQTT = "664361"; //senha MQTT (vem de fábrica) kitchen:449087 victor:374028
#endif

String chipId;
String message = "";
String TOPICO_SUBSCRIBE;     //tópico MQTT de escuta (servidor envia para módulo)
String TOPICO_PUBLISH;    //tópico MQTT de envio (módulo para servidor)
String ID_MQTT;     //id mqtt (para identificação de sessão), deve ser único (vem de fábrica)
String LOGIN_MQTT;     //login mqtt (para se autenticar no mosquitto, própria de cada módulo); (vem de fábrica)
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
String messageToSend = "";
int rele1=0;
int rele2=0;
String rele1_rf = "";
String rele2_rf = "";
bool confirm2_rf = 0;
String nome_rf = "";
int rfindex = -1;
String sensor_abre = "";
String sensor_fecha = "";
String eventoAbertura = "0";
bool swreset = 0;
bool envioRapido = 0;
int loop1mincont = 0;

//Functions
//setup
void initSerial();
void initWiFi();
void initMQTT();
void InitOutput(void);

//Connections
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);

//Mensagens MQTT (interface com Morpheus)
//s2m: Server To Module = Módulo recebe
void parseS2Mmsg(String S2Mmsg);
//configuration
void time_config(String msg);
void name_config(String msg);
void comunication_config(String msg);
void RF_config(String msg);
void display_config(String msg);

void auto1_config(String msg);
void auto2_config(String msg);

//action_request
void rele1_action(String msg);
void rele2_action(String msg);
void SW_restart(String msg);
void autoReset_test(String msg);

//data_transmission


//m2s: Module To Server = Módulo manda
//confirmation
String time_confirm(unsigned long ts);
String name_confirm(unsigned long ts, String nome, String rele1nome, String rele2nome);
String comunication_confirm(unsigned long ts, String ssid, String password, String ip_local, String ap_mod, String ap_name, String ap_password);
String RF_confirm(unsigned long ts, String nome, String value);
String display_confirm(unsigned long ts, int displaytype, int backlight);
String SWreset_confirm(unsigned long ts);
String autoReset_confirm(unsigned long ts);

String auto1_confirm(int initial_time1, int final_time1,int time_keepon1,int time_deslmanual1);
String auto2_confirm(int initial_time2, int final_time2,int time_keepon2,int time_deslmanual2);

//data_transmission
String UmiTempPresReles(unsigned long ts, int umidade, float temp, bool pres, bool rele1, bool rele2, String abertura, int luz);

//data_request

#ifdef Acesso
int contAberto = 0;
int alarmeAtivo = 0;

String EstAcesso(unsigned long ts,String estAberto,int alarme,int contAberto);
void alarm_config(String msg);
void password_config(String msg);
void abertura_portao(String msg);
String alarm_confirm(int alarme, int alarme_tempo);
String password_confirm(String old_password,String new_password);
String abertura_portao_confirm(unsigned long ts);
#endif

// ----- Time
#include <TimeLib.h>
#include <WiFiUdp.h>
WiFiUDP Udp;
String ntpServerName[5];
int timeZone;
unsigned int TimeNtpCount=10;
unsigned int TimeNtpErrCount;
unsigned int localPort = 8888;  // local port to listen for UDP packets
byte prevTimeStatus;

byte prevHour;

#include <ESP8266HTTPUpdateServer.h>
ESP8266HTTPUpdateServer httpUpdater;
String update_path = "/firmware";
String update_username;
String update_password;

String User, prevUser;

// ----- BLYNK
#define BLYNK_MAX_SENDBYTES 1200 // default message length is 120 symbols max is 1200
#include <BlynkSimpleEsp8266.h>
String blynkAuth;
String BlinkNotifyBuffer;
bool BlynkNotifyIni;
bool BlynkNotifyWiFi;
unsigned int BlynkConnErrCnt;
String BlynkEmailBuffer;
unsigned long BlynkWaitMillis;
unsigned long BlynkEmailWaitSecs;
unsigned int reenviaremail; // em horas, 0=desativado
bool prevBlynkSt;

#ifdef LCD_SCL
// ----- LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#ifdef Jarinu
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address
#else
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // Set the LCD I2C address
#endif
String linha[4];
byte lcdMode;
byte prev_lcdMode=255;
byte lcdCnt; // segundos
byte prevSecondLCD;
unsigned long prevMillisLCD;
byte backlight, prev_backlight;
#endif

// ----- DHTL
#include "DHT.h"
DHT dht(DHTPin, DHT11);
float Umidade, Temperatura;
float prevUmidade, prevTemperatura;
unsigned int Lumi, prevLumi;
unsigned long prevMillisDHT, prevMillisLumi;
float UmiOffset, TempOffset;

// ----- RF 433
unsigned long prevBuffer;
unsigned long prevMillisRx;
String lastRFcode;
int addCode = -1;
byte addCodeCnt;
String RFcode[10];
String RFcodeTitle[10];
String lastRFcodeH;

String backgroundColor;
String Color1;
String Color2;

// -----myip
String myip, myipPrev;
unsigned myipCnt; // segundos até a próxima verificação
String myipServer[3];
String myipReq[3];
unsigned int myipErrCnt;

unsigned int AutoResetTestCnt;
byte ResetCnt;

// ----- LOG
String msg;
String log_str;
String log_opc;
//String sendLogBuffer; unsigned long sendLogNextMillis; String sendLogURL; uint16_t sendLogPort;

unsigned int loop100count;
unsigned long prevMillis;

// ----- Alertas e Alarmes
byte AlarmInp[2];
bool AlarmNtf[2];
unsigned int AlarmSeg[2];
unsigned int AlarmCnt[2]; // em segundos
bool AlarmEml[2];

byte SensoresCnt;

unsigned int minFreeHeap, prev_minFreeHeap;

byte localPingErrCnt;

void setup() {
  Serial.begin(115200); Serial.println(); Serial.println();
  addLog(F("Iniciando"));
  minFreeHeap=ESP.getFreeHeap();
  msg=F("FreeHeap: "); msg+=ESP.getFreeHeap(); addLog(msg);
  pinMode(LedPin, OUTPUT);
  pinMode(pinRF_RX, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(Rele01Pin, OUTPUT);
  pinMode(Rele02Pin, OUTPUT);
  #ifdef Entrada1Pin
  pinMode(Entrada1Pin, INPUT);
  #endif
  #ifdef Entrada2Pin
  pinMode(Entrada2Pin, INPUT);
  #endif
  digitalWrite(buzzerPin, HIGH); delay(20); digitalWrite(buzzerPin, LOW);
  //digitalWrite(Rele01Pin, LOW); digitalWrite(Rele02Pin, LOW);
  Rele[0]=digitalRead(Rele01Pin); prevRele[0]=!Rele[0];
  Rele[1]=digitalRead(Rele02Pin); prevRele[1]=!Rele[1];
  String sa=F("00000000"); sa+=String(ESP.getChipId(),HEX);
  sa=sa.substring(sa.length()-8);sa.toUpperCase();
  msg=F("ChipId: "); msg+=sa; addLog(msg);
  preset();
  EEPROM.begin(512); lerEeprom();
  
  //msg=F("ESP.getFlashChipSize() = ");   msg+=ESP.getFlashChipSize();    addLog(msg);
  //msg=F("ESP.getFlashChipSpeed() = ");  msg+=ESP.getFlashChipSpeed();   addLog(msg);
  //msg=F("ESP.getCycleCount() = ");      msg+=ESP.getCycleCount();       addLog(msg);
  msg=F("Versao: "); msg+=Versao; addLog(msg);
  msg=F("DeviceID: "); msg+=DeviceID; addLog(msg);
  prevAPmode=APmode;
  #ifdef LCD_SCL
  prev_backlight = !backlight;
  #endif
  if(BlynkNotifyIni) BlinkNotifyBuffer += F("ligado, ");
  #ifdef LCD_SCL
  // ----- LCD
  lcd.begin(16, 2);
  lcd.setBacklight(backlight);
  lcd.setCursor(0, 0); lcd.print(centralizar(DeviceID));
  lcd.setCursor(0, 1); lcd.print(centralizar("Hedwig Project"));
  #endif
  MDNS.addService("http", "tcp", 80); MDNS.begin(DeviceID.c_str());
  //wifiSetup();
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect();
  //
  webServerSetup();
  
  // ----- WebUpdater
  String todos = users;
  update_username = todos.substring(0,todos.indexOf(","));
  todos = todos.substring(todos.indexOf(",")+1);
  update_password = todos.substring(0,todos.indexOf(";"));
  todos = todos.substring(todos.indexOf(";")+1);
  httpUpdater.setup(&server, update_path.c_str(), update_username.c_str(), update_password.c_str());
  //
  for(byte i=0;i<sizeof(Reles);i++) prevRele[i]=Rele[i];
  //
  #ifdef Mini
    Btn1=digitalRead(Entrada1Pin); prevBtn1=Btn1;
    Btn2=digitalRead(Entrada2Pin); prevBtn2=Btn2;
  #endif
  
  prevWiFistatus=10;
  prevTimeStatus=timeNotSet;
  //
  prevMillisDHT=millis();
  myipCnt=1;
  prevMillis = millis();
  wifiMillis=millis();
  msg=F("FreeHeap: "); msg+=ESP.getFreeHeap(); addLog(msg);
  addLog(F("LOOP"));

  // ----- MQTT
  chipId = ESP.getChipId();
  //chipId = 765914;
  while (8-chipId.length()!=0){
    chipId = "0"+chipId;
  }
  //Serial.print(F("ChipId="));
  //Serial.println(chipId);
  TOPICO_SUBSCRIBE = "hw/"+chipId+"/s2m";     //tópico MQTT de escuta (servidor envia para módulo)
  TOPICO_PUBLISH = "hw/"+chipId+"/m2s";    //tópico MQTT de envio (módulo para servidor)
  ID_MQTT = chipId;     //id mqtt (para identificação de sessão), deve ser único (vem de fábrica)
  Serial.println(ID_MQTT);
  Serial.println(SENHA_MQTT);
  Serial.println(TOPICO_PUBLISH);
  Serial.println(TOPICO_SUBSCRIBE);
  Serial.println(essid);
  Serial.println(epass);
  LOGIN_MQTT = chipId;     //login mqtt (para se autenticar no mosquitto, própria de cada módulo); (vem de fábrica)
  initMQTT();
}

void loop100(){
  if ((millis() - prevMillis) > 2000){
    unsigned int i;
    i=millis()-prevMillis;
    addLog(String(i)+"ms");
  }
  if ((millis() - prevMillis) < 100) return;
  prevMillis = millis();
  loop100count++; //if (loop100count >= 1200) loop100count = 0; else loop100count++;
  if (loop100count % 10 == 0 && AutoResetTestCnt==0) digitalWrite(LedPin, LOW);
  if (loop100count % 10 == 1) digitalWrite(LedPin, HIGH);
  if (loop100count % 10 == 2) if(WiFi.status() != WL_CONNECTED) if(AutoResetTestCnt==0) digitalWrite(LedPin, LOW);
  if (loop100count % 10 == 3) digitalWrite(LedPin, HIGH);
  if (loop100count % 10 == 0) ntpConnect();
  if (loop100count % 10 == 1) AlarmLoop();
  if (loop100count % 10 == 2) myipRun();
  if (loop100count%10==3) if(BlynkEmailWaitSecs>0) BlynkEmailWaitSecs--;
  if (loop100count%10==4 && AutoResetTestCnt>0) AutoResetTestCnt++;
  #ifdef Btn2Pin
  if (loop100count % 2 == 1) if(AutoResetTestCnt==0) VerificaBotao2();
  #endif
  if (loop100count%10==1 && ResetCnt>0) {
    ResetCnt++;
    digitalWrite(buzzerPin,1);
    delay(10);
    digitalWrite(buzzerPin,0);
  }
  addRFcode();
  // ----- ReleCnt
  for(byte i=0;i<2;i++){
    if(ReleCnt[i]>0) {
      ReleCnt[i]--;
      if(ReleCnt[i]==0) Rele[i]=false;
    }
    if(ReleAutoWait[i]>0) ReleAutoWait[i]--;
  }
  // ----- SensorCnt
  for(byte i=0;i<Sensores;i++){
    if(Sensor[i]) {
      SensorCnt[i]++;
    }else{
      SensorCnt[i]=0;
    }
  }
  if(RFcode[3].length()==0 && SensorCnt[1] == 30){
    Sensor[1]=false;
    SensorCnt[1]=0;
  }
  //
  verificarEntradas();
  // ----- FreeHeap
  if((loop100count%600==500) && (prev_minFreeHeap!=minFreeHeap)){
    prev_minFreeHeap=minFreeHeap;
    msg=F("minFreeHeap: "); msg+=minFreeHeap; addLog(msg);
  }
  //
  if (loop100count%100==7) localPing();
  
  // ----- Envio de msgs MQTT
  if (loop100count%100==8) enviaMsgMQTT();
  if ((loop100count%10==5)and(envioRapido)){
    enviaMsgMQTT();
    envioRapido = false;
  }

  #ifdef Acesso
  if (loop100count%100==6){ //a cada 10s
    loop1mincont++;
    if (loop1mincont == 6){ //a cada 1min
      if (eventoAbertura == "1") contAberto++;
      else contAberto = 0;
      loop1mincont = 0;
    }
  }
  #endif
  
  if (loop100count%100==9) VerificaConexoesWiFIEMQTT();  //garante funcionamento das conexões WiFi e ao broker MQTT
}

void loop() {
  rf433rx();
  loop100();
  server.handleClient();
  blynkLoop();
  wifiLoop();
  VerificaEstado();
  #ifdef LCD_SCL
  LCDloop();
  #endif
  DHTLloop();
  if(minFreeHeap>ESP.getFreeHeap()) minFreeHeap=ESP.getFreeHeap();
  if(minFreeHeap<12000) ESP.reset();
  MQTT.loop();  //keep-alive da comunicação com broker MQTT
}

// Basic71024 if(log_str.length()>4000) log_str=log_str.substring(log_str.indexOf(";")+1);
// Basic71024 if(minFreeHeap<12000) ESP.reset();
// Basic71024 void handleJSON() log=0 melhorias para economizar memória dinâmica
// Basic71024a String secToDMHS (unsigned long ia){ (menos zeros não significativos)
// Basic71024a Temperatura com 1 casa decimal String(Temperatura,1)
// Basic71026a localPing() aba wifi e loop100
// Basic71026b wifiMillis=millis()+(30000*wifiErrCnt);
// 71027  wifi
// 71027  unsigned int loop100count;
// 71027  loop100count++; //if (loop100count >= 1200) loop100count = 0; else loop100count++;
// 71027b Firmware
// 71027c aba id ok
// 71028a wifiMillis=millis();
// 71031a wifi
// 71031b void localPing() if(localPingErrCnt==10) WiFi.disconnect(); // após 100 segundos | if(localPingErrCnt>=20) ESP.reset(); // reset após 200 segundos
// 71031b void void apCon() addLog(F("AP: Falhou")); delay(1000); ESP.reset();

