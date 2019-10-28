/*
 * STEPS FOR STARTING ESP8266 FOR SELF PROGRAMMERS
 * 
1.Include header file
2.select id and password(ssid - service set identifier and password)
3.Create server
4.select baud rate
5.begin the wifi
6.wait WHILE  connecting
7.start the server
8.print the ip for connection*/








#include <ESP8266WiFi.h>    //header file






const char* ssid="Kakashi";//SSID=service set identifier(basically name of wifi)
const char* password="0987654321";//password of wifi


int button=5;
int LED=4;
WiFiServer server(80);//Create a server that listen to incoming connections 


int current=0;
int last=0;

void setup()
{
  Serial.begin(115200);
  delay(10);

 //Led instiallization
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);

  //Button intialization
  pinMode(button,INPUT_PULLUP);

  //connect to wifi
  Serial.println();
  Serial.println();
  Serial.println("Connecting to:");
  Serial.print(ssid);

  WiFi.begin(ssid,password);   //here wifi is intitalized


  //while connecting
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.println(".");
    }
 Serial.println("");
 Serial.println("Connected");

 //Start the server
 server.begin();
  Serial.println("Server started");


//Print the ip address
Serial.print("Use this ip in address bar to connect:");
Serial.print("http://");
Serial.println(WiFi.localIP());
Serial.println("/");

    }


void loop()
{
   //Check if client connected
  WiFiClient client=server.available();
 
  //code for manual control
  while(!client)
   {
    if(digitalRead(button)==LOW)
         {current=1;}
      else
         {current=0;}
         
        if(last==current)
          {return;}
      digitalWrite(LED,current);
      last=current;
   }


//wait until cliet sends some data
   Serial.println("New client");
   while(!client.available())
    {
     delay(1);
     }

 //Read the first line of request
   String request=client.readStringUntil('/r');
   Serial.println(request);
   client.flush();
          
 //Match request
 int value=LOW;
  if (request.indexOf("/LED=ON") != -1)  
  {
    digitalWrite(LED, HIGH);
    value = HIGH;
  }
   else if (request.indexOf("/LED=OFF") != -1)
    {
    digitalWrite(LED,LOW);
    value = LOW;
    }
 // Set ledPin according to the request

 
  // Return the response
  //HTML/CSS code for web page

  //initaliztion of HTML 
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");                              //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");//this following line makes the web page responsive in any web browser.
 
  
 
  //styling the Button(change as per your own choice)
  //We style our buttons with the #4CAF50 color, without border, text in white color, 
  //and with this padding: 40px 40px. 
 // We also set the text-decoration to none, define the font size, the margin, 
 //and the cursor to a pointer.
 client.println("<style>html { font-family: Comic sans ms; display: inline-block; margin: 0px auto; text-align: center;}");
 client.println(".button { background-color: #d4af37; border: line; color: black; padding: 40px 40px;}");
 client.println("text-decoration: none; font-size: 50px; margin: 2px; cursor: pointer;}");
 client.println(".button2 {background-color: #555555;}</style></head>");

 // Web Page Heading
 client.println("<body><h1>ESP8266 Web Server</h1>");


 //Showing current LED state
 client.print("Led pin is now: ");

  if(value==LOW)
  { 
    client.print("Off");
    client.println("<p><a href=\"/LED=ON\"\"><button class=\"button\">Turn ON </button></a></p>");
    //client.println("<br><br>");
  }
  else
  {
    client.print("On");
    client.println("<p><a href=\"/LED=OFF\"\"><button class=\"button button2\">Turn OFF </button></a></p>");  
  }
  client.println("</body></html>");//CLOSING HTML WEBPAGE
  delay(1);
  Serial.println("Client disonnected");//closing connection
  Serial.println("");

    
    }
