int max_vals[5] = {0, 0, 0, 0, 0};        // usada na função calibrar, guarda os MAIORES valores lidos para cada dedo                
int min_vals[5] = {0, 0, 0, 0, 0};        // usada na função calibrar, guarda os MENORES valores lidos para cada dedo
long avg_vals[5] = {0, 0, 0, 0, 0};       // usada na função calibrar, guarda A MEDIA dos valores lidos para cada dedo
byte pins[5] = {A3, A2, A1, A0, A10};     // pinos de entrada para os sensores
int leds[5] = {2, 4, 5, 6, 7};            // pinos de saída para os leds (não será usado na versão final mas ajuda a visualizar se algum sensor não esta funcionando)
int values[5][200];                       // vetor com o numero de leituras a serem feitas, o numero da segunda dimensão DEVE coincidir com o numero de leituras na linha 36              
bool calibrado = false;                   // booleano para saber se a calibração foi feita e liberar a leitura dos sensores para acender os leds

void setup() {                            // 3 == botão para iniciar calibração, 2,4,5,6,7 == pinos de saída (linha 5)
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);  
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}
void loop() {
  if (digitalRead(3) == LOW)                                                    // se o pino 3 está no GND, chama a função calibrar
    calibrar();
  if (calibrado) {                                                              // se já foi feita a calibração, acende ou apaga os leds a partir da leitura dos sensores
    for (int finger = 0; finger < 5; finger++) {                                // para cada um dos 5 dedos:
      if(analogRead(pins[finger]) < (avg_vals[finger] + min_vals[finger])/2)    // se o que foi lido pelo arduino é menor que a (média + mínimo)/2 (média e mínimo do dedo em questão) (foi a melhor forma que encontrei para detectar o dedo flexionado)
        digitalWrite(leds[finger], HIGH);                                       // acende o led associado ao dedo, senão, apaga o led associado ao dedo
      else
        digitalWrite(leds[finger], LOW);
    }
  }
  delay(200);                                                                   // aguarda 0.2 segundo
}
void calibrar() {                                     
  Serial.println("calibrando");
  bool found = false;                                 // booleano para controlar quando um valor de leitura (distinto do que já foi identificado) é identificado
  int qt_found[5] = {0, 0, 0, 0, 0};                  // quantidade de valores encontrados para cada dedo, inicialmente zero para cada dedo

  for (int i = 0; i < 200; i++) {                     // i = quantidade de vezes que a medição será feita, 200 = 20 segundos, se tiver um intervalor de 100ms entre leituras (200*100ms)
    for (int finger = 0; finger < 5; finger ++) {     // para cada um dos 5 dedos
      found = false;                    
      int val = analogRead(pins[finger]);             // pega a medição do sensor do dedo
      for (int j = 0; j < qt_found[finger]; j++) {    // de j = 0 até j = quantidade de valores distintos encontrados para o dedo
        if (values[finger][j] == val)                 // se for medição repetida (valor já está no vetor), seta found para true
          found = true;
      }
      if (!found) {                                   // se o valor medido não está no vetor inserir o valor no vetor (found = false)
        values[finger][qt_found[finger]] = val;       // insere o valor no vetor
        qt_found[finger] = qt_found[finger] + 1;      // aumenta em 1 o numero de valores encontrados para o dedo
      }
    }
    delay(100);

  }

  for (int finger = 0; finger < 5; finger ++) {                 // para cada um dos 5 dedos
      max_vals[finger] = values[finger][0];                     // inicia o valor máximo de cada dedo como o primeiro valor identificado
      min_vals[finger] = values[finger][0];                     // inicia o valor mínimo de cada dedo como o primeiro valor identificado
      avg_vals[finger] = 0;                                     // inicia a média dos valores encontrados de cada dedo como zero
  }

  for (int finger = 0; finger < 5; finger ++) {                 // para cada um dos 5 dedos
    for (int i = 0; i < qt_found[finger]; i++) {                // para cada um dos valores encontrados para cada dedo
      if (values[finger][i] > max_vals[finger])                 // se o valor é maior que o que foi identificado como máximo até agora
        max_vals[finger] = values[finger][i];                   // seta o valor como máximo
      if (values[finger][i] < min_vals[finger])                 // se o valor é menor que o que foi identificado como mínimo até agora
        min_vals[finger] = values[finger][i];                   // seta o valor como mínimo
      avg_vals[finger] = avg_vals[finger] + values[finger][i];  // soma o valor em uma variavel para tirar a média quando finalizar a soma
        
    }
    avg_vals[finger] = avg_vals[finger] / qt_found[finger];     // divide por K a soma dos K valores identificados

    Serial.print("finger ");
    Serial.print(finger);
    Serial.print(" -- max: ");
    Serial.print(max_vals[finger]);
    Serial.print(" -- min: ");
    Serial.print(min_vals[finger]);
    Serial.print(" -- avg: ");
    Serial.println(avg_vals[finger]);
  }
  
  calibrado = true;                                             // com calibrado == true os leds podem ser acendidos

}
