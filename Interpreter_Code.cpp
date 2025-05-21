#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Flex sensor pins
#define FLEX_SENSOR_1 32
#define FLEX_SENSOR_2 33
#define FLEX_SENSOR_3 34
#define FLEX_SENSOR_4 35
#define FLEX_SENSOR_5 36

// Create an MPU6050 object
Adafruit_MPU6050 mpu;

// Global variables for flex states
int a, b, c, d, e;

void setup() 
{
  // Initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial) 
  {
    delay(10); // Wait for Serial Monitor to initialize
  }
  Serial.println("Initializing...");

  // Initialize MPU6050
  if (!mpu.begin()) 
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1) 
    {
      delay(10);
    }
  }
  Serial.println("MPU6050 initialized!");

  // Configure MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // Configure flex sensor pins as input
  pinMode(FLEX_SENSOR_1, INPUT);
  pinMode(FLEX_SENSOR_2, INPUT);
  pinMode(FLEX_SENSOR_3, INPUT);
  pinMode(FLEX_SENSOR_4, INPUT);
  pinMode(FLEX_SENSOR_5, INPUT);

  Serial.println("Setup complete!");
}

void loop() 
{
  // Read raw flex sensor values
  int rawFlex1 = analogRead(FLEX_SENSOR_1);
  int rawFlex2 = analogRead(FLEX_SENSOR_2);
  int rawFlex3 = analogRead(FLEX_SENSOR_3);
  int rawFlex4 = analogRead(FLEX_SENSOR_4);
  int rawFlex5 = analogRead(FLEX_SENSOR_5);

  // Map raw values to range 0–1024
  int flex1 = map(rawFlex1, 0, 4095, 0, 1024);
  int flex2 = map(rawFlex2, 0, 4095, 0, 1024);
  int flex3 = map(rawFlex3, 0, 4095, 0, 1024);
  int flex4 = map(rawFlex4, 0, 4095, 0, 1024);
  int flex5 = map(rawFlex5, 0, 4095, 0, 1024);

  // Classify flex sensor states
  a = (flex1 < 100) ? 1 : (flex1 < 650) ? 2 : 3;
  b = (flex2 < 450) ? 1 : (flex2 < 750) ? 2 : 3;
  c = (flex3 < 500) ? 1 : (flex3 < 800) ? 2 : 3;
  d = (flex4 < 600) ? 1 : (flex4 < 800) ? 2 : 3;
  e = (flex5 < 100) ? 1 : (flex5 < 650) ? 2 : 3;

  // MPU6050 sensor events
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  // Print mapped flex sensor values
  Serial.println("Mapped Flex Sensor Values (0–1024):");
  Serial.print("Flex 1: "); Serial.print(flex1);
  Serial.print(" | Flex 2: "); Serial.print(flex2);
  Serial.print(" | Flex 3: "); Serial.print(flex3);
  Serial.print(" | Flex 4: "); Serial.print(flex4);
  Serial.print(" | Flex 5: "); Serial.println(flex5);

  // Print flex state
  Serial.println("Flex States:");
  Serial.print("Flex 1: "); Serial.print(a);
  Serial.print(" | Flex 2: "); Serial.print(b);
  Serial.print(" | Flex 3: "); Serial.print(c);
  Serial.print(" | Flex 4: "); Serial.print(d);
  Serial.print(" | Flex 5: "); Serial.println(e);

  // Uncomment the following to print MPU6050 accelerometer and gyroscope data if needed
  /*
  Serial.println("Accelerometer:");
  Serial.print("X: "); Serial.print(accel.acceleration.x); Serial.print(" m/s^2");
  Serial.print(" | Y: "); Serial.print(accel.acceleration.y); Serial.print(" m/s^2");
  Serial.print(" | Z: "); Serial.println(accel.acceleration.z); Serial.println(" m/s^2");

  Serial.println("Gyroscope:");
  Serial.print("X: "); Serial.print(gyro.gyro.x); Serial.print(" rad/s");
  Serial.print(" | Y: "); Serial.print(gyro.gyro.y); Serial.print(" rad/s");
  Serial.print(" | Z: "); Serial.println(gyro.gyro.z); Serial.println(" rad/s");
  */

  // Delay for readability
  Serial.println("\n---------------------------------\n");
  delay(2000);
}
