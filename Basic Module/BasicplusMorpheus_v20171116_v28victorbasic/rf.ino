void rf433rx(){
  unsigned long Buffer = ht6p20bRX();
  if((millis() - prevMillisRx) >= 1500){ prevBuffer = 0; }
  if(Buffer < 1) return;
  if(prevBuffer == Buffer) return;
  #ifdef debugRF
  Serial.print(F("RF Buffer: "));Serial.println(Buffer,HEX);
  #endif
  prevBuffer = Buffer;
  prevMillisRx = millis();
  /*/
  #ifdef debug
  debug.print(Buffer, BIN); debug.print(" ");
  debug.print(Buffer >> 2, HEX); debug.print(" ");
  debug.print(bitRead(Buffer,1), BIN); debug.print(" ");
  debug.print(bitRead(Buffer,0), BIN); debug.print(" ");
  debug.print(Buffer); debug.print(" ");
  debug.println();
  #endif
  /*/
  lastRFcode = String(Buffer);
  lastRFcodeH=String(Buffer,HEX); lastRFcodeH.toUpperCase();
  //msg=F("RF "); msg+=lastRFcodeH; addLog(msg);
  if(RFcode[0].indexOf(lastRFcode)>=0) {
    Rele[0]=!Rele[0];
    if(log_opc.indexOf(",Reles,")>=0) addLog(F("RF1 acionado"));
    ReleAutoWait[0]=1200;
  }
  if(RFcode[1].indexOf(lastRFcode)>=0) {
    Rele[1]=!Rele[1];
    if(log_opc.indexOf(",Reles,")>=0) addLog(F("RF2 acionado"));
    ReleAutoWait[1]=1200;
  }
  // ----- Sensor 2
  if(RFcode[2].indexOf(lastRFcode)>=0) {
    Sensor[1]=1;
  }
  if(RFcode[3].indexOf(lastRFcode)>=0) {
    Sensor[1]=0;
  }
}

void addRFcode(){ // 100ms
  if(addCode < 0) return;
  if(addCodeCnt < 1) addCode = -2; // timeout
  addCodeCnt--;
  if(lastRFcode.length() > 0){
    //if(addCode == 2) RFcode[2] = "";
    //if(addCode == 3) RFcode[3] = "";
    if(RFcode[addCode].indexOf(lastRFcode)<0){
      if(RFcode[addCode].length()>0) RFcode[addCode] += "|";
      RFcode[addCode] += lastRFcode;
      addCode = -3; // gravado
    }
  }
}

void handleRF(){
  //
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  //
  String sa;
  sa = server.arg(F("salvar")); 
  if (sa.length() > 0){
    gravarEeprom();
    pg = F("<!doctype html><html><body onload='history.go(-4)'></body></html>");
    server.send(200, "text/html", pg);
    return;
  }
  pg=F(
  "<!doctype html><html>"
  "<head>\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style type='text/css'>\n"
  "  body{margin:0}\n"
  "  html,input,select,p,td{font:18px Verdana,Arial,Helvetica,sans-serif}\n"
  "  input,select{border-radius:6px;text-align:center}\n"
  " .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
  "  p{margin:8px 0 8px 0;line-height: 150%;text-align:center}\n"
  "</style>"
  "</head>\n"
  "<body onload='iniciar()'>\n"
  
  "<script>"
  "var client = new XMLHttpRequest();\n\r\n"
  "client.onreadystatechange = function(){ if (this.readyState==4 && this.status==200){ myFunction(JSON.parse(this.responseText)); } }\r\n"
  "function myFunction(myObj) {\r\n"
  "  addCode=myObj.addCode;\r\n"
  "  addCodeCnt=myObj.addCodeCnt;\r\n");
  RFcodeTitle[0]=BtnLabel[0];
  RFcodeTitle[1]=BtnLabel[1];
  for(byte i=0;i<10;i++){ if(RFcodeTitle[i].length()<1) break;
    pg+=F("  if(addCode=="); pg+=String(i); pg+=F("){"
          "    RFcode"); pg+=String(i); pg+=F(".value='Aguardando ('+addCodeCnt+')';");
    pg+=F("  }else{\r\n");
    pg+=F("    if(typeof myObj.RFcode"); pg+=String(i); pg+=F("!='undefined'){RFcode"); pg+=String(i); pg+=F(".value=myObj.RFcode"); pg+=String(i);pg+=F(";\r\n");
    pg+=F("  }\r\n");
    pg+=F("}\r\n");
  }
  pg+=F(
  "}\r\n"
  "function timer2(){client.open('GET', 'json?fields=rfcodes', true);client.send();}"
  "function iniciar(){"
  " client.open('GET', 'json?fields=rfcodes', true);client.send();\r\n"
  " setInterval(timer2, 1000);\r\n"
  "}\r\n"
  "function clrCode(xx) { client.abort(); client.open('GET', 'json?fields=rfcodes&clrCode='+xx, true);client.send(); }\r\n"
  "function addcode(xx) { client.abort(); client.open('GET', 'json?fields=rfcodes&addCode='+xx, true);client.send(); }\r\n"
  "</script>\r\n"
  "<div align='center'>\n"
  "<table border='0' cellspacing='6' cellpadding='0' style='background-color:");  pg += backgroundColor;  pg += F(";color: #FFF;width:100%;'>"
  "  <tr height='32' style='font-size:20px'>\n"
  "    <td style='width:15%'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\n"
  "    <td style='width:70%;text-align:center'><b>Configura&ccedil;&otilde;es RF</b></td>\r\n"
  "    <td style='width:15%' align='right'><input type=button class=btn0 value='OK' onclick='location.href=\"?salvar=1\"'></td>\r\n"
  "  </tr>"
  "</table>\n"
  "<table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'>\n"
  "  <tr><td style='text-align:center;padding:8px;'>\n"
  "<div class='divbl'>"
  "<p><b>Reles</b></p>"
  "<script>"
  "  rf=new Array();"
  );
  for(byte i=0;i<10;i++){
    if(RFcodeTitle[i].length()<1) break;
    pg+=F("rf[");pg+=String(i);pg+=F("]='");pg+=RFcodeTitle[i];pg+=F("';");
  }
  pg+=F(
  " for(i=0;i<rf.length;i++){"
  "  document.writeln('<p><b>#'+i+' '+rf[i]+'</b><br>');\r\n"
  "  document.writeln('<input type=button value=C style=width:12%;height:34px onclick=clrCode('+i+')>');\r\n"
  "  document.writeln('<input type=text id=RFcode'+i+' style=width:70%;height:30px readonly>');\r\n"
  "  document.writeln('<input type=button value=+ style=width:12%;height:34px onclick=addcode('+i+')>');\r\n"
  "  document.writeln('</p>');\r\n"
  "  if(i==1){document.writeln('</div><div class=divbl>')}"
  " }\r\n"
  "</script>\r\n"
  "     </div>\r\n"
  //"<p><input type='button' value='Salvar' style='width:46%;height:34px' onclick='location.href=\"?salvar=1\"'>"
  "</td></tr></table></div>"
  "</body></html>\n");
  server.send(200, "text/html", pg);
}

unsigned long ht6p20bRX(){
  if(!digitalRead(pinRF_RX)){
    int dur, dur0, dur1;
    int lambda;      // on pulse clock width (if fosc = 2KHz than lambda = 500 us)
    unsigned long Buffer = 0;  //buffer for received data storage
    // Check how long DOUT was "0" (ZERO) (refers to PILOT CODE)
    dur0 = micros(); 
    while(!digitalRead(pinRF_RX)){
      dur = micros() - dur0; if(dur > 13800) return 0;
      }
    dur0 = micros() - dur0; if(dur0 < 9200) return 0;
    //dur0 = micros() - dur0; if(dur0 < 4600) return 0;
    lambda = dur0 / 23;
    float mn = 0.30; // 0.8
    float mx = 2.70; // 2.2
    //
      dur1 = micros();
      while(digitalRead(pinRF_RX)){
        dur = micros() - dur1; if(dur > (mx * lambda)) return 0;
        }
      dur1 = micros() - dur1; if(dur1 < (mn * lambda)) return 0;
    //
    for(int i = 0;i<28;i++){
      //
      dur0 = micros(); 
      while(!digitalRead(pinRF_RX)){
        dur = micros() - dur0; if(dur > (mx * lambda)) return 0;
        }
      dur0 = micros() - dur0; if(dur0 < (mn * lambda)) return 0;
      //
      dur1 = micros(); 
      while(digitalRead(pinRF_RX)){
        dur = micros() - dur1; if(dur > (mx * lambda)) return 0;
        }
      dur1 = micros() - dur1; if(dur1 < (mn * lambda)) return 0;
      //
      if(dur0 > dur1) {
        Buffer = (Buffer << 1) + 1;   // add "1" on data buffer
      }else{
        Buffer = (Buffer << 1);       // add "0" on data buffer
      }
    }
    #ifdef debug
    debug.println(lambda);
    #endif
    return Buffer >> 4;
  }
  return 0;
}

//Os 12 pulsos de preambulo usam a largura de pulso de 780 us:
//– The PILOT is formed by 23 cycles of “0” and one cycle of “1”;
//– The ADDRESS consists of 22 bits (66 cycles);
//– The DATA is formed by 2 bits (6 cycles).
//– The ANTI-CODE consists of 4 bits (12 cycles). It is always “0101”.

#ifdef pinRF_TX
void ht6p20bTX(unsigned int code){
  int lambda = 450; //int lambda = 478;
  for(int j=0;j<20;j++){
    /*/ Preambulo
    for(int i = 0; i < 12; i++){
      digitalWrite(pinRF_TX, HIGH);
      delayMicroseconds(lambda * 1.5);
      digitalWrite(pinRF_TX, LOW);
      delayMicroseconds(lambda * 1.5);
    }/*/
    // Pilot
    digitalWrite(pinRF_TX, LOW); //Set pinRF on LOW for 23 Lambdas
    delayMicroseconds(23 * lambda);
    digitalWrite(pinRF_TX, HIGH); //Set pinRF on HIGH for one Lambda
    delayMicroseconds(lambda);
    // 
    String addressCodeBIN;
    addressCodeBIN = F("0000000000000000000000");
    addressCodeBIN += String(code, BIN);
    addressCodeBIN = addressCodeBIN.substring(addressCodeBIN.length()-24, addressCodeBIN.length());
    for(int i=0; i<24; i++){ //Send all bits for Address Code
      char data = addressCodeBIN[i];
      sendData(data);
    }
    // sendAntiCode
    sendData('0');
    sendData('1');
    sendData('0');
    sendData('1');
    digitalWrite(pinRF_TX, LOW);
  }
}

void sendData(char data){
  int lambda = 450; //int lambda = 478;
  if(data == '0')  {
    digitalWrite(pinRF_TX, LOW);
    delayMicroseconds(lambda);
    digitalWrite(pinRF_TX, HIGH);
    delayMicroseconds(2 * lambda);
  }
  if(data == '1')  {
    digitalWrite(pinRF_TX, LOW);
    delayMicroseconds(2 * lambda);  
    digitalWrite(pinRF_TX, HIGH);
    delayMicroseconds(lambda);
  }
}
#endif
