int Luminosidade() {
  int ia = 0;
  for (int i = 0 ; i < 8; i++) {  //Média de 8 leituras
    ia += analogRead(LDR);
  }
  ia = ia / 8;
  return ia;
}
