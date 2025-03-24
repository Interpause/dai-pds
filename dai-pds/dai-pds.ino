#include "dai-pds-secret.h"
#include "dai-pds.h"
#include <Arduino.h>
#include <FirebaseClient.h>
#include <TimeLib.h>
#include "ExampleFunctions.h" // Utility functions from FirebaseClient.

ServiceAuth sa_auth(FIREBASE_CLIENT_EMAIL, FIREBASE_PROJECT_ID, PRIVATE_KEY, 3000);

FirebaseApp app;
RealtimeDatabase Database;

SSL_CLIENT ssl_client;

// This uses built-in core WiFi/Ethernet for network connection.
// See examples/App/NetworkInterfaces for more network examples.
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);

bool initTaskDone = false;

void connectWiFi()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");

  // Blink blue while connecting.
  while (WiFi.status() != WL_CONNECTED)
  {
    rgbLedWrite(RGB_BUILTIN, 0, 0, 255);
    Serial.print(".");
    delay(150);
    rgbLedWrite(RGB_BUILTIN, 0, 0, 0);
    delay(150);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void initializeFirebase()
{
  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

  set_ssl_client_insecure_and_buffer(ssl_client);

  // Assign the valid time only required for authentication process with ServiceAuth and CustomAuth.
  app.setTime(get_ntp_time());

  Serial.println("Initializing app...");
  initializeApp(aClient, app, getAuth(sa_auth), auth_debug_print, "🔐 authTask");

  app.getApp<RealtimeDatabase>(Database);

  Database.url(DATABASE_URL);
}

void getTimeString(char *buf, size_t len)
{
  time_t now = get_ntp_time();
  snprintf(buf, len, "%4d-%02d-%02d %02d:%02d:%02d", year(now), month(now), day(now), hour(now), minute(now), second(now));
}

void setup()
{
  pinMode(PIR_PIN, INPUT);
  pinMode(TONE_PIN, OUTPUT);

  Serial.begin(115200);
  delay(1000);

  connectWiFi();

  // Solid yellow till ready.
  rgbLedWrite(RGB_BUILTIN, 255, 255, 0);
  delay(150);

  initializeFirebase();
}

void loop()
{
  // To maintain the authentication and async tasks.
  app.loop();

  if (!app.ready())
    return;

  // Done any tasks that should be done only once at startup.
  if (!initTaskDone)
  {
    initTaskDone = true;
    Serial.println("Notifying server of startup...");
    char timeBuf[32];
    getTimeString(timeBuf, sizeof(timeBuf));
    String name = Database.push<String>(aClient, "/debug", "Started: " + String(timeBuf));
    show_status(name);
    print_token_type(app);

    // Green to signify ready.
    rgbLedWrite(RGB_BUILTIN, 0, 255, 0);
  }

  auto hasMotion = digitalRead(PIR_PIN);
  Serial.println("PIR: " + String(hasMotion));
  if (hasMotion)
  {
    rgbLedWrite(RGB_BUILTIN, 255, 0, 255);
    tone(TONE_PIN, 262, 0.5);
    char timeBuf[32];
    getTimeString(timeBuf, sizeof(timeBuf));
    String name = Database.push<String>(aClient, "/visitors", "Visitor at: " + String(startBuf));
    show_status(name);
  }
  else
  {
    rgbLedWrite(RGB_BUILTIN, 0, 255, 0);
    noTone(TONE_PIN);
  }
}

void show_status(const String &name)
{
  if (aClient.lastError().code() == 0)
    Firebase.printf("Success, name: %s\n", name.c_str());
  else
    Firebase.printf("Error, msg: %s, code: %d\n", aClient.lastError().message().c_str(), aClient.lastError().code());
}
