#include <TM1637Display.h>  // Biblioteca para o display de 7 segmentos

#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);
int sensor = 8, ledRed = 9, ledGreen = 10, botao = 11;
int LeituraSensor;
int ligado = 0, passou = 0, saida = 0;
bool reset = 1;  // variavel de controle
unsigned long tempo = 0, tempo_play = 0, tempo_segundo = 0;

void setup() {
  Serial.begin(115200);
  pinMode(sensor, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(botao, INPUT_PULLUP);  // define o pino do botao como entrada
  display.setBrightness(0x0f);
}

void loop() {

  if (digitalRead(botao) == LOW) {  // Se o botão for pressionado
    delay(400);
    reset = !reset;  // troca o estado do LED
    Serial.println(reset);
    display.showNumberDecEx(0, 0x40, true);
  }

  handleSensor();

  if (ligado == 1) {
    tempo_play = millis() - tempo;
    if (tempo_play > 3000) {
      saida = 1;
    }
    int minutos = tempo_play / 60000;
    int segundos = (tempo_play % 60000) / 1000;
    tempo_segundo = (minutos * 100) + segundos;
    Serial.print("Tempo cronometrado:");
    Serial.println(tempo_segundo);
    display.showNumberDecEx(tempo_segundo, 0x40, true);
  }
}

void handleSensor() {
  LeituraSensor = digitalRead(sensor);

  if (LeituraSensor == LOW && passou == 0) {
    if (ligado == 0) {
      if (reset == false) {
        tempo = millis();
        ligado = 1;
        passou = 1;
        digitalWrite(ledRed, LOW);
        digitalWrite(ledGreen, HIGH);
      }
    } else if (saida == 1) {
      digitalWrite(ledGreen, LOW);
      digitalWrite(ledRed, HIGH);
      ligado = 0;
      passou = 1;
      tempo_play = millis() - tempo;
      int minutos = tempo_play / 60000;
      int segundos = (tempo_play % 60000) / 1000;
      tempo_segundo = (minutos * 100) + segundos;
      Serial.print("Tempo total:");
      Serial.println(tempo_segundo);
      display.showNumberDecEx(tempo_segundo, 0x40, true);
      saida = 0;
      reset = true;
    }
  }

  if (LeituraSensor == HIGH && passou == 1) {
    passou = 0;
  }
}
 
