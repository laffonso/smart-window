#include <msp430g2553.h>
#include "uart.h"
#include <stdlib.h>

//Macros para se trabalhar com bits
#define setBit(valor,bit) (valor |= (1<<bit))
#define clearBit(valor,bit) (valor &= ~(1<<bit))
#define toogleBit(valor,bit) (valor^=(1<<bit))
#define testBit(valor,bit) (valor & (1<<bit))

/* Window 1
 * BOBINAS:
 * 1 ->P2.0
 * 2 ->P2.1
 * 3 ->P2.2
 * 4 ->P2.3
 * */
#define B11_on P2OUT|=BIT0          //bobina 1 liga
#define B12_on P2OUT|=BIT1          //bobina 2 liga
#define B13_on P2OUT|=BIT2         // bobina 3 liga
#define B14_on P2OUT|=BIT3         //bobina 4 liga

#define B11_off P2OUT=P2OUT&~BIT0  //bobina 1 desliga
#define B12_off P2OUT=P2OUT&~BIT1  //bobina 2 desliga
#define B13_off P2OUT=P2OUT&~BIT2  //bobina 3 desliga
#define B14_off P2OUT=P2OUT&~BIT3 //bobina 4 desliga
/* End Window 1 */

/* Window 2
 * BOBINAS:
 * 1 ->P1.0
 * 2 ->P1.6
 * 3 ->P1.7
 * 4 ->P2.4
 *  */
#define B21_on P1OUT|=BIT0         //bobina 1 liga
#define B22_on P1OUT|=BIT6         //bobina 2 liga
#define B23_on P1OUT|=BIT7         // bobina 3 liga
#define B24_on P2OUT|=BIT4         //bobina 4 liga

#define B21_off P1OUT=P1OUT&~BIT0  //bobina 1 desliga
#define B22_off P1OUT=P1OUT&~BIT6  //bobina 2 desliga
#define B23_off P1OUT=P1OUT&~BIT7  //bobina 3 desliga
#define B24_off P2OUT=P2OUT&~BIT4  //bobina 4 desliga
/* End Window 2 */

#define CAL 1 //estado de calibração
#define A 2  //estado open
#define F 3 //estado close

char rx[10];
char mensagem_tx[10];
char estado=' ';
char op[1];
char windowSelect[1];
unsigned int passos1=0;
unsigned int passos2=0;
char x[4];
int var=0;
int tempo = 0;
int a = 0;
int aberturaAtual1 = 0;
int aberturaAtual2 = 0;



void SFR_config(){
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer to prevent time out reset
    //P1DIR |=BIT0 | BIT6; //Define o Port 1 bit 0 e bit 6 como saida e bits remanescentes como entrada

    //P2DIR=0x0F;         //define o P2.0..3 como saída

    /*Window 1*/
    setBit(P2DIR,0); //P2.0 SAIDA - bobina 1
    setBit(P2DIR,1); //P2.1 SAIDA - bobina 2
    setBit(P2DIR,2); //P2.2 SAIDA - bobina 3
    setBit(P2DIR,3); //P2.3 SAIDA - bobina 4
    clearBit(P1DIR,4); //P1.4 ENTRADA - fim de curso aberta
    clearBit(P1DIR,5); //P1.5 ENTRADA - fim de curso fechada
    setBit(P1REN, 4); //PULLUP INTERNO para o P1.4
    setBit(P1REN, 5); //PULLUP INTERNO para o P1.5

    /*Window 2*/
    setBit(P1DIR,0); //P1.0 SAIDA - bobina 1
    setBit(P1DIR,6); //P1.6 SAIDA - bobina 2
    setBit(P1DIR,7); //P1.7 SAIDA - bobina 3
    setBit(P2DIR,4); //P2.4 SAIDA - bobina 4
    clearBit(P1DIR,3); //P1.3 ENTRADA - fim de curso aberta
    clearBit(P2DIR,5); //P2.5 ENTRADA - fim de curso fechada
    setBit(P1REN, 3); //PULLUP INTERNO PARA P1.3
    setBit(P2REN, 5); //PULLUP INTERNO PARA P2.5





}

//ABRE janela 1
void abreJanela1(unsigned int step, int aux) //recebe numero de passos e porcentagem
{
  unsigned long contagem=0;
  while(contagem<=((aux*step)/100))
  {
    if(contagem<(0.8*(aux*step)/100)){
      //passo 1
      B11_off;
      B12_off;
      B13_off;
      B14_on;
      __delay_cycles(5000);
      //passo 2
      B11_off;
      B12_off;
      B13_on;
      B14_on;
      __delay_cycles(5000);
      //passo 3
      B11_off;
      B12_off;
      B13_on;
      B14_off;
      __delay_cycles(5000);
      //passo 4
      B11_off;
      B12_on;
      B13_on;
      B14_off;
      __delay_cycles(5000);
      //passo 5
      B11_off;
      B12_on;
      B13_off;
      B14_off;
      __delay_cycles(5000);
      //passo 6
      B11_on;
      B12_on;
      B13_off;
      B14_off;
      __delay_cycles(5000);
      //passo 7
      B11_on;
      B12_off;
      B13_off;
      B14_off;
      __delay_cycles(5000);
      //passo 8
      B11_on;
      B12_off;
      B13_off;
      B14_on;
      __delay_cycles(5000);
    }
    else if (contagem>=(0.8*(aux*step)/100)){ //amortecimento
      //passo 1
      B11_off;
      B12_off;
      B13_off;
      B14_on;
      __delay_cycles(10000);
      //passo 2
      B11_off;
      B12_off;
      B13_on;
      B14_on;
      __delay_cycles(10000);
      //passo 3
      B11_off;
      B12_off;
      B13_on;
      B14_off;
      __delay_cycles(10000);
      //passo 4
      B11_off;
      B12_on;
      B13_on;
      B14_off;
      __delay_cycles(10000);
      //passo 5
      B11_off;
      B12_on;
      B13_off;
      B14_off;
      __delay_cycles(10000);
      //passo 6
      B11_on;
      B12_on;
      B13_off;
      B14_off;
      __delay_cycles(10000);
      //passo 7
      B11_on;
      B12_off;
      B13_off;
      B14_off;
      __delay_cycles(10000);
      //passo 8
      B11_on;
      B12_off;
      B13_off;
      B14_on;
      __delay_cycles(10000);
    }
    contagem++;
  }
  estado=' ';
}

//ABRE janela 2
void abreJanela2(unsigned int step, int aux) //recebe numero de passos e porcentagem
{
  unsigned long contagem=0;
  while(contagem<=((aux*step)/100))
  {
    if(contagem<(0.8*(aux*step)/100)){
      //passo 1
      B21_off;
      B22_off;
      B23_off;
      B24_on;
      __delay_cycles(5000);
      //passo 2
      B21_off;
      B22_off;
      B23_on;
      B24_on;
      __delay_cycles(5000);
      //passo 3
      B21_off;
      B22_off;
      B23_on;
      B24_off;
      __delay_cycles(5000);
      //passo 4
      B21_off;
      B22_on;
      B23_on;
      B24_off;
      __delay_cycles(5000);
      //passo 5
      B21_off;
      B22_on;
      B23_off;
      B24_off;
      __delay_cycles(5000);
      //passo 6
      B21_on;
      B22_on;
      B23_off;
      B24_off;
      __delay_cycles(5000);
      //passo 7
      B21_on;
      B22_off;
      B23_off;
      B24_off;
      __delay_cycles(5000);
      //passo 8
      B21_on;
      B22_off;
      B23_off;
      B24_on;
      __delay_cycles(5000);
    }
    else if (contagem>=(0.8*(aux*step)/100)){ //amortecimento
      //passo 1
      B21_off;
      B22_off;
      B23_off;
      B24_on;
      __delay_cycles(10000);
      //passo 2
      B21_off;
      B22_off;
      B23_on;
      B24_on;
      __delay_cycles(10000);
      //passo 3
      B21_off;
      B22_off;
      B23_on;
      B24_off;
      __delay_cycles(10000);
      //passo 4
      B21_off;
      B22_on;
      B23_on;
      B24_off;
      __delay_cycles(10000);
      //passo 5
      B21_off;
      B22_on;
      B23_off;
      B24_off;
      __delay_cycles(10000);
      //passo 6
      B21_on;
      B22_on;
      B23_off;
      B24_off;
      __delay_cycles(10000);
      //passo 7
      B21_on;
      B22_off;
      B23_off;
      B24_off;
      __delay_cycles(10000);
      //passo 8
      B21_on;
      B22_off;
      B23_off;
      B24_on;
      __delay_cycles(10000);
    }
    contagem++;
  }
  estado=' ';
}



//FECHA JANELA 1
void fechaJanela1(unsigned int step, int aux)
{
  unsigned long contagem=0;
  while(contagem<=((aux*step)/100))
  {
    if(contagem<(0.8*(aux*step)/100)){
      //passo 1
      B11_on;
      B12_off;
      B13_off;
      B14_off;
      __delay_cycles(2500);
      //passo 2
      B11_on;
      B12_on;
      B13_off;
      B14_off;
      __delay_cycles(2500);
      //passo 3
      B11_off;
      B12_on;
      B13_off;
      B14_off;
      __delay_cycles(2500);
      //passo 4
      B11_off;
      B12_on;
      B13_on;
      B14_off;
      __delay_cycles(2500);
      //passo 5
      B11_off;
      B12_off;
      B13_on;
      B14_off;
      __delay_cycles(2500);
      //passo 6
      B11_off;
      B12_off;
      B13_on;
      B14_on;
      __delay_cycles(2500);
      //passo 7
      B11_off;
      B12_off;
      B13_off;
      B14_on;
      __delay_cycles(2500);
      //passo 8
      B11_on;
      B12_off;
      B13_off;
      B14_on;
      __delay_cycles(2500);
    }
    else if (contagem>=(0.8*(aux*step)/100)){ //amortecimento
      B11_on;
      B12_off;
      B13_off;
      B14_off;
      __delay_cycles(5000);
      //passo 2
      B11_on;
      B12_on;
      B13_off;
      B14_off;
      __delay_cycles(5000);
      //passo 3
      B11_off;
      B12_on;
      B13_off;
      B14_off;
      __delay_cycles(5000);
      //passo 4
      B11_off;
      B12_on;
      B13_on;
      B14_off;
      __delay_cycles(5000);
      //passo 5
      B11_off;
      B12_off;
      B13_on;
      B14_off;
      __delay_cycles(5000);
      //passo 6
      B11_off;
      B12_off;
      B13_on;
      B14_on;
      __delay_cycles(5000);
      //passo 7
      B11_off;
      B12_off;
      B13_off;
      B14_on;
      __delay_cycles(5000);
      //passo 8
      B11_on;
      B12_off;
      B13_off;
      B14_on;
      __delay_cycles(5000);
    }
    contagem++;
  }
  estado=' ';
}

//FECHA JANELA 1
void fechaJanela2(unsigned int step, int aux)
{
  unsigned long contagem=0;
  while(contagem<=((aux*step)/100))
  {
    if(contagem<(0.8*(aux*step)/100)){
      //passo 1
      B21_on;
      B22_off;
      B23_off;
      B24_off;
      __delay_cycles(2500);
      //passo 2
      B21_on;
      B22_on;
      B23_off;
      B24_off;
      __delay_cycles(2500);
      //passo 3
      B21_off;
      B22_on;
      B23_off;
      B24_off;
      __delay_cycles(2500);
      //passo 4
      B21_off;
      B22_on;
      B23_on;
      B24_off;
      __delay_cycles(2500);
      //passo 5
      B21_off;
      B22_off;
      B23_on;
      B24_off;
      __delay_cycles(2500);
      //passo 6
      B21_off;
      B22_off;
      B23_on;
      B24_on;
      __delay_cycles(2500);
      //passo 7
      B21_off;
      B22_off;
      B23_off;
      B24_on;
      __delay_cycles(2500);
      //passo 8
      B21_on;
      B22_off;
      B23_off;
      B24_on;
      __delay_cycles(2500);
    }
    else if (contagem>=(0.8*(aux*step)/100)){ //amortecimento
      B21_on;
      B22_off;
      B23_off;
      B24_off;
      __delay_cycles(5000);
      //passo 2
      B21_on;
      B22_on;
      B23_off;
      B24_off;
      __delay_cycles(5000);
      //passo 3
      B21_off;
      B22_on;
      B23_off;
      B24_off;
      __delay_cycles(5000);
      //passo 4
      B21_off;
      B22_on;
      B23_on;
      B24_off;
      __delay_cycles(5000);
      //passo 5
      B21_off;
      B22_off;
      B23_on;
      B24_off;
      __delay_cycles(5000);
      //passo 6
      B21_off;
      B22_off;
      B23_on;
      B24_on;
      __delay_cycles(5000);
      //passo 7
      B21_off;
      B22_off;
      B23_off;
      B24_on;
      __delay_cycles(5000);
      //passo 8
      B21_on;
      B22_off;
      B23_off;
      B24_on;
      __delay_cycles(5000);
    }
    contagem++;
  }
  estado=' ';
}


//CALIBRAÇÃO JANELA 1
void calibracao1(){
    passos1 = 0;
    while(testBit(P1IN,4)) //fecha(anti-horario) a janela até encontrar o botão P1.5 aterrado
        {
            //passo 1
            B11_on;
            B12_off;
            B13_off;
            B14_off;
            __delay_cycles(5000);
            //passo 2
            B11_on;
            B12_on;
            B13_off;
            B14_off;
            __delay_cycles(5000);
            //passo 3
            B11_off;
            B12_on;
            B13_off;
            B14_off;
            __delay_cycles(5000);
            //passo 4
            B11_off;
            B12_on;
            B13_on;
            B14_off;
            __delay_cycles(5000);
            //passo 5
            B11_off;
            B12_off;
            B13_on;
            B14_off;
            __delay_cycles(5000);
            //passo 6
            B11_off;
            B12_off;
            B13_on;
            B14_on;
            __delay_cycles(5000);
            //passo 7
            B11_off;
            B12_off;
            B13_off;
            B14_on;
            __delay_cycles(5000);
            //passo 8
            B11_on;
            B12_off;
            B13_off;
            B14_on;
            __delay_cycles(5000);
        }

    while(testBit(P1IN,5)) //enquanto o reed switch nao for ativado conta os passos do motor (aqui a janela estará fechada e vai abrir totalmente(horario))
        {
            //passo 1
            B11_off;
            B12_off;
            B13_off;
            B14_on;
            __delay_cycles(5000);
            //passo 2
            B11_off;
            B12_off;
            B13_on;
            B14_on;
            __delay_cycles(5000);
            //passo 3
            B11_off;
            B12_off;
            B13_on;
            B14_off;
            __delay_cycles(5000);
            //passo 4
            B11_off;
            B12_on;
            B13_on;
            B14_off;
            __delay_cycles(5000);
            //passo 5
            B11_off;
            B12_on;
            B13_off;
            B14_off;
            __delay_cycles(5000);
            //passo 6
            B11_on;
            B12_on;
            B13_off;
            B14_off;
            __delay_cycles(5000);
            //passo 7
            B11_on;
            B12_off;
            B13_off;
            B14_off;
            __delay_cycles(5000);
            //passo 8
            B11_on;
            B12_off;
            B13_off;
            B14_on;
            __delay_cycles(5000);
            passos1++;
        }
    estado=' '; //zera estado
    aberturaAtual1 = 100;


}

//CALIBRAÇÃO JANELA 2
void calibracao2(){
    passos2 = 0;
    while(testBit(P1IN,3)) //fecha(anti-horario) a janela até encontrar o botão P1.3 aterrado
        {
            //passo 1
            B21_on;
            B22_off;
            B23_off;
            B24_off;
            __delay_cycles(5000);
            //passo 2
            B21_on;
            B22_on;
            B23_off;
            B24_off;
            __delay_cycles(5000);
            //passo 3
            B21_off;
            B22_on;
            B23_off;
            B24_off;
            __delay_cycles(5000);
            //passo 4
            B21_off;
            B22_on;
            B23_on;
            B24_off;
            __delay_cycles(5000);
            //passo 5
            B21_off;
            B22_off;
            B23_on;
            B24_off;
            __delay_cycles(5000);
            //passo 6
            B21_off;
            B22_off;
            B23_on;
            B24_on;
            __delay_cycles(5000);
            //passo 7
            B21_off;
            B22_off;
            B23_off;
            B24_on;
            __delay_cycles(5000);
            //passo 8
            B21_on;
            B22_off;
            B23_off;
            B24_on;
            __delay_cycles(5000);
        }

    while(testBit(P2IN,5)) //enquanto o reed switch nao for ativado conta os passos do motor (aqui a janela estará fechada e vai abrir totalmente(horario))
        {
            //passo 1
            B21_off;
            B22_off;
            B23_off;
            B24_on;
            __delay_cycles(5000);
            //passo 2
            B21_off;
            B22_off;
            B23_on;
            B24_on;
            __delay_cycles(5000);
            //passo 3
            B21_off;
            B22_off;
            B23_on;
            B24_off;
            __delay_cycles(5000);
            //passo 4
            B21_off;
            B22_on;
            B23_on;
            B24_off;
            __delay_cycles(5000);
            //passo 5
            B21_off;
            B22_on;
            B23_off;
            B24_off;
            __delay_cycles(5000);
            //passo 6
            B21_on;
            B22_on;
            B23_off;
            B24_off;
            __delay_cycles(5000);
            //passo 7
            B21_on;
            B22_off;
            B23_off;
            B24_off;
            __delay_cycles(5000);
            //passo 8
            B21_on;
            B22_off;
            B23_off;
            B24_on;
            __delay_cycles(5000);
            passos2++;
        }
    estado=' '; //zera estado
    aberturaAtual2 = 100;
}

void desligaBobinas1(){B11_off;B12_off;B13_off;B14_off;}
void ligaBobinas1(){B11_on;B12_on;B13_on;B14_on;}

void desligaBobinas2(){B21_off;B22_off;B23_off;B24_off;}
void ligaBobinas2(){B21_on;B22_on;B23_on;B24_on;}


int main( void )
{
    SFR_config();
    ConfigUART(9600); //Configura o baudrate da UART
    desligaBobinas1();
    desligaBobinas2();
    calibracao1();
    calibracao2();
    while(1)
    {

        if(UARTReceiveCount()==5){

            UARTReceive(&rx[0],5);//Salva em rx o dado recebido
            /*salva em
             * windowSelect qual janela vai operar(1 -> JANELA 1; 2-> JANELA 2)
             * op a operação (L-> calibração; A->ABRIR; F->FECHAR;)
             * x qual a porcentagem de abertura ou fechamento que será utilizada na operação (010,020,030,040,050,060,070,080,090,100)
            */
            for(a=0;a<5;a++)
                  {
                    if(a==0)
                      windowSelect[0]=rx[a];
                    else if(a==1)
                      op[0]=rx[a];
                    else if(a>1)
                        x[a-2] = rx[a];
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
              if(windowSelect[0] == '1'){
                  calibracao1();
                  aberturaAtual1 = 100;
              }else if(windowSelect[0] == '2'){
                  calibracao2();
                  aberturaAtual2 = 100;
              }
              break;

            case A:
                if(windowSelect[0] == '1'){
                    var=atoi(x);
                    if(aberturaAtual1 + var < 100){
                        aberturaAtual1 += var;
                        abreJanela1(passos1,var);
                    }else if(aberturaAtual1 == 100){
                        aberturaAtual1 = 100;
                    } else{
                        var = 100 - aberturaAtual1;
                        abreJanela1(passos1, var);
                        aberturaAtual1 = 100;
                    }
                }else if(windowSelect[0] == '2'){
                    var=atoi(x);
                    if(aberturaAtual2 + var < 100){
                        aberturaAtual2 += var;
                        abreJanela2(passos2,var);
                    }else if(aberturaAtual2 == 100){
                        aberturaAtual2 = 100;
                    } else{
                        var = 100 - aberturaAtual2;
                        abreJanela2(passos2, var);
                        aberturaAtual2 = 100;
                    }
                }
              break;
            case F:
                if(windowSelect[0] == '1'){
                    var=atoi(x);
                    if(aberturaAtual1 - var > 0){
                        aberturaAtual1 -= var;
                        fechaJanela1(passos1,var);
                    }else if(aberturaAtual1 == 0){
                        aberturaAtual1 = 0;
                    }else{
                        var = aberturaAtual1;
                        fechaJanela1(passos1, var);
                        aberturaAtual1 =0;
                    }
                }else if(windowSelect[0] == '2'){
                    var=atoi(x);
                    if(aberturaAtual2 - var > 0){
                        aberturaAtual2 -= var;
                        fechaJanela2(passos2,var);
                    }else if(aberturaAtual2 == 0){
                        aberturaAtual2 = 0;
                    }else{
                        var = aberturaAtual2;
                        fechaJanela2(passos2, var);
                        aberturaAtual2 =0;
                    }
                }
              break;
            }//end switch
    }//end while
}//end main
