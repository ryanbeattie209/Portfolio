# Masters Team Project
Masters Dissertation Project between collaborators from MComp and MEng master degree students.

---

## 📌 Overview
A low-cost, distributed IoT sensor network for monitoring saltwater intrusion in the Mekong Delta. The system uses a conductive salinity sensor (STM32) connected via RS485 to an ESP32-S3 communications node, which transmits over LoRaWAN to a Raspberry Pi gateway and The Things Network, with a real-time MQTT dashboard for visualisation. All electronics are housed in a custom 3D-printed, tortoise-shaped enclosure designed for hand-assembly in local makerspaces.
---

## 🛠️ Tech Stack
- **Firmware:** C++ (Arduino/RadioLib), ESP32-S3, STM32G030K8T6
- **Hardware:** Altium Designer, RS485 (SP3485), LoRaWAN (SX1262), Raspberry Pi 5
- **3D Design:** Fusion 360, Bambu Studio, FDM printing
- **Cloud/Network:** The Things Network (TTN), MQTT over WebSocket
- **Frontend:** HTML5, CSS3, JavaScript, SVG, JSON config

---

## 👥 Authors & Contributors
- James Addy
- Ryan Beattie
- Joshua Dunbar
- Radoslaw Fabian
- Aidan Tolner
- Jakub Stachowiak

---

## ✍️ My Contributions (Ryan Beattie)
- **3.1 — How Rice is Grown in the Mekong Delta** (Literature Review)
- **4.3 — 3D Design and Manufacture** (tortoise enclosure design, Fusion 360, print testing and iteration)
- **4.5 — Communications Node Firmware Development** (rebuilt ESP32-S3 LoRaWAN firmware from scratch across five modules: config, payload encoding, RS485, LoRaWAN connectivity, system management)

Also contributed to compiling and formatting the final dissertation document.

> Gen-AI (Claude Opus 4.7) was used in an assistive role during firmware development to produce reference code, which was reviewed, tested, and edited by the team — disclosed in the dissertation's Gen-AI cover sheet.
