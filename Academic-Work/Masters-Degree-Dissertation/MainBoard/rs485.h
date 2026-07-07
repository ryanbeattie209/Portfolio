#ifndef RS485_H
#define RS485_H

#include <HardwareSerial.h>
#include "payload.h"
#include "config.h"

// =============================================================================
// rs485.h - RS485 master polling handler
//
// ESP32-S3 is the bus master. Sensor boards are slaves addressed 1..N.
//   Master request:  "<ADDR>,REQ\n"           e.g. "1,REQ\n"
//   Slave response:  "<ADDR>,<SAL>,<TEMP>\n"   e.g. "1,3520,215\n"
//   SAL  = salinity    x 100 (integer)
//   TEMP = temperature x 10  (integer, signed)
//   9600 8N1. Slave responds within RS485_RESPONSE_TIMEOUT_MS, manages own DE/RE.
// =============================================================================

class RS485Handler {
public:
    void begin();
    SensorReading pollSensor(uint8_t address);

private:
    void setTransmitMode();
    void setReceiveMode();
    void sendRequest(uint8_t address);
    bool parseResponse(const char* buf, uint8_t expectedAddress, SensorReading& out);
    bool readLineWithTimeout(char* buf, size_t maxLen, uint32_t timeoutMs);
};

extern RS485Handler rs485;

#endif // RS485_H
