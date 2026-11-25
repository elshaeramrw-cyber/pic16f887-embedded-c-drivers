// DC motor control: direction (CW/CCW) + speed (PWM)
// PIC16F887, internal 8 MHz, L293D driver

void main() {
    unsigned short duty = 0;       // 0..255 (0%..100% duty)

    // ----- I/O configuration -----
    ANSEL  = 0;                    // All pins digital (no analog function)
    ANSELH = 0;

    TRISD = 0xFF;                  // PORTD all inputs (buttons)
    TRISC = 0x00;                  // PORTC output (RC2 = PWM)
    TRISB = 0x00;                  // PORTB output (RB0,RB1 = direction)

    PORTB = 0x00;                  // Motor stopped initially (IN1=IN2=0)

    // ----- PWM setup (1 kHz) -----
    PWM1_Init(1000);               // Initialize CCP1 PWM at 1 kHz
    PWM1_Start();                  // Enable PWM output on RC2
    PWM1_Set_Duty(duty);           // Start with 0% duty

    // ----- Main loop -----
    while (1) {
        // 1) Direction control: two separate buttons
        if (PORTD.RD2 == 0) {      // Clockwise button pressed
            Delay_ms(40);           // Simple debounce
            PORTB = 0b00000001;    // RB0=1, RB1=0 ? motor CW
        }
        if (PORTD.RD3 == 0) {      // Counter-clockwise button pressed
            Delay_ms(40);
            PORTB = 0b00000010;    // RB0=0, RB1=1 ? motor CCW
        }

        // 2) Speed increase button (RD0)
        if (PORTD.RD0 == 0) {
            Delay_ms(40);
            if (duty < 250) {       // Limit maximum to 250 (˜98%)
                duty = duty + 10;
                PWM1_Set_Duty(duty);
            }
        }

        // 3) Speed decrease button (RD1)
        if (PORTD.RD1 == 0) {
            Delay_ms(40);
            if (duty > 0) {
                duty = duty - 10;
                PWM1_Set_Duty(duty);
            }
        }

        Delay_ms(10);               // Small loop delay for stable reading
    }
}