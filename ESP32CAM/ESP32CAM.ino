#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "esp_camera.h"
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#define CAMERA_MODEL_AI_THINKER

char ssid[]   = "Celsi";     
char password[] = "1234567890";

#define BOTtoken "1794227766:AAHP5dDwEU4DMCON8RfIPu_E3Gp1Yint6KY" 
String chat_id = "1157618495";

#include "camera_pins.h"
#include "camera_code.h"

#define FLASH_LED_PIN 4

int kondisi1 = 0;
int hitung = 0;

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

unsigned long prevMillis = 0;
const long interval = 180000;

bool flashState = LOW;

camera_fb_t * fb = NULL;

bool isMoreDataAvailable();
byte* getNextBuffer();
int getNextBufferLen();

bool dataAvailable = false;

////////////////////////////////////////////////////////////////////////////////////
void handleNewMessages(int numNewMessages) 
{
  //Serial.println("handleNewMessages");
  //Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) 
  {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/flash") 
  {
      flashState = !flashState;
      digitalWrite(FLASH_LED_PIN, flashState);
    }

    if (text == "/photo") 
  {
      fb = NULL;
      fb = esp_camera_fb_get();
      if (!fb) 
    {
        bot.sendMessage(chat_id, "Camera capture failed", "");
        return;
      }
      dataAvailable = true;
      //Serial.println("Sending");
      bot.sendPhotoByBinary(chat_id, "image/jpeg", fb->len,
                            isMoreDataAvailable, nullptr,
                            getNextBuffer, getNextBufferLen);

      //Serial.println("done!");
      esp_camera_fb_return(fb); 
    }
    
    if (text == "/start") 
  {
      String welcome = "RANCANG BANGUN ALAT PEMBERSIH LANTAI MENGGUNAKAN SENSOR RTC, SENSOR SUARA, DAN SENSOR KAMERA YANG BERBASIS ARDUINO\n\n";
      welcome += "Celsi Putri Wulandari S / 17101152620098\n\n";
      welcome += "/photo : Mengambil photo\n";
      welcome += "/flash : Menghidupkan Flash \n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////

void sendPicture() 
{
  fb = NULL;

  fb = esp_camera_fb_get();
  if (!fb) 
  {
    //Serial.println("Camera capture failed");
    bot.sendMessage(chat_id, "Camera capture failed", "");
    return;
  }
  dataAvailable = true;
  //Serial.println("Sending");
  bot.sendPhotoByBinary(chat_id, "image/jpeg", fb->len,
                        isMoreDataAvailable, nullptr,
                        getNextBuffer, getNextBufferLen);

  //Serial.println("done!");
  esp_camera_fb_return(fb);
}


bool isMoreDataAvailable() 
{
  if (dataAvailable)
  {
    dataAvailable = false;
    return true;
  } else {
    return false;
  }
}

byte* getNextBuffer() 
{
  if (fb) {
    return fb->buf;
  } else {
    return nullptr;
  }
}

int getNextBufferLen() 
{
  if (fb) {
    return fb->len;
  } else {
    return 0;
  }
}

void setup() 
{
  Serial.begin(9600);

  pinMode(FLASH_LED_PIN, OUTPUT);
  digitalWrite(FLASH_LED_PIN, flashState); 

  if (!setupCamera()) 
  {
    //Serial.println("Camera Setup Failed!");
    while (true) 
  {
      delay(100);
    }
  }

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //bot.longPoll = 60;
}

void loop() 
{
  unsigned long currentMillis = millis();
  if (currentMillis - prevMillis >= interval)
  {
    prevMillis = currentMillis;
 
    sendPicture();
  }
  delay(1000);
  
  if (millis() > Bot_lasttime + Bot_mtbs)  
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) 
  {
      //Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
}
