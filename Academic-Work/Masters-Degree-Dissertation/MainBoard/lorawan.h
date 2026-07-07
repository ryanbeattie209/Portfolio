#ifndef LORAWAN_H
#define LORAWAN_H

#include <RadioLib.h>
#include "config.h"

// =============================================================================
// lorawan.h - LoRaWAN wrapper (RadioLib 6.6.0)
//
// Demo build: OTAA join once at boot, then transmit per-sensor uplinks on
// distinct fPorts. No deep sleep, no RTC session persistence - the node stays
// awake and loops. That deliberately removes the entire devNonce/session-restore
// problem that the previous build got wrong; add it back for field deployment
// using ropg/LoRaWAN_ESP32, not by hand.
// =============================================================================

typedef enum {
    LORAWAN_OK = 0,
    LORAWAN_ERR_JOIN_FAILED,
    LORAWAN_ERR_TX_FAILED,
    LORAWAN_ERR_NOT_JOINED,
} LoRaWANResult;

class LoRaWANHandler {
public:
    // Init radio (SPI, TCXO, RF switch), set OTAA creds, attempt join.
    // Returns true only if the join actually succeeded.
    bool begin();

    // Transmit a payload on the given fPort (1..223). Unconfirmed uplink.
    LoRaWANResult transmit(uint8_t fPort, const uint8_t* payload, size_t len);

    // True if the OTAA join succeeded.
    bool isJoined();

private:
    bool initRadio();
};

extern LoRaWANHandler lorawanHandler;

#endif // LORAWAN_H
