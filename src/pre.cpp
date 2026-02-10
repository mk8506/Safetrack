// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_GFX.h>
// #include <Adafruit_SSD1306.h>
// #include <TinyGPSPlus.h>
// #include <RadioLib.h>
// #include <SPI.h>
// #include "gpsPacket.h"
// #include "RadioHead.h"
// #include "battery.h"

// #define LED            (0 + 15)   // Onboard LED, change if different
// #define OLED_RESET     -1         // OLED reset pin
// #define GPS_SERIAL     Serial1    // GPS module serial port
// #define DISPLAY_WIDTH  128
// #define DISPLAY_HEIGHT 64

// #define SX126X_CS    (32 + 13) // P1.13
// #define SX126X_DIO1  (0 + 10)  // P0.10
// #define SX126X_BUSY  (0 + 29)  // P0.29
// #define SX126X_RESET (0 + 9)   // P0.09
// #define SX126X_RXEN  (0 + 17)  // P0.17
// #define SX126X_TXEN  RADIOLIB_NC

// //roles
// #define ROLE_TX
// //#define ROLE_RX uncomment this for the RX board

// #define BATTERY_PIN (0 + 1)
// #define BATTERY_UPDATE_INTERVAL 1000  // Read every 1 second

// Battery battery(BATTERY_PIN, BATTERY_UPDATE_INTERVAL);

// void setFlag(void);
// void setup();
// void loop();
// #ifdef ROLE_TX
// float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
// void updateBatteryDisplay();
// void drawBatteryIcon(int x, int y, float percent) ;
// #endif
// String bytesToAscii(const uint8_t *, size_t);

// // LoRa modem settings
// const float FREQ_MHZ = 906.875;
// const uint8_t SF = 11;
// const unsigned long BW = 250000;
// const uint8_t CR = 5;
// const uint16_t PREAMBLE = 16;
// const uint8_t SYNCWORD = 0x2B;

// //my
// int analogInPin;  
// int sensorValue; 
// float voltage;
// float bat_percentage;

// Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &Wire, OLED_RESET);
// uint8_t buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8];
// TinyGPSPlus gps;
// SX1262 radio = new Module(SX126X_CS, SX126X_DIO1, SX126X_RESET, SX126X_BUSY);

// char latBuffer[30], lngBuffer[30], dateBuffer[30];
// double lastLat = 0, lastLng = 0;
// uint8_t lastBattery = 0;
// TinyGPSDate lastDate;

// volatile bool receivedFlag = false; // set by ISR when a packet arrives

// void setFlag() {
//     receivedFlag = true;
// }

// void setup() {
//     pinMode(LED, OUTPUT); // set PIN15 in OUTPUT mode
//     digitalWrite(LED, LOW); // set PIN15 to LOW

// 		/* gps init */
//     GPS_SERIAL.begin(9600); // SERIAL.begin() in 9600 bits per second (baud) 
// 		// Seriel, is it an object?

// 		/*  display init */
//     if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
//         digitalWrite(LED, HIGH); // set HIGH, why?
//         while (true); // Don't proceed, loop forever
//     }

// 		//memset(buffer, 0, sizeof(buffer)); // set loop display buffer with zeros
//     display.clearDisplay();
//     display.setTextSize(1);
//     display.setTextColor(SSD1306_WHITE);
//     display.setCursor(0, 0);
//     display.println("Booting...");
//     display.display();

// 		/* battery init */
// 		battery.begin();

// 		/* radio init */
//     Serial.print(F("[SX1262] Initializing ... ")); //Prints data to the serial port asASCII text
//     int state = radio.begin();
//     if (state == RADIOLIB_ERR_NONE) {
//         Serial.println(F("success!"));
//     } else {
//         Serial.print(F("failed, code "));
//         Serial.println(state);
//         while (true) { delay(10); }
//     }

//     radio.setFrequency(FREQ_MHZ);
//     radio.setSpreadingFactor(SF);
//     radio.setBandwidth(BW);
//     radio.setCodingRate(CR);
//     radio.setPreambleLength(PREAMBLE);
//     radio.setSyncWord(SYNCWORD);
// 		//radio.setCRC(2);
//     radio.setDio1Action(setFlag);

//     Serial.print(F("[SX1262] Starting to listen ... "));
//     state = radio.startReceive();
//     if (state == RADIOLIB_ERR_NONE) {
//         Serial.println(F("success!"));
//     } else {
//         Serial.print(F("failed, code "));
//         Serial.println(state);
//         while (true) { delay(10); }
//     }
// 		Serial.println("boot successful..."); //
// 		delay(1000); //
//     display.clearDisplay();
//     display.display();
// }

// void loop() {
// 	// Update battery reading
// 	battery.update();
// 	#ifdef ROLE_TX
// 	// Check if battery has changed by at least 1%
// 	if (battery.hasChanged(1.0)) {
// 			updateBatteryDisplay();
// 	}
// 	#endif

// 	while (GPS_SERIAL.available() > 0) {
// 			gps.encode(GPS_SERIAL.read());
// 	}

// 	bool locationUpdated = gps.location.isUpdated();
// 	bool dateUpdated = gps.date.isUpdated();

// 	//displays gps coords and transmits
// 	//rn its transmitting gps coordinates back to itself

// 	#ifdef ROLE_TX 
// 		if (locationUpdated) {
// 			double lat = gps.location.lat();
// 			double lng = gps.location.lng();
// 			bool redraw = false;

// 			if (lat != lastLat) {
// 				lastLat = lat;
// 				snprintf(latBuffer, sizeof(latBuffer), "Lat: %3.6f", lat);
// 				redraw = true;
// 			}
// 			if (lng != lastLng) {
// 				lastLng = lng;
// 				snprintf(lngBuffer, sizeof(lngBuffer), "Lng: %3.6f", lng);
// 				redraw = true;
// 			}

// 			if (redraw) {
// 				// Update OLED
// 				display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
// 				display.setCursor(0, 0);
// 				display.print(latBuffer);
// 				display.setCursor(0, 10);
// 				display.print(lngBuffer);

// 				// Build GPS packet
// 				uint8_t outBuf[sizeof(GPSPacket)];
// 				buildPacket(lastLat, lastLng, outBuf);

// 				// Transmit
// 				radio.standby();
// 				int st = radio.transmit(outBuf, sizeof(GPSPacket));
// 				if (st == RADIOLIB_ERR_NONE) Serial.println("TX: GPS packet sent");
// 				else {
// 					Serial.print("TX error: "); Serial.println(st);
// 				}

// 				// Return to RX
// 				radio.startReceive();
// 			}
//     }
// 	#endif

// 	#ifdef ROLE_TX
//     if (dateUpdated) {
//         TinyGPSDate date = gps.date;
//         if (date.isValid() && date.value() != lastDate.value()) {
//             lastDate = date;
//             snprintf(dateBuffer, sizeof(dateBuffer), "Date: %02d-%02d-%02d",
//                      date.year(), date.month(), date.day());
//             display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
//             display.setCursor(0, 20);
//             display.print(dateBuffer);
//             display.display();
//         }
//     }
// 	#endif

//   //recieving gps packets
// 	#ifdef ROLE_RX
// 		if (receivedFlag) {
// 			receivedFlag = false;

// 			uint8_t inBuf[sizeof(GPSPacket)];
// 			int state = radio.readData(inBuf, sizeof(GPSPacket));

// 			if (state == RADIOLIB_ERR_NONE) {
// 				float rxLat, rxLng;
// 				uint8_t rxNode;
// 				parsePacket(inBuf, rxLat, rxLng, rxNode);

// 				Serial.print("Node "); Serial.print(rxNode);
// 				Serial.print(" Lat="); Serial.print(rxLat, 6);
// 				Serial.print(" Lng="); Serial.println(rxLng, 6);

// 				// Update OLED
// 				display.setCursor(0, 40);
// 				display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
// 				display.print("RX ");
// 				display.print(rxNode);
// 				display.print(": ");
// 				display.print(rxLat, 4);
// 				display.print(",");
// 				display.print(rxLng, 4);
// 				display.display();

// 				// Debug info
// 				Serial.print("RSSI: "); Serial.println(radio.getRSSI());
// 				Serial.print("SNR: "); Serial.println(radio.getSNR());
// 			}
// 			else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
// 				Serial.println("CRC error!");
// 				radio.startReceive();
// 			}
// 			else {
// 				Serial.print("RX error: "); Serial.println(state);
// 				radio.startReceive();
// 			}

// 			TinyGPSDate date = gps.date;
// 			snprintf(dateBuffer, sizeof(dateBuffer), "Date: %02d-%02d-%02d",
// 					date.year(), date.month(), date.day());
// 			display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
// 			display.setCursor(0,20);
// 			display.print(dateBuffer);
// 			display.display();

// 			// Re-enable RX mode
// 			radio.startReceive();
// 		}
// 	#endif
// }

// #ifdef ROLE_TX
// void updateBatteryDisplay() {
//     float batteryPercent = battery.getPercentage();
//     float batteryVoltage = battery.getVoltage();
    
//     // Clear battery area
//     display.fillRect(0, 30, 128, 20, SSD1306_BLACK);
    
//     // Display battery percentage
//     display.setTextColor(SSD1306_WHITE);
//     display.setCursor(0, 30);
//     display.print("Battery: ");
//     display.print(batteryPercent, 1);
//     display.print("%");
//     // Display voltage
//     display.setCursor(0, 40);
//     display.print("Voltage: ");
//     display.print(batteryVoltage, 2);
//     display.print("V");
    
//     // Draw battery icon (optional)
//     drawBatteryIcon(100, 30, batteryPercent);
    
//     display.display();
    
//     Serial.print("Display updated - Battery: ");
//     Serial.print(batteryPercent, 1);
//     Serial.println("%");
// }

// // Optional: Draw a battery icon
// void drawBatteryIcon(int x, int y, float percent) {
//     // Battery outline
//     display.drawRect(x, y, 20, 10, SSD1306_WHITE);
//     display.fillRect(x + 20, y + 3, 2, 4, SSD1306_WHITE);  // Terminal
    
//     // Fill level
//     int fillWidth = (int)((percent / 100.0) * 18);
//     display.fillRect(x + 1, y + 1, fillWidth, 8, SSD1306_WHITE);
// }
// #endif

// String bytesToAscii(const uint8_t *buf, size_t len) {
//   String s;
//   s.reserve(len);
//   for (size_t i = 0; i < len; i++) {
//     char c = (char)buf[i];
//     if (c >= 32 && c <= 126) {
//       s += c;            // printable ASCII
//     } else {
//       s += '.';          // non-printable → dot
//     }
//   }
//   return s;
// }
