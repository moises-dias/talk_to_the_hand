
int values[100];
void setup(){
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
}
void loop(){
  if (digitalRead(3) == LOW) // se conectar o 3 no ground chama a função calibrar
    calibrar();

  // código apenas para mostrar leituras
  int sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  delay(3000);
}
void calibrar() {
  Serial.println("calibrando");
  bool found; 
  int qt_found = 0; // quantidade de valores encontrados

  for(int i = 0; i < 100; i++) // inicia o vetor com qlqr valor, nem precisa na real
    values[i] = -1;
  
  for(int i = 0; i < 100; i++) { // i = quantidade de vezes que a medição será feita
    found = false;
    int val = analogRead(A0);
    for(int j = 0; j < qt_found; j++) { // de i = 0 até i = quantidade de valores inseridos no vetor
      if(values[j] == val) // se for medição repetida desconsidera
        found = true;
    }
    if(!found) { // se o valor medido não está no vetor inserir o valor no vetor
      values[qt_found] = val; 
      qt_found = qt_found + 1;
    }
    delay(100);
  }

  // auto explicativo, a partir dos valores coletados anteriormente achar o maximo minimo e media
  int max_val = values[0];
  int min_val = values[0];
  int avg_val = 0;

  for(int i = 0; i < qt_found; i++) {
    if (values[i] > max_val)
      max_val = values[i];
    if (values[i] < min_val)
      min_val = values[i];
    avg_val = avg_val + values[i];
  }
  avg_val = avg_val / qt_found;
  Serial.print("max ");
  Serial.println(max_val);
  Serial.print("min ");
  Serial.println(min_val);
  Serial.print("avg ");
  Serial.println(avg_val);
  
}
