#include <U8glib.h>
//font：
#define setFont_0 u8g_font_timB14
#define setFont_1 u8g_font_9x15
#define setFont_2 u8g_font_fixed_v0r
#define setFont_3 u8g_font_timB18
#define setFont_4 u8g_font_timB14
#define setFont_5 u8g_font_timB10
#define setFont_6 u8g_font_5x7
#define setFont_7 u8g_font_6x10
#define setFont_8 u8g_font_7x13
#define setFont_9 u8g_font_9x15
#define setFont_10 u8g_font_unifont
#define setFont_11 u8g_font_fixed_v0r
#define setFont_12 u8g_font_chikitar
//number：
#define setFont_num  :u8g_font_freedoomr25n
  
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
#define SSID        "rxy" 
#define PASSWORD    "18500648822"
#define DEVICEID    "578099605" 
String apiKey = "UU65otDMqz3V8V8=Jq1XLP5LGwM=";
/**/
#define HOST_NAME   "api.heclouds.com"//建立tcp连接的api地址
#define HOST_PORT   (80)
#define check_time 10//定义检查时间
unsigned long send_time=millis();//触摸传感器采样时间计数器
unsigned long rev_time=10;//接收数据时间
#include <ESP8266.h>
#include <SoftwareSerial.h>
#define EspSerial mySerial
#define UARTSPEED  9600
SoftwareSerial mySerial(2, 3); 
ESP8266 wifi(&EspSerial);
//ESP8266 wifi(Serial1);                                      //定义一个ESP8266（wifi）的对象
String jsonToSend="yourenlaile";  
String postString;
const int buttonPin=2 ;//读取触摸开关值的端口
int buttonState=0;//初始化状态值
void updateSensorData() {
  if (wifi.createTCP(HOST_NAME, HOST_PORT)) { //建立TCP连接，如果失败，不能发送该数据
    Serial.print("create tcp ok\r\n");
  jsonToSend="{\"Light\":";
jsonToSend+="\""+String(1)+"\"";
        jsonToSend+="}";
   postString="POST /devices/";
    postString+=DEVICEID;
    postString+="/datapoints?type=3 HTTP/1.1";
    postString+="\r\n";
    postString+="api-key:";
    postString+=apiKey;
    postString+="\r\n";
    postString+="Host:api.heclouds.com\r\n";
    postString+="Connection:close\r\n";
    postString+="Content-Length:";
    postString+=jsonToSend.length();
    postString+="\r\n";
    postString+="\r\n";
    postString+=jsonToSend;
    postString+="\r\n";
    postString+="\r\n";
  postString+="\r\n"; 
  const char *postArray = postString.c_str();                 //将str转化为char数组
  Serial.println(postArray);
  wifi.send((const uint8_t*)postArray, strlen(postArray));    //send发送命令，参数必须是这两种格式，尤其是(const uint8_t*)
  Serial.println("send success");   
     if (wifi.releaseTCP()) {                                 //释放TCP连接
        Serial.print("release tcp ok\r\n");
        } 
     else {
        Serial.print("release tcp err\r\n");
        }
      postArray = NULL;                                       //清空数组，等待下次传输数据
  
  } else {
    Serial.print("create tcp err\r\n");
  }
}
static const byte  GETDATA[]  PROGMEM = {
   "GET https://api.heclouds.com/devices/578099605//datapoints?type=3 HTTP/1.1\r\nHost:api.heclouds.com\r\napi-key: UU65otDMqz3V8V8=Jq1XLP5LGwM=8Sro=\r\nConnection: close\r\n\r\n"
};
void revData() { wifi.sendFromFlash(GETDATA, sizeof(GETDATA)); //从Flash读取发送内容，节约内存

  uint8_t buffer[512] = {0};
  uint32_t len = wifi.recv(buffer, sizeof(buffer), 20000);
  if(len>0){ u8g.firstPage();  
  do {
    draw(1);
  } while( u8g.nextPage() );
  delay(1000);}
  else{ Serial.println("no people");
   u8g.firstPage();  
  do {
   draw(0);
  } while( u8g.nextPage() );
  delay(1000); }
}
void draw(int x) {
  u8g.setFont(setFont_0);
  u8g.setPrintPos(0, 30);
  if(x==1)
  u8g.print("people");//显示"people"
else u8g.print("no people");
}
void setup() {
 Serial.begin(115200);
  pinMode(buttonPin, INPUT);//配置引脚的模式为输入模式
    while (!Serial); // wait for Leonardo enumeration, others continue immediately
    Serial.print(F("setup begin\r\n"));
    delay(100);

  WifiInit(EspSerial, UARTSPEED);

  Serial.print(F("FW Version:"));
  Serial.println(wifi.getVersion().c_str());

  if (wifi.setOprToStationSoftAP()) {
    Serial.print(F("to station + softap ok\r\n"));
  } else {
    Serial.print(F("to station + softap err\r\n"));
  }

  if (wifi.joinAP(SSID, PASSWORD)) {
    Serial.print(F("Join AP success\r\n"));

    Serial.print(F("IP:"));
    Serial.println( wifi.getLocalIP().c_str());
  } else {
    Serial.print(F("Join AP failure\r\n"));
  }

  if (wifi.disableMUX()) {
    Serial.print(F("single ok\r\n"));
  } else {
    Serial.print(F("single err\r\n"));
  }

  Serial.print(F("setup end\r\n"));
      Serial.println("system completed!");//调试信息
}
void loop() {
 if(send_time>millis())  send_time=millis();
 if(millis()- send_time>check_time)  
 buttonState = digitalRead(buttonPin);//读取按键的状态  
if(buttonState==1){updateSensorData();                                     //将数据上传到服务器的函数
send_time=millis();}
if(millis()-rev_time>check_time) {revData();
rev_time=millis();}
}
