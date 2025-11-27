# DCIS (Digital Clock in Intergalactic Systems)

An Arduino-based digital clock that simulates time dilation effects from Einstein's theory of relativity. The clock adjusts its speed based on velocity (special relativity) and gravitational fields (general relativity), displayed on a 16x2 LCD screen.

## Overview

This project demonstrates how time flows differently under extreme conditions:
- **Special Relativity**: Time slows down as velocity approaches the speed of light
- **General Relativity**: Time slows down in strong gravitational fields

The clock calculates time dilation factors and runs faster or slower accordingly, giving you a hands-on experience with relativistic physics.

## Features

- Real-time clock with millisecond precision
- Calculates time dilation from both velocity and gravity
- Pre-configured scenarios (Earth, Sun, Neutron Star, Near-lightspeed)
- Custom parameter input for any scenario
- LCD display showing time and dilation factor
- Serial monitor interface for configuration

## Hardware Requirements

- Arduino board (The Uno R3 was used for this project)
- 16x2 I2C LCD display
- Jumper wires

## Wiring

Connect the I2C LCD to Arduino:
- **VCC** → 5V
- **GND** → GND
- **SDA** → A4 
- **SCL** → A5 

## Software Requirements

### Arduino IDE
Download from [arduino.cc](https://www.arduino.cc/en/software)

### Required Libraries
Install through Arduino Library Manager (Sketch → Include Library → Manage Libraries):
- **Wire** (built-in)
- **LiquidCrystal_I2C** by Frank de Brabander

## Installation

1. Clone or download this repository:
```bash
git clone https://github.com/Glaecier/DCIS.git
```

2. Open `DCIS.ino` in Arduino IDE

3. Install required libraries if not already installed

4. Connect your Arduino board via USB

5. Select your board and port:
   - Tools → Board → Select your Arduino model
   - Tools → Port → Select the correct COM port

6. Upload the code to your Arduino

## Usage

### Initial Setup

1. Open Serial Monitor (Tools → Serial Monitor) at 9600 baud
2. Enter the initial time in 24-hour format (HH:MM:SS)
   - Example: `14:30:00`

### Select a Scenario

Choose from 4 predefined scenarios, or create your own using the "custom" option.

### Reading the Display

**Top Row**: Current time with milliseconds
- Format: `HH:MM:SS:mmm AM/PM`

**Bottom Row**: Dilation factor
- Format: `DF:x.xxxxxx`
- Values < 1.0: Clock runs faster than normal to accomodate for slowing down of time
- Values > 1.0: Clock runs slower than normal to accomodate for speeding up of time

## Physics Behind the Clock

### Time Dilation Formulas

The clock calculates time dilation using three components:

**Lorentz Factor (Special Relativity):**
```
γ = 1 / √(1 - v²/c²)
```

**Schwarzschild Factor (General Relativity):**
```
t = √(1 - 2GM/rc²)
```

**Total Dilation Factor:**
```
Total = γ × √(1 - 2GM/rc²)
```

Where:
- `γ` (gamma) = Lorentz factor for velocity
- `t` = time dilation factor from gravity
- `v` = velocity
- `c` = speed of light (299,792,458 m/s)
- `G` = gravitational constant (6.674×10⁻¹¹ N⋅m²/kg²)
- `M` = mass of gravitational body
- `r` = distance from center of mass

## Limitations

- Dilation factor capped between 0.01x (100x faster) and 10x (10x slower) for practical display
- Does not account for acceleration effects
- Simplified model assumes constant velocity and static gravitational field
- Arduino's floating-point precision may introduce small errors in extreme scenarios

## Troubleshooting

**LCD shows nothing:**
- Check I2C address (default 0x27, some use 0x3F)
- Verify wiring connections
- Adjust LCD contrast potentiometer

**Serial Monitor not responding:**
- Check baud rate is set to 9600
- Ensure correct COM port is selected
- Try resetting the Arduino

**Invalid time dilation values:**
- Ensure velocity doesn't exceed speed of light
- Check that distance is not too small (avoid division by zero)
- Verify mass values are reasonable

## License

This project is open source and available under the MIT License.

## References

• Special Relativity with Clock Synchronization – Rothenstein et al. (2005) https://arxiv.org/pdf/physics/0512013.pdf

• Relativistic Effects of the Rotation of the Earth on Remote Clock Synchronization – NASA Tech Report (1974)
  https://ntrs.nasa.gov/api/citations/19750019126/downloads/19750019126.pdf
  
• Relativity, GPS, and the Validity of Common-View Synchronization – Eric L. Michelsen (2012) https://arxiv.org/pdf/1203.1816.pdf

## Author

Yatharth Agrawal

---

Experience Einstein's relativity on your desk
