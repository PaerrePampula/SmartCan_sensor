#include "Adafruit_GFX.h"
#include "Adafruit_SSD1331.h"
#include "DistanceSensor.h"
#include "mbed.h"
#include <MQTTClientMbedOs.h>
#include <cstdio>

#include "ESP8266Interface.h"
#define Black 0x0000
#define Blue 0x001F
#define Red 0xF800
#define Green 0x07E0
#define Cyan 0x07FF
#define Magenta 0xF81F
#define Yellow 0xFFE0
#define White 0xFFFF
ESP8266Interface esp(MBED_CONF_APP_ESP_TX_PIN, MBED_CONF_APP_ESP_RX_PIN);
DistanceSensor disSensor(D0, D1);
TCPSocket socket;
MQTTClient client(&socket);
// Store device IP
SocketAddress deviceIP;
// Store broker IP
SocketAddress MQTTBroker;

Timer bounceBackTimer;
Timeout timerTimeOut;
DigitalOut led(LED1);
InterruptIn magContact(D12);
Adafruit_SSD1331 OLED(D9, D6, D10, D11, NC,
                      D13); // cs, res, dc, mosi, (nc), sck
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;
bool newEvent = false;
// Fields
bool canMeasure = true;
static int foundDistance = 0;
static int foundDifference;
static unsigned int calibratedDistance = 0;

void initializeNetwork() 
{

  MQTTClient client(&socket);

  printf("\nConnecting wifi..\n");

  int ret = esp.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD,
                        NSAPI_SECURITY_WPA_WPA2);

  if (ret != 0) {
    printf("\nConnection error\n");
  } else {
    printf("\nConnection success\n");
  }
  esp.get_ip_address(&deviceIP);
  printf("IP via DHCP: %s\n", deviceIP.get_ip_address());
  esp.gethostbyname(MBED_CONF_APP_MQTT_BROKER_HOSTNAME, &MQTTBroker, NSAPI_IPv4,
                    "esp");
  MQTTBroker.set_port(MBED_CONF_APP_MQTT_BROKER_PORT);

  // client.publish(MBED_CONF_APP_MQTT_TOPIC, msg);
}
// MQTT::Message createMQTTMessage()
//{
// char buffer[128];
// sprintf(buffer, "{ ""CanPercent"":""%i", foundDifference);

// return msg;
//}
void initialize() 
{
  // VCCEN = 1;
  // PMODEN = 1;
  // Need to set to High, if these two are not connected to the 3.3V power
  // supply directly.
  OLED.begin(); // initialization of display object
  OLED.clearScreen();
  OLED.fillScreen(White);   // background screen in blue
  OLED.setTextColor(Black); // colour of text in cyan
  OLED.setCursor(0, 20);    // cursor is in x=0 and y=0
  OLED.setTextSize(1);
  OLED.printf("Hello from SmartCan!\n[Calibrating...]");
  calibratedDistance = disSensor.getDistanceInCm();
  OLED.setCursor(0, 20); // cursor is in x=0 and y=0
  ThisThread::sleep_for(2s);
  OLED.clearScreen();
  OLED.fillScreen(White); // background screen in blue
  OLED.printf("[DONE]\nDepth:%dcm", calibratedDistance);
  ThisThread::sleep_for(2s);
  OLED.clearScreen();
}
void updateScreenInfo() 
{
  OLED.clearScreen();
  OLED.fillScreen(White); // background screen in blue
  OLED.setCursor(0, 15);  // cursor is in x=0 and y=15
  OLED.setTextSize(2);    // size of text
  OLED.setTextColor(Red); // text in red colour
  float depthDifference =
      ((float)foundDistance / (float)calibratedDistance) * 100;
  printf("%f D", depthDifference);
  foundDifference = 100 - (int)depthDifference;
  OLED.printf("%i%% Full!", foundDifference);
}
void distanceCalculationHandle() 
{
  ThisThread::sleep_for(100ms);
  foundDistance = disSensor.getDistanceInCm();
  if (foundDistance > calibratedDistance) 
  {
    foundDistance = 0;
  }

  // printf("cm:%u",foundDistance );
}
void getDistanceFromSensor() 
{
  queue.call(&distanceCalculationHandle);

  newEvent = true;
}
void enableMeasure() 
{ 
    canMeasure = !canMeasure; 
}
void startDist(void) 
{
  if (canMeasure) 
  {
    // Disable possible measuring
    enableMeasure();
    // Get the distance from sensor
    getDistanceFromSensor();
    // Wait for 500ms, then re-enable measuring
    timerTimeOut.attach(callback(&enableMeasure), 1500ms);
  }
}

int main() 
{
  initialize();
  initializeNetwork();
  // Start the event queue
  t.start(callback(&queue, &EventQueue::dispatch_forever));

  magContact.fall(queue.event(&startDist));

while (1) 
    {
        if (newEvent) 
        {

            ThisThread::sleep_for(2000ms);
            printf("%i\n", foundDistance);

            updateScreenInfo();
            newEvent = !newEvent;

            MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
            data.MQTTVersion = 3;
            char *id = MBED_CONF_APP_MQTT_ID;
            data.clientID.cstring = id;

            char buffer[64];
            sprintf(buffer, "{ \"canpercent\":\"%i\" }", foundDifference);
            MQTT::Message msg;
            msg.qos = MQTT::QOS0;
            msg.retained = true;
            msg.dup = true;

            msg.payload = (void *)buffer;
            msg.payloadlen = strlen(buffer);
            // MQTT::Message msg =createMQTTMessage();
            socket.open(&esp);
            socket.connect(MQTTBroker);
            client.connect(data);
            client.publish(MBED_CONF_APP_MQTT_TOPIC, msg);
            client.disconnect();
        }
    }
}
