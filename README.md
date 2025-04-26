# Alcohol Detection and Speed Limiter System

This Arduino-based project controls a motor's speed based on alcohol detection using a gas sensor. It includes authentication, speed limit control via serial commands, visual indication using a NeoPixel strip, and a 16x2 LCD for real-time feedback.

![Image](https://github.com/user-attachments/assets/a84d944a-bc98-4098-8812-039d20778bff)

## 🚀 Features

- 🔐 **Password Authentication** to prevent unauthorized access
- 🧪 **Alcohol Detection** using a gas sensor (e.g., MQ-3)
- ⚙️ **Motor Speed Control** via a potentiometer
- 🧠 **Dynamic Speed Limit** set through serial input
- 🌈 **NeoPixel Alert Strip** turns red on alcohol detection
- 💡 **360° Warning Light Activation** when alcohol is detected to alert nearby drivers
- 📟 **LCD Display** shows current speed and alcohol status
- 🪛 Configurable alcohol threshold and speed limits

## 🛠️ Components Used

- Arduino Uno or compatible board
- MQ-3 or MQ gas sensor
- Potentiometer
- NeoPixel strip (Adafruit NeoPixel, 4 LEDs)
- 16x2 LCD
- Motor (simulated via PWM on pin 3)
- Jumper wires, breadboard, power source

## ⚙️ Pin Configuration

| Component          | Arduino Pin |
|-------------------|-------------|
| Motor (PWM)       | D3          |
| Potentiometer     | A0          |
| Gas Sensor        | A1          |
| NeoPixel Strip    | D10         |
| LCD RS            | D12         |
| LCD E             | D11         |
| LCD D4–D7         | D5–D2       |

## 💻 Serial Commands

- `SET LIMIT <value>` — Set a custom speed limit (1–120 km/h)

## 🔐 Default Password - ABCD


This can be changed in the code as needed.

## 🧠 How It Works

Once the system is powered on, the LCD prompts for a password, which must be entered through the Serial Monitor. Upon successful authentication, the user can issue commands like `SET LIMIT 80` to set a custom speed limit.

The potentiometer simulates throttle control. Its analog value is mapped to a speed value (0–120 km/h). Simultaneously, the alcohol sensor reads gas levels. If alcohol is detected above a set threshold (default: 500), the system:

If alcohol is detected above a threshold:
- Speed limit drops to 30 km/h
- NeoPixel strip glows red
- **A 360° LED warning light turns ON** to notify nearby drivers
- LCD displays "Alcohol Detected"

The current speed is then enforced based on the lesser of the potentiometer-derived value and the active speed limit. Motor control is done via PWM output to the specified pin, reflecting the adjusted speed.

LCD displays the current speed and whether alcohol is detected or not, updating every 500 milliseconds.

## 📃 License

This project is open-source under the MIT License. Contributions are welcome!

