//----------------------بِسْمِ اللَّهِ الرَّحْمَنِ الرَّحِيم --------------------//
//----------- Arduino Nano Automatic Pesticide Solar Charge Robot------------//
//                SERIAL ONLY !               //
//----------- Made by : Lukman Nul Hakim & Juliansyah Akbar------------//
//----------- Last Update 26-6-2022-----Created 12-4-2022--------------//


#define Rl1 3  //// kiri
#define Rl2 2  //// kanan
#define LED 12
int pump;

void setup() {
  Serial.begin(115200); 
  pinMode(Rl1,OUTPUT);
  pinMode(Rl2,OUTPUT);
  pinMode(LED,OUTPUT);
  digitalWrite(Rl2,LOW);
  digitalWrite(Rl1,LOW);
  delay(1500);
  digitalWrite(Rl2,HIGH);
  digitalWrite(Rl1,HIGH);
}
 
void loop() {
  ///////////////////////////
  if (Serial.available() > 0){
    digitalWrite(LED,HIGH);
    pump = Serial.read();
    if (pump == 0){            ///kiriii
      digitalWrite(Rl1,HIGH);
    }
    else if (pump == 1){
      digitalWrite(Rl1,LOW);
    }
    ///////////////////////////
    if (pump == 2){           ///kanannn
      digitalWrite(Rl2,HIGH);
    }
    else if (pump == 3){
      digitalWrite(Rl2,LOW);
    }
  }
}
