void handleDeviceID(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  String sa;
  sa=F("DeviceID");
  if(server.hasArg(sa)){
    DeviceID=server.arg(sa);
    pg = F(
      "<html>\n"
      "<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
      "<body>\n"
      "<script>\n"
      "function timer1(){\n"
      "  if(location.href.indexOf('/DeviceID')<0){history.go(-1);}else{history.go(-4);}\n"
      "}\n"
      "setTimeout(timer1, 5000);\n"
      "</script>\n"
      "Reiniciando...\n"
      "</html>\n"
      );
    server.send(200, "text/html", pg);
    gravarEeprom();
    #ifdef LCD_SCL
    lcd.setCursor(0, 0); lcd.print(centralizar(DeviceID));
    lcd.setCursor(0, 1); lcd.print(centralizar(F("Reiniciando")));
    #endif
    delay(1000);
    ESP.reset();
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
  "  input{border-radius:6px;height:40px;text-align:center}\n"
  "  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:8px;border-color:#e1e1e1}\n"
  "</style>\n"
  "</head>\r\n"
  "<body>\n"
  "<div align='center'>"
  "  <table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='max-width:360px;"
  "  background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  "<tr height='32' style='font-size:20px'>\n"
  "<td width=15% align=left><input id=btnVoltar type=button class=btn0 onclick='history.go(-1)' value='<'></td>\n"
  "<td width=70% align=center><b>Nome do M&oacute;dulo</b></td>\n"
  "<td width=15% align=right></td>\n"
  "</tr></table>\n"
  "<table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px;'><tr>"
  "<td style='text-align:center;padding:8px;width:100%'>"
  "<div class='divbl' align='center'>\n"
  "<form id=form1 method=get>\n"
  "<input type=text id=DeviceID name=DeviceID style='width:90%;margin-bottom:6px;' value='");pg+=DeviceID;pg+=F("'>\n"
  "<br><input type=submit value='Salvar e Reiniciar'style='width:70%'>\n"
  "</form>\n"
  "</div>\n"
  "</td></tr></table></div></body></html>\n"
  );
  server.send(200, "text/html", pg);
}

