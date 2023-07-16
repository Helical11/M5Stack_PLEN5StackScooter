#include <M5Unified.h>
//#include "PLEN5Stack.h"
//#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <ESPNowW.h>

uint8_t mac[] = {0xA4, 0xCF, 0x12, 0x44, 0x22, 0x14};

void onRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4],
             mac_addr[5]);
    Serial.print("Last Packet Recv from: ");
    Serial.println(macStr);
    Serial.print("Last Packet Recv Data: ");
    // if it could be a string, print as one
    if (data[data_len - 1] == 0)
        Serial.printf("%s\n", data);
    // additionally print as hex
    for (int i = 0; i < data_len; i++) {
        Serial.printf("%x ", data[i]);
    }
    Serial.println("");
}

void setup() {
    Serial.begin(115200);
    Serial.println("ESPNow receiver Demo");
    WiFi.mode(WIFI_MODE_STA);
    ESPNow.set_mac(mac);
    WiFi.disconnect();
    ESPNow.init();
    ESPNow.reg_recv_cb(onRecv);
}

void loop() {}