sbit lcd_RS at RC0_bit;
sbit lcd_en at RC1_bit;
sbit lcd_d4 at RC2_bit;
sbit lcd_d5 at RC3_bit;
sbit lcd_d6 at RC4_bit;
sbit lcd_d7 at RC5_bit;
sbit lcd_RS_direction at trisC0_bit;
sbit LCD_EN_direction at TRISC1_bit;
sbit LCD_D4_direction at TRISC2_bit;
sbit LCD_D5_direction at TRISC3_bit;
sbit LCD_D6_direction at TRISC4_bit;
sbit LCD_D7_direction at trisc5_bit;
void main() {
unsigned char x[]={'0','1','2','3','4','5','6','7','8','9'};
int i=1;
int o=0;
int t=0;
trisc=0;
portC=0;
LCD_init();
lcd_cmd(_lcd_clear);
lcd_cmd(_lcd_cursor_off);
while(1){
 lcd_out(1,i,"PIC16F887 Ready");
 delay_ms(500);
 i++;
 if (i>=8){
 i=1;
 }
 lcd_out(2,4,x[o]);
 lcd_out(2,5,x[t]);
 delay_ms(500);
 o++;
 if (o>9){
 t++;
 o=0;
 if (t>9){
 t=0;
 o=0;
 }
 }
 lcd_cmd(_lcd_clear);
}

}