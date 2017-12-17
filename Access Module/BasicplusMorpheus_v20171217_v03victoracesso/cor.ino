void handleCor(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  String sa; byte gravar=0;
  sa = F("BackgroundColor"); if(server.arg(sa).length()>0){ backgroundColor = "#"+server.arg(sa); gravar++;}
  sa = F("Color1"); if(server.arg(sa).length()>0){ Color1 = "#"+server.arg(sa); gravar++;}
  sa = F("Color2"); if(server.arg(sa).length()>0){ Color2 = "#"+server.arg(sa); gravar++;}
  if(gravar>0){
    gravarEeprom();
    pg = F("<!doctype html><html><body onload='history.go(-3)'></html>");
    server.send(200, "text/html", pg);
    return;
  }
  pg=F(
  "<!doctype html><html><head>\r\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style type='text/css'>\r\n"
  "  body{margin:0}\r\n"
  "  html,input,select,button{font:18px Verdana,Arial,Helvetica,sans-serif}\r\n"
  "  input,button{border-radius:6px;text-align:center;height:26px}\r\n"
  "  p{margin-left:0;margin-top:18px;margin-bottom:0px;text-align:center}\r\n"
  "  table {border-style:none;width:100%}\r\n"
  "  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\r\n"
  "</style>\r\n"
  "</head>\r\n"
  "<body><div align='center'>\r\n"
  "<script>\r\n"
  "function salvar(){\r\n"
  "  url='?BackgroundColor='+BackgroundColor.value.substring(1)+'&Color1='+Color1.value.substring(1)+'&Color2='+Color2.value.substring(1);\r\n"
  "  location.href=url;\r\n"
  "}\r\n"
  "</script>\r\n"
  
  "<div align=center width=100%>\n"
  "<table id='header' border='0' cellspacing='6' cellpadding='0' style='background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  "<tr height='32' style='font-size:20px'>"
  "  <td width=15%><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>"
  "  <td width=70% align=center><b>CORES</b></td>"
  "  <td style='width:15%' align='right'><input type=button class=btn0 onclick='salvar();' value='OK'></td>\n"
  "</tr>\r\n"
  "</table>\r\n"
  "<table cellspacing='0' cellpadding='8'><tr><td>\r\n"
  "<div class='divbl'>\r\n"
  "<p>Cor de Fundo <input type=color id=BackgroundColor value='");pg+=backgroundColor;pg+=F("'"
  " onchange='header.style.backgroundColor=this.value;submit.style.backgroundColor=this.value'></p>\r\n"
  "<p>Texto 1 <input type=color id=Color1 value='");pg+=Color1;pg+=F("'"
  "  onchange='header.style.color=this.value'></p>\r\n"
  "<p>Texto 2 <input type=color id=Color2 value='");pg+=Color2;pg+=F("'"
  " onchange='submit.style.color=this.value'></p>\r\n"
  "<p><button id=submit onclick='salvar();' style='background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color2;pg+=F(";height:32px;width:120px;margin-bottom:10px'>"
  "  <b>SALVAR</b></button>\r\n"
  "</p>\r\n"
  "</div>\r\n"
  "</td></tr></table></div></body></html>");
  server.send(200, "text/html", pg);
}

