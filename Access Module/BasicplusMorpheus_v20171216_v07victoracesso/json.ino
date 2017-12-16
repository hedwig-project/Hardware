void handleJSON(){
  String pg;
  String sa, sb;
  sa=F("log"); if(server.hasArg(sa)){
    unsigned long ia=server.arg(sa).toInt();
    pg="";
    sb=F("ChipId"); if(sb==server.arg(sa)){
      sa=F("00000000");
      sa+=String(ESP.getChipId(),HEX);
      sa=sa.substring(sa.length()-8);
      sa.toUpperCase();
      pg+=F("ChipId: "); pg+=sa;
      pg+=F(","); pg+=DeviceID;
      server.send(200, "text/plain", pg);
      return;
    }
    if(timeStatus()!=timeNotSet){
      //
      int i=0;
      while(log_str.indexOf(";",i)>=0){
        String sc=log_str.substring(i,log_str.indexOf(";",i));
        unsigned long ib=sc.substring(0,sc.indexOf(" ")).toInt();
        if(ib>ia) break;
        i=log_str.indexOf(";",i)+1;
      }
      //
      server.send(200, "text/plain", log_str.substring(i));
      return;
    }
  }
  
  String cmd;
  bool reinicia=false;
  pg=F("{");  
  sa=F("time"); if(server.hasArg(sa)) wsTimeSet(server.arg(sa));          // time
  sa=F("addCode"); if(server.hasArg(sa)) { addCode=server.arg(sa).toInt(); lastRFcode = ""; addCodeCnt = 150; } // 100 = 10 segundos
  sa=F("clrCode"); if(server.hasArg(sa)) { RFcode[server.arg(sa).toInt()] = ""; addCode=-1; addCodeCnt = 0; }
  // ----- cmd
  sa=F("cmd"); if(server.hasArg(sa)) cmd=server.arg(sa);
  // -----
  String fields;
  fields=F(
  "DeviceID,ChipId,hhmmss,ddmmaaaa,Temperatura,Umidade,Lumi,Sensores"
  ",Lamps,ReleTempoDesl0,ReleTempoDesl1,ReleCnt0,ReleCnt1"
  ",mqtt,BlynkConnErrCnt,WiFi,Board,Versao,millis"
  ",BackgroundColor,Color1,Color2,timeStatus"
  ",BtnLabel0,BtnLabel1,BtnLabel"
  ",lcdMode,backlight,APmode,ipMode");
  sa=F("fields"); if(server.hasArg(sa)){fields = server.arg(sa);}
  fields=","+fields+",";
  //ESP.getFlashChipId()
  if(fields.indexOf(F(",DeviceID,"))>=0){pg+=F("\"DeviceID\":\"");pg+=DeviceID;pg+=F("\",");}
  sa=F("00000000"); sa+=String(ESP.getChipId(),HEX);sa=sa.substring(sa.length()-8);sa.toUpperCase();
  if(fields.indexOf(F(",ChipId,"))>=0){pg+=F("\"ChipId\":\"");pg+=sa;pg+=F("\",");}
  if(fields.indexOf(F(",hhmmss,"))>=0){ pg += F("\"hhmmss\":\""); pg += hhmmss(); pg += F("\","); }
  if(fields.indexOf(F(",ddmmaaaa,"))>=0){ pg += F("\"ddmmaaaa\":\""); pg += ddmmaaaa(); pg += F("\","); }
  if(fields.indexOf(F(",Temperatura,"))>=0){ pg += F("\"Temperatura\":"); pg+=String(Temperatura,1); pg+=F(","); }
  if(fields.indexOf(F(",Umidade,"))>=0){ pg += F("\"Umidade\":"); pg += String(Umidade,0); pg += F(","); }
  if(fields.indexOf(F(",Lumi,"))>=0){ pg += F("\"Lumi\":\""); pg += String(Lumi); pg += F("\","); }
  if(fields.indexOf(F(",Sensores,"))>=0){
    for(byte i=1;i<=Sensores;i++){
      pg+=F("\"Sensor");pg+=String(i);pg+=F("\":"); pg += String(Sensor[i-1]); pg += F(",");
    }
  }
  if(fields.indexOf(F(",Lamps,"))>=0){ for(byte i=1;i<=Reles;i++){ pg += F("\"Lamp");pg+=String(i);pg+=F("\":"); pg += String(Rele[i-1]); pg += F(","); } }
  if(fields.indexOf(F(",ReleTempoDesl0,"))>=0){pg+=F("\"ReleTempoDesl0\":");pg+=ReleTempoDesl[0];pg+=F(",");}
  if(fields.indexOf(F(",ReleTempoDesl1,"))>=0){pg+=F("\"ReleTempoDesl1\":");pg+=ReleTempoDesl[1];pg+=F(",");}
  if(fields.indexOf(F(",ReleCnt0,"))>=0){pg+=F("\"ReleCnt0\":\"");pg+=secToDMHS(ReleCnt[0]/10);pg+=F("\",");}
  if(fields.indexOf(F(",ReleCnt1,"))>=0){pg+=F("\"ReleCnt1\":\"");pg+=secToDMHS(ReleCnt[1]/10);pg+=F("\",");}
  if(fields.indexOf(F(",BlynkConnErrCnt,"))>=0){ if(blynkAuth.length()>0) { pg += F("\"BlynkConnErrCnt\":"); pg +=String(BlynkConnErrCnt); pg += F(","); } }
  if(fields.indexOf(F(",WiFi,"))>=0){ if(WiFi.status() == WL_CONNECTED){pg += F("\"WiFi\":"); pg += String(WiFi.RSSI()); pg += F(",");} }
  if(fields.indexOf(F(",Board,"))>=0){ pg += F("\"Board\":\""); pg += Board; pg += F("\","); }
  if(fields.indexOf(F(",BackgroundColor,"))>=0){ pg += F("\"BackgroundColor\":\""); pg+=backgroundColor; pg += F("\","); }
  if(fields.indexOf(F(",Color1,"))>=0){pg += F("\"Color1\":\""); pg+=Color1; pg += F("\",");}
  if(fields.indexOf(F(",Color2,"))>=0){pg += F("\"Color2\":\""); pg+=Color2; pg += F("\",");}
  if(fields.indexOf(F(",timeStatus,"))>=0){ if(timeStatus() == timeNotSet){pg+=F("\"timeStatus\":");pg+=String(timeStatus());pg+=F(",");  }}
  #ifdef LCD_SCL
  if(fields.indexOf(F(",lcdMode,"))>=0){ pg+=F("\"lcdMode\":"); pg+=lcdMode; pg+=F(","); }
  if(fields.indexOf(F(",backlight,"))>=0){ pg+=F("\"backlight\":"); pg+=backlight; pg+=F(","); }
  #endif
  //
  if(fields.indexOf(F(",essid,"))>=0){pg+=F("\"essid\":\"");pg+=essid;pg+=F("\",");}
  //if(fields.indexOf(F(",wifiscan,"))>=0) pg+=wifiscan();
  if(AutoResetTestCnt>0) { pg+= F("\"AutoResetTestCnt\":"); pg+=String(AutoResetTestCnt-1); pg+=F(","); }
  //
  if(fields.indexOf(F(",rfcodes,"))>=0){
    pg+=F("\"addCode\":");pg+=String(addCode);pg+=F(",");
    pg+=F("\"addCodeCnt\":");pg+=String(addCodeCnt/10);pg+=F(",");
    for(byte i=0;i<10;i++){
      if(RFcodeTitle[i].length()<1) break;
      pg+=F("\"RFcode");pg+=String(i);pg+=F("\":\"");pg+=RFcode[i];pg+=F("\",");
    }
  }
  if(addCode==-2)addCode=-1;
  if(addCode==-3)addCode=-1;
  //
  if(fields.indexOf(F(",APmode,"))>=0){ pg+=F("\"APmode\":"); pg+=APmode; pg+=F(","); }
  if(fields.indexOf(F(",ipMode,"))>=0){ pg+=F("\"ipMode\":"); pg+=ipMode; pg+=F(","); }
  // ----- LOG
  if(fields.indexOf(F(",log,"))>=0 && timeStatus()!=timeNotSet){ pg+=F("\"log\":\""); pg+=log_str; pg+=F("\","); }
  /*/
  sa=F("log"); if(server.hasArg(sa)){
    msg=F("log="); msg+=server.arg(sa); addLog(msg);
    pg+=F("\"log\":\""); pg+=log_str; pg+=F("\",");
  }
  /*/
  if(fields.indexOf(F(",BtnLabel0,"))>=0){pg+=F("\"BtnLabel0\":\"");pg+=BtnLabel[0];pg+=F("\",");}
  if(fields.indexOf(F(",BtnLabel1,"))>=0){pg+=F("\"BtnLabel1\":\"");pg+=BtnLabel[1];pg+=F("\",");}
  if(fields.indexOf(F(",Reles,"))>=0){
    pg+=F("\"Reles\":[");
    pg+=Rele[0]; pg+=F(",");
    pg+=Rele[1]; pg+=F("],");
  }
  if(fields.indexOf(F(",Sensores2,"))>=0){
    pg+=F("\"Sensores2\":[");
    for(byte i=0;i<Sensores;i++){
      pg += String(Sensor[i]);
      if(i<(Sensores-1)){pg += F(",");}else{pg += F("],");}
    }
  }
  if(fields.indexOf(F(",BtnLabel,"))>=0){
    pg+=F("\"BtnLabel\":[\"");
    pg+=BtnLabel[0]; pg+=F("\",\"");
    pg+=BtnLabel[1]; pg+=F("\"],");
  }
  // -----
  if(fields.indexOf(F(",Versao,"))>=0){ pg += F("\"Versao\":\""); pg += Versao; pg += F("\","); }
  if(fields.indexOf(F(",millis,"))>=0){ pg+=F("\"millis\":\""); pg+=secToDMHS(millis()/1000); pg+=F("\","); }  
  if(fields.indexOf(F(",FreeHeap,"))>=0){ pg+=F("\"FreeHeap\":"); pg+=ESP.getFreeHeap(); pg+=F(","); }
  if(minFreeHeap>ESP.getFreeHeap()) minFreeHeap=ESP.getFreeHeap();
  if(fields.indexOf(F(",minFreeHeap,"))>=0){ pg+=F("\"minFreeHeap\":"); pg+=minFreeHeap; pg+=F(","); }
  // -----
  if(pg.substring(pg.length()-1)==",") pg = pg.substring(0,pg.length()-1); pg += F("}");
  server.send(200, "text/plain", pg); server.handleClient();
  if(reinicia) ESP.reset();
}
