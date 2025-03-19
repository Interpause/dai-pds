#include "dai-pds-secret.h"
#include <Arduino.h>
#include <FirebaseClient.h>
#include "ExampleFunctions.h" // Utility functions from FirebaseClient.

ServiceAuth sa_auth(FIREBASE_CLIENT_EMAIL, FIREBASE_PROJECT_ID, PRIVATE_KEY, 3000);

FirebaseApp app;

SSL_CLIENT ssl_client;

// This uses built-in core WiFi/Ethernet for network connection.
// See examples/App/NetworkInterfaces for more network examples.
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);

bool taskComplete = false;

void setup()
{
  Serial.begin(115200);
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

  // Or intialize the app and wait.
  // initializeApp(aClient, app, getAuth(sa_auth), 120 * 1000, auth_debug_print);
}

void loop()
{
  // To maintain the authentication and async tasks.
  app.loop();

  if (app.ready() && !taskComplete)
  {
    taskComplete = true;
    print_token_type(app);
  }
}
