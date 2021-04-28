#include "muestras_corriente.h"
const int N = 4;
float X[N];
const int fs = 2;
float Ts, interval;
float Y[N];
uint16_t xIndex, yIndex, vilIndex;

uint32_t tNow, tLast;

void setup() {
  Serial.begin(115200);
  Ts = 1/(float) fs;
  interval = 1000 * Ts;
}

void loop() {

tNow = millis();
if(tNow - tLast >= interval){
  tLast = tNow;

  X[xIndex] = ViL[vilIndex];
 // Serial.println(1000 * X[xIndex]);

  for(int i = 0; i < N; i++){
    uint16_t index = getIndex(xIndex, -i, N);
   // Serial.println(index);
    Serial.println(1000 * X[index]);
  }


  
  xIndex = (xIndex + 1) & 3;
  vilIndex = (vilIndex + 1) & 63;
  Serial.println('\n');
}





}



uint16_t getIndex(uint16_t i, int offset, int N) {
  int pos = i + offset;
  if (pos < 0)
    return pos + N;
  else if (pos > N)
    return N - pos;
  else
    return pos;
}
