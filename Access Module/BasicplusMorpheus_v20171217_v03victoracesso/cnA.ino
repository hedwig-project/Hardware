void handleAdvConfig(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  pg+=F(
  "<!doctype html><html>"
  "<head>\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\r\n"
    
  "<style>\r\n"
  "  html,input,p{font:18px Verdana,Arial,Helvetica,sans-serif}\r\n"
  "  h2{margin-top:30px;text-align:center}\r\n"
  "  input{text-align:center; border-radius:6px}\r\n"
  "  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .btn1{height:42px;width:100%;margin-bottom:6px;}\r\n"
  "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
  "</style>\r\n"
    
    "</head>"

"<body style='margin:0' onload='iniciar()'>\r\n"
  
        "<div style='width:100%' align='center'>"
        "  <table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='max-width:360px;"
        "    background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
        "    <tr height='32' style='font-size:20px'>\r\n"
        "      <td style='width:15%;text-align:left'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\r\n"
        "      <td style='width:70%;text-align:center'><b>Config Avan&ccedil;adas</b></td>\r\n"
        "      <td style='width:15%' align='right'></td>\r\n"
        "    </tr>\r\n"
        "  </table>\r\n"
        "  <table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px;'>\n"
        "   <tr><td style='text-align:center;padding:8px;'>\n"
        "     <div class='divbl'>"
        
  "<input type=button class=btn1 onclick='location.href=\"dht\"' value='Temperatura e Umidade'>\n"
  "<input type=button class=btn1 onclick='location.href=\"rf\"' value='RF 433'>\n"
  "<input type=button class=btn1 onclick='location.href=\"users\"' value='Usu&aacute;rios'>\n"
  "<input type=button class=btn1 onclick='location.href=\"blynk\"' value='BLYNK'>\n"
  "<input type=button class=btn1 onclick='location.href=\"wificon\"' value='WiFi'>\n"
  "<input type=button class=btn1 onclick='location.href=\"DeviceID\"' value='Nome do M&oacute;dulo'>\n"
  "<input type=button class=btn1 onclick='location.href=\"ap\"' value='Ponto de Acesso' style='margin-bottom:0;'>\n"

        "     </div>\r\n"
        "  </td></tr></table>\n"
        "</div></body></html>\r\n");
  server.send(200, "text/html", pg);  
}



