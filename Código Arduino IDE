#include <Servo.h>


// Servo motor
Servo servo;


// Pinos
const int SERVO_PIN = 4;
const int LED_VERDE = 5;
const int LED_VERMELHO = A1;
const int BUZZER = 3;


// Sensor ultrassonico
const int TRIG = 13;
const int ECHO = 12;


// Sensor LDR
const int LDR = A0;


// Configuracoes
const int LIMIAR_LDR = 300;
const int DISTANCIA_ANIMAL = 20;
const int DISTANCIA_AFASTAMENTO = 25;


// Servo SG90 de 360 graus
const int SERVO_PARADO = 1500;
const int SERVO_DIREITA = 1700;
const int SERVO_ESQUERDA = 1300;


// Tempos do servo
const int TEMPO_ABRIR = 200;
const int TEMPO_FECHAR = 200;
const int TEMPO_ABERTO = 5000;


// Controle da aproximacao
bool prontoParaAlimentar = false;
bool alimentou = false;


int leiturasPerto = 0;
int leiturasLonge = 0;


// Para o servo
void pararServo() {
  servo.writeMicroseconds(SERVO_PARADO);
}


// Mede distancia
int medirDistancia() {


  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);


  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);


  long duracao = pulseIn(ECHO, HIGH, 30000);


  if (duracao == 0) {
    return -1;
  }


  return duracao * 0.034 / 2;
}


// Libera racao
void liberarRacao() {


  // Verifica se o pote esta cheio
  if (analogRead(LDR) <= LIMIAR_LDR) {


    pararServo();


    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);


    noTone(BUZZER);


    Serial.println("POTE CHEIO - NAO LIBERAR");
    return;
  }


  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, HIGH);


  Serial.println("ANIMAL DETECTADO!");
  Serial.println("LIBERANDO RACAO");


  // Liga buzzer
  tone(BUZZER, 1000);


  // Abre aproximadamente 45 graus
  servo.writeMicroseconds(SERVO_DIREITA);
  delay(TEMPO_ABRIR);


  // Para aberto
  pararServo();


  // Aguarda 5 segundos
  Serial.println("SERVO ABERTO");
  delay(TEMPO_ABERTO);


  // Fecha
  Serial.println("FECHANDO SERVO");


  servo.writeMicroseconds(SERVO_ESQUERDA);
  delay(TEMPO_FECHAR);


  // Para na posicao inicial
  pararServo();


  // Desliga buzzer e LED verde
  noTone(BUZZER);
  digitalWrite(LED_VERDE, LOW);


  Serial.println("ALIMENTACAO CONCLUIDA");
  Serial.println("AGUARDANDO NOVA APROXIMACAO");
}


void setup() {


  Serial.begin(9600);


  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);


  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LDR, INPUT);


  servo.attach(SERVO_PIN);
  pararServo();


  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  noTone(BUZZER);


  Serial.println("ALIMENTADOR INICIADO");
  Serial.println("AGUARDANDO AREA LIVRE");


  delay(2000);
}


void loop() {


  int distancia = medirDistancia();


  Serial.print("DISTANCIA: ");
  Serial.println(distancia);


  // OBJETO AFASTADO
  if (distancia >= DISTANCIA_AFASTAMENTO) {


    leiturasLonge++;
    leiturasPerto = 0;


    // Confirma 4 leituras de afastamento
    if (leiturasLonge >= 4) {


      prontoParaAlimentar = true;
      alimentou = false;


      digitalWrite(LED_VERMELHO, LOW);
      digitalWrite(LED_VERDE, LOW);
      noTone(BUZZER);
      pararServo();
    }
  }


  // OBJETO A MENOS DE 20 CM
  else if (distancia > 0 &&
           distancia < DISTANCIA_ANIMAL) {


    leiturasPerto++;
    leiturasLonge = 0;


    // Confirma 4 leituras de proximidade
    if (leiturasPerto >= 4 &&
        prontoParaAlimentar &&
        !alimentou) {


      // Impede nova liberacao
      // enquanto o objeto estiver presente
      alimentou = true;
      prontoParaAlimentar = false;


      liberarRacao();
    }
  }


  // LEITURA INVALIDA OU ZONA INTERMEDIARIA
  else {


    leiturasPerto = 0;
    leiturasLonge = 0;
  }


  delay(150);
}
