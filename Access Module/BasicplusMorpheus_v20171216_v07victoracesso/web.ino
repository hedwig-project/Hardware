void webServerSetup() {
  String sa;
  sa = F("/"); server.on(sa.c_str(), handleMais);
  //sa = F("/menu.htm"); server.on(sa.c_str(), handleMenu);
  sa = F("/home"); server.on(sa.c_str(), handleHome);
  sa = F("/json"); server.on(sa.c_str(), handleJSON);
  sa = F("/wificon"); server.on(sa.c_str(), handleWiFiCon);
  sa = F("/config"); server.on(sa.c_str(), handleConfig);
  sa = F("/tools"); server.on(sa.c_str(), handleTools);
  sa = F("/advconfig"); server.on(sa.c_str(), handleAdvConfig);
  sa = F("/users"); server.on(sa.c_str(), handleUsers);
  sa = F("/blynk"); server.on(sa.c_str(), handleBlynk);
  #ifdef LCD_SCL
  sa = F("/lcd"); server.on(sa.c_str(), handleLCD);
  #endif
  sa = F("/rf"); server.on(sa.c_str(), handleRF);
  sa = F("/ap"); server.on(sa.c_str(), handleAP);
  sa = F("/reles"); server.on(sa.c_str(), handleReles);
  sa = F("/alarm"); server.on(sa.c_str(), handleAlarm);
  sa = F("/keyb"); server.on(sa.c_str(), handleKeyb);
  sa = F("/log"); server.on(sa.c_str(), handleLog);
  sa = F("/maiscnf"); server.on(sa.c_str(), handleMaisCnf);
  sa = F("/maislay"); server.on(sa.c_str(), handleMaisLayout);
  sa = F("/cor"); server.on(sa.c_str(), handleCor);
  sa = F("/DeviceID"); server.on(sa.c_str(), handleDeviceID);
  sa = F("/dht"); server.on(sa.c_str(), handleDHT);
  sa = F("/fir"); server.on(sa.c_str(), handleFir);
  sa = F("/logcnf"); server.on(sa.c_str(), handleLogCnf);
  server.begin();
}


