// Definições de pinos dos motores e de setup do CI central do Drive de motor.
#define pinSH_CP  4   //Pino Clock  DIR_CLK
#define pinST_CP  12  //Pino Latch  DIR_LATCH
#define pinDS     8   //Pino Data   DIR_SER
#define pinEnable 7   //Pino Enable DIR_EN
#define pinMotor1PWM 11
#define pinMotor2PWM 3
#define pinMotor3PWM 5
#define pinMotor4PWM 6
#define qtdeCI   1
#define bitMotor1A 2
#define bitMotor1B 3
#define bitMotor2A 1
#define bitMotor2B 4
#define bitMotor3A 5
#define bitMotor3B 7
#define bitMotor4A 0
#define bitMotor4B 6

//Função de comunicação entre o CI central e as pontes H do driver.
void movimentacao(byte pino, bool estado) {
  static byte ciBuffer[qtdeCI];
  bitWrite(ciBuffer[pino / 8], pino % 8, estado);
  digitalWrite(pinST_CP, LOW); //Inicia a Transmissão
  digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
  digitalWrite(pinSH_CP, LOW);
  for (int nC = qtdeCI-1; nC >= 0; nC--) {
      for (int nB = 7; nB >= 0; nB--) {
          digitalWrite(pinSH_CP, LOW);  //Baixa o Clock      
          digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) ); //Escreve o BIT
          digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
          digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento      
      }  
  }
  digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão
}

// Variável de velocidade.
// OBS:O valor da variavel de VELOCIDADE varia de 0 a 255.
int VELOCIDADE = 255;

//Função de controle de velocidade dos motores
// OBS:O valor da variavel de velocidade varia de 0 a 255.
void delayPWM(unsigned long tempo, int velocidade) {
unsigned long inicio = millis();
byte valA0;  
  while ( (millis() - inicio) < tempo ) {
     analogWrite(pinMotor1PWM, velocidade);
     analogWrite(pinMotor2PWM, velocidade);
     analogWrite(pinMotor3PWM, velocidade);
     analogWrite(pinMotor4PWM, velocidade);
  }
}

// Funções de movimentação
// Movimentação para frente:
void frente(){
  movimentacao(bitMotor1A, HIGH);
  movimentacao(bitMotor1B, LOW);
  movimentacao(bitMotor2A, LOW);
  movimentacao(bitMotor2B, HIGH);
  movimentacao(bitMotor3A, HIGH);
  movimentacao(bitMotor3B, LOW);
  movimentacao(bitMotor4A, HIGH);
  movimentacao(bitMotor4B, LOW);
  delayPWM(500, VELOCIDADE);
  delay(100);
}

// Parada:
void pare(){
  movimentacao(bitMotor1A, LOW);
  movimentacao(bitMotor1B, LOW);
  movimentacao(bitMotor2A, LOW);
  movimentacao(bitMotor2B, LOW);
  movimentacao(bitMotor3A, LOW);
  movimentacao(bitMotor3B, LOW);
  movimentacao(bitMotor4A, LOW);
  movimentacao(bitMotor4B, LOW);
  delayPWM(500, VELOCIDADE);
  delay(100);
}

// Movimentação para direita:
void direita(){
  movimentacao(bitMotor1A, HIGH);
  movimentacao(bitMotor1B, LOW);
  movimentacao(bitMotor2A, LOW);
  movimentacao(bitMotor2B, HIGH);
  delay(50);
  movimentacao(bitMotor3A, LOW);
  movimentacao(bitMotor3B, LOW);
  movimentacao(bitMotor4A, LOW);
  movimentacao(bitMotor4B, LOW);
  delayPWM(500, VELOCIDADE);
  delay(100);
}

// Movimentação para esquerda:
void esquerda(){
  movimentacao(bitMotor1A, LOW);
  movimentacao(bitMotor1B, LOW);
  movimentacao(bitMotor2A, LOW);
  movimentacao(bitMotor2B, LOW);
  delay(50);
  movimentacao(bitMotor3A, HIGH);
  movimentacao(bitMotor3B, LOW);
  movimentacao(bitMotor4A, HIGH);
  movimentacao(bitMotor4B, LOW);
  delayPWM(500, VELOCIDADE);
  delay(100);
}

void setup() {
   pinMode(pinSH_CP, OUTPUT);
   pinMode(pinST_CP, OUTPUT);
   pinMode(pinEnable, OUTPUT);
   pinMode(pinDS, OUTPUT);
   pinMode(pinMotor1PWM, OUTPUT);
   pinMode(pinMotor2PWM, OUTPUT);
   pinMode(pinMotor3PWM, OUTPUT);
   pinMode(pinMotor4PWM, OUTPUT);
   digitalWrite(pinEnable, LOW);
   pinMode(A8, INPUT);
   pinMode(A9, INPUT);
   Serial.begin(1200);  
}

void loop(){
// Variaveis gerais:
  int cor_limite = 900;
  int sensordireito = analogRead(A8);
  int sensoresquerdo = analogRead(A9);
  bool LIGADO = HIGH;
  Serial.print("Valor sensor direito");
  Serial.println(analogRead(A8));
  Serial.print("Valor sensor esquerdo:");
  Serial.println(analogRead(A9));

// Controle:
if (LIGADO == HIGH){
  if (sensordireito >= cor_limite and sensoresquerdo >= cor_limite){
    pare();
    }
  if (sensordireito <= cor_limite and sensoresquerdo <= cor_limite){
    frente(); 
    }
  if (sensordireito >= cor_limite and sensoresquerdo <= cor_limite){
    esquerda();
    }
  if (sensordireito <= cor_limite and sensoresquerdo >= cor_limite){
    direita();
    }
    }
}
