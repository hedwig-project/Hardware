void initSerial() //Inicia comunicação serial a baud rate de 115200 para debug
{
    Serial.begin(115200);
}

void InitOutput(void) //Inicializa o output em nível lógico baixo
{
    pinMode(D4, OUTPUT);
    digitalWrite(D4, HIGH);          
}
