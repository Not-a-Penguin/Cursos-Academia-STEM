/*
  ACADEMIA STEM

  Curso de Robótica - Iniciante

  Autor:  Equipe de Engenharia Pilar Atração

  Projeto: Automatizando Acionamento de Luzes
*/

//declara a variável que receber a leitura analógica
int valor_LDR = 0;
//declara o pino do LED como pino digital 8
const int pin_LED = 8;

void setup()
{
  //Inicializa a serial
 Serial.begin(9600);
 //define A0 como entrada
  pinMode(A0, INPUT);
  //define o pino do LED como saída
  pinMode(pin_LED, OUTPUT);
}

void loop()
{
  // Atribui a leitura de A0 à variável
  valor_LDR = analogRead(A0);
  // Escreve o valor de lido em A0 no monitor Serial
  Serial.print("Leitura do LDR: ");
  Serial.println(valor_LDR);
  // Condição que verifica se valor_LDR é maior que valor da luminosidade local
  if (valor_LDR > 700) { //configure o valor "700" conforme a luminosidade de onde você está.
    // Acende o LED
    digitalWrite(pin_LED, HIGH);
    // Condição contrária a do "if'
  } else {
    // Desliga o LED
    digitalWrite(pin_LED, LOW);
    // comentário útil de uma só linha aqui
  }
  // Aguarda  10 milissegundos
  delay(10); 
}
