#define trig1 5 //depan
#define echo1 4
#define trig2 A4 //kanan
#define echo2 A3
#define trig3 A2 //kiri
#define echo3 A1
#define ea 6
#define m1 7
#define m2 8
#define m3 9
#define m4 10
#define eb 11
#define limit_jarak 20
#define Rl1 3
#define Rl2 2

long echotime;
int distance;
long echotime1; 
long echotime2;
long echotime3;
int range1;
int range2;
int range3;
bool dt_kiri, dt_kanan, dt_depan;
byte bit_control;
String data_sensor;
int pump;

void setup() {
  Serial.begin(115200);
  pinMode(trig1, OUTPUT); 
  pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT); 
  pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT); 
  pinMode(echo3, INPUT);
  pinMode(m1,OUTPUT);
  pinMode(m2,OUTPUT);
  pinMode(ea,OUTPUT);
  pinMode(m3,OUTPUT);
  pinMode(m4,OUTPUT);
  pinMode(eb,OUTPUT);  
  pinMode(Rl1,OUTPUT);
  pinMode(Rl2,OUTPUT);
  Serial.println(F("Arduino Restart"));
  delay(2000);
}
 
void loop() {
  run_motor();     
  delay(40);
  ///////////////////////////
  if (Serial.available() > 0){
    pump = Serial.read();
    if (pump == 0){
      digitalWrite(Rl1,LOW);
    }
    else if (pump == 1){
      digitalWrite(Rl1,HIGH);
    }
    ///////////////////////////
    if (pump == 2){
      digitalWrite(Rl2,LOW);
    }
    else if (pump == 3){
      digitalWrite(Rl2,HIGH);
    }
  }
}
//void sensor1(){
//  digitalWrite(trig1, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trig1, LOW);
//  echotime= pulseIn(echo1, HIGH);
//  range1= echotime * 0.034/2;
//}
//void sensor2(){
//  digitalWrite(trig2, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trig2, LOW);
//  echotime= pulseIn(echo2, HIGH);
//  range2= echotime * 0.034/2;
//}
//void sensor3(){
//  digitalWrite(trig3, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trig3, LOW);
//  echotime= pulseIn(echo3, HIGH);
//  range3= echotime * 0.034/2;
//}

void baca_sensor(int trigPin, int echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  echotime = pulseIn(echoPin, HIGH);
  distance = echotime * 0.034/2; 
}
void get_data(){
  baca_sensor(trig3,echo3);
  range3 = distance;
  baca_sensor(trig2,echo2);
  range2 = distance;
  baca_sensor(trig1,echo1);
  range1 = distance;
}
void cacah(){
  if ( range3 >= 0 && range3 <= limit_jarak ){ dt_kiri = 1; } if ( range3 > limit_jarak ){
    dt_kiri = 0;
  }
//------------------------------------------
  if ( range2 >= 0 && range2 <= limit_jarak ){ dt_kanan = 1; } if ( range2 > limit_jarak ){
    dt_kanan = 0;
  }
//------------------------------------------
  if ( range1 >= 0 && range1 <= limit_jarak ){ dt_depan = 1; } if ( range2 > limit_jarak ){
    dt_depan = 0;
  }
//..........................................
//if ( range1 <= 50 && range1 > limit_jarak){ dt_depan = 2; }
//if ( range2 <= 50 && range2 > limit_jarak){ dt_kanan = 2; }
//if ( range3 <= 50 && range3 > limit_jarak){ dt_kiri = 2; } 
}
void control(){
  if ( dt_kiri == 1 && dt_depan == 1 && dt_kanan == 1){
    bit_control = 4;
  }
  else if ( dt_kiri == 1 && dt_depan == 0 && dt_kanan == 1){
    bit_control = 1;
  }
  else if ( range2 > 25 && range1 <= 25 && range3 > 25){
    bit_control = 6;
  }
  else if ( dt_kiri == 0 && dt_depan == 0 && dt_kanan == 1){
    bit_control = 2;
  }
  else if ( dt_kiri == 1 && dt_depan == 0 && dt_kanan == 0){
    bit_control = 3;
  }
  else if ( dt_kiri == 0 && dt_depan == 0 && dt_kanan == 0){
    bit_control = 1;
  }
}
void run_motor(){
//  sensor1(); //depan
//  sensor2(); //kanan
//  sensor3(); //kiri
  get_data();
  cacah();
//  Serial.print("Sensor depan=");
//  Serial.print(range1);
//  Serial.println(" cm");  
//  Serial.print("Sensor kanan=");
//  Serial.print(range2);
//  Serial.println(" cm"); 
//  Serial.print("Sensor kiri=");
//  Serial.print(range3);
//  Serial.println(" cm");
  control();
  switch(bit_control){
  case 1:
    maju();
  break;
  case 2:
    kiri();
  break;
  case 3:
    kanan();
  break;
  case 4:
    berhenti();
  break;
  case 5:
    slow();
  break;
  case 6:
    kanan();
  break;
  }
}
void maju()
{
  digitalWrite(m1,HIGH); //kiri
  digitalWrite(m2,LOW);
  analogWrite(ea,200);
  digitalWrite(m3,HIGH); //kanan
  digitalWrite(m4,LOW);
  analogWrite(eb,200);
}
void kiri()
{
  //belok kiri
  digitalWrite(m3,HIGH); //kiri
  digitalWrite(m4,LOW);
  analogWrite(ea,255);
  digitalWrite(m1,LOW); //kanan
  digitalWrite(m2,HIGH);
  analogWrite(eb,255);
  delay(300);
  digitalWrite(m3,HIGH); //kiri
  digitalWrite(m4,LOW);
  analogWrite(ea,255);
  digitalWrite(m1,LOW); //kanan
  digitalWrite(m2,LOW);
  analogWrite(eb,0);
  delay(300);
}
void kanan()
{
  //belok kanan
  digitalWrite(m1,HIGH); //kiri
  digitalWrite(m2,LOW);
  analogWrite(ea,255);
  digitalWrite(m3,LOW); //kanan
  digitalWrite(m4,HIGH);
  analogWrite(eb,255);
  delay(300);
  digitalWrite(m1,HIGH); //kiri
  digitalWrite(m2,LOW);
  analogWrite(ea,200);
  digitalWrite(m3,LOW); //kanan
  digitalWrite(m4,LOW);
  analogWrite(eb,0);
  delay(300);
}
void berhenti()
{
  digitalWrite(m1,LOW); //kiri
  digitalWrite(m2,LOW);
  analogWrite(ea,0);
  digitalWrite(m3,LOW); //kanan
  digitalWrite(m4,LOW);
  analogWrite(eb,0);
}
void slow()
{
  digitalWrite(m1,HIGH); //kiri
  digitalWrite(m2,LOW);
  analogWrite(ea,180);
  digitalWrite(m3,HIGH); //kanan
  digitalWrite(m4,LOW);
  analogWrite(eb,180);
}
