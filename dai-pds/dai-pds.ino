#include "dai-pds-secret.h"
#include <Arduino.h>
#include <FirebaseClient.h>
#include <TimeLib.h>
#include "ExampleFunctions.h" // Utility functions from FirebaseClient.

void show_status(const String &name);

ServiceAuth sa_auth(FIREBASE_CLIENT_EMAIL, FIREBASE_PROJECT_ID, PRIVATE_KEY, 3000);

FirebaseApp app;
RealtimeDatabase Database;

SSL_CLIENT ssl_client;

// This uses built-in core WiFi/Ethernet for network connection.
// See examples/App/NetworkInterfaces for more network examples.
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);

bool initTaskDone = false;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

  set_ssl_client_insecure_and_buffer(ssl_client);

  // Assign the valid time only required for authentication process with ServiceAuth and CustomAuth.
  app.setTime(get_ntp_time());

  Serial.println("Initializing app...");
  initializeApp(aClient, app, getAuth(sa_auth), auth_debug_print, "🔐 authTask");

  app.getApp<RealtimeDatabase>(Database);

  Database.url(DATABASE_URL);
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
    char startBuf[32];
    time_t now = get_ntp_time();
    snprintf(startBuf, 32, "Started: %4d-%02d-%02d %02d:%02d:%02d", year(now), month(now), day(now), hour(now), minute(now), second(now));
    String name = Database.push<String>(aClient, "/debug", String(startBuf));
    show_status(name);
    print_token_type(app);
  }
}

void show_status(const String &name)
{
  if (aClient.lastError().code() == 0)
    Firebase.printf("Success, name: %s\n", name.c_str());
  else
    Firebase.printf("Error, msg: %s, code: %d\n", aClient.lastError().message().c_str(), aClient.lastError().code());
}
