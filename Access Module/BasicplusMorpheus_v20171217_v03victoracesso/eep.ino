void lerEeprom(){
  String sa = "";
  for (int i = 0; i <= 3; i++) sa += char(EEPROM.read(i));
  if (sa != eepromId) {
    for(int i=0;i<=511;i++) EEPROM.write(i, 0);
    EEPROM.commit();
    gravarEeprom();
  }
  int ia = 0;
  sa = eepromRead(ia); ia += sa.length(); ia++; eepromId = sa;
  sa = eepromRead(ia); ia += sa.length(); ia++; DeviceID = sa;
  sa = eepromRead(ia); ia += sa.length(); ia++; apPass = sa;
  sa = eepromRead(ia); ia += sa.length(); ia++; //
  sa = eepromRead(ia); ia += sa.length(); ia++; APmode = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; ipMode = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; users = sa;
  //
  sa = eepromRead(ia); ia += sa.length(); ia++; //mqtt
  sa = eepromRead(ia); ia += sa.length(); ia++; //mqtt
  sa = eepromRead(ia); ia += sa.length(); ia++; blynkAuth = sa;
  sa = eepromRead(ia); ia += sa.length(); ia++; BlynkNotifyIni = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; BlynkNotifyWiFi = sa.toInt();
  #ifdef LCD_SCL
  sa = eepromRead(ia); ia += sa.length(); ia++; lcdMode = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; backlight = sa.toInt();
  #else
  sa = eepromRead(ia); ia += sa.length(); ia++; //
  sa = eepromRead(ia); ia += sa.length(); ia++; //
  #endif
  for(byte i=0;i<10;i++) {sa = eepromRead(ia); ia += sa.length(); ia++; RFcode[i] = sa;}
  sa = eepromRead(ia); ia += sa.length(); ia++; backgroundColor = sa;
  sa = eepromRead(ia); ia += sa.length(); ia++; Color1 = sa;
  sa = eepromRead(ia); ia += sa.length(); ia++; Color2 = sa;
  sa = eepromRead(ia); ia += sa.length(); ia++; essid = sa;
  sa = eepromRead(ia); ia+=sa.length(); ia++; epass =sa;
  sa = eepromRead(ia); ia+=sa.length(); ia++; BtnLabel[0] = sa;
  sa = eepromRead(ia); ia+=sa.length(); ia++; BtnLabel[1] = sa;
  sa = eepromRead(ia); ia += sa.length(); ia++; ReleTempoDesl[0] = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; ReleTempoDesl[1] = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; ReleAuto[0] = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; ReleAuto[1] = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; ReleHoraIni[0] = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; ReleHoraIni[1] = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; ReleHoraFim[0] = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; ReleHoraFim[1] = sa.toInt();
  sa = eepromRead(ia); ia+=sa.length(); ia++; ReleSenha[0] = sa;
  sa = eepromRead(ia); ia+=sa.length(); ia++; ReleSenha[1] = sa;
  for(byte i=0;i<2;i++){
    sa = eepromRead(ia); ia += sa.length(); ia++; AlarmInp[i] = sa.toInt();
    sa = eepromRead(ia); ia += sa.length(); ia++; AlarmSeg[i] = sa.toInt();
    sa = eepromRead(ia); ia += sa.length(); ia++; AlarmNtf[i] = sa.toInt();
  }
  sa = eepromRead(ia); ia += sa.length(); ia++; UmiOffset = sa.toFloat();
  sa = eepromRead(ia); ia += sa.length(); ia++; TempOffset = sa.toFloat();
  sa = eepromRead(ia); ia+=sa.length(); ia++; log_opc = sa;
  sa = eepromRead(ia); ia += sa.length(); ia++; AlarmEml[0] = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; AlarmEml[1] = sa.toInt();
  sa = eepromRead(ia); ia += sa.length(); ia++; reenviaremail = sa.toInt();
  DeviceID=limparPalavra(DeviceID);
  msg=F("EEPROM: lendo "); msg+=String(ia); msg+=F(" bytes"); addLog(msg);
}


void gravarEeprom(){
  int ia = 0;
  ia = eepromWrite(ia, eepromId);
  ia = eepromWrite(ia, DeviceID);
  ia = eepromWrite(ia, apPass);
  ia = eepromWrite(ia, " ");
  ia = eepromWrite(ia, String(APmode));
  ia = eepromWrite(ia, String(ipMode));
  ia = eepromWrite(ia, users);
  ia = eepromWrite(ia, " ");
  ia = eepromWrite(ia, " ");
  ia = eepromWrite(ia, blynkAuth);
  ia = eepromWrite(ia, String(BlynkNotifyIni));
  ia = eepromWrite(ia, String(BlynkNotifyWiFi));
  #ifdef LCD_SCL
  ia = eepromWrite(ia, String(lcdMode));
  ia = eepromWrite(ia, String(backlight));
  #else
  ia = eepromWrite(ia, "1");
  ia = eepromWrite(ia, "1");
  #endif
  for(byte i=0;i<10;i++) ia = eepromWrite(ia, RFcode[i]);
  ia = eepromWrite(ia, backgroundColor);
  ia = eepromWrite(ia, Color1);
  ia = eepromWrite(ia, Color2);
  ia = eepromWrite(ia, essid);
  ia = eepromWrite(ia, epass);
  ia = eepromWrite(ia,BtnLabel[0]);
  ia = eepromWrite(ia,BtnLabel[1]);
  ia = eepromWrite(ia, String(ReleTempoDesl[0]));
  ia = eepromWrite(ia, String(ReleTempoDesl[1]));
  ia = eepromWrite(ia, String(ReleAuto[0]));
  ia = eepromWrite(ia, String(ReleAuto[1]));
  ia = eepromWrite(ia, String(ReleHoraIni[0]));
  ia = eepromWrite(ia, String(ReleHoraIni[1]));
  ia = eepromWrite(ia, String(ReleHoraFim[0]));
  ia = eepromWrite(ia, String(ReleHoraFim[1]));
  ia = eepromWrite(ia,ReleSenha[0]);
  ia = eepromWrite(ia,ReleSenha[1]);
  for(byte i=0;i<2;i++){
    ia = eepromWrite(ia, String(AlarmInp[i]));
    ia = eepromWrite(ia, String(AlarmSeg[i]));
    ia = eepromWrite(ia, String(AlarmNtf[i]));
  }
  ia = eepromWrite(ia, String(UmiOffset));
  ia = eepromWrite(ia, String(TempOffset));
  ia = eepromWrite(ia, log_opc);
  ia = eepromWrite(ia, String(AlarmEml[0]));
  ia = eepromWrite(ia, String(AlarmEml[1]));
  ia = eepromWrite(ia, String(reenviaremail));
  EEPROM.commit();
  #ifdef LCD_SCL
  lcd.setCursor(0, 0); lcd.print(centralizar(F("EEPROM")));
  lcd.setCursor(0, 1); lcd.print(centralizar(F("Gravando")));
  lcdCnt = 3;
  #endif
  msg=F("EEPROM: gravando "); msg+=String(ia); msg+=F(" bytes"); addLog(msg);
}

String eepromRead(int ini) {
  String sa = "";
  while (EEPROM.read(ini) > 0) {
    sa += char(EEPROM.read(ini));
    ini++;
  }
  return sa;
}

int eepromWrite(int ia, String sa) {
  for (int i = 0; i < sa.length(); i++) {
    EEPROM.write(ia, sa[i]);
    ia++;
  }
  EEPROM.write(ia, 0); ia++;
  return ia;
}

String limparPalavra(String sa){
  while(sa.substring(sa.length()-1) == " ") sa.remove(sa.length()-1);
  while(sa.substring(0,1) == " ") sa.remove(0,1);
  sa.replace("  "," ");
  return sa;
}

