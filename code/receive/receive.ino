#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10);  // Initialize the RF24 object with the CE and CSN pins

const uint64_t pipe = 0xF0F0F0F0E1LL;  // Unique address for communication

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, pipe);  // Set the reading pipe
  radio.startListening();  // Start listening for incoming data
}

void loop() {
  if (radio.available()) {  // Check if data is available for reading
    int16_t EMG_pin;  // Variable to store the received EMG data
    radio.read(&EMG_pin, sizeof(EMG_pin));  // Read the EMG data from the NRF24L01 module
    Serial.println(EMG_pin);  // Print the received EMG data
  }
}
