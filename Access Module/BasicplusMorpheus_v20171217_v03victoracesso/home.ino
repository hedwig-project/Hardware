void handleHome(){
  String user = autenticar(); if(user.length()<=0) return server.requestAuthentication();
  String pg;
  String sa,sb,sc,sd,fields;
  pg=F("{");
  sa=F("time"); if(server.hasArg(sa)) wsTimeSet(server.arg(sa));          // time
  // ----- Reles L/D
  for(byte i=1;i<=Reles;i++){
    sa=F("Lamp"); sa+=String(i);
    if(server.hasArg(sa)) {
      Rele[i-1]=!Rele[i-1];
      ReleAutoWait[i-1]=1200;
      msg = F("Web"); msg+=i; msg+=F(" ("); msg+=User; msg+=F(") acionado"); addLog(msg);
    }
  }
  sa=F("fields"); if(server.hasArg(sa)){ fields=","+server.arg(sa)+",";}
  sa=F(",all,");  if(fields==sa){ fields=F(
  ",DeviceID,BackgroundColor,Color1,Color2,hhmmss,ddmmaaaa,Temperatura,Umidade,Lumi,Sensores"
  ",Lamps,ReleTempoDesl0,ReleTempoDesl1,ReleCnt0,ReleCnt1,ReleSenha0,ReleSenha1,BtnLabel0,BtnLabel1"
  ",BlynkConnErrCnt,WiFi,Board,Versao,millis,timeStatus,");}
  if(fields.indexOf(F(",DeviceID,"))>=0){pg+=F("\"DeviceID\":\"");pg+=DeviceID;pg+=F("\",");}
  if(fields.indexOf(F(",BackgroundColor,"))>=0){ pg += F("\"BackgroundColor\":\""); pg+=backgroundColor; pg += F("\","); }
  if(fields.indexOf(F(",Color1,"))>=0){pg += F("\"Color1\":\""); pg+=Color1; pg += F("\",");}
  if(fields.indexOf(F(",Color2,"))>=0){pg += F("\"Color2\":\""); pg+=Color2; pg += F("\",");}
  if(fields.indexOf(F(",hhmmss,"))>=0){ pg += F("\"hhmmss\":\""); pg += hhmmss(); pg += F("\","); }
  if(fields.indexOf(F(",ddmmaaaa,"))>=0){ pg += F("\"ddmmaaaa\":\""); pg += ddmmaaaa(); pg += F("\","); }
  if(fields.indexOf(F(",Temperatura,"))>=0 && Temperatura>0){
    pg += F("\"Temperatura\":\""); pg += String(Temperatura,0.0); pg += F("\",");
  }
  if(fields.indexOf(F(",Umidade,"))>=0 && Umidade>0){
    pg += F("\"Umidade\":\""); pg += String(Umidade,0); pg += F("\",");
  }
  if(fields.indexOf(F(",Lumi,"))>=0 && Lumi>0){
    pg += F("\"Lumi\":\""); pg += String(Lumi); pg += F("\",");
  }
  if(fields.indexOf(F(",Sensores,"))>=0){ for(byte i=1;i<=Sensores;i++){ pg+=F("\"Sensor");pg+=String(i);pg+=F("\":"); pg += String(Sensor[i-1]); pg += F(","); } }
  if(fields.indexOf(F(",Lamps,"))>=0){ for(byte i=1;i<=Reles;i++){ pg += F("\"Lamp");pg+=String(i);pg+=F("\":"); pg += String(Rele[i-1]); pg += F(","); } }
  if(fields.indexOf(F(",ReleTempoDesl0,"))>=0){pg+=F("\"ReleTempoDesl0\":");pg+=ReleTempoDesl[0];pg+=F(",");}
  if(fields.indexOf(F(",ReleTempoDesl1,"))>=0){pg+=F("\"ReleTempoDesl1\":");pg+=ReleTempoDesl[1];pg+=F(",");}
  if(fields.indexOf(F(",ReleCnt0,"))>=0){pg+=F("\"ReleCnt0\":\"");pg+=secToDMHS(ReleCnt[0]/10);pg+=F("\",");}
  if(fields.indexOf(F(",ReleCnt1,"))>=0){pg+=F("\"ReleCnt1\":\"");pg+=secToDMHS(ReleCnt[1]/10);pg+=F("\",");}
  if(fields.indexOf(F(",ReleSenha0,"))>=0){pg+=F("\"ReleSenha0\":");pg+=(ReleSenha[0].length()>0);pg+=F(",");}
  if(fields.indexOf(F(",ReleSenha1,"))>=0){pg+=F("\"ReleSenha1\":");pg+=(ReleSenha[1].length()>0);pg+=F(",");}
  if(fields.indexOf(F(",BtnLabel0,"))>=0){pg+=F("\"BtnLabel0\":\"");pg+=BtnLabel[0];pg+=F("\",");}
  if(fields.indexOf(F(",BtnLabel1,"))>=0){pg+=F("\"BtnLabel1\":\"");pg+=BtnLabel[1];pg+=F("\",");}
  if(fields.indexOf(F(",BlynkConnErrCnt,"))>=0){ if(blynkAuth.length()>0) { pg += F("\"BlynkConnErrCnt\":"); pg +=String(BlynkConnErrCnt); pg += F(","); } }
  if(fields.indexOf(F(",WiFi,"))>=0){ if(WiFi.status() == WL_CONNECTED){pg += F("\"WiFi\":"); pg += String(WiFi.RSSI()); pg += F(",");} }
  if(fields.indexOf(F(",Board,"))>=0){ pg += F("\"Board\":\""); pg += Board; pg += F("\","); }
  if(fields.indexOf(F(",Versao,"))>=0){ pg += F("\"Versao\":\""); pg += Versao; pg += F("\","); }
  if(fields.indexOf(F(",millis,"))>=0){ pg+=F("\"millis\":\""); pg+=secToDMHS(millis()/1000); pg+=F("\","); }  
  if(fields.indexOf(F(",timeStatus,"))>=0){ if(timeStatus() == timeNotSet){pg+=F("\"timeStatus\":");pg+=String(timeStatus());pg+=F(",");  }} 
  if(AutoResetTestCnt>0) { pg+= F("\"AutoResetTestCnt\":"); pg+=String(AutoResetTestCnt-1); pg+=F(","); }
  if(pg.substring(pg.length()-1)==","){ pg = pg.substring(0,pg.length()-1); pg += F("}");
    server.send(200, "text/plain", pg);
    return;
  }
  
  pg=F(
  "<!DOCTYPE html>\r\n"
  "<html>\r\n"
  "<head>\r\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style type='text/css'>\r\n"
  "  body{margin:0}\r\n"
  "  html,input,p,button{font:18px Verdana,Arial,Helvetica,sans-serif}\r\n"
  "  h2{margin-top:30px;text-align:center}\r\n"
  "  p{text-align:center;margin:8px}\r\n"
  "  input,button{text-align:center;}\r\n"
  "  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .btn1{border-radius:8px;height:40px;width:100%}\r\n"
  "  .btn2{border-radius:16px;vertical-align:bottom;font-size:18px;margin:4px");
  if(Reles<5) pg+=F(";height:140px;width:47%"); else pg+=F(";height:60px;width:47%"); pg+=F("}\r\n"
  "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\r\n"
  "  .led {background-color: #77F; height: 20px; width: 20px; border-radius: 10px; font-size: 8px; color: #FFF; vertical-align: text-bottom}\r\n"
  "  progress{text-align:center;height:18px;vertical-align:middle;font-size:14px}\r\n"
  "  progress:after{content:attr(prefix)attr(value);}\r\n"
  "</style>\r\n"
  "</head>\r\n"
  
  "<script>"
  "var cmd='';"
  "var color='#FFF';"
  "var BtnLabel=['',''];"
  "var ReleCnt=['',''];"
  "var ReleTempoDesl=[0,0];"
  "var ReleSenha=[0,0];"

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
  "  if(typeof myObj.timeStatus!='undefined'){"
  "    if(myObj.timeStatus==0){"
  "      d=new Date();client.abort(); client.open('GET', '?fields=all&time='+d.getTime() , true); client.send();"
  "    }"
  "  }"
  "  document.title=myObj.DeviceID;\r\n"
  "  tdtitle.innerHTML=myObj.DeviceID;\r\n"
  "  if(typeof myObj.WiFi!='undefined'){wifipb.value=myObj.WiFi+100}\r\n"
  "  if(typeof myObj.cmd!='undefined'){ sa=myObj.cmd;\r\n"
  "    if(sa=='reload'){location.reload();}\r\n"
  "  }\r\n"
  "  hhmmss.innerHTML=myObj.hhmmss;\r\n"
  "  ddmmaaaa.innerHTML=myObj.ddmmaaaa;\r\n");
  #ifdef DHTPin
  pg+=F(
  "  if(typeof myObj.Temperatura!='undefined'){Temperatura.innerHTML='<br>'+myObj.Temperatura+'&deg;C'}\r\n"
  "  if(typeof myObj.Umidade!='undefined'){Umidade.innerHTML=myObj.Umidade+'%'}\r\n");
  #endif
  #ifdef LDRPin
  pg+=F(
  "  if(typeof myObj.Lumi!='undefined'){Lumi.value = myObj.Lumi;}\r\n");
  #endif
  pg+=F(
  
  "  BackgroundColor=myObj.BackgroundColor;"
  "  Color1=myObj.Color1;"
  "  Color2=myObj.Color2;"
  
  //" msgBtn.style.backgroundColor=BackgroundColor;\r\n"
  //" msgBtn.style.color=Color1;\r\n"
  
  "  if(typeof myObj.BtnLabel0!='undefined'){BtnLabel[0]=myObj.BtnLabel0;}"
  "  if(typeof myObj.BtnLabel1!='undefined'){BtnLabel[1]=myObj.BtnLabel1;}"
  "  if(typeof myObj.ReleCnt0!='undefined'){ReleCnt[0]=myObj.ReleCnt0;}"
  "  if(typeof myObj.ReleCnt1!='undefined'){ReleCnt[1]=myObj.ReleCnt1;}"
  "  if(typeof myObj.ReleTempoDesl0!='undefined'){ReleTempoDesl[0]=myObj.ReleTempoDesl0;}"
  "  if(typeof myObj.ReleTempoDesl1!='undefined'){ReleTempoDesl[1]=myObj.ReleTempoDesl1;}"
  "  if(typeof myObj.ReleSenha0!='undefined'){ReleSenha[0]=myObj.ReleSenha0;}"
  "  if(typeof myObj.ReleSenha1!='undefined'){ReleSenha[1]=myObj.ReleSenha1;}"
  );
  for(byte i=1;i<=Sensores;i++){
    pg+=F("if(myObj.Sensor");pg+=String(i);pg+=F(">0){sa='#F00'}else{sa='#00F'}Sensor");
    pg+=String(i);pg+=F(".style.backgroundColor=sa;\r\n");
  }

  pg+=F(
  "if(myObj.Lamp1>0){sa=Color2;}else{sa=Color1}\r\n"
  "sa='<b><font color='+sa+'>';\r\n"
  "if(BtnLabel[0]!=''){ sa+='<font style=font-size:16px>'+BtnLabel[0]+'</font><br>'; }\r\n"
  "sa+='<font size=56px>1</font>';\r\n"
  "if(ReleCnt[0]!='0:00'){sa+='<br><font style=font-size:14px>'+ReleCnt[0]+'</font></b>';}\r\n"
  "Lamp1.innerHTML=sa;\r\n"
  "Lamp1.style.backgroundColor=BackgroundColor;\r\n"
  "Lamp1.style.color=Color1;\r\n"
  
  "if(myObj.Lamp2>0){sa=Color2;}else{sa=Color1}\r\n"
  "sa='<b><font color='+sa+'>';\r\n"
  "if(BtnLabel[1]!=''){ sa+='<font style=font-size:16px>'+BtnLabel[1]+'</font><br>'; }\r\n"
  "sa+='<font size=56px>2</font>';\r\n"
  "if(ReleCnt[1]!='0:00'){sa+='<font style=font-size:14px><br>'+ReleCnt[1]+'</font></b>';}\r\n"
  "Lamp2.innerHTML=sa;\r\n"
  "Lamp2.style.backgroundColor=BackgroundColor;\r\n"
  "Lamp2.style.color=Color1;\r\n"
  "  tableHeader.style.backgroundColor=BackgroundColor;\r\n"
  "  tableHeader.style.color=Color1;\r\n"
  
  "  sa='';"
  "  if(typeof myObj.AutoResetTestCnt!='undefined'){\r\n"
  "    sa+='Teste Auto-Reset '+myObj.AutoResetTestCnt;\n"
  "  }else{"
  "    if(typeof myObj.BlynkConnErrCnt!='undefined'){\r\n"
  "      if(myObj.BlynkConnErrCnt==0){sa+='BLYNK: ok'}else{sa+='<font color=red>BLYNK: !!</font>'}\r\n"
  "    }\r\n"
  "  }\r\n"
  "  Status.innerHTML=sa;"
  
  "  Footer.innerHTML=myObj.Board+' - '+myObj.Versao+' - '+myObj.millis;\r\n"
  "}\r\n"
  
  "var wait=0;\r\n"

  "function loadjson(){\r\n"
  "    client.open('GET', '?fields=all', true); client.timeout = 20000; client.send();\r\n"
  "}\r\n"
  
  "function iniciar(){\r\n"
  "  setTimeout(loadjson,100);"
  "  setInterval(loadjson, 1000);\r\n"
  "}\r\n"

  "function btnRele(xx){\r\n"
  "  if(ReleSenha[xx]){location.href='keyb?rele='+xx; return;}"
  "  if(ReleTempoDesl[xx]<600&&ReleTempoDesl[xx]>0){ if(!confirm('Abrir/Fechar '+BtnLabel[xx])){return;} }\r\n"
  "  sa=xx+1;\r\n"
  "  client.abort(); client.open('GET','?fields=all&Lamp'+sa+'=1', true); client.send();\r\n"
  "}\r\n"

  "</script>\r\n"

  "<body onload='iniciar()'>\r\n"
  
  "<div style='width:100%' align='center'>"
  "  <table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='max-width:360px;background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>\n"
  "    <tr height='32' style='font-size:20px'>\r\n"
  "      <td style='width:10%;text-align:left'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\r\n"
  "      <td style='width:80%;text-align:center'><b id='tdtitle'>");pg+=DeviceID;pg+=F("</b></td>\r\n"
  "      <td style='width:10%' align='right'><input type=button class=btn0 onclick='location.href=\"config\"' value='#'></td>\r\n"
  "    </tr>\r\n"
  "  </table>\r\n"
  
  "  <table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px;'>\n"
  "   <tr><td style='text-align:center;padding:8px;'>\n"

  "     <div class='divbl'>"
  "      <p style='margin-bottom:0px;'><span id='hhmmss' style='margin:0px; font-size:32px;'>&nbsp;</span> <span id='ddmmaaaa' style='margin:0px; font-size:20px;'></span>"
  "      <font style='font-size:28px'><span id='Temperatura' style='margin:0px;'></span> <span id='Umidade' style='margin:0px; font-size:28px;'></span></font></p>"
  "<p style=margin-bottom:0;margin-top:0 ><progress style='width:120px;' prefix='Luz ' id='Lumi' min='0' max='900'></progress>"
  );
  if(Sensores>0){
    if(Sensores>4) pg+=F("<br>Sensores\r\n"); else pg+=F("&nbsp;Sensores\r\n");
    for(byte i=1;i<=Sensores;i++){
      pg+=F("<input type='button' id='Sensor");pg+=String(i);pg+=F("' class='led' value='");pg+=String(i);pg+=F("'>");
    }
  }
  pg+=F(
  "      </p>\r\n"
  "     </div>\r\n"
  );
  if(Reles>0){
    pg+=F("     <div class='divbl'>\r\n");
    
    //pg+=F("<button id='Lamp1' onclick='location.href=\"?Lamp=1\";' class='btn2' style='background-color:");pg+=BackgroundColor;pg+=F(";'></button>");

pg+=F("<p style='font-size:16px;margin-top:0px;margin-bottom:0px'>Ol&aacute; "); pg += user; pg += F("</p>");

    
    pg+=F("<button id='Lamp1' class='btn2' onclick='this.innerHTML=\"...\";btnRele(0);' style='background-color:");pg+=backgroundColor;pg+=F(";'></button>");
    pg+=F("<button id='Lamp2' class='btn2' onclick='this.innerHTML=\"...\";btnRele(1);' style='background-color:");pg+=backgroundColor;pg+=F(";'></button>");
    
    //pg+=F("<button id='Lamp2' class='btn2' onclick='this.innerHTML=\"...\";client.abort();"
    //"client.open(\"GET\",\"json?Lamp2=1\", true); client.send();' style='background-color:");pg+=BackgroundColor;pg+=F(";'></button>\r\n");
  }
  pg+=F(
  "     </div>\r\n"

"<table width='100%' border='0' cellspacing='0' cellpadding='0'><tr>"
"  <td width=20%>"
"    <button onclick='location.href=\"log\";'"
"      style='border-radius:8px;width:80px;height:60px;margin:0;background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";'>Log</button>"
"  </td>"
"  <td width=80%>"
"<div class='divbl' style='margin-bottom:0;margin-left:8px;height:48px;padding:2px;'>\n"
"  <span id='Status' style='font-size:14px;'></span><br>\n"
"  <progress max='70' id='wifipb' style='width:220px' prefix='WiFi '></progress>"
"</div>"
"</td></tr></table>"
  
  "<p style='font-size:12px;'><span id='Footer'></span></p>\r\n"

  "  </td></tr></table>\n"
  "</div>"
  "</body></html>\r\n");
  server.send(200, "text/html", pg); server.handleClient();
}

