void handleMaisLayout(){
  String user = autenticar(); if(user.length()<=0) return server.requestAuthentication();
  String pg;
  pg=F(
  "<!-- maislay.htm -->\n"
  "<html><head>\n"
  "<meta name='viewport' content='width=device-width, initial-scale=1'>\n"
  "<meta http-equiv='Content-Type' content='text/html; charset=utf-8'>\n"
  "<title>");pg+=DeviceID;pg+=F("</title>\n"
  "<style type=\"text/css\">\n"
  "  body{margin:0;\n"
  "  background-repeat: no-repeat;\n"
  " background-position: 0px 0px;\n"
  " }\n"
  "\n"
  "  html, input, button {font: 16px Verdana, Arial, Helvetica, sans-serif;}\n"
  "  .btn0{border-radius:6px;width:42px;height:32px}\n"
  "  .btn2{\n"
  "    position:absolute;\n"
  "    padding:0px;\n"
  "    margin:0px;\n"
  "    width:177px;\n"
  "    border-radius:12px;vertical-align:middle; font-size:16px;\n"
  "    background-color:#ccccff;\n"
  "  }\n"
  "</style>\n"
  "<script type=\"text/javascript\">\n"
  "document.oncontextmenu = function(){return false;};\n"
  "var ie=document.all;\n"
  "var nn6=document.getElementById&&!document.all;\n"
  "var isdrag=false;\n"
  "var x,y;\n"
  "var dobj;\n"
  " \n"
"function movemouse(e){\n"
"  if (isdrag){\n"
"    dobj.style.left = nn6 ? tx + e.clientX - x : tx + event.clientX - x;\n"
"    dobj.style.top  = nn6 ? ty + e.clientY - y : ty + event.clientY - y;\n"
"    return false;\n"
"  }\n"
"}\n"
" \n"
"function selectmouse(e){\n"
"  var fobj       = nn6 ? e.target : event.srcElement;\n"
"  var topelement = nn6 ? \"HTML\" : \"BODY\";\n"
"  while (fobj.tagName != topelement && fobj.className != \"btn2\"){\n"
"    fobj = nn6 ? fobj.parentNode : fobj.parentElement;\n"
"  }\n"
" \n"
"  if (fobj.className==\"btn2\"){\n"
"    isdrag = true;\n"
"    dobj = fobj;\n"
"    tx = parseInt(dobj.style.left+0);\n"
"    ty = parseInt(dobj.style.top+0);\n"
"    x = nn6 ? e.clientX : event.clientX;\n"
"    y = nn6 ? e.clientY : event.clientY;\n"
"    document.onmousemove=movemouse;\n"
"    return false;\n"
"  }\n"
"}\n"
" \n"
"document.onmousedown=selectmouse;\n"
"document.onmouseup=alinhar;\n"
"function alinhar(e){\n"
"  if(!isdrag) return false;;\n"
"  isdrag=false;\n"
"  px=dobj.style.left;px=px.substring(0,px.indexOf('px'));px++;px--;\n"
"  py=dobj.style.top;py=py.substring(0,py.indexOf('px'));py++;py--;\n"
"  px-=2;\n"
"  py-=46;\n"
"  px=Math.round((px-5)/(180/4)) * (180/4);\n"
"  py=Math.round((py-5)/((btnHeight+2)/4)) * ((btnHeight+2)/4);\n"
"  px+=2;\n"
"  py+=46;\n"
"  if(px<2) px=2;\n"
"  if(py<46) py=46;\n"
"  dobj.style.left = px+'px'; dobj.style.top = py+'px';\n"
"  salvarPos();\n"
"}\n"

"function salvarPos(){\n"
" sa='';sb='';\n"
" for(i in lista){\n"
"   sa+=btn[i].style.left+';';\n"
"   sb+=btn[i].style.top+';';\n"
" }\n"
" localStorage.setItem('listaPosX', sa);\n"
" localStorage.setItem('listaPosY', sb);\n"
"}\n"
"//  Move Fim -->\n"
"\n"

"function iniciar(){\n"
"  divBody.style.height=document.body.clientHeight;\n"
"  if(document.body.clientWidth>600){\n"
"    dbch=document.body.clientHeight;\n"
"    sa=document.body.clientWidth+'px '+dbch+'px';\n"
"    document.body.style.backgroundSize=sa;\n"
"    document.body.style.backgroundImage=\"url(640.jpg)\";\n"
"    //tableHeader.style.visibility='hidden';\n"
"  }else{\n"
"    tableHeader.style.visibility='visible';\n"
"  }\n"
"  init();\n"
"  \n"
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
"    i=0;\n"
"    while(sa.indexOf(';')>=0){\n"
"      sb=sa.substring(0,sa.indexOf(';'));\n"
"      sa=sa.substring(sa.indexOf(';')+1);\n"
"      lista[lista.length]=sb;\n"
"      DeviceID=sb.substring(0,sb.indexOf(','));\n"
"      sb=sb.substring(sb.indexOf(',')+1);\n"
"      Link=sb.substring(0,sb.indexOf(','));\n"
"      lista2[i]=Link;\n"
"      i++;\n"
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

"   for(i in lista){\n"
"     sa=lista[i]; DeviceID=sa.substring(0,sa.indexOf(','));\n"
"     Link=lista2[i];\n"
"     btn[i] = document.createElement(\"BUTTON\");\n"
"     document.body.appendChild(btn[i]);\n"
"     btn[i].className ='btn2';\n"
"     btn[i].innerHTML='<b>'+DeviceID+'</b>';\n"
"     if(listaPosX[i]<'0') listaPosX[i]='0';\n"
"      if(listaPosY[i]<0) listaPosY[i]='0';\n"
"      btn[i].style.left = listaPosX[i];\n"
"      btn[i].style.top = listaPosY[i]+'px';\n"
"      btn[i].style.height = btnHeight+'px';\n"
"    }\n"
"  }\n"
"}\n"
"\n"
"function touchHandler(event){\n"
"    var touches = event.changedTouches,\n"
"        first = touches[0],\n"
"        type = \"\";\n"
"    switch(event.type){\n"
"        case \"touchstart\": type = \"mousedown\"; break;\n"
"        case \"touchmove\":  type = \"mousemove\"; break;        \n"
"        case \"touchend\":   type = \"mouseup\";   break;\n"
"        default: return;\n"
"    }\n"
"    var simulatedEvent = document.createEvent(\"MouseEvent\");\n"
"    simulatedEvent.initMouseEvent(\n"
"   type, true, true, window, 1, \n"
"   first.screenX, first.screenY, \n"
"   first.clientX, first.clientY, false, \n"
"   false, false, false, 0/*left*/, null);\n"
"\n"
"    first.target.dispatchEvent(simulatedEvent);\n"
"    event.preventDefault();\n"
"}\n"
"\n"
"function init(){\n"
"    document.addEventListener(\"touchstart\", touchHandler, true);\n"
"    document.addEventListener(\"touchmove\", touchHandler, true);\n"
"    document.addEventListener(\"touchend\", touchHandler, true);\n"
"    document.addEventListener(\"touchcancel\", touchHandler, true);    \n"
"}\n"
"</script>\n"
"\n"
" \n"
"</head>\n"
"<body onload='iniciar()'>\n"
"<div id='divBody'>\n"
"<table id='tableHeader' border='0' cellspacing='6' cellpadding='0' style='background-color:");pg+=backgroundColor;pg+=F(";color:");pg+=Color1;pg+=F(";width:100%;'>\n"
"    <tr height='32' style='font-size:20px'>\n"
"      <td style='width:15%;text-align:left;'></td>\n"
"      <td style='width:70%;text-align:center;'><b id=bHeader>Layout</b></td>\n"
"      <td style='width:15%' align='right'></td>\n"
"    </tr>\n"
"</table>\n"
"\n"
"</div>\n"
"</body>\n"
"</html>\n"
"\n"


    
  );
  server.send(200, "text/html", pg);
}
