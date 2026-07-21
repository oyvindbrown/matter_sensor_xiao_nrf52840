# Seeed Xiao nRF52840 Matter Contact Sensor

This repository is a fork of [uamphome/matter_sensor_xiao_nrf52840](https://github.com/uamphome/matter_sensor_xiao_nrf52840) adapted for a **Matter Contact Sensor** (door/window) on the Seeed Studio Xiao nRF52840 using nRF Connect SDK / Zephyr. It prioritizes Seeed examples where possible, with Nordic Matter contact_sensor sample as base.

**Note**: Grok (built by xAI) has been used heavily to make this code work.

## Project Goal
Matter-compatible Contact Sensor on Seeed Xiao nRF52840. Uses BooleanState cluster for open/closed state. Optional reed switch on D0 (P0.02).

## Hardware
- Seeed Studio Xiao nRF52840 (or Sense): [Seeed Wiki](https://wiki.seeedstudio.com/XIAO_BLE/)
- USB-C cable, optional battery/JST connector
- **Magnet**: [AliExpress](https://www.aliexpress.com/item/1005009939392993.html)
- **Contact sensor**: [AliExpress](https://www.aliexpress.com/item/1005004517568686.html)
- Optional: Reed switch / magnetic sensor connected to D0 (P0.02) + GND

## Key Sources
- Seeed XIAO nRF52840 Zephyr: [Wiki](https://wiki.seeedstudio.com/XIAO-nRF52840-Zephyr-RTOS/) + [Zephyr Docs](https://docs.zephyrproject.org/latest/boards/seeed/xiao_ble/doc/index.html)
- Nordic Matter Contact Sensor Sample: [sdk-nrf samples/matter/contact_sensor](https://github.com/nrfconnect/sdk-nrf/tree/main/samples/matter/contact_sensor)
- Original temp/humidity base: [uamphome/matter_sensor_xiao_nrf52840](https://github.com/uamphome/matter_sensor_xiao_nrf52840)
- XIAO Matter Demo: [narangmayank/xiao_nrf52840_matter_demo](https://github.com/narangmayank/xiao_nrf52840_matter_demo)
- Nordic nRF52840 Matter code and Project CHIP / Matter SDK from GitHub.

## Build (nRF Connect SDK / VS Code)
1. Install nRF Connect SDK (v3.2.x recommended) and Toolchain via VS Code nRF Connect extension. See [Nordic docs](https://docs.nordicsemi.com/bundle/ncs-latest/page/nrf/installation/recommended_versions.html).
2. Open this project folder in VS Code.
3. Create new build configuration:
   - **Board Target**: `xiao_ble/nrf52840` (toggle "Nordic SOC" if needed)
   - **System Build (sysbuild)**: Disabled / No sysbuild (or enable if using MCUBoot)
4. Generate and Build.

Alternatively from terminal (after west init/update for your NCS version):
```bash
west build -b xiao_ble/nrf52840 -p auto
```

## Flash (UF2)
1. Locate firmware: `./build/zephyr/zephyr.uf2`
2. Plug in Xiao, double-tap RESET button → appears as USB mass storage.
3. Drag `zephyr.uf2` onto the drive. Device will reboot (ignore transient errors).

Or: `west flash --erase` (with debugger).

## Commissioning
Use CHIP Tool or any Matter controller. **The PIN code for adding to a Matter network is the standard nRF PIN `20202021`**.

Test BooleanState cluster for contact state (button or reed switch simulates open/closed).

**Note**: This code is not Matter certified; it is for developer / experimental purposes only.

## License / Usage
Anyone can change, edit, and use the code as they wish, as long as they give credit to the author. See LICENSE file for details.

## Customization
- **Contact GPIO**: Edit `boards/xiao_ble_nrf52840.overlay` for pin (default D0/P0.02).
- **App logic**: `src/app_task.cpp` — update BooleanState on GPIO change.
- **ZAP**: `src/default_zap/contact_sensor.zap` — regenerate with `west zap-gui` / `west zap-generate` if changing clusters.
- Low power: Disable unused peripherals in overlay + prj.conf.
- Partitions: See `boards/xiao_ble_nrf52840.overlay` and pm files for UF2/MCUBoot compatibility.

## Files of Interest
- `prj.conf` / `prj_debug.conf`: Kconfig for Matter, BLE, power.
- `boards/xiao_ble_nrf52840.overlay`: Device tree for Xiao + contact button + partitions.
- `src/`: App task + main + generated ZAP for contact_sensor.
- `sysbuild/`: Optional multi-image support.

Based on Nordic samples and Seeed guidance. Make this public when ready via GitHub Settings → Danger Zone → Change visibility.

Happy building!
