#include "ESP8266Interface.h"
#include <MQTTClientMbedOs.h>
#include "ntp-client/NTPClient.h"
#define ntpAddress "time.mikes.fi"  // The VTT Mikes in Helsinki
#define ntpPort 123     // Typically 123 for every NTP server

class MQTTNetworkingControl
{
    public:
    MQTTNetworkingControl(ESP8266Interface &espInterface);
    void createAndSendMQTTMessage(int depthDifference);
    private:

    void init();
    void initNetwork();
    void initNTP();
    char *id = "d:913ydf:MQTTClient:Garbage";     // d:<org-id>:<device-type>:<device-id>
    char *username = "use-token-auth";      // 
    char *password = "6+pdCOn1FT(q+A-k+y";  // auth token

    //Fields
    ESP8266Interface *esp;
    // Store device IP
    SocketAddress deviceIP;
    // Store broker IP
    SocketAddress MQTTBroker;
    TCPSocket socket;
    MQTTClient client;
    NTPClient ntp;
};