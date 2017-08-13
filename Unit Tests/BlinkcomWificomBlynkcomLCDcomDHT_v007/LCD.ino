void lcdMostrarStatus(){
  String line0;
  String line1;

  if ((millis()/1000) % 2 == 1) line0 = " "; 
  else line0 = char(175);
  line0 += " ";
  
  if (WiFi.status() == WL_CONNECTED) line0 += "W ";
  else line0 += "w ";
  if (Blynk.connected()) line0 += "B ";
  else line0 += "b ";
    
  line0 += String(int(temperatura));
  line0 += char(0);
  line0 += "C ";
  
  line0 += String(int(umidade));
  line0 += "% ";

  line1 += "  L=";
  line1 += String(Luminosidade());
  line1 += " ";

  line1 += "P=";
  line1 += String(contPresenca);
  line1 += " ";
  
  lcd.setCursor(0, 0);  //(x,y); print da primeira linha
  lcd.print(line0);

  lcd.setCursor(0, 1);  //(x,y); print da segunda linha
  lcd.print(line1);
}

void lcdsetup(){
  lcd.begin(16, 2);
  lcd.clear();
  
  byte grau[8] ={ B00001100, B00010010, B00010010, B00001100, B00000000, B00000000, B00000000, B00000000,};
  lcd.createChar(0, grau);
}

