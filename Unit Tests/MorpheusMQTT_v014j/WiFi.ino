void initWiFi() 
{
    delay(10);
    Serial.println(F("------Conexao WI-FI------"));
    Serial.print(F("Conectando-se na rede: "));
    Serial.println(SSID);
    Serial.println(F("Aguarde"));
     
    reconectWiFi();
}

void reconectWiFi() //Reconecta-se ao WiFi
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print(F("Conectado com sucesso na rede "));
    Serial.println(SSID);
    Serial.print(F("IP obtido="));
    Serial.println(WiFi.localIP());
}
