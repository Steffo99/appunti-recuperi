# MSP430

## [Licenza CC-BY-3.0-IT](https://creativecommons.org/licenses/by/3.0/it/)

`Copyright (c) 2019 Stefano Pigozzi`

In pratica, potete riutilizzare il file dove, come e quando vi pare, ma dovete **inserire il mio nome** nei documenti in cui questo viene usato.

Capito, prof.? Non si copia e non si spaccia per proprio!


## _Prerequisito:_ Le maschere

Prima di programmare l'MSP430 bisogna sapere cosa sono e come usare le **maschere** (bit mask) in C.

Una maschera è un'operazione che permette di modificare solo _alcuni_ bit di una variabile a più di un bit, senza cambiare gli altri.

### Esempi

```c
int variabile = 0;

//Imposta il secondo bit a 1, senza modificare gli altri 7.
variabile |= 0b0000 0010;
//Imposta il quinto bit a 1, senza modificare gli altri 7.
variabile |= 0b0001 0000;

//In questo punto del codice, variabile vale 0001 0010.

//Imposta il secondo bit a 0, senza modificare gli altri 7.
variabile &=~ 0b0000 0010;

//In questo punto del codice, variabile vale 0001 0000.
```

In particolare, se si sta modificando codice dell'MSP, è possibile usare le costanti `BITn`:

```c
int variabile = 0;

//Imposta il secondo bit a 1, senza modificare gli altri 7.
variabile |= BIT1;
//Imposta il quinto bit a 1, senza modificare gli altri 7.
variabile |= BIT4;

//In questo punto del codice, variabile vale 0001 0010.

//Imposta il secondo bit a 0, senza modificare gli altri 7.
variabile &=~ BIT1;

//In questo punto del codice, variabile vale 0001 0000.
```

Infine, puoi usare una maschera per leggere un solo bit alla volta!
```c
//Leggi solo il quinto bit.
variabile & BIT4 // --> 0001 0000

//Esegui il codice se il quinto bit è 1
if(variabile & BIT4) {
    //Codice goes here
}

//Esegui il codice se il quinto bit è 0
if(!(variabile & BIT4)) {
    //It'sa-me, code!
}
```

## _Prerequisito:_ Differenze nel C dell'MSP430

Il C dell'MSP430 è simile al C normale, ma ha alcune piccole differenze.

- L'MSP430 **NON HA** i `bool`, vanno rappresentati come `int` che sono `false` se sono 0 e `true` se sono qualsiasi altro numero.
- Alcune variabili, come `P1IN`, vengono cambiate dall'esterno del codice (vedi sotto).

## Usare le porte

### Le porte

Le porte sono quei pin/buchi che vedete sull'MSP sulla sinistra e sulla destra.

Hanno un nome che va da **P1.0** a **P1.7** e da **P1.0** a **P5.0**.

Ogni porta ha quattro bit associati:

- `DIR`(ezione): specifica se una particolare porta è un **output** (1) o un **input** (0).
- `OUT`(put): se la porta è un output, decide cosa mandare fuori da quel bit, se un 0 o un 1.
- `IN`(put): ha sempre il valore dell'input dell'ingresso desiderato; non può essere modificata.
- `SEL`(ezione): visto che tutti i pin possono fare due cose diverse, seleziona quale cosa delle due devono fare: funzionano da **Input/Output** (0) oppure usano la **funzione secondaria** (1).

Per (s)comodità, nel codice C questi bit sono raggruppati in **gruppi di 8**:  
tutte le porte da P1.0 a P1.7 sono raggruppate nelle variabili `P1DIR`, `P1OUT`, `P1IN` e `P1SEL`; quelle da P2.0 a P2.7 in `P2DIR`, `P2OUT`, etc.

```c
//P1.5 è un output
P1DIR |= BIT5;
//P1.5 è un input
P1DIR &=~ BIT5;
```

```c
//Fai uscire un 1 da P1.5
P1OUT |= BIT5;
//Fai uscire uno 0 da P1.5
P1OUT &=~ BIT5;
```

### Le funzioni

Nonostante il C dell'MSP430 sia una cosa paciugata e stranissima, per fortuna le funzioni rimangono uguali a quelle del C standard:

```c
int nomeFunzione(int parametri) {
    //Codice qui...
}
```

### _Esempio

```c
//Scrivi una funzione di inizializzazione
void initAll() {
    //Ho attaccato un led a P1.0; è quindi un I/O; per la precisione, un output.
    P1SEL &=~ BIT0;
    P1DIR |= BIT0;
    //Ho attaccato un interruttore a P1.1; è quindi un I/O; per la precisione, un input.
    P1SEL &=~ BIT1;
    P1DIR &=~ BIT1;
}

//Scrivi una funzione che cambi lo stato del led di P1.0
void led1(int state) {
    if(state != 0) {
        //Setta a 1
        P1OUT |= BIT0;
    }
    else {
        //Setta a 0
        P1OUT &=~ BIT0;
    }
}

//Scrivi una funzione che cambi lo stato del led di P4.7
void led2(int state) {
    //Posso omettere l' != 0, perchè è implicito in tutti gli if senza altre operazioni
    if(state) {
        //Setta a 1
        P4OUT |= BIT7;
    }
    else {
        //Setta a 0
        P4OUT &=~ BIT7;
    }
}

//Scrivi una funzione che legga il valore dello switch in P2.1
int readSwitch() {
    return (P2IN & BIT1);
}

//Puoi anche usare delle variabili nel tuo codice!
//Creo una variabile globale.
int statoPrecedente;

//Scrivi una funzione che controlli se lo switch P2.1 ha cambiato valore  
int debounce() {
    int statoAttuale = readSwitch();
    if(statoAttuale != statoPrecedente) {
        return 1;
    }
    else {
        return 0;
    }
}

//Scrivi una funzione che cambi stato al led1
int toggleLed1() {
    int statoAttuale = P1OUT & BIT0;
    if(statoAttuale) {
        led1(0);
    }
    else {
        led1(1);
    }
}

//...c'è altro, ma non ho il testo della verifica, quindi non lo so...
```


## Usare il timer

Ricordi quando parlavo delle funzioni secondarie? Beh, il timer è una di queste.

E' utile per ~~fare niente~~ eseguire una certa funzione dopo che è passato un po' di tempo; ad esempio, per fare lampeggiare i led con un intervallo.

Ha un _bazillione_ di opzioni che non vengono usate mai da nessuno principalmente perchè nessuno sa che esistono; qui sotto elenco solo quelle che vengono tipicamente fatte al Fermi.

### Contare il tempo

Sembrerà sorprendente, ma la funzione principale del timer è quella di **contare il tempo** passato da un certo momento, all'avanti (tipo un cronometro) o all'indietro (tipo un countdown).

Conta il tempo in __microsecondi__ (1 secondo = 1 000 000 microsecondi).