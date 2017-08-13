// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor


void readTemp(){
  float t2 = dht.readTemperature(); 
  if (!isnan(t2)) { //isnan = is not a number
    if (temperatura > 0) {
      temperatura = ((temperatura * 2.0) + t2) / 3.0;
    } else {
      temperatura = t2;
    }
  }
}

void readHumi(){
  float h2 = dht.readHumidity(); 
  if (!isnan(h2)){
    if (umidade > 0) {
      umidade = ((umidade * 2.0) + h2) / 3.0;
    } else {
      umidade = h2;
    }
  }
}

