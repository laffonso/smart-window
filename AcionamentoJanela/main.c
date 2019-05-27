#include <msp430g2553.h>
#include "uart.h"
#include <stdlib.h>

//Macros para se trabalhar com bits
#define setBit(valor,bit) (valor |= (1<<bit))
#define clearBit(valor,bit) (valor &= ~(1<<bit))
#define toogleBit(valor,bit) (valor^=(1<<bit))
#define testBit(valor,bit) (valor & (1<<bit))

#define B1_on P2OUT|=BIT0          //bobina 1 liga
#define B2_on P2OUT|=BIT1          //bobina 2 liga
#define B3_on P2OUT|=BIT2         // bobina 3 liga
#define B4_on P2OUT|=BIT3         //bobina 4 liga

#define B1_off P2OUT=P2OUT&~BIT0  //bobina 1 desliga
#define B2_off P2OUT=P2OUT&~BIT1  //bobina 2 desliga
#define B3_off P2OUT=P2OUT&~BIT2  //bobina 3 desliga
#define B4_off P2OUT=P2OUT&~BIT3 //bobina 4 desliga

#define sc_off (P1IN&BIT5)==BIT5//reed switch do começo desativado com 1
#define sf_off (P1IN&BIT7)==BIT7 //reed switch do final desativado com 1
#define CAL 1 //estado de calibração
#define A 2  //estado open
#define F 3 //estado close

char rx[10];
char mensagem_tx[10];
char estado=' ';
char op[1];
unsigned int passos=0;
char x[4];
int var=0;
int tempo = 0;
int a = 0;
int aberturaAtual = 0;



void SFR_config(){
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer to prevent time out reset
    P1DIR |=BIT0 | BIT6; //Define o Port 1 bit 0 e bit 6 como saida e bits remanescentes como entrada

    //P2DIR=0x0F;         //define o P2.0..3 como saída

    setBit(P2DIR,0); //P2.0 SAIDA
    setBit(P2DIR,1); //P2.1 SAIDA
    setBit(P2DIR,2); //P2.2 SAIDA
    setBit(P2DIR,3); //P2.3 SAIDA
    setBit(P1DIR,0); //P1.0 SAIDA
    setBit(P1DIR,6); //P1.6 SAIDA
    //clearBit(P1DIR,4); //P1.4 ENTRADA
   // clearBit(P1DIR,5); //P1.5 ENTRADA



    setBit(P1REN, 4); //PULLUP INTERNO para o P1.4
    setBit(P1REN, 5); //PULLUP INTERNO para o P1.5
    clearBit(P1OUT,0); //zera o bit0 do P1
    clearBit(P1OUT,6); //zera o bit6 do P1
}

//ABRE
void abreJanela(unsigned int step, int aux) //recebe numero de passos e porcentagem
{
  unsigned long contagem=0;
  while(contagem<=((aux*step)/100))
  {
    if(contagem<(0.8*(aux*step)/100)){
      //passo 1
      B1_off;
      B2_off;
      B3_off;
      B4_on;
      __delay_cycles(5000);
      //passo 2
      B1_off;
      B2_off;
      B3_on;
      B4_on;
      __delay_cycles(5000);
      //passo 3
      B1_off;
      B2_off;
      B3_on;
      B4_off;
      __delay_cycles(5000);
      //passo 4
      B1_off;
      B2_on;
      B3_on;
      B4_off;
      __delay_cycles(5000);
      //passo 5
      B1_off;
      B2_on;
      B3_off;
      B4_off;
      __delay_cycles(5000);
      //passo 6
      B1_on;
      B2_on;
      B3_off;
      B4_off;
      __delay_cycles(5000);
      //passo 7
      B1_on;
      B2_off;
      B3_off;
      B4_off;
      __delay_cycles(5000);
      //passo 8
      B1_on;
      B2_off;
      B3_off;
      B4_on;
      __delay_cycles(5000);
    }
    else if (contagem>=(0.8*(aux*step)/100)){ //amortecimento
      //passo 1
      B1_off;
      B2_off;
      B3_off;
      B4_on;
      __delay_cycles(10000);
      //passo 2
      B1_off;
      B2_off;
      B3_on;
      B4_on;
      __delay_cycles(10000);
      //passo 3
      B1_off;
      B2_off;
      B3_on;
      B4_off;
      __delay_cycles(10000);
      //passo 4
      B1_off;
      B2_on;
      B3_on;
      B4_off;
      __delay_cycles(10000);
      //passo 5
      B1_off;
      B2_on;
      B3_off;
      B4_off;
      __delay_cycles(10000);
      //passo 6
      B1_on;
      B2_on;
      B3_off;
      B4_off;
      __delay_cycles(10000);
      //passo 7
      B1_on;
      B2_off;
      B3_off;
      B4_off;
      __delay_cycles(10000);
      //passo 8
      B1_on;
      B2_off;
      B3_off;
      B4_on;
      __delay_cycles(10000);
    }
    contagem++;
  }
  estado=' ';
}

//FECHA
void fechaJanela(unsigned int step, int aux)
{
  unsigned long contagem=0;
  while(contagem<=((aux*step)/100))
  {
    if(contagem<(0.8*(aux*step)/100)){
      //passo 1
      B1_on;
      B2_off;
      B3_off;
      B4_off;
      __delay_cycles(2500);
      //passo 2
      B1_on;
      B2_on;
      B3_off;
      B4_off;
      __delay_cycles(2500);
      //passo 3
      B1_off;
      B2_on;
      B3_off;
      B4_off;
      __delay_cycles(2500);
      //passo 4
      B1_off;
      B2_on;
      B3_on;
      B4_off;
      __delay_cycles(2500);
      //passo 5
      B1_off;
      B2_off;
      B3_on;
      B4_off;
      __delay_cycles(2500);
      //passo 6
      B1_off;
      B2_off;
      B3_on;
      B4_on;
      __delay_cycles(2500);
      //passo 7
      B1_off;
      B2_off;
      B3_off;
      B4_on;
      __delay_cycles(2500);
      //passo 8
      B1_on;
      B2_off;
      B3_off;
      B4_on;
      __delay_cycles(2500);
    }
    else if (contagem>=(0.8*(aux*step)/100)){ //amortecimento
      B1_on;
      B2_off;
      B3_off;
      B4_off;
      __delay_cycles(5000);
      //passo 2
      B1_on;
      B2_on;
      B3_off;
      B4_off;
      __delay_cycles(5000);
      //passo 3
      B1_off;
      B2_on;
      B3_off;
      B4_off;
      __delay_cycles(5000);
      //passo 4
      B1_off;
      B2_on;
      B3_on;
      B4_off;
      __delay_cycles(5000);
      //passo 5
      B1_off;
      B2_off;
      B3_on;
      B4_off;
      __delay_cycles(5000);
      //passo 6
      B1_off;
      B2_off;
      B3_on;
      B4_on;
      __delay_cycles(5000);
      //passo 7
      B1_off;
      B2_off;
      B3_off;
      B4_on;
      __delay_cycles(5000);
      //passo 8
      B1_on;
      B2_off;
      B3_off;
      B4_on;
      __delay_cycles(5000);
    }
    contagem++;
  }
  estado=' ';
}

void calibracao(){
    passos = 0;
    while(testBit(P1IN,4)) //fecha(anti-horario) a janela até encontrar o botão P1.5 aterrado
        {
            //passo 1
            B1_on;
            B2_off;
            B3_off;
            B4_off;
            __delay_cycles(5000);
            //passo 2
            B1_on;
            B2_on;
            B3_off;
            B4_off;
            __delay_cycles(5000);
            //passo 3
            B1_off;
            B2_on;
            B3_off;
            B4_off;
            __delay_cycles(5000);
            //passo 4
            B1_off;
            B2_on;
            B3_on;
            B4_off;
            __delay_cycles(5000);
            //passo 5
            B1_off;
            B2_off;
            B3_on;
            B4_off;
            __delay_cycles(5000);
            //passo 6
            B1_off;
            B2_off;
            B3_on;
            B4_on;
            __delay_cycles(5000);
            //passo 7
            B1_off;
            B2_off;
            B3_off;
            B4_on;
            __delay_cycles(5000);
            //passo 8
            B1_on;
            B2_off;
            B3_off;
            B4_on;
            __delay_cycles(5000);
        }

    while(testBit(P1IN,5)) //enquanto o reed switch nao for ativado conta os passos do motor (aqui a janela estará fechada e vai abrir totalmente(horario))
        {
            //passo 1
            B1_off;
            B2_off;
            B3_off;
            B4_on;
            __delay_cycles(5000);
            //passo 2
            B1_off;
            B2_off;
            B3_on;
            B4_on;
            __delay_cycles(5000);
            //passo 3
            B1_off;
            B2_off;
            B3_on;
            B4_off;
            __delay_cycles(5000);
            //passo 4
            B1_off;
            B2_on;
            B3_on;
            B4_off;
            __delay_cycles(5000);
            //passo 5
            B1_off;
            B2_on;
            B3_off;
            B4_off;
            __delay_cycles(5000);
            //passo 6
            B1_on;
            B2_on;
            B3_off;
            B4_off;
            __delay_cycles(5000);
            //passo 7
            B1_on;
            B2_off;
            B3_off;
            B4_off;
            __delay_cycles(5000);
            //passo 8
            B1_on;
            B2_off;
            B3_off;
            B4_on;
            __delay_cycles(5000);
            passos++;
        }
    estado=' '; //zera estado
    aberturaAtual = 100;


}

void desligaBobinas(){B1_off;B2_off;B3_off;B4_off;}
void ligaBobinas(){B1_on;B2_on;B3_on;B4_on;}


int main( void )
{
    SFR_config();
    ConfigUART(9600); //Configura o baudrate da UART
    desligaBobinas();
    calibracao();
    while(1)
    {

        if(UARTReceiveCount()==4){

            UARTReceive(&rx[0],4);//Salva em rx o dado recebido
            for(a=0;a<4;a++) //salva em op (L-> calibração; A->ABRIR; F->FECHAR;) e em x (010,020,030,040,050,060,070,080,090,100)
                  {
                    if(a==0)
                      op[0]=rx[a];
                    else if(a>0)
                      x[a-1]=rx[a];
                  }
            if(op[0]=='L')
            {
                estado=CAL;
            }
            else if(op[0]=='A')
            {
                estado=A;
            }
            else if(op[0]=='F')
            {
                estado=F;
            }
        }
        switch (estado)
            {
            case CAL:
              calibracao();
              aberturaAtual = 100;
              //fechaJanela(passos, 50);
              break;

            case A:
              var=atoi(x);
              if(aberturaAtual + var < 100){
                  aberturaAtual += var;
                  abreJanela(passos,var);
              }else{
                  var = 100 - aberturaAtual;
                  abreJanela(passos, var);
                  aberturaAtual = 100;
              }
              break;
            case F:
              var=atoi(x);
              if(aberturaAtual - var > 0){
                  aberturaAtual -= var;
                  fechaJanela(passos,var);
              }else{
                  var = aberturaAtual;
                  fechaJanela(passos, var);
                  aberturaAtual =0;
              }
              break;
            }
    }
}
