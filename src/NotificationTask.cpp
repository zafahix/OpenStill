#include "NotificationTask.h"

NotificationTaskClass::NotificationTaskClass(StillDataContextClass &context, SettingsClass &settings) : _context(context), _settings(settings)
{
}

void NotificationTaskClass::exec()
{
  for (int i = 0; i < 10; i++)
  {
    if (!_context.notifications[i].isSent)
    {
      sendNotification(_context.notifications[i].name, _context.notifications[i].message);
      _context.notifications[i].isSent = true;
      return;
    }
  }
}

uint32_t NotificationTaskClass::timeOfNextCheck()
{
  setTriggered(true);
  return millisToMicros(10000);
}

void NotificationTaskClass::sendNotification(String title, String message)
{
    const char *host = "ntfy.sh";
    const uint16_t port = 443;
    String path = "/" + String(_settings.pushNotificationCode);
    Serial.println("Sending notification to ntfy.sh: " + message);

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient https;
    if (https.begin(client, host, port, path))
    {
        https.addHeader("Title", title);
        https.addHeader("Content-Type", "text/plain");
        int httpsCode = https.POST(message);
        if (httpsCode > 0)
        {
            Serial.print("ntfy response code: ");
            Serial.println(httpsCode);
            Serial.println(https.getString());
        }
        else
        {
            Serial.print("failed to POST to ntfy.sh");
        }
        https.end();
    }
    else
    {
        Serial.print("failed to connect to ntfy.sh");
    }
}
