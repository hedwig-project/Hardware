#define LedPin D4
int loop100count;
unsigned long prevMillis;
#include <ESP8266WiFi.h>
String essid, epass;
WiFiClient client;
String server = "192.168.0.111";
uint16_t port=8080;
String pg; //String para montar request HTTP

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  
  pinMode(LedPin, OUTPUT);
  
  WiFi.mode(WIFI_AP_STA);
  delay(100);
  WiFi.disconnect();
  delay(100);
  WiFi.softAPdisconnect(true);
  delay(100);
  
  essid="YTD960495veredas";
  epass="784257hue031";
  if(essid!=""){
    Serial.print("\nStarting connection to WiFi");
    WiFi.begin(essid.c_str(), epass.c_str());
    for(byte i=0;i<30;i++){
      if(WiFi.status()==WL_CONNECTED) break;
      Serial.print(".");
      digitalWrite(LedPin, LOW); delay(500);
      digitalWrite(LedPin, HIGH); delay(500);
    }
  }
  Serial.println();
  Serial.println("Connected to wifi");
  delay(100);
  Serial.println();

  initialConfig();
  //deleteModule();
}

void loop() {
  if ((millis() - prevMillis) >= 100){
    prevMillis = millis();
    if (loop100count >= 1000) loop100count = 0; else loop100count++;
    if (loop100count % 10 == 0) digitalWrite(LedPin, LOW);
    if (loop100count % 10 == 1) digitalWrite(LedPin, HIGH);
    if (loop100count % 10 == 2) if(WiFi.status() != WL_CONNECTED) digitalWrite(LedPin, LOW);
    if (loop100count % 10 == 3) digitalWrite(LedPin, HIGH);
    if (loop100count % 300 == 0) dataTransmission();
    if (loop100count % 490 == 0) configuration();
    if (loop100count % 750 == 0) actionRequest();
  }


  if(client.available()) {  //Print resposta HTTP
    //char c = client.read(); Serial.print(c);
    while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    client.stop();
    Serial.println();
  }
}

void initialConfig(){
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server.c_str(), port)) { //Conecta ao HTTP
    Serial.println("connected to server");

  pg = "";
  pg+=F(
"{"
    "\"configurationId\": \"1\","
    "\"timestamp\": \"1499717103422\","
    "\"morpheusConfiguration\": {"
        "\"register\": ["
            "{"
                "\"moduleId\": \"318381\","
                "\"moduleName\": \"kitchen\","
                "\"moduleTopic\": \"hw/kitchen318381\","
                "\"receiveMessagesAtMostEvery\": \"10:m\","
                "\"qos\": \"1\""
            "}"
        "]"
    "},"   
    "\"modulesConfiguration\": []"
"}"
    );
    
    client.println("POST /message/configuration HTTP/1.1");
    client.println("Host: "+server+":"+String(port));
    //client.println("Authorization: Basic YWRtaW46MTIzNA==");
    client.println("Content-Type: application/json");
    client.println("Cache-Control: no-cache");
    client.println("Content-Length: "+String(pg.length()));
    client.println("Connection: close");
    client.println();
    client.println(pg);
  }else{
    Serial.println("not connected to server");
  }
}

void dataTransmission(){
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server.c_str(), port)) { //Conecta ao HTTP
    Serial.println("connected to server");

  pg = "";
  pg+=F(
"["
  "{"
    "\"topic\": \"hw/kitchen318381/s2m\","
    "\"controlParameters\": ["
      "{"
        "\"parameter\": \"ts\","
        "\"value\": 1500914158"
      "},"
      "{"
        "\"parameter\": \"ty\","
        "\"value\": \"rele1_action\""
      "}"
      "],"
    "\"payload\" : \"rele1: 0\""
  "},"
  "{"
    "\"topic\": \"hw/kitchen318381/s2m\","
    "\"controlParameters\": ["
      "{"
        "\"parameter\": \"ts\","
        "\"value\": 1500914158"
      "},"
      "{"
        "\"parameter\": \"ty\","
        "\"value\": \"rele1_action\""
      "}"
      "],"
    "\"payload\" : \"rele1: 1\""
  "}"
"]"
    );
    
    client.println("POST /message/data_transmission HTTP/1.1");
    client.println("Host: "+server+":"+String(port));
    //client.println("Authorization: Basic YWRtaW46MTIzNA==");
    client.println("Content-Type: application/json");
    client.println("Cache-Control: no-cache");
    client.println("Content-Length: "+String(pg.length()));
    client.println("Connection: close");
    client.println();
    client.println(pg);
  }else{
    Serial.println("not connected to server");
  }
}

void configuration(){
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server.c_str(), port)) { //Conecta ao HTTP
    Serial.println("connected to server");

  pg = "";
  pg+=F(
"{"
    "\"configurationId\": \"1\","
    "\"timestamp\": \"1499717103422\"," 
    "\"modulesConfiguration\": ["
      "{"
        "\"moduleId\": \"318381\","
            "\"moduleName\": \"kitchen\","
            "\"moduleTopic\": \"hw/kitchen318381\","
            "\"messages\": ["
              "{"
                "\"controlParameters\": ["
            "{"
              "\"parameter\": \"ts\","
              "\"value\": 1500914158"
            "},"
            "{"
              "\"parameter\": \"ty\","
              "\"value\": \"rf_config\""
            "}"
                "],"
                "\"payload\": \"displaytype: <1|2|3> backlight: <0 para desligar, 1 para ligar>\""
              "}"
            "]"
      "}"
    "]"
"}"
    );
    
    client.println("POST /message/configuration HTTP/1.1");
    client.println("Host: "+server+":"+String(port));
    //client.println("Authorization: Basic YWRtaW46MTIzNA==");
    client.println("Content-Type: application/json");
    client.println("Cache-Control: no-cache");
    client.println("Content-Length: "+String(pg.length()));
    client.println("Connection: close");
    client.println();
    client.println(pg);
  }else{
    Serial.println("not connected to server");
  }
}

void deleteModule(){
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server.c_str(), port)) { //Conecta ao HTTP
    Serial.println("connected to server");

  pg = "";
  pg+=F(
"{"
"    \"configurationId\": \"1\","
"    \"timestamp\": \"1499717103422\",   "
"    \"modulesConfiguration\": ["
"      {"
"       \"moduleId\": \"318381\","
"            \"moduleName\": \"kitchen\","
"            \"moduleTopic\": \"hw/kitchen318381\","
"            \"unregister\": true,"
"            \"messages\": []"
"     }"
"    ]"
"}"
    );
    
    client.println("POST /message/configuration HTTP/1.1");
    client.println("Host: "+server+":"+String(port));
    //client.println("Authorization: Basic YWRtaW46MTIzNA==");
    client.println("Content-Type: application/json");
    client.println("Cache-Control: no-cache");
    client.println("Content-Length: "+String(pg.length()));
    client.println("Connection: close");
    client.println();
    client.println(pg);
  }else{
    Serial.println("not connected to server");
  }
}

void actionRequest(){
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server.c_str(), port)) { //Conecta ao HTTP
    Serial.println("connected to server");

  pg = "";
  pg+=F(
"["
"  {"
"   \"topic\": \"hw/kitchen318381/s2m\","
"   \"controlParameters\": ["
"     {"
"       \"parameter\": \"ts\","
"       \"value\": 1500914158"
"     },"
"     {"
"       \"parameter\": \"ty\","
"       \"value\": \"rele1_action\""
"     }"
"     ],"
"   \"payload\" : \"rele1: 0\""
" },"
" {"
"   \"topic\": \"hw/kitchen318381/s2m\","
"   \"controlParameters\": ["
"     {"
"       \"parameter\": \"ts\","
"       \"value\": 1500914158"
"     },"
"     {"
"       \"parameter\": \"ty\","
"       \"value\": \"rele1_action\""
"     }"
"     ],"
"   \"payload\" : \"rele1: 1\""
" }"
"]"

    );
    
    client.println("POST /message/action_request HTTP/1.1");
    client.println("Host: "+server+":"+String(port));
    //client.println("Authorization: Basic YWRtaW46MTIzNA==");
    client.println("Content-Type: application/json");
    client.println("Cache-Control: no-cache");
    client.println("Content-Length: "+String(pg.length()));
    client.println("Connection: close");
    client.println();
    client.println(pg);
  }else{
    Serial.println("not connected to server");
  }
}
