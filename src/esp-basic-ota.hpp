#pragma once

#include <ArduinoOTA.h>
#include <esp-basic-plugin.hpp>
#include <string>

// #define BASIC_OTA_DEBUG
// debug printing macros
#ifdef BASIC_OTA_DEBUG
#define DEBUG_PRINTER Serial
// clang-format off
#define BASIC_OTA_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define BASIC_OTA_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#define BASIC_OTA_PRINTF(...) { DEBUG_PRINTER.printf(__VA_ARGS__); }
#else
#define BASIC_OTA_PRINT(...) {}
#define BASIC_OTA_PRINTLN(...) {}
#define BASIC_OTA_PRINTF(...) {}
#endif
// clang-format on

#define UPDATE_REBOOT_DELAY 100

class BasicOTA : public BasicPlugin {
  public:
	BasicOTA(const char* hostname);
	BasicOTA();

	void setup();
	inline void begin() {
		ArduinoOTA.begin();
		BASIC_OTA_PRINTLN("OTA started!");
	};
	inline void end() {
		ArduinoOTA.end();
		BASIC_OTA_PRINTLN("OTA stopped!");
	};
	inline bool handle(bool autoReboot = true) {
		ArduinoOTA.handle();
		if (autoReboot && _reboot) {
			static u_long rebootTimer = millis();
			if (millis() - rebootTimer > UPDATE_REBOOT_DELAY) {
				rebootTimer = millis();
				ESP.restart();
			}
		}
		return _reboot;
	};

  private:
	bool _reboot;
	std::string _hostname;
};
