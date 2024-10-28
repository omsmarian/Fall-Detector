#include <Arduino.h>
#include <Wire.h>

 //Analog port 4 (A4) = SDA (serial data)
//Analog port 5 (A5) = SCL (serial clock)
#define SIGNAL_PATH_RESET 0x68
#define I2C_SLV0_ADDR 0x37
#define ACCEL_CONFIG 0x1C
#define FF_THR 0x1D // Motion detection threshold bits [7:0]
#define FF_DUR 0x1E // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
#define MOT_DETECT_CTRL 0x69
#define INT_ENABLE 0x38
#define WHO_AM_I_MPU6050 0x75 // Should return 0x68
#define INT_STATUS 0x3A
//when nothing connected to AD0 than address is 0x68
#define ADO 0
#if ADO
#define MPU6050_ADDRESS 0x69 // Device address when ADO = 1
#else
#define MPU6050_ADDRESS 0x68 // Device address when ADO = 0
#endif

int wakePin = 2; // pin used for waking up  
int flag = 0;

ICACHE_RAM_ATTR void miua()
{
  flag = !flag;
}


/*    Example for using write byte
      Configure the accelerometer for self-test
      writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0xF0); // Enable self test on all three axes and set accelerometer range to +/- 8 g */
void writeByte(uint8_t address, uint8_t subAddress, uint8_t data) {
  Wire.begin();
  Wire.beginTransmission(address); // Initialize the Tx buffer
  Wire.write(subAddress); // Put slave register address in Tx buffer
  Wire.write(data); // Put data in Tx buffer
  Wire.endTransmission(); // Send the Tx buffer
  //  Serial.println("mnnj");

}

//example showing using readbytev   ----    readByte(MPU6050_ADDRESS, GYRO_CONFIG);
uint8_t readByte(uint8_t address, uint8_t subAddress) {
  uint8_t data; // `data` will store the register data   
  Wire.beginTransmission(address); // Initialize the Tx buffer
  Wire.write(subAddress); // Put slave register address in Tx buffer
  Wire.endTransmission(false); // Send the Tx buffer, but send a restart to keep connection alive
  Wire.requestFrom(address, (uint8_t) 1); // Read one byte from slave register address 
  data = Wire.read(); // Fill Rx buffer with result
  return data; // Return data read from slave register
}

void setup() {
  
  /*
   * #define SIGNAL_PATH_RESET  0x68
     #define I2C_SLV0_ADDR      0x37
     #define ACCEL_CONFIG       0x1C 
     #define MOT_THR            0x1F                    // Motion detection threshold bits [7:0]
     #define MOT_DUR            0x20                    // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
     #define MOT_DETECT_CTRL    0x69
     #define INT_ENABLE         0x38
     #define WHO_AM_I_MPU6050   0x75                      // Should return 0x68
     #define INT_STATUS 0x3A*/
  Serial.begin(115200);
  writeByte(MPU6050_ADDRESS, 0x6B, 0x00);
  writeByte(MPU6050_ADDRESS, SIGNAL_PATH_RESET, 0x07);  //Reset all internal signal paths in the MPU-6050 by writing 0x07 to register 0x68;
  writeByte(MPU6050_ADDRESS, I2C_SLV0_ADDR, 0x00);      //write register 0x37 to select how to use the interrupt pin. For an active high, push-pull signal that stays until register (decimal) 58 is read, write 0x20.
  writeByte(MPU6050_ADDRESS, ACCEL_CONFIG, 0x01);       //Write register 28 (==0x1C) to set the Digital High Pass Filter, bits 3:0. For example set it to 0x01 for 5Hz. (These 3 bits are grey in the data sheet, but they are used! Leaving them 0 means the filter always outputs 0.)
  writeByte(MPU6050_ADDRESS, FF_THR, 60);               //Write the desired Motion threshold to register 0x1F (For example, write decimal 20).  
  writeByte(MPU6050_ADDRESS, FF_DUR, 30);               //Set motion detect duration to 1  ms; LSB is 1 ms @ 1 kHz rate  
  writeByte(MPU6050_ADDRESS, MOT_DETECT_CTRL, 0x14);    // metemos 1ms mas de calibracion y configuramos el FF
  writeByte(MPU6050_ADDRESS, INT_ENABLE, 0x80);         //write register 0x38, bit 6 (0x40), to enable motion detection interrupt.     
  writeByte(MPU6050_ADDRESS, 0x37, 160);                // now INT pin is active low

  pinMode(2, INPUT);                                    // sets the digital pin 7 as input

  pinMode(D5, INPUT_PULLUP);                            // wakePin is pin no. 2
  attachInterrupt(digitalPinToInterrupt(D5), miua, FALLING);
  pinMode(LED_BUILTIN, OUTPUT);                         //   led is pin no. 13
  digitalWrite(LED_BUILTIN, LOW);                       // turn onn LED
  
}

uint8_t count;
uint16_t readdata;
void loop() {
  if (flag) 
  {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
  }

  readdata = readByte(MPU6050_ADDRESS, 0x3A);
  if(readdata == 0b10000001 || count == 100)
  {
    Serial.print(readdata, BIN);
    Serial.print(",");
    readdata = readByte(MPU6050_ADDRESS, 0x37);
    Serial.println(readdata, BIN);
    count = 0;
  }
  count++;
}