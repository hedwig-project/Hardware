void DHTLloop(){
  if((millis()-prevMillisDHT)>10000){
    prevMillisDHT=millis();
    readTemp();
    readHumi();
  }
  if((millis()-prevMillisLumi)>50){
    prevMillisLumi=millis();
    ReadLumi();
  }
}

//float UmiOffset, TempOffset;

void readTemp(){
  float t2 = dht.readTemperature()+TempOffset;
  if (!isnan(t2)) {
    if (Temperatura > 0) {
      Temperatura = ((Temperatura * 2.0) + t2) / 3.0;
    } else {
      Temperatura = t2;
    }
  }
}

void readHumi(){
  float h2 = dht.readHumidity() + UmiOffset;
  if (!isnan(h2)){
    if (Umidade > 0) {
      Umidade = ((Umidade * 2.0) + h2) / 3.0;
    } else {
      Umidade = h2;
    }
  }
}

void ReadLumi(){ // a cada 50ms
  int ia = analogRead(LDRPin);
  if(ia>50 and ia<950){
    Lumi = ia;
    #ifdef LCD_SCL
    if(backlight==2){
      if(analogRead(LDRPin)<500) lcd.setBacklight(false);
      if(analogRead(LDRPin)>550) lcd.setBacklight(true);
    }
    #endif
  }
  // Botão 1
  #ifdef Btn1Pin
    if(Btn1_wait>0){
      Btn1_wait--;
    }else{
      bool ba=(analogRead(Btn1Pin)<25);
      if(Btn1!=ba){
        Btn1=ba;
        Btn1_wait=1;
      }
    }
  #endif
  // Botão Reset/Hard Reset
  if(ia>950){
    if(ResetCnt==0) ResetCnt=1;
  }else{
    if(ResetCnt>5){ // Botão Hard Reset
      HardReset(); // na aba too (tools)
    }
    if(ResetCnt>1) { // Botão Reset
      // Reset
      #ifdef LCD_SCL
      lcd.setCursor(0, 0); lcd.print(centralizar(F("Reset")));
      lcd.setCursor(0, 1); lcd.print(centralizar(F("Reiniciando")));
      #endif
      delay(2000);
      ESP.reset();
    }
    ResetCnt=0;
  }
  //
}

void handleDHT(){
  String pg;
  String sa,fields;
  sa="fields"; if(server.hasArg(sa)){
    fields=","+server.arg(sa)+",";
    pg=F("{");
    if(fields.indexOf(F(",Temperatura,"))>=0){ pg += F("\"Temperatura\":"); pg += String(Temperatura,1); pg += F(","); }
    if(fields.indexOf(F(",Umidade,"))>=0){ pg += F("\"Umidade\":"); pg += String(Umidade,0); pg += F(","); }
    if(pg.substring(pg.length()-1)==",") pg = pg.substring(0,pg.length()-1); pg += F("}");
    server.send(200, "text/plain", pg);
    return; 
  }
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  
  sa="UmiOffset"; if(server.hasArg(sa)){
    sa=server.arg(sa);
    sa.replace("%2C",".");
    UmiOffset=sa.toFloat();
    sa="TempOffset"; if(server.hasArg(sa)){
      sa=server.arg(sa);
      sa.replace("%2C",".");
      TempOffset=sa.toFloat();
      pg = F("<!doctype html><html><body onload='history.go(-1)'></html>");
      server.send(200, "text/html", pg);
      gravarEeprom();
      Umidade=0.0; Temperatura=0.0;
      prevMillisDHT=0;
      return;
    }
  }
  
  pg=F(
"<html>\r\n"
"<head>\r\n"
"<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
"<title>");pg+=DeviceID;pg+=F("</title>\r\n"
"<style type='text/css'>\r\n"
"  body{margin:0}\r\n"
"  html,input,p,button{font:18px Verdana,Arial,Helvetica,sans-serif}\r\n"
"  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
"  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\r\n"
"  input{text-align:center; border-radius:6px}"
"</style>\r\n"
"</head>\r\n"
"<body onload='iniciar()'>\r\n"
"<script>"

  "var client = new XMLHttpRequest();\n\r\n"
  "client.onreadystatechange = function() {\r\n"
  "  if (this.readyState==4){\r\n"
  "    if(this.status==200){\r\n"
  "      myFunction(JSON.parse(this.responseText));\r\n"
  "    }\r\n"
  "    wait=0;\r\n"
  "  }\r\n"
  "}\r\n"
  "function myFunction(myObj) {\r\n"
  "  if(typeof myObj.Temperatura!='undefined') Temperatura.innerHTML=myObj.Temperatura;"
  "  if(typeof myObj.Umidade!='undefined') Umidade.innerHTML=myObj.Umidade;"
  "}"
  "function loadjson(){"
  "    client.open('GET', '?fields=Temperatura,Umidade', true); client.timeout = 950; client.send();\r\n"
  "}"

"  function iniciar(){"
"    setInterval(loadjson, 1000);\r\n"
"  }"
"</script>\r\n"
"<div style='width:100%' align='center'>"

"<table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='width:100%;max-width:360px;"
"background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";'>\n"
"    <tr height='32' style='font-size:20px'>\r\n"
"      <td style='width:10%;text-align:left'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\r\n"
"      <td style='width:80%;text-align:center'><b id='tdtitle'>DHT</b></td>\r\n"
"      <td style='width:10%' align='right'><input type=button class=btn0 onclick='form1.submit();' value='OK'></td>\r\n"
"    </tr>\r\n"
"  </table>\r\n"

"  <table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'>\n"
"   <tr><td style='text-align:center;padding:8px;'>\n"

"<div class='divbl' style='font-size:28px;'>\n"
"<span id=Temperatura></span> &deg;C <span id=Umidade></span>%"
"</form>\n"
"</div>\n"
"<div class='divbl'>\n"
"<form id='form1' method='get'>\n"
"Temperatura (offset)<br><input type=text id=TempOffset name=TempOffset value='");pg+=String(TempOffset,1);pg+=F("'><br>\n"
"Umidade (offset)<br><input type=text id=UmiOffset name=UmiOffset value='");pg+=String(UmiOffset,1);pg+=F("'>\n"
"</form>\n"
"</div>\n"
"</td></tr></table>"

"</div>"
"</body></html>\r\n"
  );
  server.send(200, "text/html", pg); server.handleClient();
}

