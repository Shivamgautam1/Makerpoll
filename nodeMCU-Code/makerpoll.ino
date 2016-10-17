
/* This Project is developed by Shivam Gautam (https://github.com/Shivamgautam1/Makerpoll/)
 * Please note that we do not support politics nor any of these politicians
 * For any queries contact me at shivamgautam1@gmail.com
 */

#include <ESP8266WiFi.h>

const char* ssid     = "Your-SSID";// Your WiFi
const char* password = "SSID-Password";// The Password
const char* host = "data.sparkfun.com";// Host to send data
const char* streamId   = "roaE2LAoD2TlA6ElNpEg";// This is my stream data.sparkfun.com/makerpolll, Please do not misuse.
const char* privateKey = "jkRlvb2kovCWrx1WDg1e";// Private Key for the stream
bool voteCasted = false;// Set the voteCaste initially be undone
int hillPin = D1;// Pin to connect for Hillary
int trumPin = D2;// Pin to connect for Trump
int hillary = 0;// Initial vote count
int trump = 0;// Initial vote count
// Creating a function to post data to sparkfun
void postPoll(int hillary, int trump){
    Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
  }
  
  // We now create a URI for the request
  String url = "/input/";
  url += streamId;
  url += "?private_key=";
  url += privateKey;
  url += "&hillary=";
  url += hillary;
  url += "&trump=";
  url += trump;
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  }


void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(hillPin, INPUT);
  pinMode(trumPin, INPUT);
}
void loop() {
  delay(10000);
  int hill = digitalRead(hillPin);// Read Hillary Pin
  int trum = digitalRead(trumPin);// Read Trump Pin
  if(!voteCasted){
    //Check if vote is casted to Hillary
      if(hill == HIGH && trum == LOW){
          postPoll(1, 0);
          voteCasted = true;// Vote has been casted set to True
        }
    //Check if vote is casted to Trump
      else if(trum == HIGH && hill == LOW){
          postPoll(0, 1);
          voteCasted = true;// Vote has been casted set to True
        }
        else{
            postPoll(0, 0);
          }
    }
}

