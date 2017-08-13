String wifiStatusText(byte x) {
  String result;
  if (x == 0) result = F("WL_IDLE_STATUS");
  if (x == 1) result = F("WL_NO_SSID_AVAIL");
  if (x == 2) result = F("WL_SCAN_COMPLETED");
  if (x == 3) result = F("WL_CONNECTED");
  if (x == 4) result = F("WL_CONNECT_FAILED");
  if (x == 5) result = F("WL_CONNECTION_LOST");
  if (x == 6) result = F("WL_DISCONNECTED");
  if (x == 255) result = F("WL_NO_SHIELD");
  if (result == "") result = String(x);
  return result;
}
