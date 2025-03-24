#include <Arduino.h>

#define PIR_PIN 14
#define TONE_PIN 12

void show_status(const String &name);
void connectWiFi();
void initializeFirebase();
void getTimeString(char *buf, size_t len);
