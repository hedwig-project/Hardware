/*/
// ----- Alertas e Alarmes
byte AlarmInp[2];
unsigned int AlarmSeg[2];
unsigned int AlarmCnt[2]; // em segundos
bool AlarmNtf[2];
bool AlarmEml[2];
/*/

void AlarmLoop(){ // a cada 1 segundo
  String pg;
  for(byte i=0;i<2;i++){
    if(AlarmInp[i]>0){
      if(Sensor[AlarmInp[i]-1]){
        if(AlarmCnt[i]==AlarmSeg[i]){
          pg=F("Sensor "); pg+=AlarmInp[i]; pg+=F(" aberto");
          if(AlarmSeg[i]>0){pg+=F(" a mais de "); pg+=AlarmSeg[i]; pg+=F("seg");}
          // enviar notificação
          if(AlarmNtf[i] == 1){
            BlinkNotifyBuffer += pg+",";
          }
          // enviar email
          if(AlarmEml[i] == 1){
            BlynkEmailBuffer=pg;
          }
        }
        AlarmCnt[i]++;
      }else{
        AlarmCnt[i]=0;
      }
    }else{
      AlarmCnt[i]=0;
    }
  }
  if(AlarmCnt[0]>AlarmSeg[0] || AlarmCnt[1]>AlarmSeg[1]){
    digitalWrite(buzzerPin, HIGH);
    delay(20);
    digitalWrite(buzzerPin, LOW);
  }
}

void handleAlarm(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  String sa,sb,sc,sd,fields;
  sa=F("AlarmInp0");if(server.hasArg(sa)) AlarmInp[0]=server.arg(sa).toInt();
  sa=F("AlarmSeg0");if(server.hasArg(sa)) AlarmSeg[0]=server.arg(sa).toInt();
  sa=F("AlarmNtf0");if(server.hasArg(sa)) AlarmNtf[0]=server.arg(sa).toInt();
  sa=F("AlarmInp1");if(server.hasArg(sa)) AlarmInp[1]=server.arg(sa).toInt();
  sa=F("AlarmSeg1");if(server.hasArg(sa)) AlarmSeg[1]=server.arg(sa).toInt();
  sa=F("AlarmNtf1");if(server.hasArg(sa)) AlarmNtf[1]=server.arg(sa).toInt();
  sa=F("AlarmEml0");if(server.hasArg(sa)) AlarmEml[0]=server.arg(sa).toInt();
  sa=F("AlarmEml1");if(server.hasArg(sa)) AlarmEml[1]=server.arg(sa).toInt();
  if(server.hasArg(sa)){
    gravarEeprom();
    pg = F("<!doctype html><html><body onload='history.go(-2)'></html>");
    server.send(200, "text/html", pg);
    return;
  }
  
  pg=F(
  "<!DOCTYPE html>\r\n"
  "<html>\r\n"
  "<head>\r\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style>\r\n"
  "  body{margin:0}\r\n"
  "  html,input,p,select{font:18px Verdana,Arial,Helvetica,sans-serif}\r\n"
  "  p{text-align:center;margin:8px;line-height:200%}\r\n"
  "  input,select{text-align:center; border-radius:6px}\r\n"
  "  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
  "</style>\r\n"
  "</head>"
  "<body onload=iniciar()>\r\n"
  "<script>\r\n"
  "function iniciar(){"
  "  AlarmInp0.value=");pg+=AlarmInp[0];pg+=F(";"
  "  AlarmSeg0.value=");pg+=AlarmSeg[0];pg+=F(";"
  "  AlarmNtf0.value=");pg+=AlarmNtf[0];pg+=F(";"
  "  AlarmInp1.value=");pg+=AlarmInp[1];pg+=F(";"
  "  AlarmSeg1.value=");pg+=AlarmSeg[1];pg+=F(";"
  "  AlarmNtf1.value=");pg+=AlarmNtf[1];pg+=F(";"
  "  AlarmEml0.value=");pg+=AlarmEml[0];pg+=F(";"
  "  AlarmEml1.value=");pg+=AlarmEml[1];pg+=F(";"
  "}"
  "</script>\r\n"
  "<div style='width:100%' align='center'>\r\n"
  "  <table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  "    <tr height='32' style='font-size:20px'>\r\n"
  "      <td style='width:15%;text-align:left'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\r\n"
  "      <td style='width:70%;text-align:center'><b>Alertas e Alarmes</b></td>\r\n"
  "      <td style='width:15%' align='right'><input type=button class=btn0 onclick='form1.submit()' value='OK'></td>\r\n"
  "    </tr>\r\n"
  "  </table>\r\n"
  "  <table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'>\n"
  "   <tr><td style='text-align:center;padding:8px;'>\n"
  "<form id=form1 method=get>\r\n"
  );
  for(byte i=0;i<2;i++){
    byte j=i+1;
    pg+=F("<div class=divbl><p>\r\n"
    "<b>Alarme ");pg+=j;pg+=F("</b> <select name=AlarmInp");pg+=i;pg+=F(" id=AlarmInp");pg+=i;pg+=F(">\r\n"
    "<option value=0>Desativado</option>\r\n"
    "<option value=1>Sensor 1</option>\r\n"
    "<option value=2>Sensor 2</option>\r\n"
    "</select><br>\r\n"
    " aberto por mais de <select name=AlarmSeg");pg+=i;pg+=F(" id=AlarmSeg");pg+=i;pg+=F(">\r\n"
    "<script>for(i=0;i<60;i+=15){document.writeln('<option value='+i+'>'+i+' seg</option>');}</script>\r\n"
    "<script>for(i=1;i<=10;i++){j=i*60;document.writeln('<option value='+j+'>'+i+' min</option>');}</script>\r\n"
    "</select>\r\n"
    
    "<br>Notificar <select name=AlarmNtf");pg+=i;pg+=F(" id=AlarmNtf");pg+=i;pg+=F(">\r\n"
    "<option value=0>N&atilde;o</option>\r\n"
    "<option value=1>Sim (Blynk)</option>\r\n"
    "</select>\r\n"
    
    "<br>Enviar email <select name=AlarmEml");pg+=i;pg+=F(" id=AlarmEml");pg+=i;pg+=F(">\r\n"
    "<option value=0>N&atilde;o</option>\r\n"
    "<option value=1>Sim (Blynk)</option>\r\n"
    "</select>\r\n"
    
    "</p></div>\r\n"
    );
  }
  pg+=F("</form>"
  "  </td></tr></table>\n"
  "     </div>\r\n"
  "</div></body></html>\r\n");
  server.send(200, "text/html", pg);  
}

