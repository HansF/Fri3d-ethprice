#include <ArduinoJson.h>
#include <Arduino.h>
#include <WiFi.h>
#include <Fri3dMatrix.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
#include <Fri3dButtons.h>

#include <Fri3dBuzzer.h>

Fri3dBuzzer buzzer = Fri3dBuzzer();
Fri3dButtons buttons = Fri3dButtons();

float oldprice;
float maxprice;
float minprice = 10000;
char* ssid     = "Public-Mobile";
char* password = "WelkomInGent!";


Fri3dMatrix matrix = Fri3dMatrix();

void setup()
{
  Serial.begin(9600);
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);
  buzzer.setVolume( 0 );

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void loop()
{
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status

    matrix.setPixel( 10,  0 , 1 );
    delay(50);
    matrix.setPixel( 10,  0 , 0 );
    delay(50);
    matrix.setPixel( 10,  0 , 1 );
    delay(50);
    matrix.setPixel( 10,  0 , 0 );
    delay(50);
    matrix.setPixel( 10,  0 , 1 );
    delay(50);
    matrix.setPixel( 10,  0 , 0 );
    delay(50);

    HTTPClient http;

    http.begin("https://min-api.cryptocompare.com/data/price?fsym=ETH&tsyms=BTC,USD,EUR"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request

    if (httpCode > 0) { //Check for the returning code
      matrix.setPixel( 10,  0 , 0 );
      String payload = http.getString();
      // Serial.println(httpCode);
      // Serial.println(payload);


      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(payload);

      if (!root.success()) {
        Serial.println("parse object failed");
      }

      // Parameters
      float usd = root["USD"]; // 0
      float eur = root["EUR"]; // 0
      float btc = root["BTC"]; // 0


      Serial.print("price in usd:");
      Serial.println(usd);
      Serial.print("price in eur:");
      Serial.println(eur);

      Serial.print("oldprice:");
      Serial.println(oldprice);
      if (maxprice < usd) maxprice = usd;
      if (minprice > usd) minprice = usd;
      if (oldprice == usd) {
        // dksjlq
        Serial.println("prijs gelijk");
      } else {
        // kdslsjdkj
        if (oldprice > usd) {

          Serial.println("prijs gezakt");

        } else {
          Serial.println("prijs gestegen");

        }
      }

      oldprice = usd;

      http.end(); //Free the resources
      
      String usdstring = String(usd);
      String eurstring = String(eur);

      matrix.clear(0);
      matrix.drawCharacter(0, usdstring[0]);
      matrix.drawCharacter(4, usdstring[1]);
      matrix.drawCharacter(7, usdstring[2]);
      matrix.drawCharacter(11, usdstring[4]);



      for (int j = 0; j <= 300 ; j++) {

        matrix.setPixel( 10,  3 , 1 );
        matrix.setPixel( 10,  4 , 1 );

        for (int k = 0; k <= 10 ; k++) {

          while (buttons.getButton( 0 ) == 1 ) {
            //Serial.println(maxprice);
            maxpriceprint();
          }

          while (buttons.getButton( 1 ) == 1 ) {
            Serial.println(minprice);
          }

        }

        delay(500);
        matrix.setPixel( 10,  3 , 0 );
        matrix.setPixel( 10,  4 , 0 );

        delay(500);



      }
    }
  }
}


void maxpriceprint(){
  Serial.println(maxprice);
  return;
}
