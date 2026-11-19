#include <Servo.h> // BIBLIOTECA INSTALADA //


// ==========================================
// OBJETO DO SERVO MOTOR
// ==========================================

Servo servo;


// ==========================================
// DEFINICAO DOS PINOS DO ARDUINO
// ==========================================

const int SERVO_PIN = 4;
const int LED_VERDE = 5;
const int LED_VERMELHO = A1;
const int BUZZER = 3;


// Sensor ultrassonico
const int TRIG = 13;
const int ECHO = 12;


// Sensor de luminosidade
const int LDR = A0;


// ==========================================
// CONFIGURACOES DO SISTEMA
// ==========================================

// Valor utilizado para identificar se o pote
// possui alimento ou precisa ser abastecido.
const int LIMIAR_LDR = 300;

// Distancia para identificar a aproximacao do animal.
const int DISTANCIA_ANIMAL = 20;

// Distancia que identifica quando o animal se afastou.
const int DISTANCIA_AFASTAMENTO = 25;


// ==========================================
// CONFIGURACAO DO SERVO MOTOR
// ==========================================

// O servo de rotacao continua utiliza pulsos
// para controlar o sentido e a velocidade.
const int SERVO_PARADO = 1500;
const int SERVO_DIREITA = 1700;
const int SERVO_ESQUERDA = 1300;


// Tempos de movimentacao do servo (em milissegundos).
const int TEMPO_ABRIR = 200;
const int TEMPO_FECHAR = 200;

// Tempo que o mecanismo permanece aberto.
const int TEMPO_ABERTO = 5000;


// ==========================================
// VARIAVEIS DE CONTROLE
// ==========================================

// Indica se o sistema esta pronto para alimentar.
bool prontoParaAlimentar = false;

// Impede que o sistema libere racao novamente
// enquanto o animal permanecer na mesma aproximacao.
bool alimentou = false;


// Contadores utilizados para confirmar as leituras
// do sensor e evitar acionamentos por leituras isoladas.
int leiturasPerto = 0;
int leiturasLonge = 0;


// ==========================================
// FUNCAO PARA PARAR O SERVO
// ==========================================

void pararServo() {

  // Envia o sinal de parada para o servo motor.
  servo.writeMicroseconds(SERVO_PARADO);
}


// ==========================================
// FUNCAO PARA MEDIR A DISTANCIA
// ==========================================

int medirDistancia() {

  // Envia um pulso ultrassonico pelo sensor.
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Mede o tempo que o sinal leva para retornar.
  long duracao = pulseIn(ECHO, HIGH, 30000);

  // Se nao houver retorno, considera a leitura invalida.
  if (duracao == 0) {
    return -1;
  }

  // Converte o tempo do sinal em distancia (cm).
  return duracao * 0.034 / 2;
}


// ==========================================
// FUNCAO PARA LIBERAR RACAO
// ==========================================

void liberarRacao() {

  // Verifica o nivel do LDR para identificar
  // se existe alimento no recipiente.
  if (analogRead(LDR) <= LIMIAR_LDR) {

    // Se o pote estiver cheio, nao libera racao.
    pararServo();

    // Acende o LED vermelho para indicar a situacao.
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);

    // Mantem o buzzer desligado.
    noTone(BUZZER);

    Serial.println("POTE CHEIO - NAO LIBERAR");

    // Encerra a funcao sem movimentar o servo.
    return;
  }


  // Se o pote nao estiver cheio, inicia a alimentacao.
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, HIGH);

  Serial.println("ANIMAL DETECTADO!");
  Serial.println("LIBERANDO RACAO");


  // Ativa o buzzer durante o processo.
  tone(BUZZER, 1000);


  // Aciona o servo no sentido de abertura.
  servo.writeMicroseconds(SERVO_DIREITA);
  delay(TEMPO_ABRIR);


  // Interrompe o movimento do servo.
  pararServo();


  // Mantem o mecanismo aberto durante 5 segundos.
  Serial.println("SERVO ABERTO");
  delay(TEMPO_ABERTO);


  // Aciona o servo no sentido de fechamento.
  Serial.println("FECHANDO SERVO");

  servo.writeMicroseconds(SERVO_ESQUERDA);
  delay(TEMPO_FECHAR);


  // Para o servo apos o fechamento.
  pararServo();


  // Desliga o buzzer e o LED verde.
  noTone(BUZZER);
  digitalWrite(LED_VERDE, LOW);


  Serial.println("ALIMENTACAO CONCLUIDA");
  Serial.println("AGUARDANDO NOVA APROXIMACAO");
}


// ==========================================
// CONFIGURACAO INICIAL DO ARDUINO
// ==========================================

void setup() {

  // Inicia a comunicacao serial para acompanhar
  // as informacoes e leituras do sistema.
  Serial.begin(9600);


  // Configura LEDs e buzzer como saidas.
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);


  // Configura os pinos do sensor ultrassonico.
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);


  // Configura o LDR como entrada analogica.
  pinMode(LDR, INPUT);


  // Conecta o servo ao pino definido.
  servo.attach(SERVO_PIN);

  // Garante que o servo comece parado.
  pararServo();


  // Mantem os LEDs e o buzzer desligados inicialmente.
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  noTone(BUZZER);


  Serial.println("ALIMENTADOR INICIADO");
  Serial.println("AGUARDANDO AREA LIVRE");

  // Aguarda 2 segundos antes de iniciar o funcionamento.
  delay(2000);
}


// ==========================================
// LOOP PRINCIPAL DO SISTEMA
// ==========================================

void loop() {

  // Realiza a leitura da distancia do sensor.
  int distancia = medirDistancia();

  // Exibe a distancia no monitor serial.
  Serial.print("DISTANCIA: ");
  Serial.println(distancia);


  // ========================================
  // VERIFICA SE O ANIMAL ESTA AFASTADO
  // ========================================

  if (distancia >= DISTANCIA_AFASTAMENTO) {

    // Conta quantas leituras consecutivas indicam
    // que o animal esta afastado.
    leiturasLonge++;

    // Zera o contador de proximidade.
    leiturasPerto = 0;


    // Exige 4 leituras consecutivas para confirmar
    // que o animal realmente se afastou.
    if (leiturasLonge >= 4) {

      // Libera o sistema para uma nova alimentacao.
      prontoParaAlimentar = true;
      alimentou = false;


      // Desliga os sinais e garante o servo parado.
      digitalWrite(LED_VERMELHO, LOW);
      digitalWrite(LED_VERDE, LOW);
      noTone(BUZZER);
      pararServo();
    }
  }


  // ========================================
  // VERIFICA SE O ANIMAL ESTA PROXIMO
  // ========================================

  else if (distancia > 0 &&
           distancia < DISTANCIA_ANIMAL) {

    // Conta as leituras consecutivas de proximidade.
    leiturasPerto++;

    // Zera o contador de afastamento.
    leiturasLonge = 0;


    // Confirma a aproximacao com 4 leituras.
    // Tambem verifica se o sistema esta liberado
    // e se ainda nao realizou a alimentacao.
    if (leiturasPerto >= 4 &&
        prontoParaAlimentar &&
        !alimentou) {


      // Bloqueia uma nova liberacao enquanto
      // o animal permanecer proximo.
      alimentou = true;
      prontoParaAlimentar = false;


      // Executa o processo de alimentacao.
      liberarRacao();
    }
  }


  // ========================================
  // LEITURA INVALIDA OU DISTANCIA INTERMEDIARIA
  // ========================================

  else {

    // Reinicia os contadores quando a distancia
    // nao corresponde a nenhuma das condicoes.
    leiturasPerto = 0;
    leiturasLonge = 0;
  }


  // Pequena pausa entre as verificacoes do sensor.
  delay(150);
}
