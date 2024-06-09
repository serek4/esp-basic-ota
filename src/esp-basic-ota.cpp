#include "esp-basic-ota.hpp"

BasicOTA::BasicOTA(const char* hostname)
    : _reboot(false)
    , _hostname(hostname)
    , _logger(nullptr) {
}
BasicOTA::BasicOTA()
    : _reboot(false)
    , _logger(nullptr) {
#ifdef ARDUINO_ARCH_ESP32
	char id[13];
	sprintf(id, "%012llX", ESP.getEfuseMac());
	// read from back to front by 2 characters
	_hostname = (std::string) "esp_" + id[4] + id[5] + id[2] + id[3] + id[0] + id[1];
#elif defined(ARDUINO_ARCH_ESP8266)
	char id[7];
	sprintf(id, "%06X", ESP.getChipId());
	_hostname = (std::string) "esp_" + id;
#endif
}
void BasicOTA::addLogger(void (*logger)(String logLevel, String msg)) {
	_logger = logger;
}
void BasicOTA::setup() {
	ArduinoOTA.setRebootOnSuccess(false);
	ArduinoOTA.setHostname(_hostname.c_str());
	ArduinoOTA.onStart([&]() {
		BASIC_OTA_PRINTLN("OTA update started");
	});
	ArduinoOTA.onEnd([&]() {
		BASIC_OTA_PRINTLN("\nEnd");
		if (_logger != nullptr) { (*_logger)("ota", "firmware updated"); }
		_reboot = true;
	});
	ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
		uint8_t percent = progress / (total / 100);
		static uint8_t lastPercent = percent;
		if (percent != lastPercent) {
			BASIC_OTA_PRINTF("Progress: %u%%\n", percent);
			lastPercent = percent;
		}
	});
	ArduinoOTA.onError([&](ota_error_t error) {
		BASIC_OTA_PRINTF("Update Error[%u]: ", error);
		if (_logger != nullptr) { (*_logger)("ota", "firmware update failed " + String(error, 10)); }
		if (error == OTA_AUTH_ERROR) {
			BASIC_OTA_PRINTLN("Auth Failed");
		} else if (error == OTA_BEGIN_ERROR) {
			BASIC_OTA_PRINTLN("Begin Failed");
		} else if (error == OTA_CONNECT_ERROR) {
			BASIC_OTA_PRINTLN("Connect Failed");
		} else if (error == OTA_RECEIVE_ERROR) {
			BASIC_OTA_PRINTLN("Receive Failed");
		} else if (error == OTA_END_ERROR) {
			BASIC_OTA_PRINTLN("End Failed");
		}
	});
}
