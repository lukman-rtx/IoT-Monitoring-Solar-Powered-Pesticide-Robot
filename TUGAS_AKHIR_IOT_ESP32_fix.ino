//----------------------بِسْمِ اللَّهِ الرَّحْمَنِ الرَّحِيم --------------------//
//----------- ESP 32 Automatic Pesticide Solar Charge Robot------------//
//----------- Made by : Lukman Nul Hakim & Juliansyah Akbar------------//
//----------- Last Update 19-6-2022-----Created 12-4-2022--------------//

//Library
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#define BLYNK_PRINT Serial 

//Password Wifi
char auth[] = "t_cJJp2dHz_WD1UTlRicjjKU0PjnQNdE"; 
char ssid[] = "ar12";
char pass[] = "bismillah";

//Stopwatch
int jam, menit, detik, miliDetik;
unsigned long over;
unsigned long mulai, selesai, dataStopWatch;
int i=0;
int fPaus = 0;
long lastButton = 0; 
long delayAntiBouncing = 50; 
long dataPaus = 0;
long dataPausA = 0;

//NeverEndStopwatch
int jamA, menitA, detikA, miliDetikA;
unsigned long overA;
unsigned long mulaiA, selesaiA, dataStopWatchA;

//Estimasi
int jamB, menitB, detikB;
unsigned long overB;

//Arus Tegangan Daya
float VoutB = 0.00;
float VinB = 0.00;
int valB = 0;

float VoutP = 0.00;
float VinP = 0.00;
int valP = 0;

//ACS
float ArusHasilP = 0.0;
float ArusBacaP = 0.0;
float BaseVolP = 1.683;

float ArusHasilB = 0.0;
float ArusBacaB = 0.0;
float BaseVolB = 1.683;

//Daya
float WattB = 0.0;
float WattP = 0.0;

//Estimasi
float avg_curr = 0.0;
float mA_old = 0.0;
float mAnew = 0.0;
float mA = 0.0;
int sisa_batt = 0;
float jam_sisa = 0.0;
int jam_hitung =0;

void setup() {
  dataStopWatch = 0;
  dataPaus = 0;
  pinMode(23, OUTPUT); //RLPanel
  pinMode(22, INPUT);  //RLDaya
  pinMode(19, INPUT);
  pinMode(36, INPUT); //Vb
  pinMode(39, INPUT); //Vp
  pinMode(34, INPUT); //Ap
  pinMode(35, INPUT); //Ab
  uint32_t currentFrequency;
  Blynk.begin(auth, ssid, pass);
  Serial.begin(115200); 
  Serial.print("Terhubung ke ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
  delay(100);
  Serial.print(".");
// Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi Terhubung !");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  }
}

bool isFirstConnect = true;
BLYNK_CONNECTED() {
  if (isFirstConnect)
  {
    Blynk.syncAll();
    isFirstConnect = false;
  }
}

void loop() {
  Blynk.run();
  stopwatch();
  rumus_arustegangan_avg();
  estimasi_daya();
  relayLogic();
//  digitalWrite(22, LOW);
}

void stopwatch(){
  if (digitalRead(22) == LOW){
     dataPaus = dataStopWatch;
     mulai = millis();
  }
  if (digitalRead(19) == HIGH && digitalRead(22) == LOW){
    i=0;
    dataStopWatch = 0;
    dataPaus = 0;   
    jam =0;
    selesai =0;
    mulai =0;
    over =0;
    menit =0;
    detik =0;
    miliDetik =0;  
    dataStopWatchA = 0;
    jamA =0;
    selesaiA =0;
    overA =0;
    menitA =0;
    detikA =0;
    miliDetikA =0;  
    avg_curr = 0.0;
    mA_old = 0.0;
    mA = 0.0;
    mAnew = 0.0;
    jamB =0;
    overB =0;
    menitB =0;
    detikB =0;
    jam_sisa =0;
    jam_hitung =0;
    Blynk.virtualWrite(V4, (String)jam + " : " + (String)menit + " : " + (String)detik + " : " + (String)miliDetik);
    Blynk.virtualWrite(V11, (String)jamA + " : " + (String)menitA + " : " + (String)detikA + " : " + (String)miliDetikA);
  }
  if (digitalRead(22) == HIGH){
    i = 1;
    selesai = millis(); 
    dataStopWatch = selesai - mulai;
    dataStopWatch = dataPaus + dataStopWatch;
    jam = int(dataStopWatch / 3600000);
    over = dataStopWatch % 3600000;
    menit = int(over / 60000);
    over = over % 60000;
    detik = int(over / 1000);
    miliDetik = over % 1000;
    Blynk.virtualWrite(V4, (String)jam + " : " + (String)menit + " : " + (String)detik + " : " + (String)miliDetik);
  }
  if (i == 1){
    selesaiA = millis(); 
    dataStopWatchA = selesaiA - mulaiA;
    dataStopWatchA = dataPausA + dataStopWatchA;
    jamA = int(dataStopWatchA / 3600000);
    overA = dataStopWatchA % 3600000;
    menitA = int(overA / 60000);
    overA = overA % 60000;
    detikA = int(overA / 1000);
    miliDetikA = overA % 1000;
    Blynk.virtualWrite(V11, (String)jamA + " : " + (String)menitA + " : " + (String)detikA + " : " + (String)miliDetikA);
  }
  if (i == 0){
     dataPausA = dataStopWatchA;
     mulaiA = millis();
  }
}

void rumus_arustegangan_avg(){
//------------------------ARUS-------------------------------------
//------------------------Pakai----------------
  int invertACSB = (map(analogRead(35), 0, 4095, 4095, 0));
  ArusHasilB = (((invertACSB) * (3.3 / 4095.0)) - BaseVolB ) / 0.34;
  Blynk.virtualWrite(V7,ArusHasilB);
//------------------------Charge---------------
  int invertACSP = (map(analogRead(34), 0, 4095, 4095, 0));
  ArusHasilP = (((invertACSP) * (3.3 / 4095.0)) - BaseVolP ) / 0.34;
  Blynk.virtualWrite(V2,ArusHasilP);
  

//-----------------------TEGANGAN----------------------------------  
//------------------------Baterai---------------
  valB = analogRead(36); 
  VoutB = (valB * 3.33) / 4095.0;
  VinB = VoutB / (7450.0/(7450.0+31000.0));
  Blynk.virtualWrite(V6,VinB);
//-------------------------Panel-----------------
  valP = analogRead(39); 
  VoutP = (valP * 3.33) / 4095.0;
  VinP = VoutP / (7450.0/(7450.0+31000.0));
  Blynk.virtualWrite(V1,VinP);

//---------------------------DAYA-----------------------------------
//-----------------------------------------------
  WattB = VinB*ArusHasilB;
  Blynk.virtualWrite(V5,(String)WattB + " W");
  Blynk.virtualWrite(V12,WattB);
  WattP = VinP*ArusHasilP;
  Blynk.virtualWrite(V8,(String)WattP + " W");
  Blynk.virtualWrite(V13,WattP);
}

void estimasi_daya(){
  sisa_batt = VinB*1000;
  sisa_batt = constrain(sisa_batt,11400,14200);
  sisa_batt = map(sisa_batt,11400,14200,0,6000);
  sisa_batt = constrain(sisa_batt,0,6000);
//--------------------AVERAGE ARUS-----------------
  mA = ArusHasilB*1000;
  mAnew = mA + mA_old;
  avg_curr = mAnew / detikA;
  mA_old = mAnew;
//-------------------HITUNG ESTIMASI---------------
  jam_sisa = ((sisa_batt/avg_curr)*3600);
  jam_hitung = jam_sisa;
  jamB = int(jam_hitung / 3600);
  overB = jam_hitung % 3600;
  menitB = int(overB / 60);
  overB = overB % 60;
  detikB = int(overB / 1);  
  Blynk.virtualWrite(V3, (String)jamB + " : " + (String)menitB + " : " + (String)detikB);
}
 
void relayLogic(){
  //-------------------------PANEL-----------------//
  if (VinP <= 10.9 || VinP >= 15.0){
  Blynk.virtualWrite(V9,"Not Charging");
  }
  else {
  Blynk.virtualWrite(V9,"Charging");  
  }
  
  //-------------------------DAYA/LFD-----------------//
  if (VinB >= 11.8 && VinB <= 14.0){
  Blynk.virtualWrite(V10,"Normal");  
  }
  else if (VinB <= 11.7 && VinB >= 11.3){
  Blynk.virtualWrite(V10,"Low");  
  }
  else if (VinB <= 11.4){
  Blynk.virtualWrite(V10,"WARNING"); 
  // digitalWrite(22, LOW);          //---LFD-----
  }
  else if (VinB >= 14.1){
  Blynk.virtualWrite(V10,"OVERCHARGE");  
  }
}
