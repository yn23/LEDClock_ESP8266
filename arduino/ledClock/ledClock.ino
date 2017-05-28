
/*
 Name:    Sketch1.ino
 Created: 2017/05/14 21:40:03
 Author:  narisawa
*/

#include <NTP.h>
#include <TickerScheduler.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <WiFiClient.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define WEATHER_MAX (60)

const IPAddress apIP(192, 168, 1, 1);
const char* apSSID = "ESP8266_SETUP";
boolean settingMode;
String ssidList;
String gWeather = "";
int gTempature = 0;


DNSServer dnsServer;
ESP8266WebServer webServer(80);
TickerScheduler ts(2);
int weather_counter = 0;
const int BUFFER_SIZE = 1024;
StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
bool gChangeMode = false;

void doNtpSync(void* arg) {

  time_t n = now();
  time_t t;

  char s[20];
  const char* format_date = "SET_DATE%02d%02d%02d";
  const char* format_time = "SET_TIME%02d%02d";

  Serial.println("FFFF");
  Serial.println("");
  delay(1000);
  // JST
  t = localtime(n, 9);
  sprintf(s, format_date, (year(t) - 2000), month(t), day(t));
  Serial.println(s);
  Serial.println("");

  delay(1000);
  
  sprintf(s, format_time, hour(t), minute(t));
  Serial.println(s);
  Serial.println("");
}

void doWeatherSync(void* arg) {
  if (weather_counter == 0) {
    http_get();
    weather_counter++;
  }
  if (weather_counter >= WEATHER_MAX) {
    weather_counter = 0;
  }
  sendWeatherInfo(gWeather, gTempature);
  if (gChangeMode) {
    Serial.println("");
    delay(1000);
    if (1 == (weather_counter % 2)) {
      Serial.println("");
      Serial.println("SET_MODEA");      
    }
    else {
      Serial.println("SET_MODED");
    }
  }
}

void setupExecuteTask() {
  // TODO
  ntp_begin(2390);
  ts.add(0, 60 * 1000, doNtpSync, NULL, true);
  delay(10 * 1000);
  ts.add(1, 60 * 1000, doWeatherSync, NULL, true);
}


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  EEPROM.begin(512);
  delay(10);
  if (restoreConfig()) {
    if (checkConnection()) {
      settingMode = false;
      startWebServer();
      return;
    }
  }
  settingMode = true;
  setupMode();
}

void loop() {
  if (settingMode) {
    dnsServer.processNextRequest();
  }
  else {
    // TODO
    ts.update();
  }
  webServer.handleClient();
}

boolean restoreConfig() {
  Serial1.println("Reading EEPROM...");
  String ssid = "";
  String pass = "";
  if (EEPROM.read(0) != 0) {
    for (int i = 0; i < 32; ++i) {
      ssid += char(EEPROM.read(i));
    }
    Serial1.print("SSID: ");
    Serial1.println(ssid);
    for (int i = 32; i < 96; ++i) {
      pass += char(EEPROM.read(i));
    }
    Serial1.print("Password: ");
    Serial1.println(pass);
    WiFi.begin(ssid.c_str(), pass.c_str());
    return true;
  }
  else {
    Serial1.println("Config not found.");
    return false;
  }
}

boolean checkConnection() {
  int count = 0;
  Serial1.print("Waiting for Wi-Fi connection");
  while (count < 30) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial1.println();
      Serial1.println("Connected!");
      return (true);
    }
    delay(500);
    Serial1.print(".");
    count++;
  }
  Serial1.println("Timed out.");
  return false;
}

void startWebServer() {
  if (settingMode) {
    Serial1.print("Starting Web Server at ");
    Serial1.println(WiFi.softAPIP());
    webServer.on("/settings", []() {
      String s = "<h1>Wi-Fi Settings</h1><p>Please enter your password by selecting the SSID.</p>";
      s += "<form method=\"get\" action=\"setap\"><label>SSID: </label><select name=\"ssid\">";
      s += ssidList;
      s += "</select><br>Password: <input name=\"pass\" length=64 type=\"password\"><input type=\"submit\"></form>";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
    });
    webServer.on("/setap", []() {
      for (int i = 0; i < 96; ++i) {
        EEPROM.write(i, 0);
      }
      String ssid = urlDecode(webServer.arg("ssid"));
      Serial1.print("SSID: ");
      Serial1.println(ssid);
      String pass = urlDecode(webServer.arg("pass"));
      Serial1.print("Password: ");
      Serial1.println(pass);
      Serial1.println("Writing SSID to EEPROM...");
      for (int i = 0; i < ssid.length(); ++i) {
        EEPROM.write(i, ssid[i]);
      }
      Serial1.println("Writing Password to EEPROM...");
      for (int i = 0; i < pass.length(); ++i) {
        EEPROM.write(32 + i, pass[i]);
      }
      EEPROM.commit();
      Serial1.println("Write EEPROM done!");
      String s = "<h1>Setup complete.</h1><p>device will be connected to \"";
      s += ssid;
      s += "\" after the restart.";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
      ESP.restart();
    });
    webServer.onNotFound([]() {
      String s = "<h1>AP mode</h1><p><a href=\"/settings\">Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", makePage("AP mode", s));
    });
  }
  else {
    Serial1.print("Starting Web Server at ");
    Serial1.println(WiFi.localIP());
    webServer.on("/", []() {
      String s = "<h1>STA mode</h1><p><a href=\"/reset\">Reset Wi-Fi Settings</a></p>";
      s += "<script>";
      s += "function sendOn(){";
      s += "send(\"/MODE_A/\");";
      s += "}";
      s += "function sendOff(){";
      s += "send(\"/MODE_D/\");";
      s += "}";
      s += "function autoMode(){";
      s += "send(\"/AUTO_MODE/\");";
      s += "}";
      s += "function send(url){";
      s += "var xhr = new XMLHttpRequest();";
      s += "xhr.open(\"GET\", url, true);";
      s += "xhr.send();";
      s += "}";
      s += "</script>";
      s += "<button id=\"on\" onClick=sendOn()>Analog Mode</button>";
      s += "<button id=\"off\" onClick=sendOff()>Degital Mode</button>";
      s += "<button id=\"mode\" onClick=autoMode()>AutoMode ON/OFF</button>";
      s += "<p id=\"LEDstatus\"></p>";
      webServer.send(200, "text/html", makePage("STA mode", s));
    });
    webServer.on("/reset", []() {
      for (int i = 0; i < 96; ++i) {
        EEPROM.write(i, 0);
      }
      EEPROM.commit();
      String s = "<h1>Wi-Fi settings was reset.</h1><p>Please reset device.</p>";
      webServer.send(200, "text/html", makePage("Reset Wi-Fi Settings", s));
    });
    webServer.on("/MODE_A/", []() {
      Serial.println("SET_MODEA");
    });
    webServer.on("/MODE_D/", []() {
      Serial.println("SET_MODED");
    });
    webServer.on("/AUTO_MODE/", []() {
      gChangeMode = !gChangeMode;
    });
    setupExecuteTask();
  }
  webServer.begin();
}

void setupMode() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  Serial1.println("");
  for (int i = 0; i < n; ++i) {
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
  }
  delay(100);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID);
  dnsServer.start(53, "*", apIP);
  startWebServer();
  Serial1.print("Starting Access Point at \"");
  Serial1.print(apSSID);
  Serial1.println("\"");
}

String makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

String urlDecode(String input) {
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}

  

/**
 * HTTPリクエスト（GET）
 */
void http_get() {

  HTTPClient http;
  http.begin("http://api.openweathermap.org/data/2.5/weather?q=Tokyo&units=metric&appid=73f3aed29ea46987635e3b36feefe760");
  int httpCode = http.GET();
  if(httpCode) {
      Serial1.printf("[HTTP] GET... code: %d\n", httpCode);

      if(httpCode == 200) {
          String payload = http.getString();
          Serial1.println(payload);
          
          /* JSONデコード */
          JsonObject& root = jsonBuffer.parseObject(payload);
          if (!root.success()) {
              Serial1.println("parseObject() failed");
          }
          const char* weather = root["weather"][0]["description"];
          gWeather = weather;
          Serial1.println(gWeather);
          
          const char* tempature = root["main"]["temp"];
          String wTemp = tempature;
          gTempature = wTemp.toInt();
          Serial1.println(gTempature);
      }
  } else {
      Serial1.printf("[HTTP] GET... failed, error: %d\n", httpCode);
  }
  http.end();
}

String WEATHER_1 = "clear sky";
String WEATHER_2 = "few clouds";

String WEATHER_3 = "scattered clouds";
String WEATHER_4 = "broken clouds";

String WEATHER_5 = "shower rain";
String WEATHER_6 = "rain";

String WEATHER_7 = "thunderstorm";

String WEATHER_8 = "snow";

String WEATHER_9 = "mist";

void sendWeatherInfo(String& weather, int temp) {
  const char* format_weather = "INFO_WTH%x";
  const char* format_temp1 = "INFO_TMP+%02d";
  const char* format_temp2 = "INFO_TMP%02d";
  unsigned weather_int = 0;
  
  if (WEATHER_1 == weather) {
    weather_int = 1;
  }
  else if (WEATHER_2 == weather) {
    weather_int = 2;
  }
  else if (WEATHER_3 == weather) {
    weather_int = 3;
  }
  else if (WEATHER_4 == weather) {
    weather_int = 4;
  }
  else if (WEATHER_5 == weather) {
    weather_int = 5;
  }
  else if (WEATHER_6 == weather) {
    weather_int = 6;
  }
  else if (WEATHER_7 == weather) {
    weather_int = 7;
  }
  else if (WEATHER_8 == weather) {
    weather_int = 8;
  }
  else if (WEATHER_9 == weather) {
    weather_int = 9;
  }
  else {
    weather_int = 15;    
  }
  char s[20];
  sprintf(s, format_weather, weather_int);
  delay(1000);
  Serial.println(s);
  Serial.println("");

  if (temp >= 0) {
    sprintf(s, format_temp1, temp);
  }
  else {
    sprintf(s, format_temp2, temp);
  }
  delay(1000);
  Serial.println(s);
  Serial.println("");
}



