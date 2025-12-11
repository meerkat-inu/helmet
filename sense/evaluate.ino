#include "evaluate.h"

void evaluate_risk(void) {
  float diff_az = max_az - min_az;
  float diff_ax = max_ax - min_ax;
  float diff_gz = max_gz - min_gz;
  float diff_gx = max_gx - min_gx;

  //Serial.println(String(diff_gx) + "\t" + String(diff_gz));

  float greater_g = diff_gz > diff_gx ? diff_gz : diff_gx;
  if (greater_g > 250 ||
        ((diff_az > 2.0 || diff_ax > 1.2) && greater_g > 200)) {
    Serial.println("[2단계]");
    //playSoundByLevel(2);
  }
  else if (greater_g > 200 ||
        ((diff_az > 1.0 || diff_ax > 0.8) && greater_g > 100)) {
    Serial.println("[1단계]"); 
    //playSoundByLevel(1);
  }
  Serial.println("diff_az: " + String(diff_az) + "\tdiff_ax: " + String(diff_ax));
  Serial.println("greater_g: " + String(greater_g));
}
