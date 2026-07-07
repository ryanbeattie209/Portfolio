#include "rs485.h"
#include "config.h"
#include <Arduino.h>

// =============================================================================
// rs485.cpp - RS485 master polling (unchanged from the reviewed original;
// the half-duplex DE/RE direction handling here was already correct).
// =============================================================================

static HardwareSerial RS485Serial(2);   // UART2 on ESP32-S3

RS485Handler rs485;

void RS485Handler::begin() {
    pinMode(RS485_DIR_PIN, OUTPUT);
    setReceiveMode();

    RS485Serial.begin(RS485_BAUD_RATE, RS485_SERIAL_CFG, RS485_RX_PIN, RS485_TX_PIN);

    DEBUG_PRINTLN("[RS485] Initialised");
}

SensorReading RS485Handler::pollSensor(uint8_t address) {
    SensorReading result;
    result.sensorId    = address;
    result.salinity    = 0.0f;
    result.temperature = 0.0f;
    result.valid       = false;

    while (RS485Serial.available()) RS485Serial.read();   // flush stale RX

    sendRequest(address);

    setReceiveMode();
    delay(RS485_TURNAROUND_DELAY_MS);

    char lineBuf[64] = {0};
    bool got = readLineWithTimeout(lineBuf, sizeof(lineBuf), RS485_RESPONSE_TIMEOUT_MS);

    if (!got) {
        DEBUG_PRINTF("[RS485] Timeout waiting for sensor %d\n", address);
        return result;
    }
    if (!parseResponse(lineBuf, address, result)) {
        DEBUG_PRINTF("[RS485] Bad response from sensor %d: %s\n", address, lineBuf);
        return result;
    }

    DEBUG_PRINTF("[RS485] Sensor %d: salinity=%.2f ppt, temp=%.1f C\n",
        address, result.salinity, result.temperature);
    return result;
}

void RS485Handler::setTransmitMode() {
    digitalWrite(RS485_DIR_PIN, HIGH);
    delayMicroseconds(10);
}

void RS485Handler::setReceiveMode() {
    RS485Serial.flush();          // wait for TX to fully drain before releasing bus
    delayMicroseconds(10);
    digitalWrite(RS485_DIR_PIN, LOW);
}

void RS485Handler::sendRequest(uint8_t address) {
    setTransmitMode();
    char req[16];
    snprintf(req, sizeof(req), "%d,REQ\n", address);
    RS485Serial.print(req);
    setReceiveMode();
    DEBUG_PRINTF("[RS485] Sent request to sensor %d\n", address);
}

bool RS485Handler::readLineWithTimeout(char* buf, size_t maxLen, uint32_t timeoutMs) {
    uint32_t start = millis();
    size_t pos = 0;
    while ((millis() - start) < timeoutMs) {
        if (RS485Serial.available()) {
            char c = (char)RS485Serial.read();
            if (c == '\n') { buf[pos] = '\0'; return pos > 0; }
            if (c == '\r') continue;
            if (pos < maxLen - 1) buf[pos++] = c;
            else { DEBUG_PRINTLN("[RS485] RX buffer overflow"); return false; }
        }
    }
    return false;
}

bool RS485Handler::parseResponse(const char* buf, uint8_t expectedAddress, SensorReading& out) {
    char tmp[64];
    strncpy(tmp, buf, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    char* token = strtok(tmp, ",");
    if (token == nullptr) return false;
    uint8_t addr = (uint8_t)atoi(token);
    if (addr != expectedAddress) {
        DEBUG_PRINTF("[RS485] Address mismatch: expected %d got %d\n", expectedAddress, addr);
        return false;
    }

    token = strtok(nullptr, ",");
    if (token == nullptr) return false;
    for (char* p = token; *p; p++)
        if (*p != '-' && (*p < '0' || *p > '9')) { DEBUG_PRINTLN("[RS485] Non-numeric salinity"); return false; }
    int salInt = atoi(token);

    token = strtok(nullptr, ",");
    if (token == nullptr) return false;
    for (char* p = token; *p; p++)
        if (*p != '-' && (*p < '0' || *p > '9')) { DEBUG_PRINTLN("[RS485] Non-numeric temperature"); return false; }
    int tempInt = atoi(token);

    if (salInt < 0 || salInt > 6000)   { DEBUG_PRINTF("[RS485] Salinity out of range: %d\n", salInt);  return false; }
    if (tempInt < -50 || tempInt > 500){ DEBUG_PRINTF("[RS485] Temp out of range: %d\n", tempInt);     return false; }

    out.sensorId    = addr;
    out.salinity    = salInt  / 100.0f;
    out.temperature = tempInt / 10.0f;
    out.valid       = true;
    return true;
}
