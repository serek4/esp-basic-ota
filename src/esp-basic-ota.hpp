#pragma once

#include <ArduinoOTA.h>
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

class BasicOTA {
  public:
	BasicOTA();
	BasicOTA(const char* hostname);

	void addLogger(void (*logger)(String logLevel, String msg));
	void setup();
	inline void begin() {
		ArduinoOTA.begin();
		BASIC_OTA_PRINTLN("OTA started!");
	};
	inline void handle() {
		ArduinoOTA.handle();
		if (_reboot) {
			static u_long rebootTimer = millis();
			if (millis() - rebootTimer > UPDATE_REBOOT_DELAY) {
				rebootTimer = millis();
				ESP.restart();
			}
		}
	};

  private:
	bool _reboot;
	std::string _hostname;
	void (*_logger)(String logLevel, String msg);
};
