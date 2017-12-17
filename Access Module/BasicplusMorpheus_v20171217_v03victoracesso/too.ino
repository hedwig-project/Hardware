void handleTools(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  String sa;
  String cmd;
  sa=F("cmd");if(server.hasArg(sa)) cmd=server.arg(sa);
  sa=F("AutoResetTest");if(cmd==sa){
    AutoResetTestCnt=1;
    pg=F("{\"cmd\":\"voltar2\"}");
    server.send(200, "text/plain", pg);
    //msg=user; msg+=F(" > Auto teste");addLog(msg);
  }
  sa=F("reiniciar");if(cmd==sa) {
    pg = F("<!doctype html><html><body onload='history.go(-3);'></html>");
    server.send(200, "text/html", pg);
    #ifdef LCD_SCL
      lcd.setCursor(0, 0); lcd.print(centralizar(F(" ")));
      lcd.setCursor(0, 1); lcd.print(centralizar(F("Reiniciando")));
    #endif
    delay(1000);
    ESP.reset();
  }
  sa=F("desconectar");if(cmd==sa){
    pg=F("{\"cmd\":\"voltar2\"}");
    server.send(200, "text/plain", pg);
    WiFi.disconnect();
    essid=""; epass="";
    gravarEeprom();
    ESP.reset();
  }

  sa=F("hardreset");if(cmd==sa){
    pg=F("{\"cmd\":\"voltar2\"}");
    server.send(200, "text/plain", pg);
    HardReset();
  }
  
  pg=F(
  "<html><head>\r\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style>\r\n"
  "  body{margin:0}\r\n"
  " html,input,p{font:18px Verdana,Arial,Helvetica,sans-serif}\r\n"
  " h2{margin-top:30px;text-align:center}\r\n"
  " p{text-align:center;margin:8px}\r\n"
  " input{text-align:center; border-radius:6px}\r\n"
  " .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .btn1{height:42px;width:100%;margin-bottom:6px;}\n"
  " .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
  "</style>\r\n"
  "</head>"
  "<body>\r\n"
  
  "<script>"
  "var cmd='';"
  "  var client = new XMLHttpRequest();\n\r\n"
  "  client.onreadystatechange = function(){ if(this.readyState==4 && this.status==200){ myFunction(JSON.parse(this.responseText)); } }\r\n"
  "  function myFunction(myObj) {\r\n"
  "    if(typeof myObj.cmd!='undefined'){\r\n"
  "      sa=myObj.cmd;\r\n"
  "      if(sa=='voltar2'){history.go(-2);}\r\n"
  "      if(sa=='w10voltar2'){setTimeout(function(){history.go(-2)}, 10000);}\r\n"
  "    }\r\n"
  "  }\r\n"
  
  "function AutoResetTest(){client.open('GET', '?cmd=AutoResetTest', true);client.send();}\n"
  
  "function reiniciar(){location.href='?cmd=reiniciar';}\n"
  
  "function hardreset(){\n"
  "  if(confirm('Hard Reset - Restaurar configuracoes de fabrica.')){"
  "    client.open('GET', '?cmd=hardreset', true);client.send();"
  "  }"
  "}"
  
  "</script>\r\n"
  
  "<div style='width:100%' align='center'>"
  "  <table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='max-width:360px;"
  "    background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  "    <tr height='32' style='font-size:20px'>\r\n"
  "      <td style='width:15%;text-align:left'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\r\n"
  "<td style='width:70%;text-align:center'><b>Ferramentas</b></td>\r\n"
  "      <td style='width:15%' align='right'></td>\r\n"
  "    </tr>\r\n"
  "  </table>\r\n");
  pg+=F(
"  <table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'>\n"
"   <tr><td style='text-align:center;padding:8px;'>\n"
"     <div class='divbl'>"
  "<input type=button class=btn1 onclick='AutoResetTest()' value='Teste Auto-Reset'>\n"
  "<input type=button class=btn1 onclick='reiniciar()' value='Reiniciar'>\n"
  );
  if(WiFi.status() == WL_CONNECTED) pg+=F("<input type=button class=btn1 onclick='desconectar()' value='Desconectar'>\n");
  pg+=F(
  "<input type=button class=btn1 onclick='hardreset()' value='Hard Reset'>\n"
  "<input type=button class=btn1 onclick='location.href=\"fir\"' value='Firmware'>\n"
  
  "       </div>\r\n"
  "    </td></tr></table>\n"
  "  </div></body></html>\r\n"
  );
  server.send(200, "text/html", pg);  
}

void HardReset(){
    essid=""; epass="";
    WiFi.disconnect();
    WiFi.softAPdisconnect(true);
    preset();
    gravarEeprom();
    #ifdef LCD_SCL
    lcd.setCursor(0, 0); lcd.print(centralizar(F("Hard Reset")));
    lcd.setCursor(0, 1); lcd.print(centralizar(F("Desconectando")));
    delay(1000);
    lcd.setCursor(0, 0); lcd.print(centralizar(F("RESTAURANDO CONF")));
    lcd.setCursor(0, 1); lcd.print(centralizar(F("DE FABRICA")));
    #endif
    delay(1000);
    ESP.reset();  
}

