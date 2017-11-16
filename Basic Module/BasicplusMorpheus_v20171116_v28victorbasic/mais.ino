void handleMais(){
  String pg;
  String fields,sa;
  sa=F("email"); if(server.hasArg(sa)){
    sa=server.arg(sa); if(sa=="ok"){
      if(BlynkEmailBuffer.length()>0){
        BlynkEmailBuffer="";
        BlynkEmailWaitSecs=0;
        msg=F("email=ok");addLog(msg);
        pg=F("<html><meta name='viewport' content='width=device-width, initial-scale=1'>\n"
          "<body onload='setTimeout(function(){location.reload();}, 3000);'>OK</body></html>\n");
        server.send(200, "text/html", pg);
        return;
      }
    }
  }
  sa=F("Rele"); if(server.hasArg(sa)){
    byte RLn=server.arg(sa).toInt();
    pg="";
    if(ReleSenha[RLn].length()>0){
      pg+=F("{\"mensagem\":\"Senha\",\"href\":\"");
      //pg+=server.client().remoteIP().toString();
      pg+=myip;
      pg+=F("/home\"}");
    }else{
      if(ReleTempoDesl[RLn]<=100&&ReleTempoDesl[RLn]>0){
        pg+=F("{\"mensagem\":\"Confimar\",\"href\":\"");
        //pg+=server.client().remoteIP().toString();
        pg+=myip;
        pg+=F("/home\"}");
      }
    }
    if(pg==""){
      pg=F("{\"mensagem\":\"OK\"}");
      Rele[RLn]=!Rele[RLn];
      ReleAutoWait[RLn]=1200;
      msg=F("Web"); msg+=String(RLn+1); msg+=F(" ("); msg+=User; msg+=F(") acionado"); addLog(msg);
    }
    server.send(200, "text/plain", pg);
    return;
  }
  
  String user = autenticar(); if(user.length()<=0) return server.requestAuthentication();
  
  sa=F("CONFIG");
  if(DeviceID==sa){
    if(WiFi.status()==WL_IDLE_STATUS){handleWiFiCon();return;}
    handleDeviceID();return;
  }
  if(server.client().localIP()==WiFi.softAPIP()){handleHome();return;}
     
  fields=F("Temperatura,Umidade,Lumi,Reles,Sensores2,Versao");
  sa=F("fields"); if(server.hasArg(sa)){fields=server.arg(sa);}
  
  pg=F(
"<!-- index.htm -->\n"
"<html><head>\n"
"<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n"
"<link rel='icon' type='image/x-icon' href='favicon.ico'>\n"
"<title></title>\n"
"<style type=\"text/css\">\n"
"  body{margin:0; background-repeat: no-repeat; background-position: 0px 0px;}\n"
"  html, input, button {font: 16px Verdana, Arial, Helvetica, sans-serif;}\n"
" .btn0{border-radius:6px;width:42px;height:32px}\n"
" .btn2{\n"
"    position:absolute;\n"
"    padding:0px;\n"
"    margin:0px;\n"
"    width:177px;\n"
"    border-radius:12px;\n"
"    font-size:16px;\n"
"  }\n"
" .divbl{border-style:none;margin-bottom:0px;padding:0px;}\n"
" .btn3{\n"
"   height:28px;padding:0;width:46px;margin-top:4px;\n"
"   border-radius:6px;\n"
"   font-size:14px;background-color:#77F;color:#FFF;vertical-align:text-bottom}\n"
" .led2{padding:0;font-weight:bold;border: 1px solid gray;background-color:#77F;height:16px;width:16px;border-radius:8px;font-size:8px;color:#FFF;vertical-align:text-bottom}\n"
" p{margin:0;padding:0}\n"
"</style>\n"
"</head>\n"
"<body onLoad='iniciar()'>\n"
"<script type=\"text/javascript\">\n"
"//document.oncontextmenu = function(){return false;};\n"
"essid='';\n"
"var client2 = new XMLHttpRequest();\n"
"client2.onreadystatechange = function() {\n"
"    if(this.readyState==4){\n"
"     if(this.status==200){\n"
"       span02.innerHTML+=this.responseText+'<br>';\n"
"       var myObj=JSON.parse(this.responseText);\n"
"     }\n"
"   }\n"
" }\n"
"\n"
"function btnRele(rln,lnk,i){\n"
"  url=lnk+'/?Rele='+rln;\n"
"  client2.open('GET',url, true);\n"
"  client2.timeout=2000;\n"
"  client2.send();\n"
"}\n"
"var client = new XMLHttpRequest();\n"
"client.onreadystatechange = function() {\n"
" if (this.readyState==4){\n"
"   sb=lista[cnt]; DeviceID=sb.substring(0,sb.indexOf(','));\n"
"   BackgroundColor='silver'; Color1='white'; Color2='red';\n"
"   Temperatura=''; Umidade=''; Lumi='';\n"
"   Link=lista2[cnt];\n"
"   if(this.status==200){\n"
"      var myObj=JSON.parse(this.responseText);\n"
"      if(typeof myObj.essid!='undefined' && essid=='') {\n"
"        essid=myObj.essid;"
"        if(document.body.clientWidth<=6000){bHeader.innerHTML=essid;}\n"
"        document.title=essid;\n"
"      }\n"
"      if(typeof myObj.DeviceID!='undefined') DeviceID=myObj.DeviceID;\n"
"      if(typeof myObj.Temperatura!='undefined') Temperatura=myObj.Temperatura;\n"
"      if(typeof myObj.Umidade!='undefined') Umidade=myObj.Umidade;\n"
"      if(typeof myObj.Lumi!='undefined') Lumi=myObj.Lumi;\n"
"      if(typeof myObj.BackgroundColor!='undefined') BackgroundColor=myObj.BackgroundColor;\n"
"      if(typeof myObj.Color1!='undefined') Color1=myObj.Color1;\n"
"      if(typeof myObj.Color2!='undefined') Color2=myObj.Color2;\n"
"      pg+='<a onmousedown=\"location.href=\\\''+Link+'/home\\\';\">';\n"
"      li=0;\n"
"      if(fields.indexOf('DeviceID')>=0){\n"
"        pg+='<b><font style=\"font-size:18px\">'+DeviceID+'</font>';\n"
"        li++;\n"
"      }\n"
"     \n"
"  if(Temperatura!=''||Umidade!='') if(li>0) pg+='<br>';\n"
"  \n"
"     if(Temperatura!=''){\n"
"       pg+='<font style=\"font-size:18px\">'+Temperatura+'&deg;C';\n"
"       li++;\n"
"     }\n"
"     if(Umidade!='') {pg+=' '+Umidade+'%';\n"
"       li++;\n"
"     }   \n"
"     pg+='</font></b>';\n"
"  if(typeof myObj.Sensores2!='undefined'||Lumi>0) if(li>0) pg+='<br>';\n"
"     pg+='<font style=\"font-size:12px;line-height:20px\"><b>';\n"
"     \n"
// ----- Sensores
"     if(typeof myObj.Sensores2!='undefined'){\n"
"       for(i=0;i<myObj.Sensores2.length;i++){\n"
"         j=i+1;\n"
"         pg+='<input type=button class=led2 value='+j;\n"
          "bgc='#7777FF';"
          "if(myObj.Sensores2[i]>0.33) bgc='#FFFF00';"
          "if(myObj.Sensores2[i]>0.67) bgc='#FF0000';"
          "pg+=' style=background-color:'+bgc;"
"         pg+=';vertical-align:middle> ';\n"
"       }\n"
"       li++\n"
"     }\n"
// -----
"     if(Lumi>0){\n"
"       pg+=' <progress style=\"width:40%;height:10px;vertical-align:middle;\" min=0 max=1000 value='+Lumi+'>'+Lumi+'</progress>';\n"
"       li++;\n"
"     }\n"
"     pg+='</a>';\n"
"  BtnLabel=new Array();\n"
"  if(typeof myObj.BtnLabel!='undefined') BtnLabel=myObj.BtnLabel;\n"
"     if(typeof myObj.Reles!='undefined'){\n"
"        if(li>0) pg+='<br>';\n"
"       for(i=0;i<myObj.Reles.length;i++){\n"
"         pg+='<input type=button class=btn3 value=\"';\n"
"         sa=i+1;\n"
"         if(i<BtnLabel.length){           sa=BtnLabel[i];\n"
"             if(myObj.Reles.length>2) sa=sa.substring(0,3);\n"
"             sa=sa.substring(0,6);\n"
"            }\n"
"           pg+=sa;\n"
"           pg+='\" style=\"vertical-align:middle;';\n"
"           if(myObj.Reles.length<3) pg+='width:68px;';\n"
"           if(myObj.Reles[i]){\n"
"             pg+='background-color:#F55;\"';\n"
"             pg+='onmousedown=\"this.style.backgroundColor=\\\'#77F\\\';';\n"
"           }else{\n"
"             pg+='background-color:#77F;\"';\n"
"             pg+='onmousedown=\"this.style.backgroundColor=\\\'#F55\\\';';\n"
"           }\n"
"           pg+='btnRele('+i+',\\\''+Link+'\\\','+cnt+');\"';\n"
"           pg+='> ';\n"
"           li++;\n"
"         }\n"
"       }\n"
"       pg+='</b>';\n"
"       if(li>0) pg+='<br>';\n"
"       if(typeof myObj.Versao!='undefined'){ pg+='('+myObj.Versao+')'; }\n"
"       pg+='</p>';\n"
"     }else{\n"
"       pg+='<a onmousedown=\"location.href=\\\''+Link+'/home\\\';\">';\n"
"       pg+='<b>';\n"
"       pg+=DeviceID;\n"
"       pg+='</b><br>off-line</a>';\n"
"     }\n"
"     pg+='</font>';\n"
"     btn[cnt].style.color=Color1;\n"
"     btn[cnt].style.backgroundColor=BackgroundColor;\n"
"     btn[cnt].innerHTML=pg;\n"
"     pg='';\n"
"     cnt++;\n"
"     if(cnt<lista2.length){\n"
"       loadJson();\n"
"     }else{\n"
"       pg=\"\";\n"
"       cnt=0;\n"
"       loadJson();\n"
"     }\n"
"   }\n"
" }\n"
"\n"
"function loadJson(){\n"
" url=lista2[cnt]+'/json?fields=essid,BtnLabel,'+fields;\n"
" client.open('GET', url, true); client.timeout=2000; client.send();\n"
"}\n"

"function iniciar(){\n"
"  fields='DeviceID,BackgroundColor,Color1,Temperatura,Umidade,Lumi,Reles,Sensores2';\n"
"  if(localStorage.getItem('fields')){fields=localStorage.getItem('fields');}\n"
"  btnHeight=10;\n"
"  if(fields.indexOf('DeviceID')>=0) btnHeight+=24;\n"
"  if(fields.indexOf('Temperatura')>=0 || fields.indexOf('Umidade')>=0) btnHeight+=24;\n"
"  if(fields.indexOf('Sensores')>=0||fields.indexOf('Lumi')>=0) btnHeight+=22;\n"
"  if(fields.indexOf('Reles')>=0) btnHeight+=38;\n"
"  if(fields.indexOf('Versao')>=0) btnHeight+=20;\n"
"  \n"
"  lista=new Array();lista2=new Array();\n"
"  if(localStorage.getItem('lista')){\n"
"    sa = localStorage.getItem('lista');\n"
"    while(sa.indexOf(';')>=0){\n"
"      sb=sa.substring(0,sa.indexOf(';'));\n"
"      sa=sa.substring(sa.indexOf(';')+1);\n"
"      lista[lista.length]=sb;\n"
"      DeviceID=sb.substring(0,sb.indexOf(','));\n"
"      sb=sb.substring(sb.indexOf(',')+1);\n"
"      Link=sb.substring(0,sb.indexOf(','));\n"
"      sb=sb.substring(sb.indexOf(',')+1);\n"
"      bgc=sb.substring(0,sb.indexOf(','));\n"
"      boc=sb.substring(sb.indexOf(',')+1);\n"
"      lista2[lista2.length]=Link;\n"
"    }\n"
"    btn = new Array();\n"
"    px=2;py=46;\n"
"    listaPosX=new Array(); listaPosY=new Array();\n"
"    for(i in lista){\n"
"      listaPosX[i]=px+'px';\n"
"      listaPosY[i]=py;\n"
"      px+=180; if(px>182){px=2;py+=btnHeight+2;}\n"
"    }\n"

"if(localStorage.getItem('listaPosX')){\n"
"  sa=localStorage.getItem('listaPosX');\n"
"  i=0;\n"
"  while(sa.indexOf(';')>=0){\n"
"    listaPosX[i]=sa.substring(0,sa.indexOf(';'));\n"
"    i++;\n"
"    sa=sa.substring(sa.indexOf(';')+1);\n"
"  }\n"
"}\n"
"if(localStorage.getItem('listaPosY')){\n"
"  sa=localStorage.getItem('listaPosY');\n"
"  i=0;\n"
"  while(sa.indexOf('px;')>=0){\n"
"    listaPosY[i]=sa.substring(0,sa.indexOf('px;'));\n"
"    i++;\n"
"    sa=sa.substring(sa.indexOf('px;')+3);\n"
"  }\n"
"}\n"

"    for(i in lista){\n"
"   sa=lista[i]; DeviceID=sa.substring(0,sa.indexOf(','));\n"
"   Link=lista2[i];\n"
"   btn[i] = document.createElement(\"button\");\n"
"   document.body.appendChild(btn[i]);\n"
"   btn[i].className ='btn2';\n"
"   \n"
"   \n"
"   \n"
"   pg='<a onclick=location.href=location.href=\"'+Link+'/home\";><b>'+DeviceID;\n"
"   pg+='<br>off-line</b></a>';\n"
"   btn[i].innerHTML=pg;\n"
"   if(listaPosX[i]<'0') listaPosX[i]='0';\n"
"   if(listaPosY[i]<0) listaPosY[i]='0';\n"
"   btn[i].style.left = listaPosX[i];\n"
"   btn[i].style.top = listaPosY[i];\n"
"   btn[i].style.height = btnHeight+'px';\n"
"   btn[i].style.zIndex=i;\n"
"    }\n"
"  }else{\n"
"    span01.innerHTML='<p align=center><br><br><br>clique no botão [#] para iniciar busca automática de módulos.</p>'; \n"
"  }\n"
"  cnt=0;\n"
"  pg=\"\";\n"
"  loadJson();\n"
"}\n"
"</script>\n"
"<div id='divBody'><span id=span01></span></div>"
"<table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;position:absolute;top:0;left:0;z-index:100;'>\n"
"  <tr height='32' style='font-size:20px'>\n"
"      <td style='width:15%;text-align:left;'><input type='button' class='btn0' value='-' onclick='location.reload();'></td>\n"
"      <td style='width:70%;text-align:center;'><b id=bHeader>");pg+=essid;pg+=F("</b></td>\n"
"      <td style='width:15%' align='right'><input type='button' class='btn0' value='#' onclick='location.href=\"maiscnf\";'></td>\n"
"    </tr>\n"
"  </table>\n"
"</body>\n"
"</html>\n"
"\n"
  );
  server.send(200, "text/html", pg);
}

