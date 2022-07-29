float SetPoint, Sv;
float e[3] = {0.0, 0.0, 0.0};
int u[2] = {0.0, 0.0};
int Aux, cont;
int up = 12;
int down = 11;
float Kp = 3.6;
float Ki = 0;
float Kd = 1.5;
float C0 = 0, C1 = 0, C2 = 0;
float tiempo, ts, timePrev;
float A_3 = 0;

void setup()
{
  pinMode(up, OUTPUT);
  pinMode(down, OUTPUT);
  Serial.begin(115200);
  tiempo = millis();
  delay(300);
}

void loop() {
  timePrev = tiempo;
  tiempo = millis();
  ts = (tiempo - timePrev) / 1000;
  
  C2 = (Ki*ts)/2;
  A_3 = Kd/ts;
  C0 = Kp+C2+A_3;
  C1 = C2+C2-A_3;
  
  if(Serial.available())
  {
    String c = Serial.readString();
    e[0] = c.toFloat();
    //Sv = c.toFloat();
    Serial.print("Valor: ");
    Serial.println(e[0]);
  }

  // Calculo del PID
  u[0] = C0*e[0]+C1*e[1]+C2*e[2];
  Aux = u[0];

  // Saturador
  if(u[0] > 39) u[0] = 39;
  if(u[0] < 0.0 && u[0] >= -39) u[0] = u[0]*-1;
  if(u[0] < -39) u[0] = 39;

  if(Aux < 0 && cont >= 2)
  {
    for(int i = 0; i < u[0]; i++)
    {
      if(Serial.available())
    {
      String c = Serial.readString();
      e[0] = c.toFloat();
      Serial.println(e[0]);
    }
      digitalWrite(up, HIGH);
      delay(400);
      digitalWrite(up, LOW);
      delay(10);
    }
      delay(10);
  }

  else if(Aux > 0 && cont >= 2)
  {
    for(int i = 0; i < u[0]; i++)
    {
      if(Serial.available())
    {
      String c = Serial.readString();
      e[0] = c.toFloat();
      Serial.println(e[0]);
    }
      digitalWrite(down, HIGH);
      delay(400);
      digitalWrite(down, LOW);
      delay(10);
    }
      delay(10);
  }

  // Corrimientos
  e[2] = e[1];
  e[1] = e[0];
  u[1] = u[0];

  Serial.print(u[0]);
  Serial.print("  ");
  Serial.print(e[0]);
  Serial.print("  ");
  Serial.print(Sv);
  Serial.print("  ");
  Serial.println(tiempo);

  delay(1500);
  cont++;
}
