#include "WiFi.h"
#include <Ticker.h>

const char *ssid = "PLDTMyDSLBiz-Home";
const char *password = "TCB03051993";


/*
    WL_CONNECTED: assigned when connected to a WiFi network;
    WL_NO_SHIELD: assigned when no WiFi shield is present;
    WL_IDLE_STATUS: it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED);
    WL_NO_SSID_AVAIL: assigned when no SSID are available;
    WL_SCAN_COMPLETED: assigned when the scan networks is completed;
    WL_CONNECT_FAILED: assigned when the connection fails for all the attempts;
    WL_CONNECTION_LOST: assigned when the connection is lost;
    WL_DISCONNECTED: assigned when disconnected from a network;
*/

Logger WiFi_Logger("WiFi");
IPAddress ip(10,1,255,129);  //Node static IP
IPAddress gateway(10,1,0,1);
IPAddress subnet(255,255,255,0);


Ticker async_bootWifi;
Ticker async_checkWifi;

bool connectionDisplayed =false;
uint8_t connectAttempts = 0;
uint8_t ssidAttempts = 0;

void async_ConnectToWiFi();
void async_CheckConnection();

const char *known_ssid[2]={"DNA_Secondary","PLDTMyDSLBiz-Home"};
const char *known_password[2]={"9492423670mlink","TCB03051993"};


String printEncryptionType(int thisType) {
    String EncType;
    // read the encryption type and print out the name:
    switch (thisType) {
      case ENC_TYPE_WEP:
        EncType="WEP";
        
        break;
      case ENC_TYPE_TKIP:
        EncType="WPA";
        break;
      case ENC_TYPE_CCMP:
        EncType="WPA2";
        break;
      case ENC_TYPE_NONE:
        EncType="None";
        break;
      case ENC_TYPE_AUTO:
        EncType="Auto";
        break;
    }

    return EncType;
  }

String listNetworks() {
    String List = "";

    // scan for nearby networks:
    Serial.println("** Scan Networks **");
    int numSsid = WiFi.scanNetworks();
    if (numSsid == -1) {
      Serial.println("Couldn't get a wifi connection");
      while (true);
    }
  
    // print the list of networks seen:
    Serial.print("number of available networks:");
    Serial.println(numSsid);
  
    // print the network number and name for each network found:
    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
        List+=String(WiFi.SSID(thisNet))+",";
        List+=String(WiFi.RSSI(thisNet))+",";
        List+=printEncryptionType(WiFi.encryptionType(thisNet))+'\n';
      ;
    }

    return List;
  }

void async_ConnectToWiFi(){
   if(WiFi.status() != WL_CONNECTED  ) {
   } else {
        connectAttempts= 0;
        
        async_bootWifi.detach();
   }
}

void async_CheckConnection(){
    if(WiFi.status() == WL_CONNECTED) {
        if(!connectionDisplayed){
            IPAddress local = WiFi.localIP();
            String local_string = String(local[0]) +"."+ String(local[1]) +"."+ String(local[2]) +"."+ String(local[3]);   
            WiFi_Logger.log( "Connected to " + String(ssid) );
            WiFi_Logger.log( "IP address: " + local_string );
            connectionDisplayed = true;
        }
    }
    else if( WiFi.status() == WL_DISCONNECTED  ){
        connectionDisplayed=false;
        WiFi_Logger.log ( "WiFi Disconnected" );
        ssidAttempts ++;
        WiFi_Logger.log("Connecting to "+ String(known_ssid[ssidAttempts]));
        WiFi.config(ip, gateway, subnet);
        WiFi.begin(known_ssid[ssidAttempts], known_password[ssidAttempts]);
    } 
}


void boot_WiFi(){
    
    WiFi_Logger.enable();

    WiFi.config(ip, gateway, subnet);
    WiFi.begin(known_ssid[ssidAttempts], known_password[ssidAttempts]);
    // Wait for connection

    //async_bootWifi.attach(1, async_ConnectToWiFi);
    async_checkWifi.attach(5, async_CheckConnection);
}