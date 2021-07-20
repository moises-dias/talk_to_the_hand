// O tempo para leitura pela função analogRead() é por volta de 100 micro segundos -> https://www.embarcados.com.br/arduino-entradas-analogicas/

// leds do display de 7 segmentos
int LEDs[] = {22,23,1,3,19,18,5,21};
// pinos analógicos para leitura
int analogPins[] = {13, 12, 14, 27, 26}; // leds para a leitura dos resistores
// valores de cada dedo
int dedos[] = {0, 0, 0, 0, 0};
int dedos_final[] = {0, 0, 0, 0, 0};
// valor do giroscopio -> eixo y
int eixoy = 0;
int eixoy_final = 0;
int eixoy_port = 25;
// referência que define se o dedo está dobrado ou não
int ref = 500;

// ======================================================================================================================================================================================
// letras do alfabeto -> led states to display
int nda[] = {0, 0, 0, 0, 0, 0, 0, 1};   // nenhuma letra
int a[] = {1, 1, 1, 0, 1, 1, 1, 1};   // A
int b[] = {1, 1, 1, 1, 1, 0, 0, 0};   // b
int c[] = {0, 1, 1, 1, 0, 0, 1, 0};   // C
int d[] = {1, 0, 1, 1, 1, 1, 0, 0};   // d
int e[] = {1, 1, 1, 1, 0, 0, 1, 0};   // E
int f[] = {1, 1, 1, 0, 0, 0, 1, 0};   // F
int g[] = {0, 1, 1, 1, 1, 0, 1, 0};   // G
int h[] = {1, 1, 1, 0, 1, 0, 0, 0};   // h
int ii[] = {0, 1, 1, 0, 0, 0, 0, 0};   // I
int j[] = {0, 0, 1, 1, 1, 1, 0, 0};   // J
int k[] = {1, 1, 1, 0, 1, 1, 0, 1};   // K = X.
int l[] = {0, 1, 1, 1, 0, 0, 0, 0};   // L
int m[] = {1, 0, 1, 0, 1, 0, 0, 1};   // m = n.
int n[] = {1, 0, 1, 0, 1, 0, 0, 0};   // n
int o[] = {0, 1, 1, 1, 1, 1, 1, 0};   // O
int p[] = {1, 1, 1, 0, 0, 1, 1, 0};   // P
int q[] = {0, 1, 1, 1, 1, 1, 1, 1};   // Q = O.
int r[] = {1, 0, 1, 0, 0, 0, 0, 0};   // r
int s[] = {1, 1, 0, 1, 1, 0, 1, 0};   // S
int t[] = {1, 1, 1, 1, 0, 0, 0, 0};   // t
int u[] = {0, 1, 1, 1, 1, 1, 0, 0};   // U
int v[] = {0, 0, 1, 1, 1, 0, 0, 0};   // v
int w[] = {0, 0, 1, 1, 1, 0, 0, 1};   // w = v.
int x[] = {1, 1, 1, 0, 1, 1, 0, 0};   // X
int y[] = {1, 1, 0, 1, 1, 1, 0, 0};   // y
int z[] = {1, 0, 1, 1, 0, 1, 1, 0};   // Z

// sinais do alfabeto (começando pelo dedão) -> led states to display
int sinala[] = {0, 1, 1, 1, 1};   // A
int sinalb[] = {1, 0, 0, 0, 0};   // b
int sinalc[] = {1, 1, 1, 1, 1};   // C *
int sinald[] = {1, 0, 1, 1, 1};   // d
int sinale[] = {1, 1, 1, 1, 1};   // E *
int sinalf[] = {0, 1, 0, 0, 0};   // F
int sinalg[] = {1, 0, 1, 1, 1};   // G
int sinalh[] = {0, 0, 1, 1, 1};   // h
int sinali[] = {1, 1, 1, 1, 0};   // I
int sinalj[] = {1, 1, 1, 1, 0};   // J
int sinalk[] = {0, 0, 1, 1, 1};   // K = X.
int sinall[] = {0, 0, 1, 1, 1};   // L
int sinalm[] = {1, 0, 0, 0, 1};   // m = n.
int sinaln[] = {1, 0, 0, 1, 1};   // n
int sinalo[] = {1, 1, 1, 1, 1};   // O
int sinalp[] = {0, 0, 1, 1, 1};   // P
int sinalq[] = {1, 0, 1, 1, 1};   // Q = O.
int sinalr[] = {0, 0, 0, 1, 1};   // r
int sinals[] = {1, 1, 1, 1, 1};   // S
int sinalt[] = {0, 1, 0, 0, 0};   // t
int sinalu[] = {1, 0, 0, 1, 1};   // U
int sinalv[] = {1, 0, 0, 1, 1};   // v
int sinalw[] = {1, 0, 0, 0, 1};   // w = v.
int sinalx[] = {1, 1, 1, 1, 1};   // X
int sinaly[] = {0, 1, 1, 1, 0};   // y
int sinalz[] = {1, 0, 1, 1, 1};   // Z
// ======================================================================================================================================================================================

// setando as entradas dos displays de 7 segmentos
void setup() {
  Serial.begin(9600);
  for (int i=0; i<8; i++) {
    pinMode(LEDs[i], OUTPUT);
  }
}

// print dos dedos
void printaVetor(int* vetor, int tam) {
  for( int i=0 ; i<tam ; i++ ) {
    Serial.print(vetor[i]);
  }
  Serial.print("\n");
}

// compara dois verotes de mesmo tamanho e retorna se são ou não iguais
bool iguais(int *v1, int *v2, int tam) {
    int i, cont = 1;
    for(i=0; i < tam; i++) {
        if (v1[i] != v2[i]) {
            return false;
        }
    }
    return true;
}

// escreve no led
void escreve(int* pos, int tam){
  for( int i=0 ; i<tam ; i++ ) {
    if( pos[i] == 1 ) {
      digitalWrite(LEDs[i], HIGH);
    }
    else {
      digitalWrite(LEDs[i], LOW);
    }
  }
}


// MAIN ========================================================================================================================================================================
void loop() {
  Serial.print("--------->\n");
  // lendo cada sensor e atribuindo os valores a cada dedo
  for (int i=0 ; i<5 ; i++ ){
    if(analogRead(analogPins[i]) > ref) {
      dedos[i] = 1;
    }
    else {
      dedos[i] = 0;
    }
    if(analogRead(eixoy_port) > ref) {
      eixoy = 1;
    }
    else {
      eixoy = -1;
    }
  }
  printaVetor(dedos, 5);
  Serial.println(eixoy);
  // esperando para ver se o dedo continua dobrado e se não é sinal intermediário
  delay(2000); // ms
  // pegando dados uma segunda vez
  for (int i=0 ; i<5 ; i++ ){
    if(analogRead(analogPins[i]) > ref) {
      dedos_final[i] = 1;
    }
    else {
      dedos_final[i] = 0;
    }
    if(analogRead(eixoy_port) > ref) {
      eixoy_final = 1;
    }
    else {
      eixoy_final = -1;
    }
  }
  printaVetor(dedos_final, 5);
  Serial.println(eixoy);
  // verificando se os valores coletados batem
  if(eixoy == eixoy_final and iguais(dedos, dedos_final, 5) ){ // eh um sinal e não um intermediário
    if(iguais(dedos, sinala, 5)){
       Serial.println("a");
       escreve(a, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinalb, 5)){
      Serial.println("b");
       escreve(b, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinalc, 5)){
      Serial.println("c");
       escreve(c, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == 1 and iguais(dedos, sinald, 5)){
      Serial.println("d");
       escreve(d, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinale, 5)){
      Serial.println("e");
       escreve(e, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinalf, 5)){
      Serial.println("f");
       escreve(f, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == 1 and iguais(dedos, sinalg, 5)){
      Serial.println("g");
       escreve(g, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == 1 and iguais(dedos, sinalh, 5)){
      Serial.println("h");
       escreve(h, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == 1 and iguais(dedos, sinali, 5)){
      Serial.println("i");
       escreve(ii, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == -1 and iguais(dedos, sinalj, 5)){
      Serial.println("j");
       escreve(j, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == 1 and iguais(dedos, sinalk, 5)){
      Serial.println("k");
       escreve(k, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == 1 and iguais(dedos, sinall, 5)){
      Serial.println("l");
       escreve(l, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == 1 and iguais(dedos, sinalm, 5)){
      Serial.println("m");
       escreve(m, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == -1 and iguais(dedos, sinaln, 5)){
      Serial.println("n");
       escreve(n, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinalo, 5)){
      Serial.println("o");
       escreve(o, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == -1 and iguais(dedos, sinalp, 5)){
      Serial.println("p");
       escreve(p, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == -1 and iguais(dedos, sinalq, 5)){
      Serial.println("q");
       escreve(q, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinalr, 5)){
      Serial.println("r");
       escreve(r, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinals, 5)){
      Serial.println("s");
       escreve(s, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinalt, 5)){
      Serial.println("t");
       escreve(t, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == 1 and iguais(dedos, sinalu, 5)){
      Serial.println("u");
       escreve(u, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == 1 and iguais(dedos, sinalv, 5)){
      Serial.println("v");
       escreve(v, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(eixoy == -1 and iguais(dedos, sinalw, 5)){
      Serial.println("w");
       escreve(w, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinalx, 5)){
      Serial.println("x");
       escreve(x, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinaly, 5)){
      Serial.println("y");
       escreve(y, 8); // tamanho do vetor do display e não dos dedos
    }
    else if(iguais(dedos, sinalz, 5)){
      Serial.println("z");
       escreve(z, 8); // tamanho do vetor do display e não dos dedos
    }
    else {
      Serial.println("nda");
      escreve(nda, 8); // nenhuma letra, desliga o display
    }
  }
  else {
    escreve(nda, 8); // nenhuma letra, desliga o display
  }
  
}
