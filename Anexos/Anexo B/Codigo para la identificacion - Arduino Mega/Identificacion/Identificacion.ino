int led = 13;
int pulso = 12;
int cont = 0;

void setup() {

  pinMode(led, OUTPUT);
  Serial.begin(115200);

}

void loop() {

  if(Serial.available())
  {
    float value;
    String c = Serial.readString(); // Leemos la posición de la partícula
    value = c.toFloat();    // Convertimos de string a float
    
    if(value > 55.2)    // Ubicamos la particula en el cero del levitador
    {
      Serial.println(value);
      digitalWrite(pulso, HIGH);
      delay(350);
      digitalWrite(pulso, LOW);
      delay(10);
    }
    
    if(cont <= 15 && value <= 55)   // Enviamos pulsos de 0 a 15 en rampa
    {
      digitalWrite(pulso, HIGH);
      delay(350);
      digitalWrite(pulso, LOW);
      delay(10);
      Serial.print(value);
      Serial.print(" ");
      Serial.println(cont);
      cont++;
    }
  }
}
