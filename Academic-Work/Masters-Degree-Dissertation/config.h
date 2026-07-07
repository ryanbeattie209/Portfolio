#ifndef CONFIG_H
#define CONFIG_H

// =============================================================================
// config.h - central configuration for the ESP32-S3 LoRaWAN comms node
//
// Targets RadioLib 6.6.0 EXACTLY. Install that version via Library Manager.
// (The API differs between 6.4 / 6.5 / 6.6 - "latest" may not compile.)
//
// THREE switches you set per build:
//   BOARD_TYPE     - which physical board you are flashing
//   SENSOR_SOURCE  - real RS485 poll, or synthetic data for the "pretend" nodes
//   ENABLE_LORAWAN - 0 for bench testing without a gateway, 1 for real uplinks
// =============================================================================


// -----------------------------------------------------------------------------
// 1) BOARD SELECT
// -----------------------------------------------------------------------------
#define BOARD_CUSTOM_PCB   1   // your designed comms board (ESP32-S3-WROOM-1)
#define BOARD_XIAO         2   // Seeed XIAO ESP32S3 + Wio-SX1262 kit

#define BOARD_TYPE         BOARD_XIAO     // <-- CHANGE per board being flashed


// -----------------------------------------------------------------------------
// 2) SENSOR SOURCE
//    RS485 on the real node (polls the bus / USB-RS485 emulator).
//    FAKE  on the "pretend" nodes - generates synthetic readings in firmware.
// -----------------------------------------------------------------------------
#define SENSOR_RS485       1
#define SENSOR_FAKE        2

#define SENSOR_SOURCE      SENSOR_FAKE    // <-- RS485 on real node, FAKE elsewhere


// -----------------------------------------------------------------------------
// 3) LORAWAN ENABLE
//    0 = bench mode: NO radio init, NO join. Loop polls/fakes -> packs ->
//        prints payload over USB serial. Use this to test WITHOUT a gateway.
//    1 = full mode: OTAA join + per-fPort uplinks. Needs a gateway in range.
// -----------------------------------------------------------------------------
#define ENABLE_LORAWAN     0              // <-- 0 today (no gateway), 1 at the router


// =============================================================================
// PIN MAPS  (selected automatically by BOARD_TYPE)
// =============================================================================
#if BOARD_TYPE == BOARD_CUSTOM_PCB
  // ---- Your designed board (pins confirmed against schematic) ----
  #define RS485_TX_PIN     17     // DI  - ESP32 TX -> SP3485
  #define RS485_RX_PIN     18     // RO  - SP3485 -> ESP32 RX
  #define RS485_DIR_PIN     8     // RE_DE direction control

  #define LORA_NSS_PIN     10
  #define LORA_MOSI_PIN    11
  #define LORA_CLK_PIN     12     // SCK
  #define LORA_MISO_PIN    13
  #define LORA_RFSW_PIN    14     // antenna / RF switch
  #define LORA_DIO1_PIN    21
  #define LORA_RST_PIN     47
  #define LORA_BUSY_PIN     9

  #define LED_PIN          48     // onboard RGB LED

#elif BOARD_TYPE == BOARD_XIAO
  // ---- Seeed XIAO ESP32S3 + Wio-SX1262 (B2B connector mapping) ----
  // SX1262 pins are FIXED by the B2B connector - do not change these.
  #define LORA_NSS_PIN     41
  #define LORA_CLK_PIN      7     // SCK
  #define LORA_MOSI_PIN     9
  #define LORA_MISO_PIN     8
  #define LORA_RST_PIN     42
  #define LORA_BUSY_PIN    40
  #define LORA_DIO1_PIN    39
  #define LORA_RFSW_PIN    38     // antenna switch

  // RS485 for the bench test: external SP3485 breakout wired to free XIAO pads.
  // These are the D6/D7/D1 pads - adjust to wherever you actually solder.
  #define RS485_TX_PIN     43     // D6 (U0TXD pad) -> SP3485 DI
  #define RS485_RX_PIN     44     // D7 (U0RXD pad) <- SP3485 RO
  #define RS485_DIR_PIN     1     // D0 -> SP3485 RE_DE

  #define LED_PIN          21     // XIAO user LED (cosmetic only - verify/ignore)

#else
  #error "Set BOARD_TYPE to BOARD_CUSTOM_PCB or BOARD_XIAO"
#endif


// -----------------------------------------------------------------------------
// RS485 bus
// -----------------------------------------------------------------------------
#define RS485_BAUD_RATE            9600
#define RS485_SERIAL_CFG           SERIAL_8N1
#define RS485_RESPONSE_TIMEOUT_MS  500
#define RS485_TURNAROUND_DELAY_MS  10


// -----------------------------------------------------------------------------
// LoRa / LoRaWAN
// -----------------------------------------------------------------------------
#define LORA_FREQUENCY     868.0   // EU868 - do not change for UK/EU

// NOTE: The Wio-SX1262 module uses a 1.8V TCXO. This is set in lorawan.cpp
// at radio.begin(). The default RadioLib TCXO voltage (1.6V) can leave the
// radio failing to initialise on this module - this caught the previous build.

// LoRaWAN credentials (OTAA). Each PHYSICAL board needs its OWN DevEUI:
// register three separate end devices on TTN (Generate the DevEUI in the
// console - the SX1262 has no hardware EUI to read). JoinEUI all-zeros is fine.
// Register the devices as LoRaWAN 1.0.4; nwkKey is set equal to appKey below.
#define LORAWAN_DEV_EUI    { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA }
#define LORAWAN_JOIN_EUI   { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define LORAWAN_APP_KEY    { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, \
                             0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC }

#define LORAWAN_SUBBAND    0       // EU868: 0 = all channels (correct)


// -----------------------------------------------------------------------------
// Sensor network / fPort mapping
// -----------------------------------------------------------------------------
#define SENSOR_COUNT       3       // sensor addresses 1..SENSOR_COUNT
#define LORAWAN_BASE_FPORT 1       // sensor N -> fPort (BASE + N - 1)


// -----------------------------------------------------------------------------
// Timing
// DEMO_MODE only affects the cycle interval and the demo flag bit now.
// Deep sleep has been REMOVED for the demo (see MainBoard.ino) - the node
// stays awake and loops. Re-add deep sleep + session persistence later via
// the ropg/LoRaWAN_ESP32 library, NOT by hand.
// -----------------------------------------------------------------------------
#define DEMO_MODE          true

#if DEMO_MODE
  #define SLEEP_INTERVAL_MS    10000UL   // 10 s between cycles
#else
  #define SLEEP_INTERVAL_MS    900000UL  // 15 min
#endif

// Gap between the per-sensor uplinks within one cycle.
// 3 s keeps 3 rapid uplinks clear of EU868 duty-cycle throttling.
#define INTER_UPLINK_DELAY_MS  3000UL


// -----------------------------------------------------------------------------
// Debug serial
// -----------------------------------------------------------------------------
#define DEBUG_SERIAL       true

#if DEBUG_SERIAL
  #define DEBUG_PRINT(x)    Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
  #define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(...)
#endif

#endif // CONFIG_H
