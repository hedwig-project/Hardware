void addLog(String xx){
  String sa;
  unsigned long t2;
  t2=millis()/1000;
  //t2=t;
  int dias=t2/86400;
  if(dias>0) sa+= String(dias)+".";
  if(hour(t2)<10)sa+="0";
  sa+=String(hour(t2))+":";
  if(minute(t2)<10)sa+="0";
  sa+=String(minute(t2))+":";
  if(second(t2)<10)sa+="0";
  sa+=String(second(t2));
  sa+=F(" ");
  sa+=xx;
  Serial.println(sa); //Print no serial
  //
  time_t t = now(); //timeZoneChk(); t += timeZone * SECS_PER_HOUR;
  if(log_str.length()>4000) log_str=log_str.substring(log_str.indexOf(";")+1);
  sa=t; sa+=F(" "); sa+=xx; sa+=F(";");
  log_str+=sa;
}

void ajustarHoraLog(){
  time_t t = now(); //timeZoneChk(); t += timeZone * SECS_PER_HOUR;
  unsigned long t2; t2=millis()/1000;
  unsigned long somar = t-t2;
  String sa = log_str;
  String sb,sc;
  log_str="";
  while(sa.indexOf(";")>=0){
    sb = sa.substring(0,sa.indexOf(" "));
    sa = sa.substring(sa.indexOf(" ")+1);
    sc = sa.substring(0,sa.indexOf(";")+1);
    sa = sa.substring(sa.indexOf(";")+1);
    if(sb.toInt()<100000){
      sb = String(sb.toInt()+somar);
    }
    log_str += sb+" "+sc;
  }
  //if(sendLogURL.length()>0) sendLogBuffer+=log_str;
}

void handleLog(){
  String user = autenticar(); if(user.length()<=0) return server.requestAuthentication();
  String pg;
  pg=F(
  "<html>\r\n"
  "<head>\r\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style>\r\n"
  "  body{margin:0;}\r\n"
  "  .btn0{border-radius:6px;width:42px;height:32px}\r\n"
  "  .btn1{border-radius:6px;height:32px}\r\n"
  "</style>\r\n"
  "</head>"
  "<body onload='iniciar()'>\r\n"
  "<script>\r\n"
  "var logFile='';\r\n"
  "var client = new XMLHttpRequest();\r\n"
  "client.onreadystatechange = function() {\r\n"
  "  if (this.readyState==4 && this.status==200){\r\n"
  "    var myObj=JSON.parse(this.responseText);\r\n"
  "    if(typeof myObj.log!='undefined'){\r\n"
  "      sa=myObj.log;\r\n"
  "      sb='';\r\n"
  "      while(logFile.indexOf(sa)<0 && sa.length>0){\r\n"
  "        sa=sa.substring(0,sa.lastIndexOf(';'));\r\n"
  "        sb=sa.substring(sa.lastIndexOf(';')+1)+';'+sb;\r\n"
  "        sa=sa.substring(0,sa.lastIndexOf(';')+1);\r\n"
  "      }\r\n"
  "      if(sb.length>0){\r\n"
  "        logFile+=sb;\r\n"
  "        localStorage.setItem('logFile', logFile);\r\n"
  "        mostrarLog();\r\n"
  "      }\r\n"
  "    }\r\n"
  "  }\r\n"
  "  if (this.readyState==4){var d = new Date(); span01.innerHTML=d.toLocaleString();}\r\n"
  "}\r\n"
  "function loadjson(){\r\n"
//  "  client.open('GET', 'http://179.212.163.32:8002/json?fields=log' , true); client.send();\r\n"
  "  client.open('GET', 'json?fields=log' , true); client.send();\r\n"
  "  if(btnStart.value=='Parar') setTimeout(loadjson, 10000);\r\n"
  "}\r\n"
  
  "function iniciar(){\r\n"
  "  if(localStorage.getItem('logFile')){\r\n"
  "    logFile = localStorage.getItem('logFile');\r\n"
  "    mostrarLog();\r\n"
  "  }\r\n"
  "  loadjson();\r\n"
  "  setInterval(intervalo,100);\n"
  "}\r\n"
  
  "function start(){\r\n"
  "  if(btnStart.value!='Parar'){btnStart.value='Parar';loadjson();\r\n"
  "  }else{btnStart.value='Iniciar';}\r\n"
  "}\r\n"
  
  "function mostrarLog(){\r\n"
  "  sa=logFile; sc='';\r\n"
  "  while(sa.indexOf(';')>=0){\r\n"
  "    sb=sa.substring(0,sa.indexOf(' '));\r\n"
  "    sa=sa.substring(sa.indexOf(' '));\r\n"
  "    sb++;sb--;sb=sb*1000;\r\n"
  "    var d = new Date(sb);\r\n"
  "    sc += d.toLocaleString();\r\n"
  "    sd = sa.substring(0,sa.indexOf(';'));\r\n"
  "    if(sd==' Iniciando'){sc+='<b>'+sd+'</b>';}else{sc+=sd;}\r\n"
  "    sc+='<br>';\r\n"
  "    sa=sa.substring(sa.indexOf(';')+1);\r\n"
  "  }\r\n"
  "  span03.innerHTML=sc;\r\n"
  "  setTimeout(function(){scrollTo(0,500000);tableh.style.top=document.body.scrollTop+'px';}, 100);\r\n"
  "}\r\n"

  "function limpar(){\r\n"
  "  if(confirm('Limpar dados salvos neste computador?')){"
  "    logFile='';\r\n"
  "    localStorage.removeItem('logFile');\r\n"
  "    location.reload();\r\n"
  "  }"
  "}\r\n"

  "function intervalo(){tableh.style.top=document.body.scrollTop+'px';}\n"

  "</script>\r\n"
  
  "<div align='center' style='position:absolute;top:40px;left:0;width:100%;'>\n"
  "<table border=0 cellspacing=0>\r\n"
  " <tr><td style='text-align:left;padding:8px;'><p style='font-size:13px;'><span id=span03></span></p></td></tr>\r\n"
  "</table>\r\n"
  "</div>\r\n"

  "<div id='tableh' align='center' style='position:absolute;top:0;left:0;width:100%;padding:0;'>\n"
  "<table border=0 cellspacing=6 cellpadding=0 style='width:100%;max-width:360px;background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";'>\r\n"
  " <tr>\r\n"
  "  <td align=left width=5% nowrap><input type='button' class='btn0' onclick='history.go(-1);' value='<'> <input type='button' class=btn1 id='btnStart' onclick='start()' value='Parar'></td>\r\n"
  "  <td align=center width=90%><span id=span01></span></td>\r\n"
  "  <td align=right width=5%><button class=btn1 onclick='limpar()'>Limpar</button></td>\r\n"
  " </tr>\r\n"
  "</table>\r\n"
  "</div>\r\n"
  
  "</body>\r\n"
  "</html>\r\n"
  );
  server.send(200, "text/html", pg);
}

void handleLogCnf(){
  String user = autenticar(); if(user != "admin") return server.requestAuthentication();
  String pg;
  String sa;
  sa = F("log_opc");
  if(server.hasArg(sa)){
    log_opc = ","+server.arg(sa)+",";
    msg=F("log_opc: "); msg+=log_opc; addLog(msg);
    gravarEeprom();
    pg = F("<!doctype html><html><body onload='history.go(-2)'></html>");
    server.send(200, "text/html", pg);
    return;
  }
  pg+=F(
"<!doctype html><html>"
"<head>\n"
"<meta name='viewport' content='width=device-width, initial-scale=1'>\r\n"
"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\r\n"
"<title>");pg+=DeviceID;pg+=F("</title>\r\n"
"<style>\r\n"
" html,input,div{font:18px Verdana,Arial,Helvetica,sans-serif}\r\n"
" h2{margin-top:30px;text-align:center}\r\n"
"  p{text-align:center;margin:8px}\r\n"
" input{text-align:center; border-radius:6px}\r\n"
" .btn0{border-radius:6px;width:42px;height:32px}\r\n"
" .btn1{height:42px;width:100%;margin-bottom:6px;}\r\n"
" .divbl{border-style:solid;border-radius:8px;margin-bottom:8px;padding:6px;border-color:#e1e1e1}\n"
"</style>\r\n"
"</head>"
"<body style='margin:0' onload='iniciar()'>\r\n"
"<script>"
"function iniciar(){");
if(log_opc.indexOf(",Lumi,")>=0){pg+=F("Lumi.checked=true;");}
if(log_opc.indexOf(",Temp,")>=0){pg+=F("Temp.checked=true;");}
if(log_opc.indexOf(",Umi,")>=0){pg+=F("Umi.checked=true;");}
if(log_opc.indexOf(",Sensores,")>=0){pg+=F("Sensores.checked=true;");}
if(log_opc.indexOf(",Reles,")>=0){pg+=F("Reles.checked=true;");}
pg+=F(
"}"
"function salvar(){"
"  sa='';"
"  if(Lumi.checked) sa+=',Lumi';\n"
"  if(Temp.checked) sa+=',Temp';\n"
"  if(Umi.checked) sa+=',Umi';\n"
"  if(Sensores.checked) sa+=',Sensores';\n"
"  if(Reles.checked) sa+=',Reles';\n"
"  if(sa.substring(0,1)==',') {sa=sa.substring(1);}"
"  location.href='?log_opc='+sa;"
"}"
"</script>"
  "<div style='width:100%' align='center'>"
  "  <table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='max-width:360px;"
  "    background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>"
  "    <tr height='32' style='font-size:20px'>\r\n"
  "      <td style='width:15%;text-align:left'><input type=button class=btn0 onclick='history.go(-1)' value='<'></td>\r\n"
  "      <td style='width:70%;text-align:center'><b>Configurar LOG</b></td>\r\n"
  "      <td style='width:15%' align='right'><input type=button class=btn0 onclick='salvar()' value='OK'></td>\r\n"
  "    </tr>\r\n"
  "  </table>\r\n"
  "  <table border='0' cellspacing='0' cellpadding='0' style='width:100%;max-width:360px;'>\n"
  "   <tr><td style='text-align:center;padding:8px;'>\n"
  "     <div class='divbl' align='center'>"
  
  "  <table><tr><td align=left>\n"
  "  <label><input type='checkbox' id='Lumi'> Luminosidade</label><br>"
  "  <label><input type='checkbox' id='Temp'> Temperatura</label><br>"
  "  <label><input type='checkbox' id='Umi'> Umidade</label><br>"
  "  <label><input type='checkbox' id='Sensores'> Sensores</label><br>"
  "  <label><input type='checkbox' id='Reles'> Reles</label><br>"
  "  </td></tr></table>\n"
  
  "     </div>\r\n"
  "  </td></tr></table>\n"
  "</div></body></html>\r\n"
  );
  server.send(200, "text/html", pg);
}

