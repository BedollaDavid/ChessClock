#include <EEPROM.h>
#include <LiquidCrystal.h>
                                    /* variables importantes*/
// los segundos se manejan en milis
long s,s1,tinicial,sini;
int m,h;
int m1,h1;
int pausa=1,pausa1=1,pausa2=1, pausatotal=0;
int seleccion=0, edihora=0,ediminuto=0,edisegundo=0,editipo=0,ediincremento=0,seg=0,seg1=0,indice=0;
int pregunta=0, jugador2=0,ya=0,respuesta=0,edihora1=0,ediminuto1=0,edisegundo1=0;
int last=0,bandera=0;
int tipo=0, incremento=10;
int movimiento1=0,movimiento2=0;
int mbutton, gano=0, anuncios=5;
int minutillos,minutillos1;
int division=32;
const int ledPin=3;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int read_main_button()
{
  int aux;
  aux = analogRead(2);
  //Serial.println(aux);
  if (aux>999)
  aux=999;

  if (aux<=100)
  { 
  /* lcd.setCursor(3,0);
   lcd.print(aux);*/
   return 0;
  }
  else
  {
    /*lcd.setCursor(10,0);
    lcd.print(aux);*/
    return 3;
  }
}

int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      
 if (adc_key_in > 1000) return btnNONE;
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;
 return btnNONE;
}

void imp(int s,int m,int h,int p)
{
 lcd.setCursor(p+5,1);
if (s<10)
{
    lcd.print(0);
    lcd.setCursor(p+6,1);
}
if (s>=0) lcd.print(s);
lcd.setCursor(p+4,1);
lcd.print(":");
   lcd.setCursor(p+2,1);
if (m<10)
{
    lcd.print(0);
    lcd.setCursor(p+3,1);
}
if (m>=0) lcd.print(m);
lcd.setCursor(p+1,1);
lcd.print(":");
lcd.setCursor(p,1);
if (h>=0) lcd.print(h);
}
int retrasosimple(int incremento, int boton)
{
  int i=0,mbutton;
  int k=(incremento*1000)/1;
  k=k/division;
  while (i<=k)
  {
    lcd_key = read_LCD_buttons();  // read the buttons
    mbutton=read_main_button();
 if (lcd_key!=btnNONE && mbutton!=boton) { 
                     return lcd_key;
               }
    delay(1);
    i=i+1;
  }
  return btnNONE; // todo bien
}
void imprimir(long *s,int *m,int *h,int p, long tinicial, long *sini,int *pausatotal)
{
 long ss=0;
  //segundos
  //////////////////////////////////////////////////
   lcd.setCursor(p+5,1);
  (*s)=(*sini)-division*millis();
if ((*s)<0)
{
  tinicial=division*millis();
  if ((*h)>0) {
  (*s)=59999;
  (*sini)=(*s)+tinicial;
  }
  if ((*m)>0){
    (*s)=59999;
  (*sini)=(*s)+tinicial;
  }
  (*m)=(*m)-1;
  //////////
  
  //////////
}
ss=(*s)/1000;

if (ss<10)
{
    lcd.print(0);
    lcd.setCursor(p+6,1);
}
if ( ss >=0 ) lcd.print(ss);
//
lcd.setCursor(p+4,1);
lcd.print(":");
//
  //minutos
  ///////////////////////////////////////////////////
   lcd.setCursor(p+2,1);
if ((*m)<0)
{
  if ((*h)>0)
  (*m)=59;
  (*h)=(*h)-1;
}
if ((*m)<10)
{
    lcd.print(0);
    lcd.setCursor(p+3,1);
} 
if ((*m)>=0) lcd.print(*m);
//
lcd.setCursor(p+1,1);
lcd.print(":");
//
//horas
/////////////////////////////////////////////////
lcd.setCursor(p,1);
if ((*h)>=0) lcd.print(*h);
////////////////////////////////////////////////
 if ((*s)<=0 && (*m)<=0 && (*h)<=0)
  (*pausatotal)=1;
}

void setup()
{
  
CLKPR=(1<<CLKPCE);
CLKPR=B0000101;
  
  
 //Serial.begin(9600);
 lcd.begin(16, 2);
 
last=EEPROM.read(0);
if (last<0 || last>200) last=0;
indice=last*8+1;

s = EEPROM.read(indice);
s1=EEPROM.read(indice+1);
if (s>59 || s<0) s=0;
if (s1>59 || s1<0) s1=0;
seg=s;
seg1=s1;

m = EEPROM.read(indice+2);
m1= EEPROM.read(indice+3);
if (m>59 || m<0) m=0;
if (m1>59 || m1<0) m1=0;

h = EEPROM.read(indice+4);
h1= EEPROM.read(indice+5);
if (h>9 || h<0) h=0;
if (h1>9 || h1<0) h1=0;

tipo = EEPROM.read(indice+6);
if (tipo>2 || tipo<0) tipo=0;
incremento=EEPROM.read(indice+7);
if (incremento>59 || incremento<0) incremento=0;
imp(s,m,h,9);
imp(s1,m1,h1,0);
s=s*1000;
s1=s1*1000;
//s=s+900;
//s1=s1+900;
minutillos=m;
minutillos1=m1;

pinMode(ledPin, OUTPUT);

}

void loop()
{
  
  if (pausatotal==0 && seleccion==0) {
    if (pausa==0)
    {
    if (pausa2==0)
 imprimir(&s,&m,&h,9,tinicial,&sini,&pausatotal);
    if (pausa1==0)
 imprimir(&s1,&m1,&h1,0,tinicial,&sini,&pausatotal); //izquierda
    }
    if (pausa==0)
    mbutton=read_main_button();
    else
    mbutton=9; // cualquier numero deberia funcionar
 lcd_key = read_LCD_buttons();  // read the buttons
 
 
 if (mbutton==0) { if (pausa1==1) 
                       {
                         movimiento1=movimiento1+1;           
//////////////////////
seg=s/1000;
 EEPROM.write(951, seg);
 EEPROM.write(952, m);
 EEPROM.write(953, h);
 seg=s1/1000;
 EEPROM.write(954, seg);
 EEPROM.write(955, m1);
 EEPROM.write(956, h1);
 EEPROM.write(957, tipo);
 EEPROM.write(958, incremento);
//////////////////////////////////////////
                         
                         if (tipo==1) lcd_key=retrasosimple(incremento,0);
                         if (tipo==0) {  if (movimiento1>1) s1=s1+incremento*1000;}
                         if (s1>60000) {s1=s1-60000; m1=m1+1; if (m1>60) {m1=m1-60; h1=h1+1; if (h1>9) h1=9;}}
                         tinicial=division*millis();
                         sini=s1+tinicial;
                       }
                       pausa2=1;
                       if (lcd_key==btnNONE || mbutton==0)
                       {pausa1=0; pausa=0;}
               } // boton derecho
 if (mbutton==3) { if (pausa2==1)
                       {
                         movimiento2=movimiento2+1;
                         //////////////////////
seg=s/1000;
 EEPROM.write(951, seg);
 EEPROM.write(952, m);
 EEPROM.write(953, h);
 seg=s1/1000;
 EEPROM.write(954, seg);
 EEPROM.write(955, m1);
 EEPROM.write(956, h1);
 EEPROM.write(957, tipo);
 EEPROM.write(958, incremento);
//////////////////////////////////////////
                         if (tipo==1) lcd_key=retrasosimple(incremento,3);
                         if (tipo==0) { if (movimiento2>1) s=s+incremento*1000;}
                         if (s>60000) {s=s-60000; m=m+1; if (m>60) {m=m-60; h=h+1; if (h>9) h=9;}}
                         tinicial=division*millis();
                         sini=s+tinicial;
                       }
                       pausa1=1;
                       if (lcd_key==btnNONE || mbutton==3)
                  { pausa2=0; pausa=0;}
               }// boton izquierdo
    
 if (lcd_key==btnSELECT) {seleccion=1; lcd.begin(16, 2); delay(500/division);} // boton select
 if (lcd_key==btnLEFT) { if (pausa==1 && pausa1==1 && pausa2==1)
                         {pausa=0; delay(500/division); }
                         else
                         {pausa=1; pausa1=1;pausa2=1; } delay(500/division); } // izquierda
 if (lcd_key==btnRIGHT) 
{
s = EEPROM.read(951);
m = EEPROM.read(952);
h = EEPROM.read(953);
s1 = EEPROM.read(954);
m1 = EEPROM.read(955);
h1 = EEPROM.read(956);
tipo = EEPROM.read(957);
incremento=EEPROM.read(958);
imp(s,m,h,9);
imp(s1,m1,h1,0);
s=s*1000;
s1=s1*1000;
delay(500/division);
 }
 if (pausa1==1 && pausa2==1 && pausa==1)
{lcd.setCursor(7,0); lcd.print("||"); }
else
{
 lcd.setCursor(7,0); lcd.print("**");
 lcd.setCursor(0,0); lcd.print(movimiento2);
 if (movimiento1>9) 
 lcd.setCursor(14,0);
 else
 lcd.setCursor(15,0);
 lcd.print(movimiento1);
}
 
}
  else
  {
    if (seleccion==0)
    {
      if (anuncios>0)
      {
      if ((s)<=0 && (m)<=0 && (h)<=0)
      gano=0;
      else
      gano=12;
      lcd.setCursor(gano,0);
      lcd.print("----");
      tone(2, 523, 300);
      digitalWrite(ledPin, HIGH);
      delay(100/division);
      digitalWrite(ledPin, LOW);
      noTone(2);
      lcd.setCursor(gano,0);
      lcd.print("GANO");
      delay(1500/division);
      anuncios=anuncios-1;
      }
      
    }
  }
  
 if (seleccion==1) {
 // CONFIGURACION ---------------------------------------------------
  if (seleccion==1 && edihora==0)
  {
    
    lcd.setCursor(0,0);
    lcd.print("Modo");
    lcd.setCursor(0,1);
    if ((last+1)<10)
    {
    lcd.print(0);
    lcd.setCursor(1,1);
    }
    if ((last+1)>=0) lcd.print((last+1));
 
    lcd_key = read_LCD_buttons();
    if (lcd_key==1 && last<=115)  {last=last+1; delay(500/division);} //arriba
    if (lcd_key==2 && last>0)  {last=last-1; delay(500/division);} //abajo
    if (lcd_key==4) { lcd.begin(16, 2);  seleccion=0;  EEPROM.write(0, last);
   lcd.setCursor(0,0); lcd.print("Presione Reset"); delay(60000);} // boton select
    if (lcd_key==0)//derecho
    {
      lcd.begin(16, 2);
      delay(500/division);
      edihora=1;
    }
  }
  if (edihora==1 && ediminuto==0)
  {
    lcd.setCursor(0,0);
    lcd.print("Edicion Horas");
    lcd.setCursor(0,1);
    lcd.print(h1);
    
    lcd_key = read_LCD_buttons();
    if (lcd_key==1 && h1<9)  {h1=h1+1; delay(500/division);} //arriba
    if (lcd_key==2 && h1>0)   {h1=h1-1; delay(500/division);} //abajo
    if (lcd_key==4) {ediminuto=1; lcd.begin(16, 2); delay(500/division); } // select
  }
  if (ediminuto==1 && edisegundo==0)
  {
    lcd.setCursor(0,0);
    lcd.print("Edicion Minutos");
    lcd.setCursor(0,1);
    if (m1<10)
    {
    lcd.print(0);
    lcd.setCursor(1,1);
    }
    if (m1>=0) lcd.print(m1);
    
    lcd_key = read_LCD_buttons();
    if (lcd_key==1 && m1<59)  {m1=m1+1; delay(500/division);} //arriba
    if (lcd_key==2 && m1>0)   {m1=m1-1; delay(500/division);} //abajo
    if (lcd_key==4) {edisegundo=1;  lcd.begin(16, 2); delay(500/division); } // select
  }
  
  if (edisegundo==1 && editipo==0)
  {
    lcd.setCursor(0,0);
    lcd.print("Edicion Segundos");
    lcd.setCursor(0,1);
    
    if (seg1<10)
    {
    lcd.print(0);
    lcd.setCursor(1,1);
    }
    if (seg1>=0) lcd.print(seg1);
    
    lcd_key = read_LCD_buttons();
    if (lcd_key==1 && seg1<59)  {seg1=seg1+1; delay(500/division);} //arriba
    if (lcd_key==2 && seg1>0)   {seg1=seg1-1; delay(500/division);} //abajo
    if (lcd_key==4) {
      editipo=1; lcd.begin(16, 2); delay(500/division);} // select
  }
  if (editipo==1 && ediincremento==0)
  {
    lcd.setCursor(0,0);
    lcd.print("0.Ficher 1.Delay");
    lcd.setCursor(0,1);
    lcd.print(tipo);
    lcd_key = read_LCD_buttons();
    if (lcd_key==1 && tipo<1)  {tipo=tipo+1; delay(500/division);} //arriba
    if (lcd_key==2 && tipo>0)   {tipo=tipo-1; delay(500/division);} //abajo
    if (lcd_key==4) { ediincremento=1; lcd.begin(16, 2); delay(500/division);}
  }
  if (ediincremento==1 && pregunta==0)
  {
    lcd.setCursor(0,0);
    lcd.print("Segundos?");
    lcd.setCursor(0,1);
    if (incremento<10)
    {
    lcd.print(0);
    lcd.setCursor(1,1);
    }
    if (incremento>=0) lcd.print(incremento);
    
    lcd_key = read_LCD_buttons();
    if (lcd_key==1 && incremento<59)  {incremento=incremento+1; delay(500/division);} //arriba
    if (lcd_key==2 && incremento>0)   {incremento=incremento-1; delay(500/division);} //abajo
    if (lcd_key==4) { pregunta=1; lcd.begin(16, 2); delay(500/division);}// select
  }
  
  if (pregunta==1 && jugador2==0)
  {
    lcd.setCursor(0,0);
    lcd.print("Igual? 0.Si 1.No");
    lcd.setCursor(0,1);
    lcd.print(respuesta);    
    lcd_key = read_LCD_buttons();
    if (lcd_key==1 && respuesta<1)  {respuesta=respuesta+1; delay(500/division);} //arriba
    if (lcd_key==2 && respuesta>0)   {respuesta=respuesta-1; delay(500/division);} //abajo
    if (lcd_key==4) { jugador2=1; if(respuesta==0) {ya=1; h=h1; m=m1;seg=seg1;} else edihora1=1; lcd.begin(16, 2); delay(500/division);}// select
  }
  if (jugador2==1 && ediminuto1==0)
  {
    lcd.setCursor(0,0);
    lcd.print("Edicion Horas");
    lcd.setCursor(0,1);
    lcd.print(h);    
    lcd_key = read_LCD_buttons();
    if (lcd_key==1 && h<9)  {h=h+1; delay(500/division);} //arriba
    if (lcd_key==2 && h>0)   {h=h-1; delay(500/division);} //abajo
    if (lcd_key==4) {ediminuto1=1; lcd.begin(16, 2); delay(500/division); } // select
  }
  if (ediminuto1==1 && edisegundo1==0)
  {
    lcd.setCursor(0,0);
    lcd.print("Edicion Minutos");
    lcd.setCursor(0,1);
    if (m<10)
    {
    lcd.print(0);
    lcd.setCursor(1,1);
    }
    if (m>=0) lcd.print(m);
    
    lcd_key = read_LCD_buttons();
    if (lcd_key==1 && m<59)  {m=m+1; delay(500/division);} //arriba
    if (lcd_key==2 && m>0)   {m=m-1; delay(500/division);} //abajo
    if (lcd_key==4) {edisegundo1=1;  lcd.begin(16, 2); delay(500/division); } // select
  }
  if (edisegundo1==1 && ya==0)
  {
    lcd.setCursor(0,0);
    lcd.print("Edicion Segundos");
    lcd.setCursor(0,1);
    
    if (seg<10)
    {
    lcd.print(0);
    lcd.setCursor(1,1);
    }
    if (seg>=0) lcd.print(seg);
    
    lcd_key = read_LCD_buttons();
    if (lcd_key==1 && seg<59)  {seg=seg+1; delay(500/division);} //arriba
    if (lcd_key==2 && seg>0)   {seg=seg-1; delay(500/division);} //abajo
    if (lcd_key==4) {
      ya=1;
    lcd.begin(16, 2); delay(500/division);} // select
  }
  if (ya==1)
  {
   ediincremento=0; editipo=0; edisegundo=0; ediminuto=0; edihora=0; seleccion=0;  lcd.begin(16, 2);
                              edisegundo1=0;ediminuto1=0;edihora1=0;
   EEPROM.write(0, last); //last
   indice=last*8+1;
   EEPROM.write(indice, seg);
   EEPROM.write(indice+1,seg1);
   EEPROM.write(indice+2, m);
   EEPROM.write(indice+3, m1);
   EEPROM.write(indice+4, h);
    EEPROM.write(indice+5, h1);
   EEPROM.write(indice+6, tipo);
   EEPROM.write(indice+7, incremento);
   ya=0;
   lcd.setCursor(0,0); lcd.print("Presione Reset"); delay(60000);
  }
} // seleccion
  
}

