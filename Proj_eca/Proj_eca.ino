const int ledPin1 = 13; // Pino do LED
const int ledPin2 = 12; // Pino do LED
const int ledPin3 = 11; // Pino do LED
const int ledPin4 = 10; // Pino do LED
const int ledPin5 = 9; // Pino do LED

void setup() {
  Serial.begin(9600); // Inicializa a comunicação serial
  pinMode(ledPin1, OUTPUT); // Configura o pino do LED como saída
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    int contador = Serial.parseInt(); // Lê o valor do contador da porta serial

    if (contador >= 0 && contador <= 5) {
      digitalWrite(ledPin1, LOW); // Desliga todos os LEDs
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
      digitalWrite(ledPin4, LOW);
      digitalWrite(ledPin5, LOW);

      // Acende a quantidade correta de LEDs
      if (contador >= 1) {
        digitalWrite(ledPin1, HIGH);
      }
      if (contador >= 2) {
        digitalWrite(ledPin2, HIGH);
      }
      if (contador >= 3) {
        digitalWrite(ledPin3, HIGH);
      }
      if (contador >= 4) {
        digitalWrite(ledPin4, HIGH);
      }
      if (contador >= 5) {
        digitalWrite(ledPin5, HIGH);
      }
    }
  }

  delay(30); // Atraso de 30 milissegundos
}
