void initMQTT() // Inicializa parâmetros de conexão MQTT(endereço do broker, porta e seta função de callback)
{
    if (WiFi.status()!=WL_CONNECTED) return;
    if (timeStatus() == timeNotSet) return;
}

void reconnectMQTT()  //Reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair).
{                     //Em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
    if (WiFi.status()!=WL_CONNECTED) return;
    if (timeStatus() == timeNotSet) return;
    if (!MQTT.connected()) 
    {
        Serial.print(F("* Tentando se conectar ao Broker MQTT: "));
        Serial.println(BROKER_MQTT);
        Serial.println(ID_MQTT);
        Serial.println(LOGIN_MQTT);
        Serial.println(SENHA_MQTT);

        MQTT.setServer(BROKER_MQTT, BROKER_PORT);
        MQTT.setCallback(mqtt_callback);
        const char* id_mqtt = ID_MQTT.c_str();
        const char* login_mqtt = LOGIN_MQTT.c_str();
        const char* senha_mqtt = SENHA_MQTT.c_str();
        if (MQTT.connect(id_mqtt,login_mqtt,senha_mqtt))
        {
            Serial.println(F("Conectado com sucesso ao broker MQTT!"));
            MQTT.subscribe(TOPICO_SUBSCRIBE.c_str()); 
        } 
        else
        {
            Serial.println(F("Falha ao reconectar no broker."));
        }
    }
}

void VerificaConexoesWiFIEMQTT(void)  //Verifica o estado das conexões WiFI e ao broker MQTT. 
{
    if (!MQTT.connected()){ //se não há conexão com o Broker, a conexão é refeita
        reconnectMQTT();
    }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) //Tratamento das mensagens recebidas
{
    String msg;

    for(int i = 0; i < length; i++) //obtém a string do payload recebido
    {
       char c = (char)payload[i];
       msg += c;
    }
   
    parseS2Mmsg(msg);
     
}

void parseS2Mmsg(String S2Mmsg)  //Parse da msg recebida
{
    String msg_id;

    //Configuration Messages
    msg_id = "time_config"; if (S2Mmsg.indexOf(msg_id)>=0) time_config(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    msg_id = "name_config"; if (S2Mmsg.indexOf(msg_id)>=0) name_config(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    msg_id = "comunication_config"; if (S2Mmsg.indexOf(msg_id)>=0) comunication_config(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    msg_id = "rf_config"; if (S2Mmsg.indexOf(msg_id)>=0) rf_config(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    msg_id = "display_config"; if (S2Mmsg.indexOf(msg_id)>=0) display_config(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));

    //Action_Request Messages
    msg_id = "rele1_action"; if (S2Mmsg.indexOf(msg_id)>=0) rele1_action(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    msg_id = "rele2_action"; if (S2Mmsg.indexOf(msg_id)>=0) rele2_action(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    msg_id = "sw_reset"; if (S2Mmsg.indexOf(msg_id)>=0) SW_restart(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    msg_id = "autoreset_test"; if (S2Mmsg.indexOf(msg_id)>=0) autoReset_test(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));

    //Acesso
    #ifdef Acesso
    msg_id = "alarm_config"; if (S2Mmsg.indexOf(msg_id)>=0) alarm_config(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    //msg_id = "auto1_config"; if (S2Mmsg.indexOf(msg_id)>=0) auto1_config(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    //msg_id = "auto2_config"; if (S2Mmsg.indexOf(msg_id)>=0) auto2_config(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    msg_id = "password_config"; if (S2Mmsg.indexOf(msg_id)>=0) password_config(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    msg_id = "abertura_portao"; if (S2Mmsg.indexOf(msg_id)>=0) abertura_portao(S2Mmsg.substring(S2Mmsg.indexOf(msg_id)+msg_id.length()));
    #endif
}

void enviaMsgMQTT(void) //Envia mensagem ao broker
{
    if (WiFi.status()!=WL_CONNECTED) return;
    if (timeStatus() == timeNotSet) return;
    if (!MQTT.connected()) return;
    if ((timeStatus() != timeNotSet)and(Temperatura != 0)and(loop1mincont%5==0)) messageToSend += UmiTempPresReles(now(),Umidade,Temperatura,Sensor[0],Rele[0],Rele[1],eventoAbertura,Lumi);
    if ((timeStatus() != timeNotSet)and(Temperatura != 0)and envioRapido) messageToSend = UmiTempPresReles(now(),Umidade,Temperatura,Sensor[0],Rele[0],Rele[1],eventoAbertura,Lumi)+messageToSend;

    #ifdef Acesso
    if (AlarmSeg[0] != 0) alarmeAtivo = 1;
    else alarmeAtivo = 0;
    if ((timeStatus() != timeNotSet)and(loop1mincont%3==0)) messageToSend += EstAcesso(now(),eventoAbertura,alarmeAtivo,contAberto);
    #endif
    
    if ((confirm2_rf)and(addCode<0)){
      messageToSend += RF_confirm(now(), nome_rf, RFcode[rfindex]);
      confirm2_rf = false;
      gravarEeprom();
    }
    if (messageToSend == "") return;
    //Serial.println(messageToSend);
    String subMessage = messageToSend.substring(0,messageToSend.indexOf("&&&")); //Pega a primeira msg da fila
    //Serial.println(subMessage);

    Serial.print(F("tam msg="));
    Serial.print(subMessage.length()+1); //Para debug de length de msg
    char messagechar[subMessage.length()+1]; //conversão para envio ao mosquitto (broker MQTT)
    subMessage.toCharArray(messagechar, subMessage.length()+1);
    
    MQTT.publish(TOPICO_PUBLISH.c_str(), messagechar,false); //messageToSend.length()+1
 
    Serial.println(F(" Mensagem enviada ao broker!"));
    //Serial.println(messageToSend);
    messageToSend = messageToSend.substring(messageToSend.indexOf("&&&\n")+4); //Retira a primeira msg da fila
    delay(1000);

    if (swreset) ESP.reset();
}

void time_config(String msg)
{
    //Serial.println("time_config");
    String id = "updated_ntp: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    long var2 = var.toInt();
    messageToSend += time_confirm(var2);

    ntpConnect();
}

void name_config(String msg)
{
    //Serial.println("name_config");
    String id = "new_name: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String nome = var;
    DeviceID = nome;

    id = "new_rele1name: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String rele1nome = var;
    BtnLabel[0] = rele1nome;

    id = "new_rele2name: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String rele2nome = var;
    BtnLabel[1] = rele2nome;
    
    messageToSend += name_confirm(now(),nome,rele1nome,rele2nome);

    gravarEeprom();
}

void comunication_config(String msg)
{
    //Serial.println("comunication_config");
    String id = "new_ssid: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String ssid = var;

    id = "new_password: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String password = var;

    id = "ip_local: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String ip_local = var;

    id = "ap_mod: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String ap_mod = var;

    id = "ap_name: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String ap_name = var;

    id = "ap_password: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String ap_password = var;
    messageToSend += comunication_confirm(now(),ssid,password,ip_local,ap_mod,ap_name,ap_password);

    apPass = ap_password;
    APmode = ap_mod; // 0=sempre 1=auto
    //ipMode = ip_local; // ip 30
    essid = ssid;
    epass = password;
    DeviceID = ap_name;

    gravarEeprom();
    delay(1000);

    ESP.reset();
}

void rf_config(String msg)
{
    Serial.println("rf_config");
    String id = "@";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var = var.substring(1,var.indexOf(":"));
    Serial.println(var);
    String nome = var;

    id = var + ": ";
    i = msg.indexOf(id);
    String var2 = msg.substring(i+id.length());
    var2.replace(char(10),char(13));
    var2.replace(char(13)+char(13),char(13));
    var2 = var2.substring(0,var2.indexOf(char(13)));
    Serial.println(var2);
    String value = var2;
    messageToSend += RF_confirm(now(),nome,value);

    rfindex = -1;
    if (var == "sensor_abre"){
      sensor_abre = var2;
      rfindex = 2;
    }
    if (var == "sensor_fecha"){
      sensor_fecha = var2;
      rfindex = 3;
    }
    if (var == "rele1_rf"){
      rele1_rf = var2;
      rfindex = 0;
    }
    if (var == "rele2_rf"){
      rele2_rf = var2;
      rfindex = 1;
    }

    if (var2 == "store"){
      addCode = rfindex;
      addCodeCnt = 150;
      lastRFcode = "";
      confirm2_rf = 1;
      nome_rf = var;
    }
    else if (var2 == "clear"){
      RFcode[rfindex] = "";
      gravarEeprom();
    }
    /*Serial.print("sensor_abre = ");
    Serial.println(sensor_abre);
    Serial.print("sensor_fecha = ");
    Serial.println(sensor_fecha);
    Serial.print("rele1_rf = ");
    Serial.println(rele1_rf);
    Serial.print("rele2_rf = ");
    Serial.println(rele2_rf);
    */
}

void display_config(String msg)
{
    //Serial.println("display_config");
    String id = "displaytype: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    int displaytype = var.toInt();
    lcdMode = displaytype - 1;

    id = "backlight: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    int backlight2 = var.toInt();
    backlight = backlight2;
    messageToSend += display_confirm(now(),displaytype,backlight2);

    gravarEeprom();
}

void rele1_action(String msg)
{
    //Serial.println("rele1_action");
    String id = "rele1: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    rele1 = var.toInt();
    #ifndef Acesso
    Rele[0] = rele1;
    #endif
    envioRapido = true;
}

void rele2_action(String msg)
{
    //Serial.println("rele2_action");
    String id = "rele2: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    rele2 = var.toInt();
    Rele[1] = rele2;
    envioRapido = true;
}

void SW_restart(String msg)
{
    //Serial.println("SW_restart");
    String id = "swreset: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);

    swreset = 1;    
    messageToSend += SWreset_confirm(now());
}

void autoReset_test(String msg)
{
    //Serial.println("SW_restart");
    String id = "autoreset: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);

    if (AutoResetTestCnt == 0) AutoResetTestCnt = 1;
    messageToSend += autoReset_confirm(now());
}

String UmiTempPresReles(unsigned long ts, int umidade, float temp, bool pres, bool rele1, bool rele2, String abertura, int luz){
    String message;
    message = F(
    "#data_transmission\n"
    "$ts:"); message += ts; message += F("\n"
    "$ty:temp_umi_pres\n"
    "@\n"
    "s1:umidade\n"
    "vl1:"); message += umidade; message += F("\n"
    "s2:temperatura\n"
    "vl2:"); message += temp; message += F("\n"
    "s3:presenca\n"
    "vl3:"); message += pres; message+=F("\n"
    "s4:rl1\n"
    "vl4:"); message += rele1; message+=F("\n"
    "s5:rl2\n"
    "vl5:"); message += rele2; message += F("\n"
    "s6:abertura\n"
    "vl6:"); message += abertura; message += F("\n"
    "s7:luz\n"
    "vl7:"); message += luz; message += F("\n"
    "@\n"
    "&&&\n"
    );
    return message;
}

String time_confirm(unsigned long ts){
    String msg;
    msg = F("#confirmation\n"
    "$ts:"); msg += ts; msg += F("\n"
    "$ty:time_confirm\n"
    "@\n"
    "ntp: "); msg += ts; msg+= F("\n"
    "@\n"
    "&&&\n"
    );
    return msg;
}

String SWreset_confirm(unsigned long ts){
    String msg;
    msg = F("#confirmation\n"
    "$ts:"); msg += ts; msg += F("\n"
    "$ty:sw_reset_confirm\n"
    "@\n"
    "swreset: 1\n"
    "@\n"
    "&&&\n"
    );
    return msg;
}

String autoReset_confirm(unsigned long ts){
    String msg;
    msg = F("#confirmation\n"
    "$ts:"); msg += ts; msg += F("\n"
    "$ty:autoreset_test_confirm\n"
    "@\n"
    "autoreset: 1\n"
    "@\n"
    "&&&\n"
    );
    return msg;
}

String RF_confirm(unsigned long ts, String nome, String value){
    String msg;
    msg = F("#confirmation\n"
    "$ts:"); msg+=ts; msg+= F("\n"
    "$ty:rf_confirm\n"
    "@\n");
    msg+=nome; msg+=F(": "); msg+=value; msg+= F("\n"
    "@\n"
    "&&&\n"
    );
    return msg;
}

String comunication_confirm(unsigned long ts, String ssid, String password, String ip_local, String ap_mod, String ap_name, String ap_password){
    String msg;
    msg = F(
    "#confirmation\n"
    "$ts:"); msg+=ts; msg+=F("\n"
    "$ty:communication_confirm\n"
    "@\n"
    "ssid: "); msg+=ssid;msg+=F("\n"
    "password: "); msg+=password;msg+=F("\n"
    "ip_local: "); msg+=ip_local;msg+=F("\n"
    "ap_mod: "); msg+=ap_mod;msg+=F("\n"
    "ap_name: "); msg+=ap_name;msg+=F("\n"
    "ap_password: "); msg+=ap_password;msg+=F("\n"
    "@\n"
    "&&&\n"
    );
    return msg;
}

String name_confirm(unsigned long ts, String nome, String rele1nome, String rele2nome){
    String msg;
    msg = F(
    "#confirmation\n"
    "$ts:"); msg+=ts; msg+=F("\n"
    "$ty:name_confirm\n"
    "@\n"
    "name: "); msg+=nome; msg+=F("\n"
    "rele1name: "); msg+=rele1nome; msg+=F("\n"
    "rele2name: "); msg+=rele2nome; msg+=F("\n"
    "@\n"
    "&&&\n"
    );
    return msg;
}

String display_confirm(unsigned long ts, int displaytype, int backlight){
    String msg;
    msg = F(
    "#confirmation\n"
    "$ts:"); msg+=ts; msg+=F("\n"
    "$ty:display_confirm\n"
    "@\n"
    "displaytype: "); msg+=displaytype; msg+=F("\n"
    "backlight: "); msg+=backlight; msg+=F("\n"
    "@\n"
    "&&&\n"
    );
    return msg;
}

#ifdef Acesso
String EstAcesso(unsigned long ts,String estAberto,int alarme,int contAberto){
    String message;
    message = F(
    "#data_transmission\n"
    "$ts:"); message += ts; message += F("\n"
    "$ty:acesso_estado\n"
    "@\n"
    "s1:abertura\n"
    "vl1:"); message += estAberto; message += F("\n"
    "s2:alarme\n"
    "vl2:"); message += alarme; message += F("\n"
    "s3:tempo_alarme\n"
    "vl3:"); message += contAberto; message+=F("\n"
    "@\n"
    "&&&\n"
    );
    return message;
}

void alarm_config(String msg)
{
    String id = "alarme: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    int alarme = var.toInt();

    id = "alarme_tempo: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    int alarme_tempo = var.toInt();

    if (alarme == 1){
      AlarmInp[0] = 2; //Sensor #1 = sensor 2, de abertura
      AlarmEml[0] = 1;
      AlarmSeg[0] = alarme_tempo*10;
      delay(1000);
      gravarEeprom();
    }
    messageToSend += alarm_confirm(alarme,AlarmSeg[0]);
}

String alarm_confirm(int alarme, int alarme_tempo){
    String msg;
    msg = F(
    "#confirmation\n"
    "$ts:"); msg+=now(); msg+=F("\n"
    "$ty:alarm_confirm\n"
    "@\n"
    "alarme: "); msg+=alarme; msg+=F("\n"
    "alarme_tempo: "); msg+=alarme_tempo; msg+=F("\n"
    "@\n"
    "&&&\n"
    );
    return msg;
}

void abertura_portao(String msg)
{
    //Serial.println("rele1_action");
    String id = "password: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    if (ReleSenha[0] == var)Rele[0] = 1;
    envioRapido = true;
}

void password_config(String msg)
{
    String id = "old_password: ";
    int i = msg.indexOf(id);
    String var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    String old_password = var;

    id = "new_password: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    String new_password = var;

    if ((ReleSenha[0] == "")or(ReleSenha[0] == old_password)){
      ReleSenha[0] = new_password;
      gravarEeprom();
    }

    messageToSend += password_confirm(old_password,ReleSenha[0]);
}

String password_confirm(String old_password,String new_password){
    String msg;
    msg = F(
    "#confirmation\n"
    "$ts:"); msg+=now(); msg+=F("\n"
    "$ty:password_confirm\n"
    "@\n"
    "password: "); msg+=new_password; msg+=F("\n"
    "@\n"
    "&&&\n"
    );
    return msg;
}
#endif
