/**
 * TTN Payload Formatter — MainBoard sensor uplinks
 *
 * Paste this into your TTN application under:
 *   Payload formatters → Uplink → Custom Javascript formatter
 *
 * Payload layout (6 bytes):
 *   [0]     sensor_id   uint8   — sensor address (1..255)
 *   [1-2]   salinity    uint16  — salinity × 100, big-endian (e.g. 3520 = 35.20 ppt)
 *   [3-4]   temperature int16   — temperature × 10, big-endian (e.g. 215 = 21.5 °C)
 *   [5]     flags       uint8   — bit 0: demo_mode, bit 1: sensor_error
 *
 * The fPort identifies which physical sensor sent the data:
 *   fPort 1 → sensor address 1
 *   fPort 2 → sensor address 2
 *   etc.
 *
 * Output fields visible in TTN data view and forwarded to integrations:
 *   sensor_id, salinity_ppt, temperature_c, demo_mode, sensor_error, fport
 */

function decodeUplink(input) {
    var bytes = input.bytes;
    var fPort = input.fPort;

    // Validate payload length
    if (bytes.length !== 6) {
        return {
            errors: ["Invalid payload length: expected 6 bytes, got " + bytes.length]
        };
    }

    // Byte 0: sensor ID
    var sensorId = bytes[0];

    // Bytes 1-2: salinity as uint16 big-endian, divide by 100 for ppt
    var salinityRaw = (bytes[1] << 8) | bytes[2];
    var salinity = salinityRaw / 100.0;

    // Bytes 3-4: temperature as int16 big-endian, divide by 10 for °C
    var tempRaw = (bytes[3] << 8) | bytes[4];
    // Convert from uint16 to signed int16
    if (tempRaw > 32767) {
        tempRaw = tempRaw - 65536;
    }
    var temperature = tempRaw / 10.0;

    // Byte 5: flags
    var flags     = bytes[5];
    var demoMode  = (flags & 0x01) !== 0;
    var sensorErr = (flags & 0x02) !== 0;

    return {
        data: {
            sensor_id:      sensorId,
            salinity_ppt:   salinity,
            temperature_c:  temperature,
            demo_mode:      demoMode,
            sensor_error:   sensorErr,
            fport:          fPort
        },
        warnings: sensorErr ? ["Sensor reported an error flag"] : []
    };
}

// Optional: encoder for downlinks (not currently used but useful for future
// remote mode switching commands from the TTN console)
function encodeDownlink(input) {
    return {
        bytes: [],
        fPort: 1
    };
}
