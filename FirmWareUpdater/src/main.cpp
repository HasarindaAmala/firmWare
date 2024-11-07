#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define LED_PIN 23

// Callback class to handle connection and disconnection events
class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) override {
      Serial.println("Device connected");
      // Optionally do something when a device connects
    }

    void onDisconnect(BLEServer* pServer) override {
      Serial.println("Device disconnected");
      // Restart advertising so the device can reconnect
      BLEDevice::startAdvertising();
      Serial.println("Restarted advertising");
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  pinMode(LED_PIN,OUTPUT);

  BLEDevice::init("Long name works now");
  BLEServer *pServer = BLEDevice::createServer();
  
  // Set our custom callbacks
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setValue("Hello World says Neil");
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // helps with iPhone connections
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  
  Serial.println("Characteristic defined! Now you can read it in your phone!");
}

void loop() {
  digitalWrite(LED_PIN,HIGH);
  delay(1000);
  digitalWrite(LED_PIN,LOW);
  delay(1000);
}
