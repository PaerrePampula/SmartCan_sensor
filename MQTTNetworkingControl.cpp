#include "MQTTNetworkingControl.h"
#include <cstdio>
MQTTNetworkingControl::MQTTNetworkingControl(ESP8266Interface& espInterface) : esp(espInterface), client(&socket), ntp(&esp)
{
    init();
}

void MQTTNetworkingControl::init()
{
    initNetwork();
    initNTP();
}
void MQTTNetworkingControl::initNetwork()
{
    printf("\n[WIFI CONNECTING...]\n");
    int ret = esp.connect(MBED_CONF_APP_WIFI_SSID, MBED_CONF_APP_WIFI_PASSWORD,
                            NSAPI_SECURITY_WPA_WPA2);
    if (ret != 0) {
        printf("\nConnection error\n");
    } else {
        printf("\nConnection success\n");
    }
    //get the hosting mqtt-broker with a www-address, configured in the JSON-file
    esp.gethostbyname(MBED_CONF_APP_MQTT_BROKER_HOSTNAME, &MQTTBroker, NSAPI_IPv4,
                        "esp");
    //Then also get the correct port set by JSON-file
    MQTTBroker.set_port(MBED_CONF_APP_MQTT_BROKER_PORT);
}
void MQTTNetworkingControl::initNTP()
{
    ntp.set_server(ntpAddress, ntpPort);
}
void MQTTNetworkingControl::createAndSendMQTTMessage(int depthDifference)
{
        char *id = "d:913ydf:MQTTClient:Garbage";     // d:<org-id>:<device-type>:<device-id>
    char *username = "use-token-auth";      // 
    char *password = "6+pdCOn1FT(q+A-k+y";  // auth token

            time_t timestamp = ntp.get_timestamp();
            timestamp += (60*60*2);  //  GMT +2 for Finnish timezone.

            MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
            data.MQTTVersion = 3;
            data.clientID.cstring = id;
            data.username.cstring = username;
            data.password.cstring = password;
            //Create a chararray, to which the information of the MQTT message is pasted on (Depth difference % and time)
            //64 should be plenty for this

            char buffer[64];
            sprintf(buffer, "{ \"canpercent\":\"%i\", \"time\":\"%lld\" }",  depthDifference, (long long)timestamp);
            printf(buffer, "\n");

            //Create an MQTT message, which is given the buffer as the payload, to be sent to the MQTT-broker
            MQTT::Message msg;
            //msg.qos = MQTT::QOS0;
            //msg.retained = false;
            //msg.dup = false;
            msg.payload = (void *)buffer;
            msg.payloadlen = strlen(buffer);
            
            socket.open(&esp);
            socket.connect(MQTTBroker);
            client.connect(data);
            client.publish(MBED_CONF_APP_MQTT_TOPIC, msg);
            client.disconnect();
            printf("DATA SENT");
}

