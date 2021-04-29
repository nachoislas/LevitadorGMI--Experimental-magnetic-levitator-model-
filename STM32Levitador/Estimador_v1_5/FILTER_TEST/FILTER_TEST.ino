//#include "muestras_corriente.h"
#include "fdacoefs.h"
//const int N = 8;

float ViL[] = {0.31221, 0.31073, 0.30926, 0.30778, 0.3063, 0.30483, 0.30335, 0.30187, 0.3004, 0.29892, 0.29745, 0.29597, 0.2945, 0.29303,
               0.29155, 0.29008, 0.28861, 0.28713, 0.28847, 0.28981, 0.29115, 0.29248, 0.29382, 0.29515, 0.29649, 0.29783, 0.29916,
               0.3005, 0.30183, 0.30316, 0.3045, 0.30583, 0.30717, 0.3085, 0.30983, 0.31116
              };

float X[N];
const int fs = 50;
float Ts, interval;
float Y[N];
uint16_t xIndex, yIndex, vilIndex;

uint32_t tNow, tLast;


//filtro
//const float b[] = {0.171185955405235 , 0.521617472171783 , 0.521617472171783 , 0.171185955405235};

void setup() {
  Serial.begin(115200);
  Ts = 1 / (float) fs;
  interval = 1000 * Ts;
}

void loop() {

  tNow = millis();
  if (tNow - tLast >= interval) {
    tLast = tNow;

    X[0] = 1000 * ViL[vilIndex];
    Y[0] = 0;

    //aplicar filtro
    for (int i = 0; i < N; i++) {
      Y[0] += B[i] * X[i];
    }

    for (int i = N - 1; i > 0; i--) {
      X[i] = X[i - 1];
      Y[i] = Y[i - 1];
    }

    Serial.println(X[0]);
    Serial.print(",");
    Serial.println(Y[0]);
    vilIndex++;
    if (vilIndex == sizeof(ViL) / sizeof(float))
      vilIndex = 0;
  }
}
