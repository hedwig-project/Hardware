void blynkLoop(){
  if (Blynk.connected()) Blynk.run();
  if(prevBlynkSt!=Blynk.connected()){
    prevBlynkSt=Blynk.connected();
    if(prevBlynkSt){
      addLog(F("BLYNK: ok"));
    }else{
      addLog(F("BLYNK: !!"));
    }
  }
  if(blynkAuth.length()==0) return;
  if(WiFi.status() != WL_CONNECTED) BlynkWaitMillis=millis()+15000;
  if (timeStatus() == timeNotSet) BlynkWaitMillis=millis()+15000;
  if(myip.length()<1) BlynkWaitMillis=millis()+15000;
  if(BlynkWaitMillis>millis()) return;
  if((BlinkNotifyBuffer.length()<1) && ((BlynkEmailBuffer.length()<1) || (BlynkEmailWaitSecs>0))) return;
  if (Blynk.connected()){
    // Notificação
    if(BlinkNotifyBuffer.length()>0){
      BlinkNotifyBuffer=BlinkNotifyBuffer.substring(0,BlinkNotifyBuffer.length()-2);
      BlinkNotifyBuffer+=".";
      msg=F("BlynkNotify: "); addLog(msg+BlinkNotifyBuffer);
      BlinkNotifyBuffer=essid+">"+DeviceID+": "+BlinkNotifyBuffer;
      Blynk.notify(BlinkNotifyBuffer);
      BlinkNotifyBuffer="";
      BlynkWaitMillis=millis()+10000;
      return;
    }
    // email
    if((BlynkEmailBuffer.length()>0) && (BlynkEmailWaitSecs==0)){
      msg=F("Enviando email");
      if(BlynkEmailBuffer=="enviarLogEmail"){
        enviarLogEmail();
      }else{
        Blynk.email(essid+" "+DeviceID, BlynkEmailBuffer);
      }
      if(reenviaremail>0){
        BlynkEmailWaitSecs=reenviaremail*3600;
        msg+=F(" e aguardando conf.");
      }else{
        BlynkEmailBuffer="";
      }
      BlynkWaitMillis=millis()+10000;
      addLog(msg);
      return;
    }
    //
  }else{
    addLog(F("BLYNK: conectando..."));
    delay(100);
    Blynk.config(blynkAuth.c_str());
    BlynkWaitMillis=millis()+10000+(BlynkConnErrCnt*10000);
    digitalWrite(LedPin, LOW);
    if (Blynk.connect()) {
      //addLog(F("BLYNK: conectado"));
      BlynkWaitMillis=millis()+1000;
      BlynkConnErrCnt=0;
    }else{
      //addLog(F("BLYNK: falhou!"));
      if(BlynkConnErrCnt<360) BlynkConnErrCnt++;
    }
  }
}

void handleBlynk(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  String sa;
  sa = "BlynkNotifyIni"; if(server.hasArg(sa)) BlynkNotifyIni = server.arg(sa).toInt();
  sa = "BlynkNotifyWiFi"; if(server.hasArg(sa)) BlynkNotifyWiFi = server.arg(sa).toInt();
  sa = "blynkAuth"; if(server.hasArg(sa)) blynkAuth = server.arg(sa);
  sa = "reenviaremail"; if(server.hasArg(sa)) reenviaremail = server.arg(sa).toInt();
  if(server.hasArg(sa)){
    pg = F("<!doctype html><html><body onload='history.go(-2)'></html>");
    server.send(200, "text/html", pg);
    Blynk.disconnect();
    BlynkWaitMillis=millis()+15000;
    gravarEeprom();
    return;
  }
  pg=F(
  "<!doctype html><html>\n"
  "<head>\r\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style type='text/css'>\n"
  "  body{margin:0}\n"
  "  html,input,select,textarea{font:18px Verdana,Arial,Helvetica,sans-serif}\n"
  "  input,select{border-radius:6px;text-align:center;height:34px}\n"
  "  textarea{border-radius:6px;font-size:20px;width:90%}\n"
  "  input[type=submit]{height:36px; width:50%}"
  "  hr{margin-top:20px;margin-bottom:0px;width:100%}"
  "  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
  "  p{margin:0 0 8px 0;line-height:240%;text-align:center}\n"
  "</style>\n"
  "</head>"
  "<body onload='iniciar()'>\n"
  
  "<script>function iniciar(){"
  "  blynkAuth.value='"); pg += blynkAuth; pg += F("';"
  "  BlynkNotifyIni.value='"); pg += BlynkNotifyIni; pg += F("';"
  "  BlynkNotifyWiFi.value='"); pg += BlynkNotifyWiFi; pg += F("';"
  "  reenviaremail.value='"); pg += reenviaremail; pg += F("';"
  "}"
  "</script>"
  "<div align='center'>"
  "<table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='max-width:360px;background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  "<tr height='32' style='font-size:20px'>\n"
  "<td style='width:15%'><input type='button' class=btn0 onclick='history.go(-1)' value='<'></td>\n"
  "<td style='width:70%;text-align:center;font-size:17px'><b>BLYNK</b></td>\n"
  "<td style='width:15%' align='right'><input type=button class=btn0 onclick='form1.submit()' value='OK'></td>\n"
  "</tr>"
  "</table>"
  "<table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'>"
  "<tr>"
  "<td style='text-align:center;padding:8px;width:100%'>"
  "<form id='form1' method='get' action=''>\n"
  
  "<div class='divbl'>"
  "         <p><b>Auth Token</b><br>"
  "         <input type=text name='blynkAuth' id='blynkAuth' style='width:94%'></p>"
  "</div>"
  
  "<div class='divbl'>"
  "       <p><b>Notifica&ccedil;&otilde;es</b><br>"
  "         ao iniciar "
  "<select name='BlynkNotifyIni' id='BlynkNotifyIni'>"
  " <option value='1'>Sim</option>"
  " <option value='0'>N&atilde;o</option>"
  "</select><br>"
  "WiFi <select name='BlynkNotifyWiFi' id='BlynkNotifyWiFi'>"
  "<option value='1'>Sim</option>"
  "<option value='0'>N&atilde;o</option>"
  "</select></p>"
  "</div>"

  "<div class='divbl'>\n"
  "<p><b>e-mail</b><br>\n"
  "Confirmar/reenviar<br>\n"
  "<select id='reenviaremail' name='reenviaremail'>\n"
  "<option value='0'>N&atilde;o</option>\n"
  "<script>for(i=1;i<=24;i++){document.writeln('<option value='+i+'>'+i+' horas</option>')}</script>\n"
  "</select></p>\n"
  "</div>\n"
  
  "</form>\n"
  "</td></tr></table></div></body></html>\n");
  server.send(200, "text/html", pg);  
}

void enviarLogEmail(){
  String pg;
        String sa=log_str, sb, sc;
        unsigned long t;
        pg="";
        if(reenviaremail>0){
          sb="Confirmar: <a href='"+myip+"?email=ok'>"+myip+"</a><br><br>";
        }else{
          sb="<a href='"+myip+"'>"+myip+"</a><br><br>";
        }
        while(sa.indexOf(";")>=0){
          t=sa.substring(0,sa.indexOf(" ")).toInt();  sa=sa.substring(sa.indexOf(" ")+1);
          sc=sa.substring(0,sa.indexOf(";"));         sa=sa.substring(sa.indexOf(";")+1);
          t += BRtimeZone(t) * SECS_PER_HOUR;
          if (day(t)<10) pg+="0";
          pg+=day(t); pg+="/"; if (month(t)<10) pg+="0";
          pg+=month(t); pg+="/"; pg+=year(t); pg+=" "; if (hour(t)<10) pg+="0";
          pg+=hour(t); pg+=":"; if (minute(t)<10) pg+="0";
          pg+=minute(t); pg+=":"; if (second(t)<10) pg+="0";
          pg+=second(t); pg+=" "+sc+"<br>";
        }
        while((sb.length()+pg.length())>1150) pg=pg.substring(pg.indexOf("<br>")+4);
  Blynk.email(essid+" "+DeviceID, sb+pg);
}

