# Knob Configurator

A simple tool to configure your knob's actions.

## Quick Start

1. **Install Python** (if not already installed)
   - Download Python from [python.org](https://www.python.org/downloads/)
   - During installation, make sure to check "Add Python to PATH"

2. **Install Required Packages**
   - Open Command Prompt (press Win + R, type `cmd`, press Enter)
   - Run these commands:
   ```sh
   pip install pyserial
   ```

3. **Download Required Tools**
   - Make sure these files are in the same folder as `knob_configurator.py`:
     - `spiffsgen.py` (from ESP-IDF's components/spiffs folder)
     - `esptool.py` (download from [here](https://github.com/espressif/esptool/raw/master/esptool.py))

4. **Run the Configurator**
   - Double-click `knob_configurator.py`
   - Or open Command Prompt in this folder and run:
   ```sh
   python knob_configurator.py
   ```

## How to Use

1. **Configure Left Knob**
   - Click in the "Left Knob" box
   - Press the key you want (e.g., Left Arrow)
   - Hold Ctrl/Shift/Alt/Win if you want modifiers

2. **Configure Right Knob**
   - Click in the "Right Knob" box
   - Press the key you want (e.g., Right Arrow)
   - Hold Ctrl/Shift/Alt/Win if you want modifiers

3. **Flash to Knob**
   - Connect your knob via USB
   - Click "Flash Configuration"
   - Wait for the success message
   - Unplug and replug your knob

## Supported Keys

- Letters (A-Z)
- Numbers (0-9)
- Arrow keys (←, →, ↑, ↓)
- Enter, Escape, Backspace, Tab, Space

## Example Configurations

1. **Volume Control**
   - Left: Down Arrow
   - Right: Up Arrow

2. **Browser Navigation**
   - Left: Left Arrow
   - Right: Right Arrow

3. **Text Selection**
   - Left: Shift + Left Arrow
   - Right: Shift + Right Arrow

4. **Window Management**
   - Left: Alt + Left Arrow
   - Right: Alt + Right Arrow

## Troubleshooting

- **"Could not find spiffsgen.py"**: Make sure `spiffsgen.py` is in the same folder as `knob_configurator.py`
- **"Could not find esptool.py"**: Download `esptool.py` from the link above and put it in the same folder
- **"No USB devices found"**: Make sure your knob is connected
- **"Could not find your knob"**: Try a different USB port
- **Flash fails**: Make sure no other program is using the knob
- **Python not found**: Make sure Python is installed and added to PATH 