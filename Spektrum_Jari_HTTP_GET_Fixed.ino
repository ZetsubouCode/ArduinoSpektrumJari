#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "AHHA";
const char* password = "biomedical";

// BIKIN NAMA VARIABEL YANG JELAS! Jangan kasih nama absurd, tidak membantu dalam memahami code.
// Tidak perlu membuat variabel yang redundan. Jika bisa dipakai lagi tanpa mengganggu flow code, jangan buat variabel baru untuk tugas yang sama.

String host = "https://deteksiguladarah-gluconov.000webhostapp.com/";

#include <ESP32Servo.h>
Servo myservo;
int servoPin = 13;

int data1[80],data2[80],data3[80],i,pinA,pinB;
char tulis[100],pos,pos1,pos2,pos3,pos4,pos5;
void setup() {
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  
  pinMode(12, INPUT_PULLUP);     // Initialize the LED_BUILTIN pin as an output
  pinMode(23, INPUT_PULLUP);
  Serial.begin(9600);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object

  ///atur sudut servo disini///
  // atur sudut 75-180
  pos1=95;
  pos2=90;
  pos3=85;
  pos4=80;
  pos5=75;
  //////////////////////////////
}

// the loop function runs over and over again forever
void loop() {
  /*for(i=0;i<50;i++){
    if(digitalRead(23) == HIGH) pinA = 1;
    else {pinA=0; if(pos < 180) pos++;}
    if(digitalRead(12) == HIGH) pinB = 1;
    else {pinB=0; if(pos > 75) pos--;}
  }*/
  int hasil;
  if (Serial.available()) {
    int inByte = Serial.read();
    if(inByte == '1'){
      AmbilData(pos1);
      AmbilData(pos2);
      AmbilData(pos3);
      AmbilData(pos4);
      AmbilData(pos5);
      
    }
  //    if(inByte == 'A'){
  //     b="1";     
  //   }
  //    if(inByte == 'B'){
  //     b="2";     
  //   }
  //    if(inByte == 'C'){
  //     b="3";     
  //   }
  //    if(inByte == 'D'){
  //     b="4";     
  //   }
    
  }
 
  hasil = 1; //dummy hasil, perlu diubah atau di assign ulang dengan value hasil aslinya dengan pemanggilan fungsi atau variabel
  String getData = (String)"hasil=" + hasil;
  
  HTTPClient http;
  http.begin(host+"check_active");
  // http.addHeader("Content-Type", "application/x-www-form-urlencoded"); <- header form tidak dibutuhkan untuk request method GET! Itu adalah header POST.
  auto httpCode = http.GET();
  String id = http.getString(); // mendapatkan id pasien dengan requeust ke website. Mengecek apakah ada yang aktif atau tidak.
  http.end();
  Serial.println(id);
  Serial.println("Kode respon website terhadap request data = "+httpCode);
  if(id!=NULL){
    http.begin(host+"registration/pasien_id/"+id+"/update_result/"+hasil);
    // http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    httpCode = http.GET();
    String payload = http.getString();

    http.end();

    Serial.println(getData);
    Serial.println(payload);
    Serial.print("Host tujuan = "+host+"id pasien = "+id+"\nHasil periksa = "+hasil);
  } else {
    Serial.println("Belum ada pasien yang mendapat akses alat deteksi! Silakan scan QR terlebih dahulu.");
  }
}

void AmbilData(int var){
      pos=var;
      if(pos < 75) pos=75;
      else if(pos>180) pos=180;
      myservo.write(pos); 
      
      for(i=0;i<50;i++) {
        data1[i]=analogRead(35);
      }
      
      sprintf(tulis,"///data sudut: %3d///",pos);
      Serial.println(tulis);
      for(i=0;i<50;i++){
        sprintf(tulis,"%2d: %4d",i+1,data1[i]);
        Serial.println(tulis);
      }
}
