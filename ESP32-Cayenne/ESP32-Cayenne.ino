#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>
#include "DHT.h"

#define LEDPIN 2
#define DHTPIN 4     // Digital pin connected to the DHT sensor

#define DHTTYPE DHT21   // DHT 21 (AM2301)

// WiFi network info.
char ssid[] = "myssid";
char wifiPassword[] = "mypass";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "cayenne_username";
char password[] = "cayenne_password";
char clientID[] = "cayenne_clientid";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
	Serial.begin(115200);
    dht.begin();
    pinMode(LEDPIN,OUTPUT);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);

}

void loop() {
	Cayenne.loop(5000);
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
	
    // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	//Cayenne.virtualWrite(0, millis());
	// Some examples of other functions you can use to send data.
	Cayenne.celsiusWrite(1, t);
    Cayenne.virtualWrite(2, h, CAYENNE_FLASH(TYPE_RELATIVE_HUMIDITY), CAYENNE_FLASH(UNIT_PERCENT));
}


// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
    
    switch (request.channel)
    {
    case 3:
        digitalWrite(LEDPIN,getValue.asInt());
        break;
    default:
        break;
    }
    
}
