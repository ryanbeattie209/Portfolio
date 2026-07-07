#!/usr/bin/env python3
"""
rs485_sensor_emulator.py

Pretends to be the STM32 salinity sensors on the RS485 bus, so you can test the
ESP32 node's polling / parsing / payload path without any real sensor hardware.

It listens on a USB-to-RS485 stick, waits for the node's polls:

    "<ADDR>,REQ\n"           e.g. "1,REQ\n"

and replies as that sensor:

    "<ADDR>,<SAL_INT>,<TEMP_INT>\n"   e.g. "1,352,283\n"

    SAL_INT  = salinity    x 100   (352  -> 3.52 ppt)
    TEMP_INT = temperature x 10    (283  -> 28.3 C)

One stick + one script emulates ALL sensor addresses on the bus, so a single
node polling addresses 1..N gets answered for each.

The values do a slow random walk (gradient: lower addresses fresher, higher
addresses more saline) so the dashboard shows live movement and salinity
occasionally crosses the ~4 ppt rice-damage threshold for the "intrusion" story.

Requirements:
    pip install pyserial

Usage:
    python rs485_sensor_emulator.py --port COM5            (Windows)
    python rs485_sensor_emulator.py --port /dev/ttyUSB0    (Linux/Mac)
    python rs485_sensor_emulator.py --port COM5 --sensors 3 --baud 9600

Notes on the adapter:
    Cheap CH340 / CP210x / FTDI USB-RS485 sticks are AUTO-DIRECTION (they flip
    TX/RX automatically). This script needs no DE/RE handling for those - just
    read and write. If yours is a rare MANUAL-direction stick with an RTS-driven
    DE pin, set --rts-dir and it will raise RTS while transmitting.
"""

import argparse
import random
import sys
import time

try:
    import serial  # pyserial
except ImportError:
    sys.exit("pyserial not installed.  Run:  pip install pyserial")


class FakeSensor:
    """Slow random walk around a per-address baseline."""
    def __init__(self, addr, base_sal, base_temp=28.0):
        self.addr = addr
        self.sal = base_sal          # ppt
        self.temp = base_temp        # C

    def step(self):
        self.sal += random.uniform(-0.30, 0.30)
        self.temp += random.uniform(-0.20, 0.20)
        self.sal = max(0.0, min(45.0, self.sal))
        self.temp = max(20.0, min(35.0, self.temp))

    def response(self):
        self.step()
        sal_int = int(round(self.sal * 100))    # x100
        temp_int = int(round(self.temp * 10))   # x10, signed
        return f"{self.addr},{sal_int},{temp_int}\n"


def build_sensors(n):
    # gradient: addr 1 inland/fresh -> higher addrs nearer coast / saltier
    return {a: FakeSensor(a, base_sal=1.0 + (a - 1) * 6.0) for a in range(1, n + 1)}


def parse_request(line):
    """Return the polled address from '<ADDR>,REQ', or None if not a request."""
    line = line.strip()
    parts = line.split(",")
    if len(parts) == 2 and parts[1].upper() == "REQ" and parts[0].isdigit():
        return int(parts[0])
    return None


def run(port, baud, n_sensors, rts_dir):
    sensors = build_sensors(n_sensors)
    ser = serial.Serial(port, baudrate=baud, bytesize=8, parity="N",
                        stopbits=1, timeout=0.05)
    print(f"Emulating sensors {list(sensors)} on {port} @ {baud} 8N1")
    print("Waiting for polls...  (Ctrl-C to stop)\n")

    buf = bytearray()
    while True:
        chunk = ser.read(64)
        if chunk:
            buf.extend(chunk)
            # process complete lines
            while b"\n" in buf:
                raw, _, rest = buf.partition(b"\n")
                buf = bytearray(rest)
                line = raw.decode("ascii", errors="ignore")
                addr = parse_request(line)
                if addr is None:
                    if line.strip():
                        print(f"  ? ignored: {line.strip()!r}")
                    continue
                if addr not in sensors:
                    print(f"  poll for unknown sensor {addr} - no reply")
                    continue

                resp = sensors[addr].response()
                if rts_dir:
                    ser.setRTS(True)        # drive DE high
                    time.sleep(0.001)
                ser.write(resp.encode("ascii"))
                ser.flush()
                if rts_dir:
                    ser.setRTS(False)
                print(f"  poll {addr} -> {resp.strip()}")


def main():
    ap = argparse.ArgumentParser(description="Fake RS485 salinity sensors")
    ap.add_argument("--port", required=True, help="serial port, e.g. COM5 or /dev/ttyUSB0")
    ap.add_argument("--baud", type=int, default=9600)
    ap.add_argument("--sensors", type=int, default=3, help="number of sensor addresses (1..N)")
    ap.add_argument("--rts-dir", action="store_true",
                    help="manual-direction stick: drive DE via RTS while transmitting")
    args = ap.parse_args()
    try:
        run(args.port, args.baud, args.sensors, args.rts_dir)
    except KeyboardInterrupt:
        print("\nstopped.")
    except serial.SerialException as e:
        sys.exit(f"serial error: {e}")


if __name__ == "__main__":
    main()
