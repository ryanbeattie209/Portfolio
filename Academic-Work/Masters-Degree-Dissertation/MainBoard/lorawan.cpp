#include "lorawan.h"
#include "config.h"
#include <Arduino.h>
#include <SPI.h>

// =============================================================================
// lorawan.cpp - RadioLib 6.6.0
//
// Key differences from the previous (non-working) build:
//   - radio.begin() sets the 1.8V TCXO required by the Wio-SX1262 module
//   - OTAA is a TWO step flow: beginOTAA() sets keys, activateOTAA() joins
//   - join success is RADIOLIB_LORAWAN_NEW_SESSION, not RADIOLIB_ERR_NONE
//   - NO getSession()/beginABP() session persistence (those signatures do not
//     exist in RadioLib) - join is performed fresh at every boot instead
// =============================================================================

// Radio + node. Pins come from config.h (board-selected).
static SX1262 radio = new Module(LORA_NSS_PIN, LORA_DIO1_PIN, LORA_RST_PIN, LORA_BUSY_PIN);
static LoRaWANNode node(&radio, &EU868, LORAWAN_SUBBAND);

// Credentials
static uint64_t devEUI  = 0;
static uint64_t joinEUI = 0;
static uint8_t  appKey[16] = LORAWAN_APP_KEY;
static uint8_t  nwkKey[16] = LORAWAN_APP_KEY;  // LoRaWAN 1.0.x on TTN: nwk == app

static bool g_joined = false;

LoRaWANHandler lorawanHandler;

// -----------------------------------------------------------------------------

bool LoRaWANHandler::initRadio() {
    // EUIs are stored MSB-first to match the TTN console display order.
    const uint8_t devEUIBytes[8]  = LORAWAN_DEV_EUI;
    const uint8_t joinEUIBytes[8] = LORAWAN_JOIN_EUI;
    devEUI = 0; joinEUI = 0;
    for (int i = 0; i < 8; i++) {
        devEUI  = (devEUI  << 8) | devEUIBytes[i];
        joinEUI = (joinEUI << 8) | joinEUIBytes[i];
    }

    // ESP32-S3 GPIO matrix: SPI pins must be set explicitly before radio init.
    SPI.begin(LORA_CLK_PIN, LORA_MISO_PIN, LORA_MOSI_PIN, LORA_NSS_PIN);

    DEBUG_PRINTLN("[LoRaWAN] radio.begin() ...");
    // The modem params here are placeholders (the LoRaWAN node overrides them).
    // What matters: tcxoVoltage = 1.8 for the Wio-SX1262 module.
    int16_t state = radio.begin(
        LORA_FREQUENCY,                      // freq (placeholder)
        125.0,                               // bw    (placeholder)
        9,                                   // sf    (placeholder)
        7,                                   // cr    (placeholder)
        RADIOLIB_SX126X_SYNC_WORD_PRIVATE,   // sync  (node sets LoRaWAN sync)
        10,                                  // power dBm (placeholder)
        8,                                   // preamble (placeholder)
        1.8,                                 // *** TCXO reference voltage ***
        false                                // use DC-DC regulator
    );
    if (state != RADIOLIB_ERR_NONE) {
        DEBUG_PRINTF("[LoRaWAN] radio.begin FAILED: %d\n", state);
        DEBUG_PRINTLN("[LoRaWAN] -> check SX1262 pins, SPI wiring, TCXO. This is a REAL bug, not a gateway issue.");
        return false;
    }

    // Wio-SX1262 routes the antenna through a GPIO-controlled RF switch.
    // Without this the radio inits fine but TX/RX silently fail.
    static const uint32_t rfswPins[] = {
        LORA_RFSW_PIN, RADIOLIB_NC, RADIOLIB_NC, RADIOLIB_NC, RADIOLIB_NC
    };
    static const Module::RfSwitchMode_t rfswTable[] = {
        { Module::MODE_IDLE, { LOW,  LOW, LOW, LOW, LOW } },
        { Module::MODE_RX,   { HIGH, LOW, LOW, LOW, LOW } },
        { Module::MODE_TX,   { HIGH, LOW, LOW, LOW, LOW } },
        END_OF_MODE_TABLE
    };
    radio.setRfSwitchTable(rfswPins, rfswTable);

    DEBUG_PRINTLN("[LoRaWAN] radio OK.");
    return true;
}

bool LoRaWANHandler::begin() {
    if (!initRadio()) {
        g_joined = false;
        return false;
    }

    // Step 1: load OTAA credentials (does NOT join).
    node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);

    // Step 2: actually join.
    DEBUG_PRINTLN("[LoRaWAN] Joining via OTAA ...");
    int16_t state = node.activateOTAA();

    if (state != RADIOLIB_LORAWAN_NEW_SESSION) {
        DEBUG_PRINTF("[LoRaWAN] JOIN did not complete (state %d)\n", state);
        DEBUG_PRINTLN("[LoRaWAN] -> If radio.begin PASSED above and this is just a timeout,");
        DEBUG_PRINTLN("[LoRaWAN]    the code is fine - there is simply no gateway in range.");
        g_joined = false;
        return false;
    }

    DEBUG_PRINTLN("[LoRaWAN] JOINED.");

    // Bench/demo only: relax duty-cycle so three rapid uplinks per cycle are not
    // throttled. For a REAL deployment, delete this line to stay EU868-compliant.
    node.setDutyCycle(false);

    g_joined = true;
    return true;
}

LoRaWANResult LoRaWANHandler::transmit(uint8_t fPort, const uint8_t* payload, size_t len) {
    if (!g_joined) {
        DEBUG_PRINTLN("[LoRaWAN] transmit skipped: not joined");
        return LORAWAN_ERR_NOT_JOINED;
    }

    // Unconfirmed uplink. Return: <0 error, 0 sent (no downlink), 1/2 downlink in RXn.
    int16_t state = node.sendReceive((uint8_t*)payload, len, fPort);

    if (state < RADIOLIB_ERR_NONE) {
        DEBUG_PRINTF("[LoRaWAN] uplink fPort %d FAILED: %d\n", fPort, state);
        return LORAWAN_ERR_TX_FAILED;
    }

    DEBUG_PRINTF("[LoRaWAN] uplink fPort %d sent (state %d)\n", fPort, state);
    return LORAWAN_OK;
}

bool LoRaWANHandler::isJoined() {
    return g_joined;
}
