void handleKeyb(){
  String pg;
  String fields,sa;
  pg=F("{");
  byte ReleN=0; sa=F("rele");
  if(!server.hasArg(sa)) return;
  ReleN=server.arg(sa).toInt();
  if(ReleSenha[0].length()!=ReleSenhaCod[0].length() || ReleSenha[1].length()!=ReleSenhaCod[1].length()) aleatorio();
  sa=F("pass");if(server.hasArg(sa)){
    sa=server.arg(sa);
    pg=F("<!DOCTYPE html><html>"
    "<head>\r\n"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
    "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
    "</head>"
    "<body onload='setTimeout(function(){history.go(-2);}, 2000);'>"
    );
    if(sa==ReleSenhaCod[ReleN]){
      Rele[ReleN]=!Rele[ReleN];
      pg+=F("Abrindo/Fechando");
      msg=F("WebSeg"); msg+=String(ReleN+1); msg+=F(" ("); msg+=User; msg+=F(") acionado"); addLog(msg);
    }else{
      pg+=F("Senha incorreta");
      msg=F("WebSeg"); msg+=String(ReleN+1); msg+=F(" ("); msg+=User; msg+=F(") senha incorreta"); addLog(msg);
    }
    pg+=F("</body></html>");
    server.send(200, "text/html", pg);
    ReleSenhaCod[ReleN]="";
    return;
  }
  
  sa=F("fields"); if(server.hasArg(sa)){ fields=","+server.arg(sa)+",";}
  sa=F(",all,");  if(fields==sa){ fields=F(",kb0,kb1,kb2,kb3,kb4,");}
  if(fields.indexOf(F(",kb0,"))>=0){pg+=F("\"kb0\":\"");pg+=teclas[0]+" - "+teclas[1];pg+=F("\",");}
  if(fields.indexOf(F(",kb1,"))>=0){pg+=F("\"kb1\":\"");pg+=teclas[2]+" - "+teclas[3];pg+=F("\",");}
  if(fields.indexOf(F(",kb2,"))>=0){pg+=F("\"kb2\":\"");pg+=teclas[4]+" - "+teclas[5];pg+=F("\",");}
  if(fields.indexOf(F(",kb3,"))>=0){pg+=F("\"kb3\":\"");pg+=teclas[6]+" - "+teclas[7];pg+=F("\",");}
  if(fields.indexOf(F(",kb4,"))>=0){pg+=F("\"kb4\":\"");pg+=teclas[8]+" - "+teclas[9];pg+=F("\",");}
  if(pg.substring(pg.length()-1)==","){ pg = pg.substring(0,pg.length()-1); pg += F("}");
    server.send(200, "text/plain", pg);
    return;
  }

  String user = autenticar(); if(user.length()<=0) return server.requestAuthentication();
  
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
  "  .btn2 {border-radius:12px;width:46%; height: 80px; font-size:32px;margin:4px;font-weight: bold;background-color:");pg+=backgroundColor;pg+=F(";"
  "    color:");pg+=Color1;pg+=F(";}"
  "</style>\r\n"
  "</head>\r\n"
  "<body onload=iniciar()>\r\n"
  
  "<script>\r\n"
  "  var client = new XMLHttpRequest();\r\n"
  "  client.onreadystatechange = function(){\r\n"
  "    if(this.readyState==4 && this.status==200){\r\n"
  "      myObj=JSON.parse(this.responseText);\r\n"
  "      kb0.innerHTML=myObj.kb0;\r\n"
  "      kb1.innerHTML=myObj.kb1;\r\n"
  "      kb2.innerHTML=myObj.kb2;\r\n"
  "      kb3.innerHTML=myObj.kb3;\r\n"
  "      kb4.innerHTML=myObj.kb4;\r\n"
  "    }\r\n"
  "  }\r\n"
  "  function iniciar(){\r\n"
  "    client.open('GET', '?rele=");pg+=ReleN;pg+=F("&fields=all', true);client.send();\r\n"
  "  }\r\n"
  "function enviar(){"
  "  location.href='?rele=");pg+=ReleN;pg+=F("&pass='+pass.value;"
  "}"
  "</script>\r\n"
    
  "<div align='center'>\r\n"
  "<table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;max-width:360px'>"
  "<tr height='32' style='font-size:20px'>\n"
  "<td width=15% align=left><input id=btnVoltar type=button class=btn0 onclick='history.go(-1)' value='<'></td>\n"
  "<td width=70% align=center><b id='tdtitle'>");pg+=BtnLabel[ReleN];pg+=F("</b></td>\n"
  "<td width=15% align=right></td>\n"
  "</tr></table>\n"
  "<table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'><tr>"
  "<td style='text-align:center;padding:8px;width:100%'>"
  
  "<div class='divbl'>\r\n"
  "<p style='margin-top:4px;margin-bottom:4px'>\r\n"
  "<input type='password' name='pass' id='pass' readonly='readonly' style='font-size:24px;border-radius:8px;width:30%; text-align:center;height:34px;vertical-align:middle'>\r\n"
  " <input type='button' value='C' onclick='pass.value=\"\"' style='font-size:18px;border-radius:8px;width:30%;height:40px;vertical-align:middle'></p>\r\n"
  "<button class='btn2' onclick='pass.value=pass.value+\"0\"' id='kb0'>&nbsp;</button>\r\n"
  "<button class='btn2' onclick='pass.value=pass.value+\"1\"' id='kb1'>&nbsp;</button><br>\r\n"
  "<button class='btn2' onclick='pass.value=pass.value+\"2\"' id='kb2'>&nbsp;</button>\r\n"
  "<button class='btn2' onclick='pass.value=pass.value+\"3\"' id='kb3'>&nbsp;</button><br>\r\n"
  "<button class='btn2' onclick='pass.value=pass.value+\"4\"' id='kb4'>&nbsp;</button>\r\n"
  "<button class='btn2' onclick='enviar()'>OK</button></div>\r\n"
  
  "</td></tr></table>"
  "</div></body></html>\n");
  server.send(200, "text/html", pg);
}

void aleatorio(){
  randomSeed(millis());
  int randNumber;
  String sa;
  for (int i = 0; i <= 9 ; i++) teclas[i] = String(i);
  for (int i = 0; i <= 9 ; i++) {
    randNumber = random(10);
    sa = teclas[i];
    teclas[i] = teclas[randNumber];
    teclas[randNumber] = sa;
  }
  String sb; int ia;
  sa = "";
  for (int i = 0; i <= 9 ; i++) sa += teclas[i];
  for(byte j=0;j<2;j++){
    ReleSenhaCod[j] = "";
    for (int i = 0; i < ReleSenha[j].length() ; i++){
      sb = ReleSenha[j].substring(i, i + 1);
      ia = sa.indexOf(sb) / 2;
      ReleSenhaCod[j] += String(ia);
    }
  }
  /*/
  Serial.println(sa);
  Serial.print(F("ReleSenha[0] = "));Serial.println(ReleSenha[0]);
  Serial.print(F("ReleSenhaCod[0] = "));Serial.println(ReleSenhaCod[0]);
  Serial.print(F("ReleSenha[1] = "));Serial.println(ReleSenha[1]);
  Serial.print(F("ReleSenhaCod[1] = "));Serial.println(ReleSenhaCod[1]);
  /*/
}

