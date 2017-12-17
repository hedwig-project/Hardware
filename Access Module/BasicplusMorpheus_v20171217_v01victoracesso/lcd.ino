#ifdef LCD_SCL

void LCDloop(){
  if(prevSecondLCD == second()) return;
  prevSecondLCD = second();
  prevMillisLCD = millis();
  if(AutoResetTestCnt>0){
    AutoResetTestCnt++;
    lcd.setCursor(0, 0); lcd.print(centralizar(F("Teste Auto-Reset")));
    lcd.setCursor(0, 1); lcd.print(centralizar(String(AutoResetTestCnt-1)));
    return;
  }
  if(lcdCnt>0){lcdCnt--;return;}
  if(lcdMode==0) MostrarRelogio1();
  if(lcdMode==1) MostrarRelogio2();
  if(lcdMode==2) MostrarRelogio3();
}

void handleLCD(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  String sa,sb;
  sa = "lcdMode"; sb = F("backlight");
  if(server.hasArg(sa) && server.hasArg(sb)){
    lcdMode = server.arg(sa).toInt();
    backlight = server.arg(sb).toInt();
    gravarEeprom();
    pg = F("<!doctype html><html><body onload='history.go(-2)'></html>");
    server.send(200, "text/html", pg);
    return;
  }
  if(server.hasArg(sa)) lcdMode = server.arg(sa).toInt();
  if(server.hasArg(sb)) backlight = server.arg(sb).toInt();
  sa="fields"; if(server.hasArg(sa)){
    pg=F("{\"lcdMode\":");pg+=lcdMode;pg=F(",");
    pg=F("\"backlight\":");pg+=backlight;pg=F("}");
    server.send(200, "text/plain", pg);
    return;
  }
  pg="";
  pg+=F(
  "<!doctype html><html>\r\n"
  "<head>\r\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style type='text/css'>\n"
  "  body{margin:0}\n"
  "  html,input,select{font:18px Verdana,Arial,Helvetica,sans-serif}\n"
  "  input {border-radius:6px;text-align:center;height:26px}\n"
  "  select{border-radius:6px;text-align:center;height:32px;width:40%}\n"
  "  p{margin:8px 0 8px 0;line-height: 200%;text-align:center}\n"
  "  .p1{font-size:14px;text-align:left;margin-left:20px}\r\n"
  "  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
  "</style>\n"
  "</head>\r\n"
  "<body onload='iniciar()'>\n"
  
  "<script>\r\n"
  
  "var client = new XMLHttpRequest();\n\r\n"
  "client.onreadystatechange = function() {\r\n"
  "  if (this.readyState==4 && this.status==200){\r\n"
  "    myObj=JSON.parse(this.responseText);\r\n"
  "    if(typeof myObj.lcdMode!='undefined'){lcdMode.value=myObj.lcdMode}\r\n"
  "    if(typeof myObj.backlight!='undefined'){backlight.value=myObj.backlight}\r\n"
  "  }\r\n"
  "}\r\n"

  "  function iniciar(){\r\n"
  "    lcdMode.value='");pg+=lcdMode;pg+=F("';\r\n"
  "    backlight.value='");pg+=backlight;pg+=F("';\r\n"
  "  }\r\n"

  "</script>\n"
  
  "<div align=center width=100%>\n"
  "  <table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  "<tr height='32' style='font-size:20px'>\n"
  "<td style='width:15%'><input type='button' onclick='history.go(-1)' value='<' style='width:42px;height:32px'></td>\n"
  "<td style='width:70%;text-align:center'><b>");pg+=DeviceID;pg+=F("</b></td>\r\n"
  "<td style='width:15%' align='right'><input id='salvarBtn' type=button class=btn0 onclick='form1.submit()' value='OK'></td>\n"
  "</tr></table>"
  "<table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'>\n"
  "<tr><td style='text-align:center;padding:8px;'>\n"
  
  "<div class='divbl'>\r\n"
  "<p><b>Display</b></p>"
  "<form id=form1 method=get>\r\n"
  "<p>Modo <select id='lcdMode' name='lcdMode' style='font-size:18px;' onchange='client.open(\"GET\", \"?fields=all&lcdMode=\"+this.value, true);client.send();'>\r\n"
  "<option value='0'>1</option>\r\n"
  "         <option value='1'>2</option>\r\n"
  "         <option value='2'>3</option>\r\n"
  "       </select><br>\r\n"
  //"       Brilho <input type='range' id='backlight' min='0' max='1'  onchange='backlightChange()' style='width:60px;' step='1'><br>"
  "Brilho "
  "<select id='backlight' name='backlight' onchange='client.open(\"GET\", \"?fields=all&backlight=\"+this.value, true);client.send();'>\r\n"
  "<option value='0'>Baixo</option>\r\n"
  "<option value='1'>Alto</option>"
  "<option value='2'>Autom&aacute;tico</option>"
  "</select>\r\n"
  //"<br><input type='button' value='Salvar' onclick='salvar()' id='salvarBtn' style='height:36px;width:50%' disabled>"
  "</p>"
  "</form>\r\n"
  "</div>\r\n"
        
  "</td></tr></table></div></body></html>\n");
  
  server.send(200, "text/html", pg);  
}


String centralizar(String sa){
  String sb; int i;
  i = sa.length(); i = (16 - i)/2; if(i < 0) i = 0;
  sb = F("        "); sb = sb.substring(0,i);
  sa = sb+sa;
  sa += F("         ");
  sa = sa.substring(0,16);
  return sa;
}

void MostrarRelogio1(){
  if(prev_lcdMode != lcdMode){
    prev_lcdMode = lcdMode;
    byte grau[8] ={ B00001100, B00010010, B00010010, B00001100, B00000000, B00000000, B00000000, B00000000,};
    lcd.createChar(0, grau);
    byte char1[8] ={ 0, 4, 12, 4, 4, 14, 0, 0};
    lcd.createChar(1, char1);
    byte char2[8] ={31,27,19,27,27,17,31,0};
    lcd.createChar(2, char2);
    byte char3[8] ={0,4,10,2,4,14,0,0};
    lcd.createChar(3, char3);
    byte char4[8] ={31,27,21,29,27,17,31,0};
    lcd.createChar(4, char4);
  }   
  char myChar = 0;
  // linha 0
  String sa;
  String stemp = String(Temperatura,0);
  String shum = String(Umidade,0);
  sa = hhmmss(); 
  sa += F(" ");
  if(stemp.length()<2) sa += F(" ");
   sa += stemp;
   sa += myChar;
  sa += F(" ");
  if(Rele[0]) sa += char(2); else sa += char(1);
  if(Rele[1]) sa += char(4); else sa += char(3);
//  sa += (despertaLigado)? F("A") : F("a");
  sa+=" ";
  lcd.setCursor(0, 0); lcd.print(sa);
  // linha 1
  sa = ddmmaa();
  sa += F(" ");
  if(shum.length()<2) sa += F(" ");
  sa += shum;
  sa += F("% ");
  if(WiFi.status() == WL_CONNECTED) sa+=F("W"); else sa+=F("w");
  if(Blynk.connected()) sa+=F("B"); else sa+=F("b");
  sa+=F(" ");
  lcd.setCursor(0, 1); lcd.print(sa);
  //
}

void MostrarRelogio2(){
  if(prev_lcdMode != lcdMode){
    prev_lcdMode = lcdMode;
    byte LT[8] = { 3, 7, 7, 7, 7, 7, 7, 3};
    byte UB[8] = {31,31, 0, 0, 0, 0, 0, 0};
    byte RT[8] = {24,28,28,28,28,28,28,24};
    byte LL[8] = { 0, 0, 0, 0, 0, 0,31,31};
    byte LB[8] = {31,31, 0, 0, 0, 0,31,31};
    lcd.createChar(0,LT);
    lcd.createChar(1,UB);
    lcd.createChar(2,RT);
    lcd.createChar(3,LL);
    lcd.createChar(4,LB);
    byte grau[8] ={ B00001100, B00010010, B00010010, B00001100, B00000000, B00000000, B00000000, B00000000};
    lcd.createChar(5, grau);
    byte myChar6[8] = {0,0,0,0,0,0,24,24}; lcd.createChar(6, myChar6);
    byte myChar7[8] = {0,0,0,0,0,0, 3, 3}; lcd.createChar(7, myChar7);
  }
  time_t t = now(); timeZoneChk(); t += timeZone * SECS_PER_HOUR;
  int minutos = minute(t);
  int horas = hour(t);
  linha[0] = ""; linha[1] = "";
  if(horas < 10) converter2(0); else converter2(horas/10);
  converter2(horas%10);
  if(second(t)%2==0){
    linha[0] += char(B10100101); linha[1] +=  char(B10100101);
  }else{
    linha[0] += F(" "); linha[1] += F(" ");;
  }
  if(minutos < 10) converter2(0); else converter2(minutos/10);
  converter2(minutos%10);
  if(Temperatura < 10) linha[0] += F(" ");
  int i;
  i = Temperatura;
  linha[0] += String(i); linha[0] += char(5);
  if(Umidade < 10) linha[1] += F(" ");
  i = Umidade;
  linha[1] += String(i); linha[1] += "%";
  lcd.setCursor(0,0); lcd.print(linha[0]);
  lcd.setCursor(0,1); lcd.print(linha[1]);
}

void converter2(int num){
  if(num == 0){
    linha[0] += char(0); linha[0] += char(1); linha[0] += char(2);
    linha[1] += char(0); linha[1] += char(3); linha[1] += char(2);
  }
  if(num == 1){
    linha[0] += char(32); linha[0] += char(0); linha[0] += char(32);
    linha[1] += char(32); linha[1] += char(0); linha[1] += char(32);
  }
  if(num == 2){
    linha[0] += char(32); linha[0] += char(4); linha[0] += char(2);
    linha[1] += char(0); linha[1] += char(3); linha[1] += char(6);
  }
  if(num == 3){
    linha[0] += char(32); linha[0] += char(4); linha[0] += char(2);
    linha[1] += char(32); linha[1] += char(3); linha[1] += char(2);
  }
  if(num == 4){
    linha[0] += char(0); linha[0] += char(3); linha[0] += char(2);
    linha[1] += char(32); linha[1] += char(32); linha[1] += char(2);
  }
  if(num == 5){
    linha[0] += char(0); linha[0] += char(4); linha[0] += char(32);
    linha[1] += char(7); linha[1] += char(3); linha[1] += char(2);
  }
  if(num == 6){
    linha[0] += char(0); linha[0] += char(4); linha[0] += char(32);
    linha[1] += char(0); linha[1] += char(3); linha[1] += char(2);
  }
  if(num == 7){
    linha[0] += char(0); linha[0] += char(1); linha[0] += char(2);
    linha[1] += char(32); linha[1] += char(32); linha[1] += char(2);
  }
  if(num == 8){
    linha[0] += char(0); linha[0] += char(4); linha[0] += char(2);
    linha[1] += char(0); linha[1] += char(3); linha[1] += char(2);
  }
  if(num == 9){
    linha[0] += char(0); linha[0] += char(4); linha[0] += char(2);
    linha[1] += char(7); linha[1] += char(3); linha[1] += char(2);
  }
}

void MostrarRelogio3(){
  if(prev_lcdMode != lcdMode){
    prev_lcdMode = lcdMode;
    byte LT[8] = { B01111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111};
    byte UB[8] = { B11111,  B11111,  B11111,  B00000,  B00000,  B00000,  B00000,  B00000};
    byte RT[8] = { B11110,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111};
    byte LL[8] = { B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B01111};
    byte LB[8] = { B00000,  B00000,  B00000,  B00000,  B00000,  B11111,  B11111,  B11111};
    byte LR[8] = { B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11110};
    byte UMB[8] ={ B11111,  B11111,  B11111,  B00000,  B00000,  B00000,  B11111,  B11111};
    byte LMB[8] ={ B11111,  B00000,  B00000,  B00000,  B00000,  B11111,  B11111,  B11111 };
    lcd.createChar(8,LT);
    lcd.createChar(9,UB);
    lcd.createChar(10,RT);
    lcd.createChar(11,LL);
    lcd.createChar(12,LB);
    lcd.createChar(5,LR);
    lcd.createChar(6,UMB);
    lcd.createChar(7,LMB);
  }
  time_t t = now(); timeZoneChk(); t += timeZone * SECS_PER_HOUR;
  int minutos = minute(t);
  int horas = hour(t);
  linha[0] = ""; linha[1] = "";
  if(horas < 10) converter3(0); else converter3(horas/10);
  linha[0] += " "; linha[1] += " ";
  converter3(horas%10);
  linha[0] += " "; linha[1] += " ";
  if(second(t)%2==0){
    linha[0] += char(B10100101); linha[1] +=  char(B10100101);
  }else{
    linha[0] += F(" "); linha[1] += F(" ");
  }
  //linha[0] += char(B10100101); linha[1] +=  char(B10100101);
  if(minutos < 10) converter3(0); else converter3(minutos/10);
  linha[0] += " "; linha[1] += " ";
  converter3(minutos%10);
  lcd.setCursor(0,0); lcd.print(linha[0]);
  lcd.setCursor(0,1); lcd.print(linha[1]);
}

void converter3(int num){
  if(num == 0){
    linha[0] += char(8); linha[0] += char(9); linha[0] += char(10);
    linha[1] += char(11); linha[1] += char(12); linha[1] += char(5);
  }
  if(num == 1){
    linha[0] += char(9); linha[0] += char(10); linha[0] += char(32);
    linha[1] += char(32); linha[1] += char(5); linha[1] += char(32);
  }
  if(num == 2){
    linha[0] += char(6); linha[0] += char(6); linha[0] += char(10);
    linha[1] += char(11); linha[1] += char(7); linha[1] += char(7);
  }
  if(num == 3){
    linha[0] += char(6); linha[0] += char(6); linha[0] += char(10);
    linha[1] += char(7); linha[1] += char(7); linha[1] += char(5);
  }
  if(num == 4){
    linha[0] += char(11); linha[0] += char(12); linha[0] += char(10);
    linha[1] += char(32); linha[1] += char(32); linha[1] += char(5);
  }
  if(num == 5){
    linha[0] += char(8); linha[0] += char(6); linha[0] += char(6);
    linha[1] += char(7); linha[1] += char(7); linha[1] += char(5);
  }
  if(num == 6){
    linha[0] += char(8); linha[0] += char(6); linha[0] += char(6);
    linha[1] += char(11); linha[1] += char(7); linha[1] += char(5);
  }
  if(num == 7){
    linha[0] += char(9); linha[0] += char(9); linha[0] += char(10);
    linha[1] += char(32); linha[1] += char(8); linha[1] += char(32);
  }
  if(num == 8){
    linha[0] += char(8); linha[0] += char(6); linha[0] += char(10);
    linha[1] += char(11); linha[1] += char(7); linha[1] += char(5);
  }
  if(num == 9){
    linha[0] += char(8); linha[0] += char(6); linha[0] += char(10);
    linha[1] += char(32); linha[1] += char(32); linha[1] += char(5);
  }
}

void lcdwifistatus(){
  //return;
  if(WiFi.status()==WL_CONNECTED){
    IPAddress ip=WiFi.localIP();
    String Linha0=String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
    lcd.setCursor(0, 0); lcd.print(centralizar(centralizar(F("WiFi Conectado"))));
    lcd.setCursor(0, 1); lcd.print(centralizar(Linha0));
    lcdCnt = 5;
  }else{
    String sa;
    sa+=DeviceID;
    if(DeviceID == "CONFIg"){ sa+=F(" "); sa+=apPass; }
    lcd.setCursor(0, 0); lcd.print(centralizar(sa));
    IPAddress ip=WiFi.softAPIP();
    String Linha0=String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
    lcd.setCursor(0, 1); lcd.print(centralizar(Linha0));
    lcdCnt = 60;
  }
}

#endif
