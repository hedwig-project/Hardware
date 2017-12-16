void ntpConnect(){ // a cada 1 segundo
  if(WiFi.status() != WL_CONNECTED) TimeNtpCount=10;
  if (timeStatus() != timeNotSet) TimeNtpCount=10;
  if(TimeNtpCount>0){ TimeNtpCount--; return; }
  Udp.begin(localPort);
  digitalWrite(LedPin, LOW);
  msg=F("NTP Server[");msg+=TimeNtpErrCount;
  msg+=F("]: ");msg+=ntpServerName[TimeNtpErrCount%5];addLog(msg);
  setSyncProvider(getNtpTime);
  if (timeStatus() == timeNotSet){
    addLog(F("TimeNTP: falhou"));
    if(TimeNtpErrCount%10==4) {
      addLog(F("NTP Reiniciando WiFi"));
      WiFi.disconnect();
    }
    if(TimeNtpErrCount%10==9) {
      addLog(F("NTP Reiniciando MODEM"));
      WiFi.disconnect();
      ////////
    }
    if(TimeNtpErrCount<360) TimeNtpErrCount++; // máximo 1 hora ou  3600 segundos
    TimeNtpCount=10*TimeNtpErrCount;
  }else{
    ajustarHoraLog();
    msg=F("TimeNTP: "); msg+=ddmmaaaa(); msg+=F(" "); msg+=hhmmss(); addLog(msg);
  }
}

void wsTimeSet(String stime){
  stime = stime.substring(0, stime.length() - 3);
  time_t itime;
  itime = stime.toInt();
  setTime(itime);
  ajustarHoraLog();
  // log
  msg=F("TimeWebServer: "); msg+=ddmmaaaa(); msg+=F(" "); msg+=hhmmss(); addLog(msg);
  //addLog(F("TimeWebServer: ok"));
}


String hhmmss() {
  time_t t = now(); timeZoneChk(); t += timeZone * SECS_PER_HOUR;
  String sa;
  if (hour(t) < 10) sa += "0";
  sa += String(hour(t)) + ":";
  if (minute(t) < 10) sa += "0";
  sa += String(minute(t)) + ":";
  if (second(t) < 10) sa += "0";
  sa += String(second(t));
  return sa;
}

String ddmmaaaa() {
  time_t t = now(); timeZoneChk(); t += timeZone * SECS_PER_HOUR;
  String sa;
  if (day(t) < 10) sa += "0";
  sa += String(day(t)) + "/";
  if (month(t) < 10) sa += "0";
  sa += String(month(t)) + "/";
  sa += String(year(t));
  return sa;
}

String ddmmaa() {
  time_t t = now(); timeZoneChk(); t += timeZone * SECS_PER_HOUR;
  String sa;
  if (day(t) < 10) sa += "0";
  sa += String(day(t)) + "/";
  if (month(t) < 10) sa += "0";
  sa += String(month(t)) + "/";
  String sb;
  sb = String(year(t));
  sb = sb.substring(2);
  sa += sb;
  return sa;
}

void timeZoneChk(){
        //horário de verão
      if(timeStatus() != timeNotSet){
        int previousSunday = day() - weekday();
        if (month() < 2 || month() > 10) timeZone = -2;
        else if (month() == 2 && previousSunday < 14) timeZone = -2;
        else if (month() == 10 && previousSunday >= 14) timeZone = -2;
        else timeZone = -3;
      }
}

int BRtimeZone (unsigned long t){
  // t += BRtimeZone() * SECS_PER_HOUR;
  int tmz;
  int previousSunday = day(t) - weekday(t);
  if (month(t) < 2 || month(t) > 10)
    tmz = -2;
  else 
    if (month(t) == 2 && previousSunday < 14)
      tmz = -2;
    else
      if (month(t) == 10 && previousSunday >= 14)
        tmz = -2;
      else
        tmz = -3;
  return tmz;
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  WiFi.hostByName(ntpServerName[TimeNtpErrCount%5].c_str(), ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL;
    }
  }
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

String secToDMHS (unsigned long ia){
  String sa;
  if(ia>=86400) {sa+=String(ia/86400);sa+=F("d");}
  if(ia>=3600){
    if((ia>=600)&&(hour(ia)<10)) sa+=F("0");
    sa+=hour(ia);sa+=F(":");
  }
  if((ia>=60)&&(minute(ia)<10)) sa+=F("0");
  sa+=minute(ia); sa+=F(":");
  if(second(ia)<10)sa+=F("0"); sa+=second(ia);
  return sa;
}

