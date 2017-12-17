void handleFir(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  pg=F(
"<html><head>\r\n"
"<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
"<title>");pg+=DeviceID;pg+=F("</title>\r\n"
"<style>\r\n"
"  body{margin:0}"
"  html,input,p{font-family:Verdana,Arial,Helvetica,sans-serif;}\r\n"
" .btn0{border-radius:6px;width:42px;height:32px}\r\n"
" .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
"</style>\r\n"

  "<script>\n"
  "sa='"); pg+=Versao;pg+=F(".ino.d1_mini.bin';\n"
  "function updateChange(){\n"
  "  sb=update.value;\n"
  "  while(sb.indexOf('\\\\')>=0) sb=sb.substring(sb.indexOf('\\\\')+1);\n"
  "  if((sa.substring(0,4)!=sb.substring(0,4))||(sb.substring(sb.length-4)!='.bin')){\n"
  "    alert(sa+'\\n'+sb+' (Arquivo rejeitado)');\n"
  "    location.reload();\n"
  "  }\n"
  "  submitBtn.disabled=false;"
  "}\n"
  "</script>\n"

"</head>"
"<body>\n"
"<div style='width:100%' align='center'>"
"  <table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='max-width:360px;background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
"    <tr height='32' style='font-size:20px'>\r\n"
"      <td style='width:15%;text-align:left'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\r\n"
"      <td style='width:70%;text-align:center'><b>Firmware</b></td>\r\n"
"      <td style='width:15%' align='right'></td>\r\n"
"    </tr>\r\n"
"  </table>\r\n"        
"  <table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px'>\n"
"   <tr><td style='text-align:center;padding:8px;'>\n"
"     <div class='divbl'>"
"<p style='margin-top:22px'>DeviceID: "); pg+=DeviceID; pg+=F("</p>"
"<p style='font-size:14px;'>Vers&atilde;o atual: "); pg+=Versao;pg+=F(".ino.d1_mini.bin</p>"
"<form method='POST' action='firmware' enctype='multipart/form-data'>\n"

  "<p><input type='file' name='update' id='update' onchange='updateChange()'></p>\n"

"<p><input id='submitBtn' type='submit' value='Update' style='border-radius:6px;width:120px;height:30px' disabled>\n"
"</p>"
"</form>"
"</div>"
"  </td></tr></table>\n"
"</div></body></html>\r\n"
    );
  server.send(200, "text/html", pg);
}

