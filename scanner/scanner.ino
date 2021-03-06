/* Inspired by cpixip.
   See http://arduino.cc/forum/index.php/topic,54795.0.html
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

const byte NRF24_CE = 8;
const byte NRF24_CSN = 10;

RF24 radio(NRF24_CE, NRF24_CSN);

const uint8_t num_channels = 128;
uint8_t values[num_channels];

void setup(void) 
{
  delay(1500);
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);  
  //SPI.begin();
  //SPI.transfer(0xFF);
  //SPI.transfer(0xFF);
  //SPI.transfer(0xFF);
  //SPI.transfer(0xFF);
  //SPI.transfer(0xFF);
  //SPI.transfer(0xFF);
  //SPI.transfer(0xFF);
  //SPI.transfer(0xFF);
  //SPI.transfer(0xFF);
  digitalWrite(4, HIGH);

  

  Serial.begin(9600);
  printf_begin();
  
  delay(500);

  radio.begin();
  radio.setAutoAck(false);
  //radio.setRetries(0, 0);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setCRCLength(RF24_CRC_16);

  radio.startListening();
  radio.printDetails();
  if (radio.isPVariant()) {
    Serial.println("Im real NRF24L01+ =) ");
  }
  else {
    Serial.println("Im FAKE NRF24L01+  :( ");
  }

  if (radio.isValid()) {
    Serial.println("Im valid =) ");
  }
  else {
    Serial.println("Im NOT valid  :( ");
  }

  radio.stopListening();

  // Print out header, high then low digit
  int i = 0;
  while ( i < num_channels )
  {
    printf("%x", i >> 4);
    ++i;
  }
  Serial.println();
  i = 0;
  while ( i < num_channels )
  {
    printf("%x", i & 0xf);
    ++i;
  }
  Serial.println();

}

const int num_reps = 100;

void loop(void)
{
  // Clear measurement values
  memset(values, 0, sizeof(values));

  // Scan all channels num_reps times
  int rep_counter = num_reps;
  while (rep_counter--)  {
    int i = num_channels;
    while (i--)    {
      // Select this channel
      radio.setChannel(i);

      // Listen for a little
      radio.startListening();
      delayMicroseconds(128);

      /*
        ___radio.testRPD()
        Test whether a signal (carrier or otherwise)
        greater than or equal to -64dBm is present on the channel
        only for +
        testRPD results are differ from testCarrier. Why?

        ___radio.testCarrier()
        for all nrf`s
      */

      // Did we get a carrier?
      if ( radio.testCarrier() ) {
        ++values[i];
      }
      radio.stopListening();
    }
  }

  // Print out channel measurements, clamped to a single hex digit
  int i = 0;
  while ( i < num_channels )
  {
    //Serial.print(".");
    //Serial.print(values[i], DEC);
    printf("%x", min(0xf, values[i] & 0xf));
    ++i;
  }
  Serial.println();
}



