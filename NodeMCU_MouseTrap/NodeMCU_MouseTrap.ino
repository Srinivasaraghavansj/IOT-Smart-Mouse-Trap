#include <ESP8266WiFi.h>
#include <time.h>
#include <Servo.h>
Servo trig;
const int tr=D4,doorsw=D2,mousesw=D3;
const int timezone = 7 * 3600;
const int dst=0;
const char* ssid = "D-Link_DIR-615";
const char* password = "vingyaninno@123$";
unsigned char status_led=0;
String Mousecaughttime="";
WiFiServer server(80);
bool mousetrapped=false;

void mousetrap();
void getcurrenttime();
void clientdisplay();


void setup() {

  pinMode(doorsw, INPUT);
  pinMode(mousesw, INPUT);
  trig.detach();
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ' " + (String)ssid + " '");
  Serial.println();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(" . ");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.localIP());

}

void loop() {
mousetrap();
clientdisplay();

}


void mousetrap()
{
  trig.detach();
  if(digitalRead(doorsw)==0 && digitalRead(mousesw)==1 && mousetrapped==false)
  {
    trig.attach(tr);
    trig.write(90);
    mousetrapped=true;
    delay(1500);
    trig.detach();
    getcurrenttime();
    clientdisplay();
  }


Serial.println("Door= "+(String)digitalRead(doorsw)+"  MousePresent= "+(String)digitalRead(mousesw)+ "  MoustTrapped = " + (String)mousetrapped);
}


void getcurrenttime()
{
   configTime(timezone,dst, "pool.ntp.org" , "time.nist.gov");
  Serial.println("Waiting for internet time\n");

  while(!(time(nullptr)))
  {
    Serial.print("  *");
    delay(500);
  }
  Serial.println("\n Time Response Success  \n");


  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);

Mousecaughttime = (String)(p_tm->tm_mday) + "/" + (String)(int)(p_tm->tm_mon + 1) + "/" + (String)(int)(p_tm->tm_year + 1900) + " " + (String)(p_tm->tm_hour) + ":" + (String)(p_tm->tm_min) + ":" + (String)(p_tm->tm_sec) ;

}



void clientdisplay()
{
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  String web = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  web += "<html>\r\n";
  web += "<body>\r\n";
  web += "<h1>That poor little mouse was caught on: </h1>\r\n";
  web +="<h2>"+Mousecaughttime+"</h2>";
  web += "</body>\r\n";
  web += "</html>\r\n";
  
  client.print(web);
}
