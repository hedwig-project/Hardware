void wifiLoop(){
  if(prevWiFistatus!=WiFi.status()){
    prevWiFistatus=WiFi.status();
    addLog(wifiStatusText(WiFi.status()));
    if(WiFi.status()==WL_CONNECTED){
      wifiErrCnt=0;
      setIP();
      String sa;
      sa=F("CONFIG");
      if((APmode==0)||(DeviceID==sa)){
        apCon();
      }else{
        WiFi.softAPdisconnect(true);
        addLog(F("AP: desconectado"));
      }
      if(BlynkNotifyWiFi)BlinkNotifyBuffer += F("conectado, ");
      myip=""; myipPrev=""; myipCnt=10; myipErrCnt=0;
    }else{
      apCon();
    }
  }
  if(essid=="") return;
  if(wifiMillis>millis()) return;
  if(WiFi.status()==WL_IDLE_STATUS){
    addLog(F("Conectando..."));
    WiFi.begin(essid.c_str(), epass.c_str());
    wifiErrCnt++;
    wifiMillis=millis()+(30000*wifiErrCnt);
    return;
  }
  if(WiFi.status()!=WL_CONNECTED){
    WiFi.disconnect();
    wifiMillis=millis()+1000;
    return;
  }
}

void apCon(){
  WiFi.mode(WIFI_AP_STA);
  delay(1000);
  if(WiFi.softAP(DeviceID.c_str(), apPass.c_str())){
    addLog(F("AP: Conectado"));
  }else{
    addLog(F("AP: Falhou")); delay(1000); ESP.reset();
  }
}

void setIP(){
  if(WiFi.status()!=WL_CONNECTED) return;
  if(ipMode<2) return;
  IPAddress ip =  WiFi.localIP();
  if(ipMode==ip[3]) return; // retorna se o ip já foi testado
  IPAddress gateway(192,168,0,1);
  IPAddress subnet(255,255,255,0);
  gateway[0] = ip[0]; gateway[1] = ip[1]; gateway[2] = ip[2];
  byte i;
  for(i=ipMode;i<=255;i++){
    msg=F("Testando IP: ");msg+=i;addLog(msg);
    ip[3]=i;
    if(!Ping.ping(ip,1)){
      delay(100);
      if(!Ping.ping(ip,1)){
        WiFi.config(ip, gateway, subnet);
        break;
      }
    }
  }
  if(ipMode!=i){
    ipMode=i;
    gravarEeprom();
  }
  msg=F("localIP: ");msg+=ipStr(WiFi.localIP());addLog(msg);
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print(centralizar(F("IP local")));
  lcd.setCursor(0, 1); lcd.print(centralizar(ipStr(WiFi.localIP())));
}

String ipStr(IPAddress ip){
  return String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]);
}

String wifiStatusText(byte x) {
  String result;
  if (x == 0) result = F("WL_IDLE_STATUS");
  if (x == 1) result = F("WL_NO_SSID_AVAIL");
  if (x == 2) result = F("WL_SCAN_COMPLETED");
  if (x == 3) result = F("WL_CONNECTED");
  if (x == 4) result = F("WL_CONNECT_FAILED");
  if (x == 5) result = F("WL_CONNECTION_LOST");
  if (x == 6) result = F("WL_DISCONNECTED");
  if (x == 255) result = F("WL_NO_SHIELD");
  if (result == "") result = String(x);
  return result;
}

void conectar(String ssid, String pass){
  String pg;
  #ifdef LCD_SCL
    lcd.setCursor(0, 0); lcd.print(centralizar(F("Conectando a")));
    lcd.setCursor(0, 1); lcd.print(centralizar(ssid));
    lcdCnt = 3;
  #endif
  pg=F("{\"cmd\":\"reload\"}");
  server.send(200, "text/plain", pg);
  if(WiFi.status()==WL_CONNECTED) WiFi.disconnect();
  msg=F("WiFi Conectando a "); msg+=ssid; addLog(msg);
  WiFi.begin(ssid.c_str(), pass.c_str());
    for(byte i=0;i<30;i++){
      addLog(wifiStatusText(WiFi.status()));
      if(WiFi.status()!=WL_DISCONNECTED) break;
      delay(1000);
    }
    //
  if(WiFi.status()==WL_CONNECTED){
    setIP();
    essid = ssid; epass = pass;
    gravarEeprom();
  }else{
    if(essid.length()>0){
      msg=wifiStatusText(WiFi.status()); addLog(msg);
      msg=F("WiFi Reconectando a "); msg+=essid; addLog(msg);
      WiFi.begin(essid.c_str(), epass.c_str());
      //if(WiFi.waitForConnectResult()!=WL_CONNECTED){
      //  WiFi.softAP(DeviceID.c_str(), apPass.c_str());
      //}
      //WiFi.softAPdisconnect(true);
      if(WiFi.status()==WL_CONNECTED) WiFi.disconnect();
      msg=F("WiFi Conectando a "); msg+=ssid; addLog(msg);
      WiFi.begin(ssid.c_str(), pass.c_str());
        //WiFi.waitForConnectResult();
        for(byte i=0;i<30;i++){
          addLog(wifiStatusText(WiFi.status()));
          if(WiFi.status()!=WL_DISCONNECTED) break;
          delay(1000);
        }
    }
  }
  prevWiFistatus=11;
}

String encryptionTypeStr(uint8_t authmode) {
    switch(authmode) {
        case ENC_TYPE_NONE:
            return "NONE";
        case ENC_TYPE_WEP:
            return "WEP";
        case ENC_TYPE_TKIP:
            return "TKIP";
        case ENC_TYPE_CCMP:
            return "CCMP";
        case ENC_TYPE_AUTO:
            return "AUTO";
        default:
            return "?";
    }
}

void handleWiFiCon(){
  String pg;
  String sa,sb,json;
  //
  sa=F("json"); if(server.hasArg(sa)){
    json = ","+server.arg(sa)+",";
    pg=F("{");
    if(json.indexOf(F(",essid,"))>=0){pg+=F("\"essid\":\"");pg+=essid;pg+=F("\",");}
    if(json.indexOf(F(",wifiscan,"))>=0) pg+=wifiscan();
    if(pg.substring(pg.length()-1)==",") pg = pg.substring(0,pg.length()-1);
    pg += F("}");
    server.send(200, "text/plain", pg);
    return;
  }
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  sa=F("essid"); sb=F("epass"); if(server.hasArg(sa) && server.hasArg(sb)){
    conectar(server.arg(sa), epass = server.arg(sb)); return; 
  }
  //
  pg=F(
"<html><head>\r\n"
"<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
"<title>");pg+=DeviceID;pg+=F("</title>\r\n"
"<style>\r\n"
" html,input,p{font:18px Verdana,Arial,Helvetica,sans-serif}\r\n"
" p{text-align:center;margin:8px}"
" input{text-align:center; border-radius:6px}"
" .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
" .inp01{width:100%;min-height:40px}\n"
" .btn0{border-radius:6px;width:42px;height:32px}\r\n"
" .btn01{width:50%;min-height:40px}\r\n"
"</style>\r\n"
"</head>\r\n"
"<body onload='iniciar()' style='margin:0'>\r\n"
"<script>"
"var client = new XMLHttpRequest();\n\r\n"
"client.timeout = 20000;\r\n"
"client.onreadystatechange = function() {\r\n"
"  if (this.readyState==4 && this.status==200){\r\n"
"    var myArr=JSON.parse(this.responseText);\r\n"
"    myFunction(myArr);\r\n"
"  }\r\n"
"}\r\n"
"function myFunction(myObj) {\r\n"
"  var cmd='';"
"  if(typeof myObj.cmd!='undefined'){ cmd=myObj.cmd; }\r\n"
"  if(cmd=='reload'){setTimeout(function(){location.reload();}, 3000);}"
"  if(typeof myObj.essid!='undefined'){essid.value=myObj.essid;}\r\n"
"  if(typeof myObj.wifiscan!='undefined'){\r\n"
"    myObj.wifiscan.sort();\r\n"
"    pg='';\r\n"
"    for(i=0;i<myObj.wifiscan.length;i++){\r\n"
"      sa=myObj.wifiscan[i];\r\n"
"      le=sa.substring(0,sa.indexOf(','));\r\n"
"      sa=sa.substring(sa.indexOf(',')+1);\r\n"
"      ch=sa.substring(0,sa.indexOf(','));\r\n"
"      sa=sa.substring(sa.indexOf(',')+1);\r\n"
"      ss=sa.substring(0,sa.indexOf(','));\r\n"
"      sa=sa.substring(sa.indexOf(',')+1);\r\n"
"      pg+='<a onclick=\"setessid(\\\''+ss+'\\\')\">';\r\n"
"      if(sa=='*'){pg+='<font color=red>';\r\n"
"      }else{pg+='<font color=blue>';}\r\n"
"      ss=ss.substring(0,19);\r\n"
"      pg+=ss+'</font>'+sa+' ('+le+') ch'+ch+'</a><br>';\r\n"
"    }\r\n"
"    span01.innerHTML=pg;\r\n"
"  }\r\n"
"}\r\n"  
  
"function conectar(){\r\n"
" sa='?essid='+essid.value+'&epass='+epass.value;\r\n"
" client.open('GET', sa, true);client.send();\r\n"
"}\r\n"

"function setessid(xx){essid.value=xx;epass.focus();}\r\n"

"  function iniciar(){\r\n"
"    if(location.href.indexOf('wificon')<0){btnVoltar.style.visibility='hidden';}"
"    client.open('GET','?json=essid,wifiscan',true); client.send();\r\n"
"  }\r\n"

"</script>\r\n"

"<div style='width:100%' align='center'>"
"  <table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='max-width:360px;"
"    background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
"    <tr height='32' style='font-size:20px'>\r\n"
"      <td style='width:15%;text-align:left'><input id=btnVoltar type=button class=btn0 onclick='history.go(-1)' value='<'></td>\r\n"
"      <td style='width:70%;text-align:center'><b>WiFi</b></td>\r\n"
"      <td style='width:15%' align=right></td>\r\n"
"    </tr>\r\n"
"  </table>\r\n"
"  <table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'>\n"
"   <tr><td style='text-align:center;padding:8px;'>\n"

"   <div class='divbl' align='center'>"
"     <p>Nome da Rede (SSID)<br><input name='essid' id='essid' type='text' class='inp01'></p>"
"     <p>Senha<br><input name='epass' id='epass' type='text' class='inp01'></p>\n"
"     <p style='margin-top:16px'><input type='button' value='Conectar' onclick='conectar()' class='btn01'></p>"
"   </div>"

"   <div class='divbl' align='center'>"
"     <p style='text-align:left;line-height:32px;margin-left:20px;text-wrap:none'>\r\n"
"     <span id='span01'>procurando...</span></p>\r\n"
"   </div>"

"  </td></tr></table>\n"
"</div>\r\n"
"</body>\r\n"
"</html>\r\n"
  );
  server.send(200, "text/html", pg);  
}

String wifiscan(){
  // scanNetworks(bool async = false, bool show_hidden = false);
  int n = WiFi.scanNetworks(false,true);
  String sa;
  if (n > 0){
    sa=F("\"wifiscan\":[");
    for (int i = 0; i < n; ++i){
      sa+=F("\"");
      sa+=WiFi.RSSI(i);sa+=F(",");
      sa+=WiFi.channel(i);sa+=F(",");
      sa+=WiFi.SSID(i);sa+=F(",");
      if(WiFi.encryptionType(i) == ENC_TYPE_NONE)sa+=F("*\""); else sa+=F(" \"");
      if(i<(n-1)) sa+=F(",");
    }
    sa+=F("],");
  }
  return sa;
}

void localPing(){ // a cada 10 seg
  if(WiFi.status() != WL_CONNECTED) return;
  IPAddress ip=WiFi.localIP();
  ip[3]=1;
  if(Ping.ping(ip,1)){
    int avg_time_ms = Ping.averageTime();
    localPingErrCnt=0;
  }else{
    localPingErrCnt++;
    msg=F("localPingErrCnt: ");msg+=localPingErrCnt; addLog(msg);
    if(localPingErrCnt==10) WiFi.disconnect(); // após 100 segundos
    if(localPingErrCnt>=20) ESP.reset(); // reset após 200 segundos
  }
}

