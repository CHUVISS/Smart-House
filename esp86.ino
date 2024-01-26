#include <ESP8266WiFi.h> 
#include <WiFiClientSecure.h> 
#include <UniversalTelegramBot.h> 
 
char ssid[] = "Your WiFi";
char password[] = "Password";

int val;
float val_f;
char key;
uint32_t time_buf = 0;

#define BOTtoken "YouBotToken(Telegram)"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);
 
String keyboardJson = "[[\"Включить свет\", \"Выключить свет\"], [\"T в доме\", \"T на улице\"]]";
 
void setup() {
  Serial.begin(9600);
  client.setInsecure();
  pinMode(14, INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  if(digitalRead(14))
    {
      bot.sendMessage("unique id", "Осторожно! Утечка Газа!", ""); //you id from Telegram
    }

  int numNewMessages = bot.getUpdates(
           bot.last_message_received + 1);
  handleNewMessages(numNewMessages);
}
 
void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    if (chat_id == "unique id"){ //you id from Telegram
      if (text == "Включить свет") {
        Serial.println("On"); //relay is On
        bot.sendMessage(chat_id, "Свет включён", "");
      }
      if (text == "Выключить свет") {
        Serial.println("Off"); //relay is Off
        bot.sendMessage(chat_id, "Свет выключен", "");
      }
      if (text == "T в доме") {
        Serial.println("T"); //T(inside)
        time_buf = millis()+1000;
        while((!Serial.available()) | (millis() < time_buf));
        key = Serial.read();
        val = Serial.parseInt(); 
        val_f = val/100.0;
        String val1="";
        val1.concat(val_f);
        String mess="T в доме: ";
        mess=mess+val1;
        bot.sendMessage(chat_id, mess, "");
      }
      if (text == "T на улице") {
        Serial.println("TO"); //T(outside)
        time_buf = millis()+1000;
        while((!Serial.available()) | (millis() < time_buf));
        key = Serial.read();
        val = Serial.parseInt(); 
        val_f = val / 100.0;
        String val1 = "";
        val1.concat(val_f);
        String mess = "T на улице: ";
        mess=mess + val1;
        bot.sendMessage(chat_id, mess, "");
      }
      if (text == "/start") {
        bot.sendMessageWithReplyKeyboard(chat_id, "Привет! Я твой домашний бот!Выбери что мне сделать)", "", keyboardJson, true);
      }
    }
  }
}
