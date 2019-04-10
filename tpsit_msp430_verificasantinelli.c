#include <msp430.h>

//Funzione inizializzazione generale
void initAll()
{
    //P1.0 - LED1 - OUT -
    //P4.7 - LED2 - OUT -
    //P1.1 - BTN1 - IN  -
    //P2.1 - BTN2 - IN  -

    P1DIR |= BIT0;
    P4DIR |= BIT7;
    P1OUT &=~ BIT0;
    P4OUT &=~ BIT7;

    P1DIR &=~ BIT1;
    P2DIR &=~ BIT1;
    P1OUT &=~ BIT1;
    P2OUT &=~ BIT1;
    P1REN |= BIT1;
    P2REN |= BIT1;
}

//Modifica stato led1
void led1(int stato){
    if(stato==1) P1OUT |= BIT0;
    else P1OUT &=~ BIT0;  
}

void led2(int stato){
    if(stato==1) P4OUT |= BIT7;
    else P4OUT &=~ BIT7;  
}


//Restituisci stato sw1

int sw1(){
    return P1IN & BIT1;
}

int sw2(){
    return P2IN & BIT1;
}

//sw1 è appena stato rilasciato?
int before = 0;

int pressedSw1(){
    int now=0;
    now=sw1();
    if(now!=before){
        before = now;
        return now;
    }
    else 
        return 0;
}

int releasedSw1(){
    int now=0;
    now=sw1();
    if(now!=before){
        before = now;
        return !now;
    }
    else 
        return 0;
}

int pressedSw2(){
    int now=0;
    now=sw2();
    if(now!=before){
        before = now;
        return now;
    }
    else 
        return 0;
}

int releasedSw2(){
    int now=0;
    now=sw2();
    if(now!=before){
        before = now;
        return !now;
    }
    else 
        return 0;
}

//Evento di sw1
//sw1 appena rilasciato -> return 3
//sw1 appena premuto -> return 2
//sw1 è rilasciato -> return 1
//sw1 è premuto -> return 0
int eventSw1() {
    if(releasedSw1()) return 3;
    else if (pressedSw1()) return 2;
    else if (sw1()) return 1;
    else return 0;
    
}

//Evento di sw2
//sw2 appena rilasciato -> return 3
//sw2 appena premuto -> return 2
//sw2 è rilasciato -> return 1
//sw2 è premuto -> return 0
int eventSw2() {
    if(releasedSw2()) return 3;
    else if (pressedSw2()) return 2;
    else if (sw2()) return 1;
    else return 0;
    
} 

//Macchina a stati finiti
int stato = 0; //Se lo stato è 1, il led dovrebbe essere acceso... Se invece è 0, il led dovrebbe essere spento.
void FSM_Led2() {
    
    if(stato == 0 & pressedSw1()) {
        led1(1);
        stato=1;
    }
    else if (stato == 1 & pressedSw2()){
        led1(0);
        stato=0;
    }
    
}


//Actually il main
int main() {
    while(1) FSM_Led2():
}