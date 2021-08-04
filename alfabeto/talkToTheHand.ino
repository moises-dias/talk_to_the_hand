// O tempo para leitura pela função analogRead() é por volta de 100 micro segundos -> https://www.embarcados.com.br/arduino-entradas-analogicas/

// referência que define se o dedo está dobrado ou não
int ref_flex = 500; /* apenas para testes */
int ref_pressao = 500;
int ref_eixoy = 500;
int ref_eixoz = 500;

// valor para testes porque eu fiquei de saco cheio de testar com potenciômetros, e como a gente já sabe que a leitura funciona e o código já ta com o Moisés
// sensores flex 0-4 / dedao 5 / indicador 6 / eixoy 7 / eixoz 8
int sensores[] = {0, 0, 0, 0, 0, 0, 0, 0, 0 };
int sensores_final[] = {0, 0, 0, 0, 0, 0, 0, 0, 0 };
int sensores_size = 9;

// amostra que pega dos sensores, valores brutos
int amostra[] = {0, 0, 0, 0, 0, 0, 0, 0, 0 };
int amostra_final[] = {0, 0, 0, 0, 0, 0, 0, 0, 0 };

// ================================================================================================================ LETRAS LIBRAS ===================================================
// sinais do alfabeto (começando pelo dedão) -> led states to display
int sinala[] = {0,1,1,1,1,0,1,1,0};   // A
int sinalb[] = {1,0,0,0,0,1,1,1,0};   // b
int sinalc[] = {1,1,1,1,1,0,1,1,0};   // C *
int sinald[] = {1,0,1,1,1,0,0,1,0};   // d
int sinale[] = {1,1,1,1,1,0,0,1,0};   // E *
int sinalf[] = {0,1,0,0,0,1,0,1,0};   // F
int sinalg[] = {1,0,1,1,1,1,0,1,0};   // G
int sinalh[] = {0,0,1,1,1,1,0,1,1};   // h
int sinali[] = {1,1,1,1,0,1,1,1,0};   // I
int sinalj[] = {1,1,1,1,0,1,1,2,1};   // J
int sinalk[] = {0,0,1,1,1,1,0,1,0};   // K = X.
int sinall[] = {0,0,1,1,1,0,0,1,0};   // L
int sinalm[] = {1,0,0,0,1,1,1,-1,0};   // m = n.
int sinaln[] = {1,0,0,1,1,1,1,-1,0};   // n
int sinalo[] = {1,1,1,1,1,1,1,1,1};   // O
int sinalp[] = {0,0,1,1,1,1,0,-1,0};   // P
int sinalq[] = {0,0,1,1,1,0,0,-1,0};   // Q = O.
int sinalr[] = {1,1,0,1,1,1,1,1,0};   // r
int sinals[] = {1,1,1,1,1,1,1,1,0};   // S
int sinalt[] = {0,1,0,0,0,0,0,1,0};   // t
int sinalu[] = {1,0,0,1,1,1,1,1,0};   // U
int sinalv[] = {1,0,0,1,1,1,0,1,0};   // v
int sinalw[] = {1,0,0,0,1,1,0,1,0};   // w = v.
int sinalx[] = {1,1,1,1,1,1,0,2,1};   // X
int sinaly[] = {0,1,1,1,0,0,1,2,1};   // y
int sinalz[] = {1,0,1,1,1,1,0,2,1};   // Z
int sinalespaco[] = {0,0,0,0,0,0,1,2,1};   // Z
// ================================================================================================================== FUNÇÔES ===================================================

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
          if (v1[i] != 2 && v2[i] != 2 && v1[i] != v2[i]) { // se nenhum dos valores for 2 e forem diferentes: 01, 10, 1-1, -11
              return false;
          }
      }
      return true;
  }

// ======================================================================================================================================================================================

// configurações
void setup() {
  Serial.begin(9600);
}

// loop
void loop() {
  Serial.print("---------------------------------------------------------------------->\n");
  printaVetor(amostra, sensores_size);
  // Convertendo os valores analógicos recebidos
  for (int i=0 ; i<5 ; i++ ){ // sensores flex
    if(amostra[i] > ref_flex) {
      sensores[i] = 1;
    }
    else {
      sensores[i] = 0;
    }
  }
  if(amostra[5] > ref_pressao) { // sensores de pressao - dedão 
    sensores[5] = 1;
  }
  else {
     sensores[5] = 0;
  }
  if(amostra[6] > ref_pressao) { // sensores de pressao - indicador
    sensores[6] = 1;
  }
  else {
     sensores[6] = 0;
  }
  if(amostra[7] > ref_eixoy) { // aceleropio - eixoy
    sensores[7] = 1;
  }
  else {
     sensores[7] = -1;
  }
  if(amostra[8] > ref_eixoz) { // aceleropio - eixoz
    sensores[8] = 1;
  }
  else {
     sensores[8] = 0;
  }
  // Convertendo os valores analógicos recebidos - parte 2
  for (int i=0 ; i<5 ; i++ ){ // sensores flex
    if(amostra_final[i] > ref_flex) {
      sensores_final[i] = 1;
    }
    else {
      sensores_final[i] = 0;
    }
  }
  if(amostra_final[5] > ref_pressao) { // sensores de pressao - dedão 
    sensores_final[5] = 1;
  }
  else {
     sensores_final[5] = 0;
  }
  if(amostra_final[6] > ref_pressao) { // sensores de pressao - indicador
    sensores_final[6] = 1;
  }
  else {
     sensores_final[6] = 0;
  }
  if(amostra_final[7] > ref_eixoy) { // aceleropio - eixoy
    sensores_final[7] = 1;
  }
  else {
     sensores_final[7] = -1;
  }
  if(amostra_final[8] > ref_eixoz) { // aceleropio - eixoz
    sensores_final[8] = 1;
  }
  else {
     sensores_final[8] = 0;
  }
  printaVetor(sensores, sensores_size);
  // comparando os valores dos sensores
  if(sensores_final[8] > sensores[8]) { // pega o maior valor que indica a rotação da mão
      sensores[8] = sensores_final[8];
  }
  if(iguais(sensores, sensores_final, sensores_size) ){ /* eh um sinal e não um intermediário */
    if(iguais(sensores, sinala, sensores_size)){
       Serial.println("a");
    }
    else if(iguais(sensores, sinalb, sensores_size)){
      Serial.println("b");
    }
    else if(iguais(sensores, sinalc, sensores_size)){
      Serial.println("c");
    }
    else if(iguais(sensores, sinald, sensores_size)){
      Serial.println("d");
    }
    else if(iguais(sensores, sinale, sensores_size)){
      Serial.println("e");
    }
    else if(iguais(sensores, sinalf, sensores_size)){
      Serial.println("f");
    }
    else if(iguais(sensores, sinalg, sensores_size)){
      Serial.println("g");
    }
    else if(iguais(sensores, sinalh, sensores_size)){
      Serial.println("h");
    }
    else if(iguais(sensores, sinali, sensores_size)){
      Serial.println("i");
    }
    else if(iguais(sensores, sinalj, sensores_size)){
      Serial.println("j");
    }
    else if(iguais(sensores, sinalk, sensores_size)){
      Serial.println("k");
    }
    else if(iguais(sensores, sinall, sensores_size)){
      Serial.println("l");
    }
    else if(iguais(sensores, sinalm, sensores_size)){
      Serial.println("m");
    }
    else if(iguais(sensores, sinaln, sensores_size)){
      Serial.println("n");
    }
    else if(iguais(sensores, sinalo, sensores_size)){
      Serial.println("o");
    }
    else if(iguais(sensores, sinalp, sensores_size)){
      Serial.println("p");
    }
    else if(iguais(sensores, sinalq, sensores_size)){
      Serial.println("q");
    }
    else if(iguais(sensores, sinalr, sensores_size)){
      Serial.println("r");
    }
    else if(iguais(sensores, sinals, sensores_size)){
      Serial.println("s");
    }
    else if(iguais(sensores, sinalt, sensores_size)){
      Serial.println("t");
    }
    else if(iguais(sensores, sinalu, sensores_size)){
      Serial.println("u");
    }
    else if(iguais(sensores, sinalv, sensores_size)){
      Serial.println("v");
    }
    else if(iguais(sensores, sinalw, sensores_size)){
      Serial.println("w");
    }
    else if(iguais(sensores, sinalx, sensores_size)){
      Serial.println("x");
    }
    else if(iguais(sensores, sinaly, sensores_size)){
      Serial.println("y");
    }
    else if(iguais(sensores, sinalz, sensores_size)){
      Serial.println("z");
    }
    else if(iguais(sensores, sinalespaco, sensores_size)){
      Serial.println("espaço");
    }
    else {
      Serial.println("..");
    }
  }
  else { /* é um intermediário e não um sinal */
    Serial.println("...");
  }

}
