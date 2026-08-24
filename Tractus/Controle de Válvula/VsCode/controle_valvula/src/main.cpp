#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define PINO_SENSOR 2
#define PINO_VALVULA 3 // trocar para A0 com o arduino uno r4 
#define PINO_POTENCIOMETRO A1
#define PINO_STOP 4
#define PINO_LED_BLUE 9
#define PINO_LED_GREEN 10
#define PINO_LED_RED 11
#define SEGUNDO 1000
#define FLUXO_MAXIMO 25
#define TEMPO_VALVULA 5000
#define FATOR_CALIBRACAO 11.0 
#define TEMPO_CALIBRACAO 2000.0

LiquidCrystal_I2C lcd(0x27, 16, 2);

double fluxo = 0.0, fluxoDesejado = 0.0, fluxoMaximo = 25.0;
volatile int contador = 0;
int contadorAnterior;
unsigned long tempoAtual, tempoAnterior = 0;
bool stop = false;

void Fluxo(){
  contador++;
}

bool MedirFluxo();
void Calibrar();
void Controle(double fluxoDesejado);
void Led();
void Display();

void setup() {
  pinMode(PINO_SENSOR, INPUT_PULLUP);
  pinMode(PINO_VALVULA, OUTPUT);
  pinMode(PINO_POTENCIOMETRO, INPUT);
  pinMode(PINO_STOP, INPUT_PULLUP);
  pinMode(PINO_LED_BLUE, OUTPUT);
  pinMode(PINO_LED_GREEN, OUTPUT);
  pinMode(PINO_LED_RED, OUTPUT);
  
  //analogWriteResolution(12);
  
  attachInterrupt(digitalPinToInterrupt(PINO_SENSOR), Fluxo, FALLING);
  Serial.begin(9600);
  analogWrite(PINO_VALVULA, 0);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Iniciando...");
  Calibrar();
}

void loop() {
  // verifica se o botao de stop esta apertado e fecha a valvula
  if (digitalRead(PINO_STOP) == HIGH){ 
    analogWrite(PINO_VALVULA, 0);
    fluxoDesejado = 0.0;
    stop = true;
    Led();
    return;
  }
  stop = false;

  int leituraPotenciometro = analogRead(PINO_POTENCIOMETRO);
  fluxoDesejado = (leituraPotenciometro / 1023.0) * 25.0;
  Controle(fluxoDesejado);
  
  if (MedirFluxo()){
    Led();
    Display();
    Serial.println("\n\nFluxo Desejado: " + String(fluxoDesejado) + " L/min\n");
    Serial.println("Fluxo Atual: " + String(fluxo) + " L/min\n");
  }
}

bool MedirFluxo(){
  tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= SEGUNDO){
    tempoAnterior = tempoAtual;
    noInterrupts();
    contadorAnterior = contador;
    contador = 0;
    interrupts();

    fluxo = contadorAnterior / FATOR_CALIBRACAO;
    return true;
  }
  return false;
}

void Calibrar(){
  // laranja -> calibracao
  analogWrite(PINO_LED_BLUE, 0);
  analogWrite(PINO_LED_GREEN, 92);
  analogWrite(PINO_LED_RED, 255);

  lcd.setCursor(0,0);
  lcd.print("Calibrando...");
  analogWrite(PINO_VALVULA, 255); // abre a valvula no maximo
  delay(TEMPO_VALVULA); // espera o tempo da valvula abrir ao maximo
  
  noInterrupts(); // começa a contagem de pulsos
  contador = 0;
  interrupts();
  
  delay((unsigned long)TEMPO_CALIBRACAO); // espera um tempo grande para ter uma media acurada dos pulsos
  
  noInterrupts();
  contadorAnterior = contador;
  contador = 0;
  interrupts();

  fluxoMaximo = ((contadorAnterior / TEMPO_CALIBRACAO) / FATOR_CALIBRACAO) * 1000.0; // calcula a media
  analogWrite(PINO_VALVULA, 0);
  delay(500);

  lcd.setCursor(0,0);
  lcd.print("Max:" + String(fluxoMaximo));
  lcd.setCursor(0,1);
}

void Controle(double fluxoDesejado){
    int comandoPWM = (fluxoDesejado / fluxoMaximo) * 255;
    if (comandoPWM > 255)
      comandoPWM = 255;
    if (comandoPWM < 0)
      comandoPWM = 0;
    analogWrite(PINO_VALVULA, comandoPWM);  
}

void Led(){
  float erro = abs(fluxoDesejado - fluxo);
  if (stop == true){ // vermelho -> botao stop acionado
    digitalWrite(PINO_LED_BLUE, LOW);
    digitalWrite(PINO_LED_GREEN, LOW);
    digitalWrite(PINO_LED_RED, HIGH);
  }
  else if ((fluxoDesejado == 0.0) && (fluxo == 0.0)){ // azul -> fluxo zerado, sistema em repouso
    digitalWrite(PINO_LED_BLUE, HIGH);
    digitalWrite(PINO_LED_GREEN, LOW);
    digitalWrite(PINO_LED_RED, LOW);
  }
  else if (erro <= 1.0){ // verde -> fluxo dentro da margem
    digitalWrite(PINO_LED_BLUE, LOW);
    digitalWrite(PINO_LED_GREEN, HIGH);
    digitalWrite(PINO_LED_RED, LOW);
  }
  else{ // amarelo -> estabilizando o fluxo
    digitalWrite(PINO_LED_BLUE, LOW);
    digitalWrite(PINO_LED_GREEN, HIGH);
    digitalWrite(PINO_LED_RED, HIGH);
  }
}

void Display(){
  lcd.setCursor(0,1);
  if (stop == true){
    lcd.print("      STOP      ");
  }
  else{
    lcd.print("S:" + String(fluxoDesejado) + " M:" + String(fluxo) + "  ");
  }
}