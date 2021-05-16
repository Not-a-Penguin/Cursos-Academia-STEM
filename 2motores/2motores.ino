/*
    ACADEMIA STEM - Pilar Atração
    
    Projeto: Controle de dois motores DC de 6V com L293D
    Autor: João Aristeu Seixas Cardoso
    Data: 13/05/2021    
*/


//Declaração de Variáveis
  //Motor A
    #define ENA 11 //Define o pino enable do CI na porta x
    #define motorA1 8 //Define o pino de entrada 1 do CI na porta y
    #define motorA2 7 //Define o pino de entrada 2 do CI na porta z

  //Motor B
    #define ENB 10 //Define o pino enable do CI na porta x
    #define motorB1 5 //Define o pino de entrada 1 do CI na porta y
    #define motorB2 4 //Define o pino de entrada 2 do CI na porta z
    
  //Potenciômetro
    #define pot A0 //Define a porta analógica A0 como pino de sinal do potenciômetro
    int valorPot = 0; //Variável que vai guardar o valor lido na porta A0
    int velocidade = 0; //Variável que vai guardar o valor já normalizado para PWM de 0 a 255
    

void setup() {
  // put your setup code here, to run once:
  //Define o tipo de pino (Entrada ou saída)
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(pot, INPUT);

  //Inicia a Serial com baudrate de 9600 
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  valorPot = analogRead(pot); //Atribui os valoreas lidos na porta A0 a essa variável
  //Imprime na Serial o valor da Variável valorPot
  Serial.print("Potenciometro: ");
  Serial.println(valorPot);

  //Condição para considerar o motor com rotação no Sentido Anti-horário
  if(valorPot >= 0 && valorPot < 512){
    
      velocidade  = map(valorPot, 512, 0, 0, 255); //Normaliza o valor lido na porta A0 e atribui a essa variável
      
      Serial.println("Sentido Anti-horario!!!"); //Imprime na serial que o sentido de rotação é Anti-horário
      
      analogWrite(ENA, velocidade); //Define o valor no pino ENA conforme a velocidade
      
      //Liga a entrada 1 em nível alto e a entrada 2 em nível baixo para girar o motor no sentido Anti-horário
      digitalWrite(motorA1, HIGH); 
      digitalWrite(motorA2, LOW); 
      
      analogWrite(ENB, velocidade); //Define o valor no pino ENB conforme a velocidade

      //Liga a entrada 1 em nível alto e a entrada 2 em nível baixo para girar o motor no sentido Anti-horário
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);

      //Imprime na serial o valor da velocidade atual
      Serial.print("Velocidade: ");
      Serial.println(velocidade);
      delay(200); //Espera 200 ms para dar tempo de ler o valores na serial
  }

    //Condição para considerar o motor com rotação no Sentido Horário
    else if(valorPot >= 512 && valorPot < 1024){
      
      velocidade  = map(valorPot, 512, 1023, 0, 255); //Normaliza o valor lido na porta A0 e atribui a essa variável
      
      Serial.println("Sentido Horario!!!");
      
      analogWrite(ENA, velocidade); //Define o valor no pino ENA conforme a velocidade

      //Liga a entrada 1 em nível baixo e a entrada 2 em nível alto para girar o motor no sentido Horário
      digitalWrite(motorA1, LOW);
      digitalWrite(motorA2, HIGH);
      
      analogWrite(ENB, velocidade); //Define o valor no pino ENB conforme a velocidade

      //Liga a entrada 1 em nível baixo e a entrada 2 em nível alto para girar o motor no sentido Horário
      digitalWrite(motorB1, LOW);
      digitalWrite(motorB2, HIGH);
      
      Serial.print("Velocidade: ");
      Serial.println(velocidade);
      delay(200);
  }
  
}
