void handleConfig(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  pg=F(
  "<!DOCTYPE html>\r\n"
  "<html>\r\n"
  "<head>\r\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style>\r\n"
  "  body{margin:0}\r\n"
  "  html,input,p{font:18px Verdana,Arial,Helvetica,sans-serif}\r\n"
  "  input{text-align:center; border-radius:6px}\r\n"
  "  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .btn1{height:42px;width:100%;margin-bottom:6px;}\r\n"
  " .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
  "</style>\r\n"
  "</head>"
  "<body onload='iniciar()'>\r\n"

  "<script>"
  "var cmd='';"
  "var client = new XMLHttpRequest();\n\r\n"
  "client.timeout = 20000;\r\n"
  "client.onreadystatechange = function() {\r\n"
  "  if (this.readyState==4 && this.status==200){\r\n"
  "    var myObj=JSON.parse(this.responseText);\r\n"
  "    myFunction(myArr);\r\n"
  "  }\r\n"
  "}\r\n"  
  "function loadjson(){\r\n"
  "  if(cmd!=''){"
  "    cmd=cmd.substring(0,cmd.length-1);\r\n"
  "    if(cmd==''){url='json';}else{url='json?'+cmd;}\r\n"
  "    cmd='';\r\n"
  "    client.open('GET', url, true);client.send();\r\n"
  "  }\r\n"
  "}\r\n"
  "</script>"

  "<script>"
  "function myFunction(myObj) {\r\n"
  "  if(typeof myObj.cmd!='undefined'){\r\n"
  "    sa=myObj.cmd;\r\n"
  "    if(sa=='goback'){history.go(-1);}\r\n"
  "  }\r\n"
  "}\r\n"  
  
  "function iniciar(){\r\n"
  //" setInterval(loadjson, 1000); \r\n"
  "}\r\n"
  "</script>\r\n"
  
  "<div style='width:100%' align='center'>"
  "<table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='max-width:360px;background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  "<tr height='32' style='font-size:20px'>\r\n"
  "<td style='width:15%;text-align:left'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\r\n"
  "<td style='width:70%;text-align:center'><b>Configura&ccedil;&otilde;es</b></td>\r\n"
  "<td style='width:15%' align='right'></td>\r\n"
  "</tr>\r\n"
  "</table>\r\n"
        
"  <table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'>\n"
"   <tr><td style='text-align:center;padding:8px;'>\n"
"     <div class='divbl'>"

#ifdef LCD_SCL
"<input type=button class=btn1 onclick='location.href=\"lcd\"' value='Display'>\n"
#endif
"<input type=button class=btn1 onclick='location.href=\"cor\"' value='Cores'>\n"
"<input type=button class=btn1 onclick='location.href=\"alarm\"' value='Alertas e Alarmes'>\n"
"<input type=button class=btn1 onclick='location.href=\"reles\"' value='Reles'>\n"
"<input type=button class=btn1 onclick='location.href=\"logcnf\"' value='LOG'>\n"
"<input type=button class=btn1 onclick='location.href=\"tools\"' value='Ferramentas'>\n"
"<input type=button class=btn1 onclick='location.href=\"advconfig\"' value='Configura&ccedil;&otilde;es Avan&ccedil;adas' style='margin-bottom:0;'>\r\n"

"     </div>\r\n"
"  </td></tr></table>\n"
"</div>"
"</body></html>\r\n"
);
  server.send(200, "text/html", pg);  
}

