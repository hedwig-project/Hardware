void myipRun(){ // a cada 1 segundo
  if(WiFi.status() != WL_CONNECTED) myipCnt=10;
  if (timeStatus() == timeNotSet) myipCnt=10;
  if(myipCnt>0) myipCnt--;
  if(myipCnt==0){
    String sa=checkMyIP();
    if(sa.length()>0){
      myip=sa;
      if(myipPrev!=myip){
        myipPrev=myip;
        BlynkEmailBuffer="enviarLogEmail";
        BlynkEmailWaitSecs=0;
      }
    }
  }
}

String checkMyIP(){
  if(WiFi.status() != WL_CONNECTED) return "";
  WiFiClient client;
  uint16_t port;
  port=80;
  msg=F("myip[");msg+=myipErrCnt; msg+=F("] ");msg+=myipServer[myipErrCnt%3];addLog(msg);
  if (client.connect(myipServer[myipErrCnt%3].c_str(), port)) {
    client.println("GET "+myipReq[myipErrCnt%3]+" HTTP/1.1");
    client.println("Host: "+myipServer[myipErrCnt%3]);
    client.println("Connection: close");
    client.println();
    //
    unsigned long i=millis();
    while(!client.available()){
      if ((millis() - i) >= 20000) break;
    }
    //
    String line, sa;
    while(client.available()) {
      line = client.readStringUntil('\r');
    }
    client.stop();
    if(line.indexOf("\"ip\"")>=0){
      sa=line;
      sa=sa.substring(sa.indexOf("\"ip\"")+4);
      sa=sa.substring(sa.indexOf(":")+1);
      sa=sa.substring(sa.indexOf("\"")+1);
      sa=sa.substring(0,sa.indexOf("\""));
      int i;
      i=8000+WiFi.localIP()[3];
      sa="http://"+sa+":"; sa+=i;
      myipErrCnt=0;
      myipCnt=21600; // segundos até a próxima verif 86400=24h 3600=1h 21600=6h
      msg=F("myip: ");msg+=sa; addLog(sa);
      return sa;
    }
  }else{
    addLog(F("myip falhou!"));
  }
  if(myipErrCnt%6==2){
    WiFi.disconnect();
    addLog(F("myip Reiniciando WiFi"));
  }
  if(myipErrCnt%6==5) {
    WiFi.disconnect();
    //reiniciar modem internet
    addLog(F("myip Reiniciando MODEM"));
  }
  myipCnt=60; // segundos até a próxima verificação
  if(myipErrCnt>=5) myipCnt=600;    //10min
  if(myipErrCnt>=11) myipCnt=6000;  //100min
  myipErrCnt++;
  return "";
}

