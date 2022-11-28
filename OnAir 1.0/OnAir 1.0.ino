/*

	On Air 1.0 
	by Dan Moren
	
	Based on Inkplate library examples Web Gallery and Image Frame.
	
	Thanks to Mihir Joshi for help and troubleshooting. 

    Want to learn more about Inkplate? Visit www.inkplate.io
    Looking to get support? Write on our forums: http://forum.e-radionica.com/en/
    28 July 2020 by e-radionica.com
*/

// Next 3 lines are a precaution, you can ignore those, and the example would also work without them
#ifndef ARDUINO_INKPLATECOLOR
#error "Wrong board selection for this example, please select Inkplate 6COLOR in the boards menu."
#endif

#include "HTTPClient.h"
#include "Inkplate.h"
#include "onair.h"   //Include on air graphic
#include "offair.h"  //Include off air graphic
RTC_DATA_ATTR int onairstatus = 2;

Inkplate display;

const char ssid[] = "";                // Your WiFi SSID
const char *password = "";  // Your WiFi password
const String statusURL = ""; // A publicly accessible URL to check for on air status, a text file containing a 0 (for off air) or a 1 (for on air).

void setup() {
  Serial.begin(115200);
  display.begin();

  // Join wifi
  while (!display.joinAP(ssid, password)) {
    Serial.println("Connecting to wifi");
  }
  Serial.println("\nWiFi OK! Downloading...");

  String payload;

  HTTPClient http;

  if (http.begin(statusURL)) {

    int httpCode = http.GET();

    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        payload = http.getString();
        Serial.println("Got HTTP OK");
      } else {
        Serial.println("HTTP failed with code");
        Serial.println(httpCode);
      }
    } else {
      Serial.println("HTTP failed with code");
      Serial.println(httpCode);
    }
  } else {
    Serial.println("No HTTP session");
  }
  int airstatus = payload.toInt();
  Serial.println("Payload: " + payload);

  if (airstatus != onairstatus) {
    if (airstatus == 1) {
      // Show On Air image
      display.drawImage(onair, 0, 0, 600, 448);
      display.display();
      Serial.print("onairstatus: ");
      Serial.println(onairstatus = airstatus);
    } else {
      // Show Off Air image
      display.drawImage(offair, 0, 0, 600, 448);
      display.display();
      Serial.print("onairstatus: ");
      Serial.println(onairstatus = airstatus);
    }
  }
  Serial.println("Going to sleep");
  esp_sleep_enable_timer_wakeup(5ll * 60 * 1000 * 1000);
  esp_deep_sleep_start();
}

void loop() {
  // Never here, as deepsleep restarts esp32
}