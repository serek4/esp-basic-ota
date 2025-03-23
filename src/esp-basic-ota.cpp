#include "esp-basic-ota.hpp"

BasicOTA::BasicOTA(const char* hostname)
    : BasicPlugin::BasicPlugin("ota")
    , _reboot(false)
    , _hostname(hostname) {
}
BasicOTA::BasicOTA()
    : BasicOTA::BasicOTA(_generateClientID().c_str()) {
}

void BasicOTA::setup() {
	ArduinoOTA.setRebootOnSuccess(false);
	ArduinoOTA.setHostname(_hostname.c_str());
	ArduinoOTA.onStart([&]() {
		BASIC_OTA_PRINTLN("OTA update started");
	});
	ArduinoOTA.onEnd([&]() {
		BASIC_OTA_PRINTLN("\nEnd");
		_log(_info_, "firmware updated");
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
		_log(_info_, "firmware update failed " + String(error, 10));
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
