void preset(){
  Versao=F("BasicplusMorpheus_v20171105u");
  Board = F("Katz-House");
  
  eepromId = F("B029"); // usar somente 4 caracteres
  DeviceID = F("CONFIG");
  apPass = F("12345678");
  APmode = 1; // 0=sempre 1=auto
  ipMode = 20; // ip 30
  users = F("admin,1234;user,1234;");
  blynkAuth = ""; //"0ff72829b44c42fe8f4acbec2e084908";
  BlynkNotifyIni=true;
  BlynkNotifyWiFi=true;
  #ifdef LCD_SCL
    lcdMode=0;
    backlight=2;
  #endif
  for(byte i=0;i<10;i++) RFcode[i]="";
  backgroundColor=F("#0000FF");
  Color1=F("#FFFFFF");
  Color2=F("#FF0000");
  essid=""; epass="";
  BtnLabel[0]=F("Rele 1");
  BtnLabel[1]=F("Rele 2");
  ReleTempoDesl[0]=0; ReleTempoDesl[1]=0; // décimos de segundos
  ReleAuto[0]=0; ReleAuto[1]=0;
  ReleHoraIni[0]=0; ReleHoraFim[0]=0;
  ReleHoraIni[1]=0; ReleHoraFim[1]=0;
  ReleSenha[0]=""; ReleSenha[1]="";
  for(byte i=0;i<2;i++){
    AlarmInp[i] = 0;
    AlarmSeg[i] = 0;
    AlarmNtf[i] = 0;
  }
  UmiOffset = 0.0; // jarinu 25.0
  TempOffset = 0.0;
  for(byte i=0;i<2;i++) AlarmEml[i]=0;
  Sensores=2;
  Reles=2;
  RFcodeTitle[0]=F("Rele 1 L/D");
  RFcodeTitle[1]=F("Rele 2 L/D");
  RFcodeTitle[2]=F("Sensor2 Abre");
  RFcodeTitle[3]=F("Sensor2 Fecha");
  //essid=F("YTD960495veredas"); epass=F("784257hue031");
  //BROKER_MQTT = F("test.mosquitto.org");
  //
  ntpServerName[0] = F("us.pool.ntp.org");
  ntpServerName[1] = F("time.nist.gov");
  ntpServerName[2] = F("time-a.timefreq.bldrdoc.gov");
  ntpServerName[3] = F("time-b.timefreq.bldrdoc.gov");
  ntpServerName[4] = F("time-c.timefreq.bldrdoc.gov");
  //
  myipServer[0]="api.ipify.org"; myipReq[0]="/?format=json";
  myipServer[1]="freegeoip.net"; myipReq[1]="/json/";
  myipServer[2]="myexternalip.com"; myipReq[2]="/json";
  //
  
//  sendLogURL="179.212.163.32"; sendLogURL="";
//  sendLogPort=8008;
  log_opc=",Lumi,Temp,Umi,Sensores,Reles,";
  reenviaremail=1; // 1 hora
}

#ifdef Btn2Pin
void VerificaBotao2(){ // a cada 200ms
  pinMode(LedPin, INPUT);
  delay(50);
  //Btn2 = !digitalRead(LedPin);
  if(Btn2_wait>0){
    Btn2_wait--;
  }else{
    bool ba=!digitalRead(LedPin);
    if(Btn2!=ba){
      Btn2=ba;
      Btn2_wait=1;
    }
  }
  pinMode(LedPin, OUTPUT);
}
#endif

void verificarEntradas(){ // a cada 100ms
  if(PresencaCnt>0) PresencaCnt--;
  if(digitalRead(InfraVermelhoPin)) PresencaCnt=600; // 1 min
  Sensor[0]=(PresencaCnt>0);
  #ifdef Mini
    Btn1=digitalRead(Entrada1Pin);
    Btn2=digitalRead(Entrada2Pin);
  #endif
}

void VerificaEstado(){ //loop
  // ----- Sensores
  for(byte s=0;s<Sensores;s++){
    if(prevSensor[s]!=Sensor[s]){
      //
      if(log_opc.indexOf(",Sensores")>=0){
        if (Sensor[s]==1){
          msg= F("S"); msg += s + 1; msg += F(" on"); addLog(msg);
          if (s==1) eventoAbertura = "1";
        } else{
          msg= F("S"); msg += s + 1; msg += F(" off"); addLog(msg);
          if (s==1) eventoAbertura = "0";
        }
      }
      envioRapido = 1;
      //
      prevSensor[s]=Sensor[s];
    }
    // ----- Auto Liga
    if(Sensor[s]){
      time_t t = now(); timeZoneChk(); t += timeZone * SECS_PER_HOUR;
      for(byte r=0;r<2;r++){
        if(ReleAuto[r]==(s+1) && ReleAutoWait[r]==0 && ReleTempoDesl[r]>=600){
          bool ativar=false;
          if(ReleHoraIni[r]<ReleHoraFim[r]){
            if(hour(t) >= ReleHoraIni[r] && hour(t) < ReleHoraFim[r]) ativar = true;
          }else{
            if(hour(t) >= ReleHoraIni[r] || hour(t) < ReleHoraFim[r]) ativar = true;
          }
          if(ReleHoraIni[r] == ReleHoraFim[r]) ativar = true;
          if (timeStatus() == timeNotSet) ativar = true;
          if(ativar){
            if(!Rele[r]){
              msg = F("Auto"); msg += String(r+1); msg += F(" acionado"); addLog(msg);
            }
            Rele[r]=true;
            ReleCnt[r]=ReleTempoDesl[r]+1;
          }
        }
      }
    }
  }
  // ----- Botões
  if(prevBtn1!=Btn1){
    if(!prevBtn1 && Btn1){
      Rele[0]=!Rele[0];
      addLog(F("Botao1 acionado"));
    }
    prevBtn1=Btn1;
    ReleAutoWait[0]=1200;
  }
  if(prevBtn2!=Btn2){
    if(!prevBtn2 && Btn2){
      Rele[1]=!Rele[1];
      addLog(F("Botao2 acionado"));
    }
    prevBtn2=Btn2;
    ReleAutoWait[1]=1200;
  }
  // ----- Reles -----
  for(byte i=0;i<Reles;i++){
    #ifdef pinRF_TX
    if(!prevRele[i]&&Rele[i]){
      if(RFcode[i].length()>0){
        unsigned int i = RFcode[i].toInt();
        ht6p20bTX(i);
      }
    }
    #endif
    if(prevRele[i]!=Rele[i]){
      prevRele[i]=Rele[i];
      if(Rele[i]&&(ReleTempoDesl[i]>0)){ReleCnt[i]=ReleTempoDesl[i];}else{ReleCnt[i]=0;}
      if(i==0) digitalWrite(Rele01Pin,Rele[i]);
      if(i==1) digitalWrite(Rele02Pin,Rele[i]);
      if(log_opc.indexOf(",Reles,")>=0){
        msg=F("Rele");msg+=String(i+1);msg+=F(": ");msg+=Rele[i];addLog(msg);
      }
    }
  }
  #ifdef LCD_SCL
  if(prev_backlight!=backlight){
    prev_backlight=backlight;
    if(backlight==0) lcd.setBacklight(false);
    if(backlight==1) lcd.setBacklight(true);
  }
  #endif

  // ----- Time Status
  time_t t = now(); timeZoneChk(); t += timeZone * SECS_PER_HOUR;
  if(prevTimeStatus!=timeStatus()){
    prevTimeStatus=timeStatus();
    if(timeStatus() != timeNotSet) {
      prevHour=hour(t);
      //CheckReleAutoSim();
    }
  }
  if(prevHour!=hour(t) & timeStatus()!=timeNotSet){
    //Serial.println(prevHour);
    if(ReleAuto[0]==255){
      if(prevHour<ReleHoraIni[0] && hour(t)>=ReleHoraIni[0]) Rele[0]=true;
      if(prevHour<ReleHoraFim[0] && hour(t)>=ReleHoraFim[0]) Rele[0]=false;
    }
    if(ReleAuto[1]==255){
      if(prevHour<ReleHoraIni[1] && hour(t)>=ReleHoraIni[1]) Rele[1]=true;
      if(prevHour<ReleHoraFim[1] && hour(t)>=ReleHoraFim[1]) Rele[1]=false;
    }
    prevHour=hour(t);
  }

  //Lumi, prevLumi
  int ia=prevLumi-Lumi;
  if(abs(ia)>100){
    if(log_opc.indexOf(",Lumi,")>=0){msg=F("Lumi=");msg+=Lumi;addLog(msg);}
    prevLumi=Lumi;
  }
  //float Umidade, Temperatura; float prevUmidade, prevTemperatura;
  ia=prevTemperatura-Temperatura;
  if(abs(ia)>0){
    prevTemperatura=Temperatura;
    if(log_opc.indexOf(",Temp,")>=0){msg=F("Temp=");msg+=String(Temperatura,1);addLog(msg);}
  }
  ia=prevUmidade-Umidade;
  if(abs(ia)>5){
    if(log_opc.indexOf(",Umi,")>=0){msg=F("Umid=");msg+=String(Umidade,0);addLog(msg);}
    prevUmidade=Umidade;
  }
  // ----- User
  if(prevUser!=User){
    prevUser=User;
    msg=F("User = ");msg+=User;addLog(msg);
  }
}

