#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <stdint.h>

// =============================================================================
// payload.h - binary payload packing for LoRaWAN uplinks
//
// 6 bytes, big-endian, one sensor per uplink:
//   [0]     sensor_id    uint8   sensor address (1..255)
//   [1-2]   salinity     uint16  salinity x 100  (3520 = 35.20 ppt)
//   [3-4]   temperature  int16   temperature x 10 (215 = 21.5 C)
//   [5]     flags        uint8   bit0 demo_mode, bit1 sensor_error
// =============================================================================

#define PAYLOAD_SIZE 6

#define FLAG_DEMO_MODE     (1 << 0)
#define FLAG_SENSOR_ERROR  (1 << 1)

typedef struct {
    uint8_t  sensorId;
    float    salinity;      // ppt
    float    temperature;   // degrees C
    bool     valid;         // false on timeout / parse error
} SensorReading;

// Pack a reading into a 6-byte buffer. Returns false if reading is invalid.
bool packPayload(const SensorReading& reading, uint8_t* buf, bool demoMode);

#endif // PAYLOAD_H
