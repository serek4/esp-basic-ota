#include <esp-basic-ota.h>
#include <esp-basic-wifi.h>

#define WIFI_SSID "your-wifi-ssid"
#define WIFI_PASS "your-wifi-password"

BasicWiFi wifi(WIFI_SSID, WIFI_PASS);
BasicOTA ota;

void setup() {
	Serial.begin(115200);
	Serial.println("");
	ota.setup();
	wifi.onGotIP(handleWiFiGotIP);
	wifi.setup();
	if (wifi.waitForConnection() == BasicWiFi::wifi_got_ip) {
	}
	Serial.println("setup done!");
}

void loop() {
	ota.handle();
	delay(10);
}

void handleWiFiGotIP(GOT_IP_HANDLER_ARGS) {
	ota.begin();
}
