void handleAP(){
  String pg;
  String sa, fields, cmd;
  pg=F("{");
  sa=F("fields"); if(server.hasArg(sa)){ fields=","+server.arg(sa)+","; }
  if(fields.indexOf(F(",DeviceID,"))>=0){pg+=F("\"DeviceID\":\"");pg+=DeviceID;pg+=F("\",");}
  if(fields.indexOf(F(",apPass,"))>=0){ pg+=F("\"apPass\":\""); pg+=apPass; pg+=F("\","); }
  sa=F("cmd"); if(server.hasArg(sa)){cmd=server.arg(sa);}
  sa=F("changeAPmode"); if(cmd==sa){
    APmode=!APmode;
    pg+=F("\"APmode\":"); pg+=APmode; pg+=F(",");
    if(pg.substring(pg.length()-1)==",") pg = pg.substring(0,pg.length()-1); pg += F("}");
    server.send(200, "text/plain", pg);
    gravarEeprom();
    if(APmode){
      addLog(F("AP Conectando"));
      WiFi.softAPdisconnect(true);
    }else{
      addLog(F("AP Desconectado"));
      WiFi.softAP(DeviceID.c_str(), apPass.c_str());
    }
    return;
  }
  if(fields.indexOf(F(",APmode,"))>=0){ pg+=F("\"APmode\":"); pg+=APmode; pg+=F(","); }
  if(pg.length()>1){
    if(pg.substring(pg.length()-1)==",") pg = pg.substring(0,pg.length()-1); pg += F("}");
    server.send(200, "text/plain", pg);
    return;
  }
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  pg=F(
  "<html>"
  "<head>\r\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style>\r\n"
  "  body{margin:0}\n"
  " html,input,p{font:18px Verdana,Arial,Helvetica,sans-serif}\r\n"
  " h2{margin-top:30px;text-align:center}\r\n"
  " input{text-align:center; border-radius:6px}\r\n"
  " .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  " .btn1{height:40px;width:80%}\r\n"
  "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:8px;border-color:#e1e1e1}\n"
  "  p{margin:8px 0 8px 0;line-height:160%;text-align:center}\n"
  "</style>\r\n"
  "</head>"
  "<body onload='iniciar()'>\r\n"

  "<script>"
  "var client = new XMLHttpRequest();\n\r\n"
  "client.onreadystatechange = function() {\r\n"
  "  if (this.readyState==4 && this.status==200){\r\n"
  "    myObj=JSON.parse(this.responseText);\r\n"
  "    DeviceID.value=myObj.DeviceID;"
  "    apPass.value=myObj.apPass;"
  "    if(typeof myObj.APmode!='undefined'){\r\n"
  "      if(myObj.APmode){APmodeBtn.value='Ativar'}else{APmodeBtn.value='Desativar'}"
  "    }\r\n"
  "    if(typeof myObj.cmd!='undefined'){\r\n"
  "      sa=myObj.cmd;\r\n"
  "      if(sa=='goHome'){history.go(-3);}\r\n"
  "    }\r\n"
  "  }\r\n"
  "}\r\n"
  "function changeAPmode(){"
  "  client.open('GET', '?fields=DeviceID,APmode,apPass&cmd=changeAPmode', true); client.send();\r\n"
  "}"
  "function iniciar(){"
  "  client.open('GET', '?fields=DeviceID,APmode,apPass', true); client.send();\r\n"
  "}"
  "function salvar(){"
  "  client.open('GET', 'json?fields=APmode&DeviceID='+DeviceID.value+'&apPass='+apPass.value , true); client.send();\r\n"
  "}"
  "</script>"

  "<div style='width:100%' align='center'>"
  "  <table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  "    <tr height='32' style='font-size:20px'>\r\n"
  "      <td style='width:15%;text-align:left'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\r\n"
  "      <td style='width:70%;text-align:center'><b>Ponto de Acesso</b></td>\r\n"
  "      <td style='width:15%' align='right'></td>\r\n"
  "    </tr>\r\n"
  "  </table>\r\n"
  
  "  <table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'>\n"
  "   <tr><td style='text-align:center;padding:8px;'>\n"
  "     <div class='divbl'>"
  "       <p><input type=button class=btn1 id='APmodeBtn' onclick='changeAPmode()' value=''></p>\r\n"
  "     </div>\r\n"
  
  "     <div class='divbl'>"
  "       <p>Nome<br><input type=text id='DeviceID' value='' style='width:80%'>\r\n"
  "       <br>Senha<br><input type=text id='apPass' value='' style='width:80%'>\r\n"
  "       <br><br><input type=button class=btn1 onclick='salvar()' value='Salvar'></p>\r\n"
  "     </div>\r\n"
  
  "  </td></tr></table>\n"
  "</div></body></html>\r\n");
  server.send(200, "text/html", pg);
}

