 #include "Web.h"
 #include "../Logger/Logger.h"
 #include "../WiFi/WiFi.h"
 #include "FS.h"
  
 ESP8266WebServer server ( 80 );
 Logger HTTP_Logger("HTTP");



 void handleRoot() {
     char temp[400];
     int sec = millis() / 1000;
     int min = sec / 60;
     int hr = min / 60;
 
     snprintf ( temp, 400,
        "<html>\
        <head>\
            <meta http-equiv='refresh' content='5'/>\
            <title>ESP8266 Demo</title>\
            <style>\
            body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
            </style>\
        </head>\
        <body>\
            <h1>Hello from ESP8266!</h1>\
            <p>Uptime: %02d:%02d:%02d</p>\
        </body>\
        </html>",
         hr, min % 60, sec % 60
     );
     server.send ( 200, "text/html", temp );
 }
 

 String formatBytes(size_t bytes){
    if (bytes < 1024){
      return String(bytes)+"B";
    } else if(bytes < (1024 * 1024)){
      return String(bytes/1024.0)+"KB";
    } else if(bytes < (1024 * 1024 * 1024)){
      return String(bytes/1024.0/1024.0)+"MB";
    } else {
      return String(bytes/1024.0/1024.0/1024.0)+"GB";
    }
  }
  
 String getContentType(String filename){
    if(server.hasArg("download")) return "application/octet-stream";
    else if(filename.endsWith(".htm")) return "text/html";
    else if(filename.endsWith(".html")) return "text/html";
    else if(filename.endsWith(".css")) return "text/css";
    else if(filename.endsWith(".js")) return "application/javascript";
    else if(filename.endsWith(".png")) return "image/png";
    else if(filename.endsWith(".gif")) return "image/gif";
    else if(filename.endsWith(".jpg")) return "image/jpeg";
    else if(filename.endsWith(".ico")) return "image/x-icon";
    else if(filename.endsWith(".xml")) return "text/xml";
    else if(filename.endsWith(".pdf")) return "application/x-pdf";
    else if(filename.endsWith(".zip")) return "application/x-zip";
    else if(filename.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";
  }

  bool handleFileRead(String path){
    

    HTTP_Logger.log("handleFileRead: " + path);
    if(path.endsWith("/")) {
      path += "index.html";
    } 
    String contentType = getContentType(path);
    String pathWithGz = path + ".gz";

    if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
      if(SPIFFS.exists(pathWithGz)){
        path += ".gz";
      }
        
      File file = SPIFFS.open(path, "r");
      size_t sent = server.streamFile(file, contentType);
      file.close();
      return true;
    }
    return false;
  }

 void serve_WiFiList(){
   server.send(200,"text",listNetworks());
 }

 void boot_WEB(){
    HTTP_Logger.enable();
    delay(500);
    SPIFFS.begin();
     {
       Dir dir = SPIFFS.openDir("/");
       while (dir.next()) {    
         String fileName = dir.fileName();
         size_t fileSize = dir.fileSize();
         Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
       }
       Serial.printf("\n");
     }

    server.on("/list", serve_WiFiList);
    server.onNotFound([](){
       if(!handleFileRead(server.uri()))
         server.send(404, "text/plain", "FileNotFound");
     });
    server.begin();
    HTTP_Logger.log ( "HTTP server started" );    
 }
