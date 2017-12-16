void handleReles(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  String sa,sb,sc,sd;
  sa=F("BtnLabel0"); sb=F("BtnLabel1"); sc=F("ReleTempoDesl0"); sd=F("ReleTempoDesl1");
  if(server.hasArg(sa)&&server.hasArg(sb)){
    BtnLabel[0]=server.arg(sa);
    BtnLabel[1]=server.arg(sb);
    ReleTempoDesl[0]=server.arg(sc).toInt();
    ReleTempoDesl[1]=server.arg(sd).toInt();
    sa=F("Rele1Auto"); if(server.hasArg(sa)) ReleAuto[0]=server.arg(sa).toInt();
    sa=F("Rele2Auto"); if(server.hasArg(sa)) ReleAuto[1]=server.arg(sa).toInt();
    sa=F("ReleHoraIni0"); if(server.hasArg(sa)) ReleHoraIni[0]=server.arg(sa).toInt();
    sa=F("ReleHoraFim0"); if(server.hasArg(sa)) ReleHoraFim[0]=server.arg(sa).toInt();
    sa=F("ReleHoraIni1"); if(server.hasArg(sa)) ReleHoraIni[1]=server.arg(sa).toInt();
    sa=F("ReleHoraFim1"); if(server.hasArg(sa)) ReleHoraFim[1]=server.arg(sa).toInt();
    sa=F("ReleSenha0"); if(server.hasArg(sa)) ReleSenha[0]=server.arg(sa);
    sa=F("ReleSenha1"); if(server.hasArg(sa)) ReleSenha[1]=server.arg(sa);
    sa=F("RFcode0"); if(server.hasArg(sa)) RFcode[0]=server.arg(sa);
    sa=F("RFcode1"); if(server.hasArg(sa)) RFcode[1]=server.arg(sa);
    #ifdef pinRF_TX
    sa=F("ReleSenha4"); if(server.hasArg(sa)) ReleSenha[4]=server.arg(sa);
    sa=F("ReleSenha5"); if(server.hasArg(sa)) ReleSenha[5]=server.arg(sa);
    sa=F("RFcode4"); if(server.hasArg(sa)) RFcode[4]=server.arg(sa);
    sa=F("RFcode5"); if(server.hasArg(sa)) RFcode[5]=server.arg(sa);
    #endif
    gravarEeprom();
    pg = F("<!doctype html><html><body onload='history.go(-3)'></html>");
    server.send(200, "text/html", pg);
    //Rele[0]=false; Rele[1]=false;
    //CheckReleAutoSim();
    return;
  }
  sa=sa=F("lastRFcode");if(server.hasArg(sa)){
    lastRFcode="";
    pg=F("{}");
    server.send(200, "text/plain", pg);
    return;
  }
  String fields;
  sa=F("fields"); if(server.hasArg(sa)){ fields=","+server.arg(sa)+","; }
  if(fields.indexOf(F(",lastRFcode,"))>=0){
    pg=F("{\"lastRFcode\":\""); pg+=lastRFcode; pg+=F("\"}");
    server.send(200, "text/plain", pg);
    return;
  }
  
  pg=F(
  "<!DOCTYPE html><html>"
  "<head>\r\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style type='text/css'>\n"
  "  body{margin:0}\n"
  "  html,input,select{font:18px Verdana,Arial,Helvetica,sans-serif}\n"
  "  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:8px;border-color:#e1e1e1}\n"
  "  p{margin:8px 0 8px 0;line-height:180%;text-align:center}\n"
  "  input,select{border-radius:6px;text-align:center;}\n"
  "</style>\n"
  "</head>\r\n"
  "<body onload=iniciar()>"
  
  "<script>"
  
  "function onch(){"
  "  i=(ReleTempoDesl0.value<3000)&&(ReleTempoDesl0.value>0);"
  "  y=(ReleTempoDesl1.value<3000)&&(ReleTempoDesl1.value>0);"
  "  Rele1Auto.disabled=i;"
  "  Rele2Auto.disabled=y;"
  "  i=i||(Rele1Auto.value<1);"
  "  y=y||(Rele2Auto.value<1);"
  "  ReleHoraIni0.disabled=i;"
  "  ReleHoraFim0.disabled=i;"
  "  ReleHoraIni1.disabled=y;"
  "  ReleHoraFim1.disabled=y;"
  "  ReleTempoDesl0.disabled=(Rele1Auto.value==255);"
  "  ReleTempoDesl1.disabled=(Rele2Auto.value==255);"
  "  if(Rele1Auto.value==255){ReleTempoDesl0.value=0;}"
  "  if(Rele2Auto.value==255){ReleTempoDesl1.value=0;}"
  "  if((ReleTempoDesl0.value<3000)&&(ReleTempoDesl0.value>0)){Rele1Auto.value=0;}"
  "  if((ReleTempoDesl1.value<3000)&&(ReleTempoDesl1.value>0)){Rele2Auto.value=0;}"
  "}"
  
  "  function iniciar(){\r\n"
  "    BtnLabel0.value='");pg+=BtnLabel[0];pg+=F("';"
  "    BtnLabel1.value='");pg+=BtnLabel[1];pg+=F("';"
  "    ReleTempoDesl0.value='");pg+=ReleTempoDesl[0];pg+=F("';"
  "    ReleTempoDesl1.value='");pg+=ReleTempoDesl[1];pg+=F("';"
  "    Rele1Auto.value='");pg+=ReleAuto[0];pg+=F("';"
  "    Rele2Auto.value='");pg+=ReleAuto[1];pg+=F("';"
  "    ReleHoraIni0.value='");pg+=ReleHoraIni[0];pg+=F("';"
  "    ReleHoraIni1.value='");pg+=ReleHoraIni[1];pg+=F("';"
  "    ReleHoraFim0.value='");pg+=ReleHoraFim[0];pg+=F("';"
  "    ReleHoraFim1.value='");pg+=ReleHoraFim[1];pg+=F("';"
  "    ReleSenha0.value='");pg+=ReleSenha[0];pg+=F("';"
  "    ReleSenha1.value='");pg+=ReleSenha[1];pg+=F("';"
  "    RFcode0.value='");pg+=RFcode[0];pg+=F("';"
  "    RFcode1.value='");pg+=RFcode[1];pg+=F("';"
  "    RFcode0value='");pg+=RFcode[0];pg+=F("';"
  "    RFcode1value='");pg+=RFcode[1];pg+=F("';"
  #ifdef pinRF_TX
  "    RFcode4value='");pg+=RFcode[4];pg+=F("';"
  "    RFcode5value='");pg+=RFcode[5];pg+=F("';"
  "    RFcode4.value='");pg+=RFcode[4];pg+=F("';"
  "    RFcode5.value='");pg+=RFcode[5];pg+=F("';"
  #endif
  "    onch();"
  "  }\r\n"
  
  "  adding=0;"
  "  addingCnt=0;"

  "var client = new XMLHttpRequest();\n\r\n"
  "client.onreadystatechange = function() {\r\n"
  "  if (this.readyState==4&&this.status==200){\r\n"
  "    myObj=JSON.parse(this.responseText);\r\n"
  "    if(typeof myObj.lastRFcode!='undefined'){"
  "      lastRFcode=myObj.lastRFcode;"
  "      if(lastRFcode.length>0){"
  "        if(adding==0&&RFcode0value.indexOf(lastRFcode)<0){"
  "          if(RFcode0value.length>0){RFcode0value+='|';}"
  "          RFcode0value+=lastRFcode;RFcode0.value=RFcode0value;"
  "          addingCnt=0;"
  "        }"
  "        if(adding==1&&RFcode1value.indexOf(lastRFcode)<0){"
  "          if(RFcode1value.length>0){RFcode1value+='|';}"
  "          RFcode1value+=lastRFcode;RFcode1.value=RFcode1value;"
  "          addingCnt=0;"
  "        }"
  "      }else{"
  "        if(addingCnt>0){"
  "          if(adding==0){RFcode0.value='Aguardando: '+addingCnt;}"
  "          if(adding==1){RFcode1.value='Aguardando: '+addingCnt;}"
  "        }else{"
  "          if(adding==0){RFcode0.value=RFcode0value;}"
  "          if(adding==1){RFcode1.value=RFcode1value;}"
  "        }"
  "      }"
  "    }\r\n"
  "  }\r\n"
  "}\r\n"

  "  function addcode(xx){"
  "    adding=xx;"
  "    addingCnt=15;"
  "    client.open('GET', '?lastRFcode=clear' , true);client.send();"
  "    setTimeout(loadJson,1000);"
  "  }"

  "  function loadJson(){"
  "    if(addingCnt>0){"
  "      client.open('GET', '?fields=lastRFcode' , true);client.send();"
  "      setTimeout(loadJson,1000);"
  "      addingCnt--;"
  "    }else{"
  "    }"
  "  }"

  "</script>"
    
  "<div align='center'>"
  "<table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  "<tr height='32' style='font-size:20px'>\n"
  "<td width=15% align=left><input id=btnVoltar type=button class=btn0 onclick='history.go(-1)' value='<'></td>\n"
  "<td width=70% align=center><b>Reles</b></td>\n"
  "<td width=15% align=right><input id=btnHome type=button class=btn0 onclick='form1.submit()' value='OK'></td>\n"
  "</tr></table>\n"
  "<table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'><tr>"
  "<td style='text-align:center;padding:8px;width:100%'>"
  
  "<script>"
  "sa='<option value=0>Nunca</option>';"
  "sa+='<option value=5>Pulso 0,5 seg</option>';"
  "sa+='<option value=10>Pulso 1 seg</option>';"
  "sa+='<option value=30>Pulso 3 seg</option>';"
  "sa+='<option value=50>Pulso 5 seg</option>';"
  "sa+='<option value=100>Pulso 10 seg</option>';"
  "sa+='<option value=3000>5 minutos</option>';"
  "sa+='<option value=6000>10 minutos</option>';"
  "sa+='<option value=12000>20 minutos</option>';"
  "sa+='<option value=18000>30 minutos</option>';"
  "sa+='<option value=36000>1 Hora</option>';"
  "sa+='<option value=72000>2 Horas</option>';"
  "sa+='<option value=144000>4 Horas</option>';"
  "sa+='<option value=288000>8 Horas</option>';"
  "</script>"

  "<form id=form1 method=get>\r\n"
  
  "<div class='divbl' align='center'>"
  "<p><b>Rele 1</b> <input type='text' id='BtnLabel0' name='BtnLabel0' style='width:70%'></p>"
  "<p>Desligar ap&oacute;s <select id=ReleTempoDesl0 name=ReleTempoDesl0 onchange='onch()'>"
  "<script>document.writeln(sa)</script>"
  "</select></p>"
  "<p>Auto Liga <select id=Rele1Auto name=Rele1Auto onchange='onch()'>"
  "<option value=0>N&atilde;o</option>"
  "<option value=1>Sensor 1</option>"
  "<option value=2>Sensor 2</option>"
  "<option value=255>Sim</option>"
  "</select></p>"
  "<p>das"
  " <select id=ReleHoraIni0 name=ReleHoraIni0><script> for(i=0;i<24;i++){document.writeln('<option value='+i+'>'+i+'</option>')}</script></select> as"
  " <select id=ReleHoraFim0 name=ReleHoraFim0><script> for(i=0;i<24;i++){document.writeln('<option value='+i+'>'+i+'</option>')}</script></select>"
  "</p>"
  "<p>Senha <input type=text id=ReleSenha0 name=ReleSenha0></p>"
  
  "<p style='line-height:120%'>Controle Remoto<br>"
  "<input type=button value=C style=width:12%;height:34px onclick='RFcode0.value=\"\";RFcode0value=\"\";'>\n"
  "<input type=text id=RFcode0 name=RFcode0 style=width:70%;height:30px>\n"
  "<input type=button value=+ style=width:12%;height:34px onclick=addcode(0)></p>"
  #ifdef pinRF_TX
  "<p style='line-height:120%'>Transmitir<br>"
  "<input type=button value=C style=width:12%;height:34px onclick='RFcode4.value=\"\";RFcode4value=\"\";'>\n"
  "<input type=text id=RFcode4 name=RFcode4 style=width:70%;height:30px>\n"
  "<input type=button value=+ style=width:12%;height:34px onclick=addcode(4)></p>"
  #endif
  "</div>"
  
  "<div class='divbl' align='center'>"
  "<p><b>Rele 2</b> <input type='text' id=BtnLabel1 name=BtnLabel1 style='width:70%'></p>"
  "<p>Desligar ap&oacute;s <select id=ReleTempoDesl1 name=ReleTempoDesl1 onchange='onch()'>"
  "<script>document.writeln(sa)</script>"
  "</select></p>"
  "<p>Auto Liga <select id=Rele2Auto name=Rele2Auto onchange='onch()'>"
  "<option value=0>N&atilde;o</option>"
  "<option value=1>Sensor 1</option>"
  "<option value=2>Sensor 2</option>"
  "<option value=255>Sim</option>"
  "</select></p>"
  "<p>das"
  " <select id=ReleHoraIni1 name=ReleHoraIni1><script> for(i=0;i<24;i++){document.writeln('<option value='+i+'>'+i+'</option>')}</script></select> as"
  " <select id=ReleHoraFim1 name=ReleHoraFim1><script> for(i=0;i<24;i++){document.writeln('<option value='+i+'>'+i+'</option>')}</script></select>"
  "</p>"
  "<p>Senha <input type=text id=ReleSenha1 name=ReleSenha1></p>"

  "<p style='line-height:120%'>Controle Remoto<br>"
  "<input type=button value=C style=width:12%;height:34px onclick='RFcode1.value=\"\";RFcode1value=\"\";'>\n"
  "<input type=text id=RFcode1 name=RFcode1 style=width:70%;height:30px>\n"
  "<input type=button value=+ style=width:12%;height:34px onclick=addcode(1)></p>"
  #ifdef pinRF_TX
  "<p style='line-height:120%'>Transmitir<br>"
  "<input type=button value=C style=width:12%;height:34px onclick='RFcode5.value=\"\";RFcode5value=\"\";'>\n"
  "<input type=text id=RFcode5 name=RFcode5 style=width:70%;height:30px>\n"
  "<input type=button value=+ style=width:12%;height:34px onclick=addcode(5)></p>"
  #endif
  
  "</div>"

  "</form>"
  
  "</td></tr></table>"
  "</div></body></html>\n");
  server.send(200, "text/html", pg);
}

void CheckReleAutoSim()
{
  //Serial.println(F("CheckReleAutoSim"));
  time_t t = now(); timeZoneChk(); t += timeZone * SECS_PER_HOUR;
  for(byte r=0;r<2;r++){
    if(ReleAuto[r]==255){ // 255 = sim
      Rele[r]=false;
      ReleCnt[r]=0;
      if(ReleHoraIni[r]<ReleHoraFim[r]){
        if(hour(t) >= ReleHoraIni[r] && hour(t) < ReleHoraFim[r])  Rele[r] = true;
      }else{
        if(hour(t) >= ReleHoraIni[r] || hour(t) < ReleHoraFim[r])  Rele[r] = true;
      }
    }
  }  
}


