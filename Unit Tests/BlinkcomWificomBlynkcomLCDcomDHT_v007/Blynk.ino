void BlynkRun(){ // Conexão, reconexão, a cada 100ms
  if (WiFi.status() == WL_CONNECTED) {  //Verifica se está conectado ao WiFi
    if (Blynk.connected()) {  //Se está conectado ao Blynk
      Blynk.run();  //Executa os procedimentos Blynk
      blynkCnt = 300; // 30 segundos, resetando o contador down para reconexão
    }else{  //Se não estiver conectado ao Blynk
      blynkCnt = blynkCnt - 1;  //Decrementa o contador
      if(blynkCnt < 1){ //A cada 30s desconectado
        blynkCnt = 300; // 30 segundos, reseta o contador down para reconexão
        Blynk.config(blynkAuth.c_str());  //Manda o token do Blynk para autenticação
        Blynk.connect(); //Conecta ao Blynk
      }
    }
  }
}


//Fazer sincronização de App + Server após desconexão

void BlynkSend(){ //Envio de Dados a cada 10s
  if (Luminosidade() != 0) Blynk.virtualWrite(vpinLDR, Luminosidade()/10.0);
  if (umidade != 0) Blynk.virtualWrite(vpinUmi, umidade);
  if (temperatura != 0) Blynk.virtualWrite(vpinTemp, temperatura);

  led2.setValue(contPresenca*(100/60));  //Sensor de Presença, escreve no server variável entre 0 e 100
}

