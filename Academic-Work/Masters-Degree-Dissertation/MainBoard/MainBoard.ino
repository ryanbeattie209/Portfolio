/*
 * MainBoard.ino  -  ESP32-S3 LoRaWAN comms node (demo build)
 *
 * Flow (no deep sleep in this build):
 *   setup(): disable WiFi/BT, init RS485, (if ENABLE_LORAWAN) join OTAA once
 *   loop():  for each sensor -> read (RS485 or fake) -> pack 6-byte payload
 *            -> print over serial -> (if joined) uplink on its fPort
 *            -> wait, then repeat the cycle
 *
 * Set BOARD_TYPE / SENSOR_SOURCE / ENABLE_LORAWAN in config.h.
 *
 * Targets RadioLib 6.6.0.
 */

#include <Arduino.h>
#include <WiFi.h>
#include "esp_bt.h"
#include "config.h"
#include "rs485.h"
#include "lorawan.h"
#include "payload.h"

// ---- fake sensor state (used when SENSOR_SOURCE == SENSOR_FAKE) -------------
static float fakeSal[SENSOR_COUNT + 1];
static float fakeTemp[SENSOR_COUNT + 1];
static bool  fakeInit = false;

static void initFakeSensors() {
    // A spatial gradient for the demo: sensor 1 = inland/fresher,
    // higher addresses = nearer the coast / more saline.
    for (uint8_t a = 1; a <= SENSOR_COUNT; a++) {
        fakeSal[a]  = 1.0f + (a - 1) * 6.0f;   // ~1, 7, 13 ppt ...
        fakeTemp[a] = 28.0f;
    }
    fakeInit = true;
}

static SensorReading makeFakeReading(uint8_t addr) {
    if (!fakeInit) initFakeSensors();

    // Random walk, clamped to sane ranges, so the dashboard shows live movement
    // and salinity occasionally drifts across the ~4 ppt rice-damage threshold.
    fakeSal[addr]  += (random(-30, 31)) / 100.0f;   // +/- 0.30 ppt
    fakeTemp[addr] += (random(-20, 21)) / 100.0f;   // +/- 0.20 C
    if (fakeSal[addr]  < 0.0f)  fakeSal[addr]  = 0.0f;
    if (fakeSal[addr]  > 45.0f) fakeSal[addr]  = 45.0f;
    if (fakeTemp[addr] < 20.0f) fakeTemp[addr] = 20.0f;
    if (fakeTemp[addr] > 35.0f) fakeTemp[addr] = 35.0f;

    SensorReading r;
    r.sensorId    = addr;
    r.salinity    = fakeSal[addr];
    r.temperature = fakeTemp[addr];
    r.valid       = true;
    return r;
}

// ---- helpers ---------------------------------------------------------------
static void printPayload(uint8_t addr, const uint8_t* p) {
    DEBUG_PRINTF("[Payload] sensor %d  bytes:", addr);
    for (uint8_t i = 0; i < PAYLOAD_SIZE; i++) DEBUG_PRINTF(" %02X", p[i]);
    uint16_t sal  = ((uint16_t)p[1] << 8) | p[2];
    int16_t  temp = (int16_t)(((uint16_t)p[3] << 8) | p[4]);
    DEBUG_PRINTF("   -> sal=%.2f ppt  temp=%.1f C  flags=0x%02X\n",
                 sal / 100.0f, temp / 10.0f, p[5]);
}

// ---------------------------------------------------------------------------

void setup() {
#if DEBUG_SERIAL
    Serial.begin(115200);
    delay(300);
#endif

    DEBUG_PRINTLN("\n=== MainBoard start ===");
    DEBUG_PRINTF("Board: %s | Sensor: %s | LoRaWAN: %s\n",
        (BOARD_TYPE == BOARD_CUSTOM_PCB) ? "CUSTOM_PCB" : "XIAO",
        (SENSOR_SOURCE == SENSOR_RS485)  ? "RS485" : "FAKE",
        ENABLE_LORAWAN ? "ON" : "OFF (bench)");

    WiFi.mode(WIFI_OFF);
    WiFi.disconnect(true);
    btStop();
    esp_bt_controller_disable();

#if SENSOR_SOURCE == SENSOR_RS485
    rs485.begin();
#else
    randomSeed(esp_random());
    initFakeSensors();
#endif

#if ENABLE_LORAWAN
    if (!lorawanHandler.begin()) {
        // Either a real radio fault, or just no gateway in range (the log above
        // says which). Either way, keep looping so the RS485/payload path is
        // still demonstrable over serial.
        DEBUG_PRINTLN("[Main] Continuing without uplinks (payloads still printed).");
    }
#else
    DEBUG_PRINTLN("[Main] Bench mode: LoRaWAN disabled, no gateway needed.");
#endif
}

void loop() {
    DEBUG_PRINTLN("\n--- cycle ---");

    for (uint8_t addr = 1; addr <= SENSOR_COUNT; addr++) {
#if SENSOR_SOURCE == SENSOR_RS485
        SensorReading reading = rs485.pollSensor(addr);
#else
        SensorReading reading = makeFakeReading(addr);
#endif
        if (!reading.valid) {
            DEBUG_PRINTF("[Main] sensor %d: no valid reading, skipping\n", addr);
            continue;
        }

        uint8_t payload[PAYLOAD_SIZE];
        if (!packPayload(reading, payload, DEMO_MODE)) {
            DEBUG_PRINTF("[Main] sensor %d: pack failed\n", addr);
            continue;
        }

        printPayload(addr, payload);

#if ENABLE_LORAWAN
        if (lorawanHandler.isJoined()) {
            uint8_t fPort = LORAWAN_BASE_FPORT + (addr - 1);
            lorawanHandler.transmit(fPort, payload, PAYLOAD_SIZE);
        }
#endif
        delay(INTER_UPLINK_DELAY_MS);
    }

    delay(SLEEP_INTERVAL_MS);
}
