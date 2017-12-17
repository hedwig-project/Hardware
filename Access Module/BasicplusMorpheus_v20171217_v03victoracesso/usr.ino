void handleUsers(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  String sa,sb,sc;
  sa = "user1";
  if(server.hasArg(sa)){
    sa = server.arg("user1");
    for(int j=1;j<11;j++){
      sa = server.arg("user"+String(j)); sa.replace(" ","");
      sb = server.arg("pass"+String(j)); sb.replace(" ","");
      if(sa.length()>0) sc += sa+","+sb+';';
    }
    pg = F("<!doctype html><html><body onload='history.go(-2)'></html>");
    server.send(200, "text/html", pg);
    users = sc;
    String todos = users;
    update_username = todos.substring(0,todos.indexOf(","));
    todos = todos.substring(todos.indexOf(",")+1);
    update_password = todos.substring(0,todos.indexOf(";"));
    todos = todos.substring(todos.indexOf(";")+1);
    gravarEeprom();
    return;
  }
  pg=F(
    "<!doctype html><html>\n"
    "<head>"
    "<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
    "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n"
    "<title>");pg+=DeviceID;pg+=F("</title>\n"
    "<style type='text/css'>\n"
    "  body{margin:0}\n"
    "  html,input,select{font:18px Verdana,Arial,Helvetica,sans-serif}\n"
    "  input{border-radius:6px;width:45%}\n"
    "  p{margin-left:8px;margin-top:8px;margin-bottom:8px}\n"
    " .btn0{border-radius:6px;width:42px;height:32px}\r\n"
    "  .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
    "</style>\n"
    "</head>"
    "<body onload='iniciar()'>\n"
    "<script>"
    "\n  function iniciar(){"
    "\n  }"
    "\n </script>"
  
  "<div align='center'>"
  
  "<table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  " <tr height='32' style='font-size:20px'>\n"
  "  <td style='width:15%'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\n"
  "  <td style='width:70%;text-align:center'><b>Usu&aacute;rios</b></td>\n"
  "  <td style='width:15%;text-align:right'><input type=button class=btn0 onclick='form1.submit();' value='OK'></td>\n"
  " </tr>"
  "</table>\n"
  
  "<table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'><tr>"
  "<td style='text-align:center;padding:8px;width:100%'>"
  );
  sa = users;
  int i = 0;
  pg += F("<form id='form1' method='get' action=''>"
  "     <div class='divbl'>");
  pg += F("<p>Nome/Senha</p>");
  while(sa.indexOf(";")>=0){    
    String sb = sa.substring(0,sa.indexOf(","));
    sa = sa.substring(sa.indexOf(",")+1);
    String sc = sa.substring(0,sa.indexOf(";"));
    sa = sa.substring(sa.indexOf(";")+1);
    i++;    
    pg += F("<p><input type='text' name='user"); pg += String(i); pg += F("' value='"); pg += sb; pg += F("' style='");
    if(i<3) pg+= F("color: gray' readonly>"); else pg += F("'>");
    pg += F(" <input type='text' name='pass"); pg += String(i); pg += F("' value='"); pg += sc; pg += F("'>");
    pg += F("</p>");
  }
  while(i<10){
    i++;
    pg += F("<p><input type='text' name='user"); pg += String(i); pg += F("' value=''>");
    pg += F(" <input type='text' name='pass"); pg += String(i); pg += F("' value=''>");
    pg += F("</p>");
  }
  pg += F(
  "     </div>\r\n"
  //"<p><input type='submit' value='Salvar' style='height:36px; width:50%'></p>"
  );
  pg += F("</form>");
  pg += F("</td></tr></table></div></body></html>\n");
  server.send(200, "text/html", pg);  
}

String autenticar(){
  String todos = users;
  while(todos.indexOf(";")>=0){
    String sa = todos.substring(0,todos.indexOf(","));
    todos = todos.substring(todos.indexOf(",")+1);
    String sb = todos.substring(0,todos.indexOf(";"));
    todos = todos.substring(todos.indexOf(";")+1);
    if(server.authenticate(sa.c_str(), sb.c_str())){
      User=sa;
      return sa;
    }
  }
  return "";
}

