void initMQTT() // Inicializa parâmetros de conexão MQTT(endereço do broker, porta e seta função de callback)
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);
    MQTT.setCallback(mqtt_callback);
}

void reconnectMQTT()  //Reconecta-se ao broker MQTT (caso ainda não esteja conectado ou em caso de a conexão cair).
{                     //Em caso de sucesso na conexão ou reconexão, o subscribe dos tópicos é refeito.
    while (!MQTT.connected()) 
    {
        Serial.print(F("* Tentando se conectar ao Broker MQTT: "));
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT,LOGIN_MQTT,SENHA_MQTT)) 
        {
            Serial.println(F("Conectado com sucesso ao broker MQTT!"));
            MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println(F("Falha ao reconectar no broker."));
            Serial.println(F("Havera nova tentativa de conexao em 2s"));
            delay(2000);
        }
    }
}

void VerificaConexoesWiFIEMQTT(void)  //Verifica o estado das conexões WiFI e ao broker MQTT. 
{
    if (!MQTT.connected()){ //se não há conexão com o Broker, a conexão é refeita
        reconnectMQTT();
    }
    reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
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
    messageToSend = time_confirm(var2);
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

    id = "new_rele1name: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String rele1nome = var;

    id = "new_rele2name: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    String rele2nome = var;
    messageToSend = name_confirm(0,nome,rele1nome,rele2nome);
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
    messageToSend = comunication_confirm(0,ssid,password,ip_local,ap_mod,ap_name,ap_password);
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
    messageToSend = RF_confirm(0,nome,value);
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

    id = "backlight: ";
    i = msg.indexOf(id);
    var = msg.substring(i+id.length());
    var.replace(char(10),char(13));
    var.replace(char(13)+char(13),char(13));
    var = var.substring(0,var.indexOf(char(13)));
    Serial.println(var);
    int backlight = var.toInt();
    messageToSend = display_confirm(0,displaytype,backlight);
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
    messageToSend = SWreset_confirm(0);
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
    messageToSend = autoReset_confirm(0);
}

void enviaMsgMQTT(void) //Envia mensagem ao broker
{
    //messageToSend = UmiTempPresReles(0,80,25,1,1,0);
    //messageToSend = time_confirm(100);
    //messageToSend = SWreset_confirm(150);
    //messageToSend = autoReset_confirm(200);
    //messageToSend = RF_confirm(175, "Portal", "03920392");
    //messageToSend = comunication_confirm(100, "ssidV", "1234", "192.168.0.20", "automatico", "Airwalk", "Pikachu");
    //messageToSend = name_confirm(27893, "Hedwig_Lamarr", "Porta1", "Porta2");
    //messageToSend = display_config(787, 2, 1);

    if (messageToSend == "") return;
    //Serial.println(messageToSend);
    
    Serial.print(F("tam msg="));
    Serial.print(messageToSend.length()+1); //Para debug de length de msg
    char messagechar[messageToSend.length()+1]; //conversão para envio ao mosquitto (broker MQTT)
    messageToSend.toCharArray(messagechar, messageToSend.length()+1);
    
    MQTT.publish(TOPICO_PUBLISH, messagechar,false); //messageToSend.length()+1
 
    Serial.println(F(" Mensagem enviada ao broker!"));
    messageToSend = "";
    delay(1000);
}

String UmiTempPresReles(unsigned long ts, int umidade, float temp, bool pres, bool rele1, bool rele2){
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
    "@\n"
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
    );
    return msg;
}

String SWreset_confirm(unsigned long ts){
    String msg;
    msg = F("#confirmation\n"
    "$ts:"); msg += ts; msg += F("\n"
    "$ty:sw_reset_confirm\n"
    "@\n"
    "swreset\n"
    "@\n"
    );
    return msg;
}

String autoReset_confirm(unsigned long ts){
    String msg;
    msg = F("#confirmation\n"
    "$ts:"); msg += ts; msg += F("\n"
    "$ty:autoreset_test_confirm\n"
    "@\n"
    "autoreset\n"
    "@\n"
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
    );
    return msg;
}
