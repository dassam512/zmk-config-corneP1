# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

This is a ZMK (Zephyr Mechanical Keyboard) firmware configuration repository for a custom "mkbd_macropad" split keyboard. The keyboard features a Corne-style layout with 5 rows, 12 columns total (6 per side), and 3 thumb keys per side. It includes OLED displays, ZMK Studio support for real-time keymap editing, and Bluetooth connectivity.

## Build Process

ZMK uses GitHub Actions for automated firmware builds. The build configuration is defined in `build.yaml`:

- **Primary build targets**: `mkbd_macropad_left` and `mkbd_macropad_right` shields on `nice_nano_v2` board
- **ZMK Studio enabled**: Left side includes `studio-rpc-usb-uart` snippet with `-DCONFIG_ZMK_STUDIO=y`
- **Settings reset**: Includes `settings_reset` shield for clearing stored settings
- **Automated builds**: Triggered on push, pull request, or manual workflow dispatch via `.github/workflows/build.yml`

**No local build commands** - this repository relies entirely on GitHub Actions for firmware compilation.

## Architecture

### Directory Structure
- `config/`: ZMK configuration files
  - `west.yml`: ZMK framework dependency manifest
  - `corne.conf`: Global keyboard configuration (power management, Bluetooth, ZMK Studio)
- `boards/shields/mkbd_macropad/`: Custom keyboard shield definition
  - `mkbd_macropad.keymap`: Complete keymap with 3 layers (default, lower, raise)
  - `mkbd_macropad.dtsi`: Hardware definition (matrix transform, GPIO mapping)
  - `mkbd_macropad_left.overlay`/`mkbd_macropad_right.overlay`: Side-specific hardware configs
  - `custom_status_screen.c`: Custom OLED display implementation
  - Kconfig files: Build system configuration

### Key Features
- **Split keyboard**: Separate left/right halves with wireless communication
- **ZMK Studio integration**: Real-time keymap editing (`CONFIG_ZMK_STUDIO=y`, unlocked via `&studio_unlock`)
- **Custom OLED display**: Shows "Hello P1!" on startup, transitions to battery/charging status
- **Power management**: Deep sleep after 15 minutes, idle timeout after 30 seconds
- **Bluetooth optimization**: Enhanced TX power, experimental features enabled

### Keymap Layers
1. **Default layer**: Standard QWERTY with modifiers
2. **Lower layer (mo 1)**: Function keys, symbols, Bluetooth controls, navigation
3. **Raise layer (mo 2)**: Numpad, special characters, system keys

## Configuration Files

- **Global settings**: `config/corne.conf` contains power management, Bluetooth, and ZMK Studio settings
- **Side-specific settings**: `mkbd_macropad_left.conf` enables display and battery reporting for left side only
- **Hardware definitions**: `.dtsi` and `.overlay` files define GPIO pins, matrix layout, and OLED integration

## Development Workflow

1. **Keymap changes**: Edit `boards/shields/mkbd_macropad/mkbd_macropad.keymap`
2. **Hardware changes**: Modify `.dtsi` and `.overlay` files for GPIO or matrix changes
3. **Display customization**: Edit `custom_status_screen.c` for OLED behavior
4. **Configuration updates**: Modify `.conf` files for feature toggles
5. **Testing**: Push changes to trigger GitHub Actions build, download firmware from Actions artifacts
6. **Deployment**: Flash `.uf2` files to respective keyboard halves

## ZMK Studio Integration

The left keyboard half supports ZMK Studio for real-time keymap editing:
- Enabled via `CONFIG_ZMK_STUDIO=y` in build configuration
- Unlocked using `&studio_unlock` key binding (currently on lower layer)
- USB-UART communication via `studio-rpc-usb-uart` snippet
- Locking disabled (`CONFIG_ZMK_STUDIO_LOCKING=n`) for unrestricted access
- **Physical layout with position map**: Includes required `keys` property in `physical_layout0` with precise coordinate mapping for all 42 keys
- **No chosen matrix transform**: Uses physical layout instead of matrix transform for Studio compatibility