void main() {
 unsigned char x[]={0b00111111,0b00000110,0b01011011,0b01001111,0b01100110,0b01101101,0b01111101,0b00000111,0b01111111,0b01101111};
 int i=0;
 int t=0;
 ansel=0;
 anselh=0;
 trisC=0;
 trisB=1;
 trisD=0;
 portc=0;
 portB=0;
 portD=1;
 while(1){
  portD.RD1=1;
  portD.RD0=0;
  portc=x[i];
  delay_ms(10);
  portD.RD1=0;
  portD.RD0=1;
  portc=x[t];
  if (portB.f0==1){
  while (portB.f0==1){}
  i++;
  if (i>=10){
   i=0;
   t++;
   if (t>=10){
    i=0;
    t=0;
   }
  }
 }
 if (portB.f1==1){
  while(portB.f1==1){}
 i--;
 if (i<=0){
 i=9;
 t--;
 if (t<=0){
  i=0;
  t=0;
 }
 }
}
}
}