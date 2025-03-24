#include <Arduino.h>

#define PIR_PIN 14
#define TONE_PIN 12
#define TONE_DELAY 400
#define LOOP_DELAY 100

void show_status(const String &name);
void connectWiFi();
void initializeFirebase();
void getTimeString(char *buf, size_t len);
void playAudio();
