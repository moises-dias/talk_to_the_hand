// imports para o aceletrometro/giroscopio
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

#include "BluetoothSerial.h" //Header File for Serial Bluetooth, will be added by default into Arduino

BluetoothSerial ESP_BT; //Object for Bluetooth

const int num_sensors = 7;                                       // numero de sensores flexíveis e de pressão 
int sensors[num_sensors] =   {25, 32, 33, 34, 35, 26, 27};       // GPIO que serão utilizados pelos sensores flexíveis e de pressão
int LEDs[num_sensors] =   {19, 16, 23, 4, 17, 12, 13};           // LEDs utilizados para debugar
int threshold[num_sensors] = {0, 0, 0, 0, 0, 0, 0};              // limiar para considerar um sensor flexionado/pressionado, será preenchido na função calibrar

bool calibrado = false;

const int num_sensors_and_gy = 9;                          // 7 sensores, eixo X giroscopio e eixo X acelerometro
const int total_signs = 28;                                // 26 letras, apagar e espaço

int g_led_pin = 5;                                         // LED utilizado para debugar o giroscopio
int g_reads_index = 7;                                     // índice do vetor reads para gravar o valor do giroscopio
float g_threshold = 3.0;                                   // considerar que a mão está girando para valores acima de 3.0
int g_count_threshold = 2;                                 // se for identificado o giro, manter ligado nas proximas 2 leituras
int g_count = 0; 

int a_led_pin = 15;                                        // LED utilizado para debugar o acelerometro
int a_reads_index = 8;                                     // índice do vetor reads para gravar o valor do acelerometro
float a_threshold = 0.0;                                   // valores positivos: mão para cima, negativos: mão para baixo

int reads[num_sensors_and_gy] = {0, 0, 0, 0, 0, 0, 0, 0, 0};        // guarda a leitura atual dos sensores
int last_reads[num_sensors_and_gy] = {0, 0, 0, 0, 0, 0, 0, 0, 0};   // guarda a ultima leitura dos sensores
int last_printed[num_sensors_and_gy] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // ultima letra enviada para o aplicativo

// os dois vetores abaixo são relacionados: signs_index[0] == letra 'a', signs[0] == valores para formar a letra 'a'
char signs_index[total_signs] = {'o', 'l', 'a', 'm', 'u', 'n', 'd', '.'};
int signs[total_signs][num_sensors_and_gy] = {
  {1, 0, 0, 0, 0, 0, 0, 0, 0}, // o
  {0, 0, 0, 1, 0, 0, 0, 0, 0}, // l
  {0, 1, 0, 0, 0, 0, 0, 0, 0}, // a
  {0, 0, 1, 0, 0, 0, 0, 0, 0}, // m
  {0, 0, 0, 0, 1, 0, 0, 0, 0}, // u
  {0, 1, 0, 1, 0, 0, 0, 0, 0}, // n
  {0, 1, 1, 0, 0, 0, 0, 0, 0}, // d
  {1, 1, 1, 1, 1, 0, 0, 0, 0}, // . (apagar)
};

Adafruit_MPU6050 mpu;          // sensor acelerometro/giroscopio

void setup(void) {
  pinMode(14, INPUT_PULLUP);                            // pino 14 é um botão para controlar quando calibrar
  for (int i = 0; i < num_sensors; i++) {               // seta os pinos dos sensores para output
    pinMode(LEDs[i], OUTPUT);
    digitalWrite(LEDs[i], LOW);
  }
  pinMode(g_led_pin, OUTPUT);
  digitalWrite(g_led_pin, LOW);
  pinMode(a_led_pin, OUTPUT);
  digitalWrite(a_led_pin, LOW);

  Serial.begin(9600);
  while (!Serial)
    delay(10);

  ESP_BT.begin("ESP32_LED_Control"); //Name of your Bluetooth Signal
  Serial.println("Bluetooth Device is Ready to Pair");

  if (!mpu.begin()) {                                   // inicia e configura o sensor acelerometro/giroscopio  
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void loop() {
  if (digitalRead(14) == LOW)                                       //calibra os sensores ao clicar no botão do GPIO 14
    calibrar();

  if (calibrado)
  {
    take_readings();                                                // take readings mapeia o vetor reads com as leituras atuais

    if (!iguais(reads, last_printed, num_sensors_and_gy)) {         // se o que foi lido é diferente da ultima letra enviada para o app
      if (iguais(reads, last_reads, num_sensors_and_gy)) {          // se o que foi lido é igual ao que foi lido a x milisegundos (confirmar que é uma leitura e não ruido)
        for (int i = 0; i < total_signs; i++) {
          if (iguais(reads, signs[i], num_sensors_and_gy)) {        // procura a letra lida no vetor signs
            Serial.println(signs_index[i]);
            ESP_BT.println(signs_index[i]);
            for (int i = 0; i < num_sensors_and_gy; i++) {
              last_printed[i] = reads[i];                           // atualiza o last_printed com a letra enviada para o app
            }
            break;                                                  // não é necessário realizar mais testes após identificar a letra
          }
        }
      }
    }

    for (int i = 0; i < num_sensors_and_gy; i++) {                  // atualiza o vetor last_reads com a ultima leitura realizada
      last_reads[i] = reads[i];
    }

  }


}

void calibrar() {
  Serial.println("calibrando");
  bool found = false;
  int qt_found[num_sensors] = {0, 0, 0, 0, 0, 0, 0};
  int max_vals[num_sensors] = {0, 0, 0, 0, 0, 0, 0};  
  int min_vals[num_sensors] = {0, 0, 0, 0, 0, 0, 0};   
  long avg_vals[num_sensors] = {0, 0, 0, 0, 0, 0, 0};
  int values[num_sensors][200]; 

  for (int i = 0; i < 200; i++) {
    for (int finger = 0; finger < num_sensors; finger ++) {
      found = false;
      int val = analogRead(sensors[finger]);
      for (int j = 0; j < qt_found[finger]; j++) {
        if (values[finger][j] == val)
          found = true;
      }
      if (!found) {
        values[finger][qt_found[finger]] = val;
        qt_found[finger] = qt_found[finger] + 1;
      }
    }
    delay(100);

  }

  for (int finger = 0; finger < num_sensors; finger ++) {
    max_vals[finger] = values[finger][0];
    min_vals[finger] = values[finger][0];
    avg_vals[finger] = 0;
  }

  for (int finger = 0; finger < num_sensors; finger ++) {
    for (int i = 0; i < qt_found[finger]; i++) {
      if (values[finger][i] > max_vals[finger])
        max_vals[finger] = values[finger][i];
      if (values[finger][i] < min_vals[finger])
        min_vals[finger] = values[finger][i];
      avg_vals[finger] = avg_vals[finger] + values[finger][i];

    }
    avg_vals[finger] = avg_vals[finger] / qt_found[finger];

    Serial.print("finger ");
    Serial.print(finger);
    Serial.print(" -- max: ");
    Serial.print(max_vals[finger]);
    Serial.print(" -- min: ");
    Serial.print(min_vals[finger]);
    Serial.print(" -- avg: ");
    Serial.println(avg_vals[finger]);
  }

  for (int finger = 0; finger < num_sensors; finger ++) {
    // threshold[finger] = (avg_vals[finger] + min_vals[finger]) / 2;
    threshold[finger] = avg_vals[finger];
  }

  // até arrumar os sensores de pressao
  threshold[5] = 4000;
  threshold[6] = 4000;

  calibrado = true;

}

void take_readings() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  for (int finger = 0; finger < num_sensors; finger++) {
    if (analogRead(sensors[finger]) < threshold[finger]) {
      reads[finger] = 1;
      digitalWrite(LEDs[finger], HIGH);
    }
    else {
      reads[finger] = 0;
      digitalWrite(LEDs[finger], LOW);
    }
  }

  if (a.acceleration.x > a_threshold) {
    reads[a_reads_index] = 1;
    digitalWrite(a_led_pin, HIGH);
  }
  else {
    reads[a_reads_index] = 0;
    digitalWrite(a_led_pin, LOW);
  }

  if (g.gyro.x > g_threshold) {
    reads[g_reads_index] = 1;
    digitalWrite(g_led_pin, HIGH);
    g_count = g_count_threshold; 
  }
  else if (g_count > 0) { // como o giroscópio é "rapido" temos que garantir que ao identificar um giro ele dure mais de uma leitura
    g_count = g_count - 1;
  }
  else {
    reads[g_reads_index] = 0;
    digitalWrite(g_led_pin, LOW);
  }

  delay(500);
}

bool iguais(int *v1, int *v2, int tam) {
  for (int i = 0; i < tam; i++)
    if (v1[i] != v2[i])
      return false;
  return true;
}
