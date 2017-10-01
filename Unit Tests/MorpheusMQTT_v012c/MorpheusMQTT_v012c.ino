#include <ESP8266WiFi.h> // Importa a Biblioteca ESP8266WiFi
#include <PubSubClient.h> // Importa a Biblioteca PubSubClient

#define TOPICO_SUBSCRIBE "hw/00765914/s2m"     //tópico MQTT de escuta (servidor envia para módulo)
#define TOPICO_PUBLISH   "hw/00765914/m2s"    //tópico MQTT de envio (módulo para servidor)
#define ID_MQTT  "00765914"     //id mqtt (para identificação de sessão), deve ser único (vem de fábrica)
#define LOGIN_MQTT  "00765914"     //login mqtt (para se autenticar no mosquitto, própria de cada módulo); (vem de fábrica)
#define SENHA_MQTT "449087" //senha MQTT (vem de fábrica)

// MQTT
const char* BROKER_MQTT = "138.197.83.143"; //URL do broker MQTT que se deseja utilizar (138.197.83.143 na cloud)
int BROKER_PORT = 1883; // Porta do Broker MQTT (1883 para conexão aberta, apenas com controle de acesso)
unsigned long chipId;
String message = "";

// WIFI
const char* SSID = "YTD960495veredas"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = "784257hue031"; // Senha da rede WI-FI que deseja se conectar

// TIME
#include <TimeLib.h>
#include <WiFiUdp.h>
WiFiUDP Udp;
String ntpServerName[5];
int timeZone;
unsigned int TimeNtpCount=10;
unsigned int TimeNtpErrCount;
unsigned int localPort = 8888;  // local port to listen for UDP packets
byte prevTimeStatus;
unsigned long nextntpmillis;

//Variáveis e objetos globais
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient
String messageToSend = "";

//Functions
//setup
void initSerial();
void initWiFi();
void initMQTT();
void InitOutput(void);

//connections
void reconectWiFi(); 
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void VerificaConexoesWiFIEMQTT(void);


//---------------------------------------------------------------//

//Mensagens MQTT (interface com Morpheus)
//--------------------------------------//
//s2m: Server To Module = Módulo recebe
void parseS2Mmsg(String S2Mmsg);
//configuration
void time_config(String msg);
void name_config(String msg);
void comunication_config(String msg);
void RF_config(String msg);
void display_config(String msg);

//action_request
void rele1_action(String msg);
void rele2_action(String msg);
void SW_restart(String msg);
void autoReset_test(String msg);

//data_transmission


//--------------------------------------//
//m2s: Module To Server = Módulo manda
//confirmation
String time_confirm(unsigned long ts);
String name_confirm(unsigned long ts, String nome, String rele1nome, String rele2nome);
String comunication_confirm(unsigned long ts, String ssid, String password, String ip_local, String ap_mod, String ap_name, String ap_password);
String RF_confirm(unsigned long ts, String nome, String value);
String display_confirm(unsigned long ts, int displaytype, int backlight);
String SWreset_confirm(unsigned long ts);
String autoReset_confirm(unsigned long ts);

//data_transmission
String UmiTempPresReles(unsigned long ts, int umidade, float temp, bool pres, bool rele1, bool rele2);

//data_request


//---------------------------------------------------------------//

void setup() 
{
    InitOutput();
    initSerial();
    initWiFi();
    initMQTT();

    chipId = ESP.getChipId();
    Serial.print(F("ChipId="));
    Serial.println(chipId);

    //NTP
    ntpServerName[0] = F("us.pool.ntp.org");
    ntpServerName[1] = F("time.nist.gov");
    ntpServerName[2] = F("time-a.timefreq.bldrdoc.gov");
    ntpServerName[3] = F("time-b.timefreq.bldrdoc.gov");
    ntpServerName[4] = F("time-c.timefreq.bldrdoc.gov");
    //
}

void loop() 
{   
    //garante funcionamento das conexões WiFi e ao broker MQTT
    VerificaConexoesWiFIEMQTT();
 
    //envia o status de todos os outputs para o Broker no protocolo esperado
    enviaMsgMQTT();
 
    //keep-alive da comunicação com broker MQTT
    MQTT.loop();

    //NTP
    ntpConnect();
}
