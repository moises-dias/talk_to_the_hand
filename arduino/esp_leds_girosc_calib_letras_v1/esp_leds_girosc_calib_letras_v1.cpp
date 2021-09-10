int LEDs[7] =   {19, 16, 23, 4, 17, 12, 13};
int sensors[7] =   {25, 32, 33, 34, 35, 26, 27};
// int LEDs[] =   {19, 15, 23, 4, 5, 12, 13, 16, 17};
// int sensors[] =   {25, 26, 27, 32, 33, 34, 35, 16, 17};
int num_sensors = 7;
int max_vals[7] = {0, 0, 0, 0, 0, 0, 0};   // colocar isso dentro do calibrar, só vai precisar do threshold
int min_vals[7] = {0, 0, 0, 0, 0, 0, 0};    // colocar isso dentro do calibrar, só vai precisar do threshold
long avg_vals[7] = {0, 0, 0, 0, 0, 0, 0}; // colocar isso dentro do calibrar, só vai precisar do threshold
int threshold[7] = {0, 0, 0, 0, 0, 0, 0};
int values[7][200];  // colocar isso dentro do calibrar
bool calibrado = false;

const int num_sensors_and_gy = 9; // quantidade de sensores e de eixos do giroscopio e acelerometro q serao usados
const int total_signs = 28; //26 letras, apagar e espaço

// int g_pin = 5;
int g_led_pin = 5;
int g_reads_index = 7;
float g_threshold = 3.0;
int g_count = 0; // se for identificado o giro, manter ligado nas proximas 3 leituras
int g_count_threshold = 2;

// int a_pin = 15;
int a_led_pin = 15;
int a_reads_index = 8;
float a_threshold = 0.0;

int reads[num_sensors_and_gy] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // 5 dedos, 2 pressao, giroscopio X e acelerometro X
int last_reads[num_sensors_and_gy] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // 5 dedos, 2 pressao, giroscopio X e acelerometro X

char signs_index[total_signs] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '.', ','};

int signs[total_signs][num_sensors_and_gy] = {
  {1, 0, 0, 0, 0, 0, 0, 0, 0}, // a
  {0, 1, 0, 0, 0, 0, 0, 0, 0}, // b
  {0, 0, 1, 0, 0, 0, 0, 0, 0}, // c
  {0, 0, 0, 1, 0, 0, 0, 0, 0}, // d
  {0, 0, 0, 0, 1, 0, 0, 0, 0}, // e
  {0, 1, 0, 0, 0, 0, 0, 0, 0}, // f
  {1, 0, 1, 1, 1, 0, 0, 0, 0}, // g
  {0, 0, 1, 1, 1, 0, 0, 0, 0}, // h
  {1, 1, 1, 1, 0, 0, 0, 0, 0}, // i
  {1, 1, 1, 1, 0, 0, 0, 0, 0}, // j
  {0, 0, 1, 1, 1, 0, 0, 0, 0}, // k
  {0, 0, 1, 1, 1, 0, 0, 0, 0}, // l
  {1, 0, 0, 0, 1, 0, 0, 0, 0}, // m
  {1, 0, 0, 1, 1, 0, 0, 0, 0}, // n
  {1, 1, 1, 1, 1, 0, 0, 0, 0}, // o
  {0, 0, 1, 1, 1, 0, 0, 0, 0}, // p
  {1, 0, 1, 1, 1, 0, 0, 0, 0}, // q
  {0, 0, 0, 1, 1, 0, 0, 0, 0}, // r
  {1, 1, 1, 1, 1, 0, 0, 0, 0}, // s
  {0, 1, 0, 0, 0, 0, 0, 0, 0}, // t
  {1, 0, 0, 1, 1, 0, 0, 0, 0}, // u
  {1, 0, 0, 1, 1, 0, 0, 0, 0}, // v
  {1, 0, 0, 0, 1, 0, 0, 0, 0}, // w
  {1, 1, 1, 1, 1, 0, 0, 0, 0}, // x
  {0, 1, 1, 1, 0, 0, 0, 0, 0}, // y
  {1, 0, 1, 1, 1, 0, 0, 0, 0}, // z
  {0, 0, 0, 0, 0, 0, 0, 0, 0}, // apagar
  {0, 0, 0, 0, 0, 0, 0, 0, 0}  // espaço
};

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

void setup(void) {
  pinMode(14, INPUT_PULLUP);
  for (int i = 0; i < num_sensors; i++) {
    pinMode(LEDs[i], OUTPUT);
    digitalWrite(LEDs[i], LOW);
  }
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  pinMode(15, OUTPUT);
  digitalWrite(15, LOW);

  Serial.begin(9600);
  while (!Serial)
    delay(10);

  if (!mpu.begin()) {
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
  if (digitalRead(14) == LOW)
    calibrar();

  if (calibrado)
  {
    take_readings();

    if (iguais(reads, last_reads, num_sensors_and_gy)) {
      for (int i = 0; i < total_signs; i++) {
        if (iguais(reads, signs[i], num_sensors_and_gy)) {
          Serial.println(signs_index[i]);
        }
      }
    }

    for (int i = 0; i < num_sensors_and_gy; i++) {
      last_reads[i] = reads[i];
    }

  }


}

void calibrar() {
  Serial.println("calibrando");
  bool found = false;
  int qt_found[7] = {0, 0, 0, 0, 0, 0, 0};

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

  // criando o threshold
  for (int finger = 0; finger < num_sensors; finger ++) {
    threshold[finger] = (avg_vals[finger] + min_vals[finger]) / 2;
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
  else if (g_count > 0) {
    g_count = g_count - 1;
  }
  else {
    reads[g_reads_index] = 0;
    digitalWrite(g_led_pin, LOW);
  }

  delay(1000);
}

bool iguais(int *v1, int *v2, int tam) {
  for (int i = 0; i < tam; i++)
    if (v1[i] != v2[i])
      return false;
  return true;
}