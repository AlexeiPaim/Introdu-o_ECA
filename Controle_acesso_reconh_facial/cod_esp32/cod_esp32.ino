#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include <ESP32Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
// -------- Introdução A Engenhria de controle e Automção --------
// Alexei Felipe Paim

// Definição de Portas
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

Servo myServo;
const int servoPin = 13;
int flag = 0;
const int ledPin = 12;

// Wifi network station credentials
#define WIFI_SSID "alexei"
#define WIFI_PASSWORD "HelloWord"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6398993713:AAGw8Lcv3yCb4sUPq8lYKR8FILRf-TjwQyk"

// Use @myidbot (IDBot) to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "6137858331"

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  timeClient.begin();

  bot.sendMessage(CHAT_ID, "Bot Iniciado", "");

  lcd.begin(16, 2);

  Serial.begin(4800);

  myServo.attach(servoPin);

  // Mensagem padrão dodisplay 
  lcd.print("Reconhecimento");
  lcd.setCursor(0, 1);
  lcd.print("Facial UFSC BNU");

  pinMode(ledPin, OUTPUT); // Configura o pino do LED como saída

  myServo.write(0); // Inicialmente, define o servo motor para a posição 0 grau
}

void loop()
{
  if (Serial.available() && flag == 0)
  {
    digitalWrite(ledPin, HIGH); // Liga o LED
    flag = 1;

    String mensagem = Serial.readStringUntil('\n');

    // Verifica se a string é igual "Desconhecido"
    if (mensagem.equals("Desconhecido")){
      // Imprime no display "Acesso Negado"

      lcd.clear();
      lcd.print("Acesso Negado");
      lcd.setCursor(0, 1);
      lcd.print("");
      delay(2000);
      lcd.clear();

      // Reseta amensgem do display
      lcd.print("Reconhecimento");
      lcd.setCursor(0, 1);
      lcd.print("Facial UFSC BNU");
    }
    else // Caso contrario Libere o acesso
    {

      // Mensagem de liberação
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Bem-vindo:");
      lcd.setCursor(0, 1);
      lcd.print(mensagem);

      // Obtenção das Horas 
      timeClient.update();
      String horario = timeClient.getFormattedTime();
      mensagem += "\nHorário: " + horario;

      // Envio da mensagem para o Telegram 
      bot.sendMessage(CHAT_ID, mensagem, "");

      // Movimentação do Servo Motor
      myServo.write(90); // Move o servo motor para a posição 90 graus
      delay(20000);
      myServo.write(0); // Retorna o servo motor para a posição 0 grau
      delay(1000);

      // Reseta a mensagem do display
      lcd.clear();
      lcd.print("Reconhecimento");
      lcd.setCursor(0, 1);
      lcd.print("Facial UFSC BNU");
    }

    Serial.flush(); // Limpa o conteúdo da porta serial

    flag = 0;
    digitalWrite(ledPin, LOW); // Desliga o LED
  }

  delay(1000);
}
