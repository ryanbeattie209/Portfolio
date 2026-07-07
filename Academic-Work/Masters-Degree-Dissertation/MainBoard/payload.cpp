#include "payload.h"
#include <stdint.h>

// =============================================================================
// payload.cpp - binary payload packing
// =============================================================================

bool packPayload(const SensorReading& reading, uint8_t* buf, bool demoMode) {
    if (!reading.valid) {
        // Caller skips the uplink entirely on an invalid reading, so the
        // sensor_error flag path was dead code in the previous build and is
        // removed. If you later want to uplink error reports, transmit a
        // reading with valid=true and set FLAG_SENSOR_ERROR explicitly.
        return false;
    }

    // Byte 0: sensor ID
    buf[0] = reading.sensorId;

    // Bytes 1-2: salinity x 100, big-endian
    uint16_t sal = (uint16_t)(reading.salinity * 100.0f);
    buf[1] = (sal >> 8) & 0xFF;
    buf[2] = sal & 0xFF;

    // Bytes 3-4: temperature x 10, signed, big-endian
    int16_t temp = (int16_t)(reading.temperature * 10.0f);
    buf[3] = (temp >> 8) & 0xFF;
    buf[4] = temp & 0xFF;

    // Byte 5: flags
    uint8_t flags = 0;
    if (demoMode) flags |= FLAG_DEMO_MODE;
    buf[5] = flags;

    return true;
}
