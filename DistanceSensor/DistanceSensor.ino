#include <DistanceGP2Y0A21YK.h>

DistanceGP2Y0A21YK Dist;
int distance;

void setup()
{
  Serial.begin(9600);
/* Connections
 *  Black - Ground
 *  Red - 5V
 *  White - A0 input
 */

  Dist.begin(A0);
}

void loop()
{
  distance = Dist.getDistanceCentimeter();
  Serial.print("\nDistance in centimeters: ");
  Serial.print(distance);
  delay(10000);                                     //make it readable

}
