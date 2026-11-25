# PIC16F887 Embedded C Peripheral Drivers & Control Firmware

Embedded C firmware developed for the **Microchip PIC16F887** microcontroller (8 MHz), covering register-level PWM generation, analog-to-digital conversion (ADC), multiplexed display driving, and DC motor H-bridge control.

## 📂 Firmware Modules

| File | Peripherals & Concepts | Description |
| :--- | :--- | :--- |
| `01_adc_voltage_lcd_display.c` | **ADC (`AN1`), 4-bit LCD** | Samples 10-bit analog sensor voltages (`0–5V`), converts raw ADC counts to formatted decimal digits, and renders real-time voltage readouts on a 16×2 character LCD. |
| `02_multiplexed_7segment_counter.c` | **GPIO, Time-Division Multiplexing** | Drives a 2-digit 7-segment display (`00–99`) using high-speed port switching (`PORTC`/`PORTD`) with edge-triggered pushbutton increment/decrement controls. |
| `03_pwm_motor_control_registers.c` | **CCP2, Timer2 (`PR2`), L293D** | Register-level 10-bit 1 kHz PWM speed controller and bidirectional H-bridge (`CW`/`CCW`/`STOP`) driver with state-machine pushbutton debouncing. |
| `04_pwm_motor_control_mikroc.c` | **CCP1 PWM, L293D Driver** | Interactive DC motor speed and direction controller using mikroC hardware PWM libraries and digital pushbutton inputs. |
| `05_lcd_scrolling_counter.c` | **4-bit LCD Interface** | Initializes a 16×2 alphanumeric LCD in 4-bit mode with custom cursor control and dual-line text/counter updates. |

## 🛠️ Hardware & Toolchain
* **Microcontroller:** Microchip PIC16F887 (8-bit CMOS, 8 MHz)
* **Drivers & Peripherals:** L293D Dual H-Bridge Motor Driver, 16×2 HD44780 LCD, Common-Cathode 7-Segment Displays
* **Compilers & Simulation:** mikroC PRO for PIC, MPLAB XC8, Proteus ISIS
