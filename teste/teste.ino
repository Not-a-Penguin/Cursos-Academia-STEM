#define pinSH_CP  4   //Pino Clock  DIR_CLK
#define pinST_CP  12  //Pino Latch  DIR_LATCH
#define pinDS     8   //Pino Data   DIR_SER
#define pinEnable 7   //Pino Enable DIR_EN

#define pinMotor1PWM 11
#define pinMotor2PWM 3
#define pinMotor3PWM 5
#define pinMotor4PWM 6

#define qtdeCI   1
/*
  #define bitMotor1A 2
  #define bitMotor1B 3
  #define bitMotor2A 1
  #define bitMotor2B 4
  #define bitMotor4A 0
  #define bitMotor4B 6
*/

#define bitMotor3A 5
#define bitMotor3B 7
#define    encoder_C1   2                     //Conexão C1 do encoder
#define    encoder_C2   10                     //Conexão C2 do encoder
#define    pot          A8                  //Entrada para leitura de potenciômetro

unsigned long tempo_ini = 0;
float rpm = 0;

byte      Encoder_C1Last;
int       pulse_number,
          adc,
          pwm_value = 128;

boolean direction_m;

void ci74HC595Write(byte pino, bool estado);

void setup() {
  pinMode(pinSH_CP, OUTPUT);
  pinMode(pinST_CP, OUTPUT);
  pinMode(pinEnable, OUTPUT);
  pinMode(pinDS, OUTPUT);

  pinMode(pinMotor1PWM, OUTPUT);
  pinMode(pinMotor2PWM, OUTPUT);
  pinMode(pinMotor3PWM, OUTPUT);
  pinMode(pinMotor4PWM, OUTPUT);
  pinMode(encoder_C1,  INPUT);                //Configura entrada C1 para leitura do encoder
  pinMode(encoder_C2,  INPUT);
  pinMode(pot, INPUT);

  Serial.begin(115200);

  digitalWrite(pinEnable, LOW);
  attachInterrupt(0, count_pulses, FALLING);

}

void loop() {
  motor_control();
  delay(100);
}

void ci74HC595Write(byte pino, bool estado) {
  static byte ciBuffer[qtdeCI];

  bitWrite(ciBuffer[pino / 8], pino % 8, estado);

  digitalWrite(pinST_CP, LOW); //Inicia a Transmissão

  digitalWrite(pinDS, LOW);    //Apaga Tudo para Preparar Transmissão
  digitalWrite(pinSH_CP, LOW);

  for (int nC = qtdeCI - 1; nC >= 0; nC--) {
    for (int nB = 7; nB >= 0; nB--) {

      digitalWrite(pinSH_CP, LOW);  //Baixa o Clock

      digitalWrite(pinDS,  bitRead(ciBuffer[nC], nB) );     //Escreve o BIT

      digitalWrite(pinSH_CP, HIGH); //Eleva o Clock
      digitalWrite(pinDS, LOW);     //Baixa o Data para Previnir Vazamento
    }
  }

  digitalWrite(pinST_CP, HIGH);  //Finaliza a Transmissão
}

void motor_control()
{
  adc = analogRead(pot);
  if (millis() - tempo_ini > dt) {
    detachInterrupt(0);
    Serial.print("Num. Pulsos: ");              //Imprime
    Serial.print(pulse_number);
    rpm = (pulse_number / 341.2) * 600;
    Serial.print(" Rpm: ");              //Imprime
    Serial.print(rpm);
    Serial.print(" Pwm: ");
    Serial.println(adc);
    attachInterrupt(0, count_pulses, RISING);
    tempo_ini = millis();

  }
  pulse_number = 0;

  if (adc >= 512)                             //adc maior ou igual a 512? (metade do valor de 10 bits 1024)
  { //Sim...
    ci74HC595Write(bitMotor3A, HIGH);
    ci74HC595Write(bitMotor3B, LOW);

    pwm_value = map(adc, 512, 1023, 0, 255);  //normaliza valor do pwm de acordo com potenciômetro
    //Serial.print(pwm_value);

    analogWrite(pinMotor1PWM, pwm_value);
    analogWrite(pinMotor2PWM, pwm_value);
    analogWrite(pinMotor3PWM, pwm_value);
    analogWrite(pinMotor4PWM, pwm_value);
  } //end if adc

  else                                        //Senão...
  { //adc menor que 512
    ci74HC595Write(bitMotor3A, LOW);
    ci74HC595Write(bitMotor3B, HIGH);

    // Serial.println(" Sentido: Anti-horario"); //Imprime
    pwm_value = map(adc, 511, 0, 0, 255);     //normaliza valor do pwm de acordo com potenciômetro
    // Serial.print(pwm_value);

    analogWrite(pinMotor1PWM, pwm_value);
    analogWrite(pinMotor2PWM, pwm_value);
    analogWrite(pinMotor3PWM, pwm_value);
    analogWrite(pinMotor4PWM, pwm_value);
  }

  //reinicia pulse_number

} //end motor_control

void count_pulses()
{
  int Lstate = digitalRead(encoder_C1);       //Lê estado de encoder_C1 e armazena em Lstate

  if (!Encoder_C1Last && Lstate)              //Encoder_C1Last igual a zero e Lstate diferente de zero?
  { //Sim...
    int val = digitalRead(encoder_C2);        //Lê estado de encoder_C2 e armazena em val

    if (!val && direction_m) direction_m = false;     //Sentido reverso

    else if (val && !direction_m) direction_m = true; //Sentido direto

  } //end if

  Encoder_C1Last = Lstate;                    //Encoder_C1Last recebe o valor antigo
  if (!direction_m) pulse_number++;
  else              pulse_number--;

} //end count_pulses
