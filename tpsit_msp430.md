# MSP430

## [Licenza CC-BY-3.0-IT](https://creativecommons.org/licenses/by/3.0/it/)

`Copyright (c) 2019 Stefano Pigozzi`

In pratica, potete riutilizzare il file dove, come e quando vi pare, ma dovete **inserire il mio nome** nei documenti in cui questo viene usato.

Capito, prof. \_\_\_\_\_\_? Non si usano appunti di altri studenti senza dargli credito!


## _Prerequisito:_ Le maschere

Prima di programmare l'MSP430 bisogna sapere cosa sono e come usare le **maschere** (bit mask) in C.

Una maschera è un'operazione che permette di modificare solo _alcuni_ bit di una variabile a più di un bit, senza cambiare gli altri.

### Esempi

```c
unsigned int variabile = 0;

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
unsigned int variabile = 0;

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

Ogni porta ha cinque bit associati:

- `DIR`(ezione): specifica se una particolare porta è un **output** (1) o un **input** (0).
- `OUT`(put): se la porta è un output, decide cosa mandare fuori da quel bit, se un 0 o un 1.
- `IN`(put): ha sempre il valore dell'input dell'ingresso desiderato; non può essere modificata.
- `SEL`(ezione): visto che tutti i pin possono fare due cose diverse, seleziona quale cosa delle due devono fare: funzionano da **Input/Output** (0) oppure usano la **funzione secondaria** (1).
- `REN`: abilita / disabilita una _resistenza di pullup/down_, **assicurati che sia sempre attiva quando usi qualcosa come input** o potrebbero succedere cose inaspettate!

Per (s)comodità, nel codice C questi bit sono raggruppati in **gruppi di 8**:  
tutte le porte da P1.0 a P1.7 sono raggruppate nelle variabili `P1DIR`, `P1OUT`, `P1IN`, `P1SEL` e `P1REN`; quelle da P2.0 a P2.7 in `P2DIR`, `P2OUT`, etc.

```c
//P1.5 è un input
P1DIR &=~ BIT5;
//Assicurati che P1REN sia a 1, P1OUT sia a 0 e P1SEL sia a 0 quando usi un input!
P1REN |= BIT5;
P1SEL &=~ BIT5;
P1OUT &=~ BIT5;
```

```c
//P1.5 è un output
P1DIR |= BIT5;

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

### Esempio

```c
//Scrivi una funzione di inizializzazione
void initAll() {
    //Ho attaccato un led a P1.0; è quindi un I/O; per la precisione, un output.
    P1SEL &=~ BIT0;
    P1DIR |= BIT0;
    //Ho attaccato un led a P4.7; è quindi un I/O; per la precisione, un output.
    P4SEL &=~ BIT7;
    P4DIR |= BIT7;
    //Ho attaccato un interruttore a P1.1; è quindi un I/O; per la precisione, un input.
    P1DIR &=~ BIT1;
    P1REN |= BIT5;
    P1SEL &=~ BIT5;
    P1OUT &=~ BIT5;
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

//Scrivi una funzione che controlli se lo switch P2.1 è appena stato spinto
int debounce() {
    int statoAttuale = readSwitch();
    if(statoAttuale != statoPrecedente && statoAttuale) {
        statoPrecedente = statoAttuale;
        return 1;
    }
    else {
        statoPrecedente = statoAttuale;
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

> TL;DR in fondo, guarda quello se hai fretta!

Il timer è una delle funzioni dell'MSP: serve per contare il tempo con una precisione altissima.

Ha millemila impostazioni, ma al Fermi se ne è sempre usata praticamente solo una.

L'MSP ha due timer che possono funzionare contemporaneamente: si chiamano `TA0` (**TA**imer **0**... Cosa? Non si scrive così timer?) e `TA1` (Well, ci ho provato).

> Qui sotto parlerò solo di `TA1`, ma `TA0` si configura nello stesso identico modo; basta mettere lo 1 invece che l'1 nel nome della variabile!

### Velocità del timer

Il timer dell'MSP può andare a **due velocità**:

- `TASSEL__SMCLK`: 1 MHz = 1 000 000 Hz
- `TASSEL__ACLK`: 32 KHz = 32 000 Hz

> Sì, quelli sono due underscore. E' fatto così.

Inoltre, queste due velocità possono essere diminuite con l'impostazione `ID`, che divide per un certo numero la frequenza:

- `ID__1`: Mantiene la frequenza uguale. What's even the point?
- `ID__2`: Divide la frequenza **per due**.
- `ID__4`: Divide la frequenza **per quattro**. Inizi a vedere un pattern?
- `ID__8`: Divide la frequenza **PER OTTO**! WOW!

> Ricordati di non confonderti con la versione con un underscore solo, che fa una cosa diversa per qualche motivo...

Combinate, le impostazioni danno questi risultati:

| | `TASSEL__SMCLK` | `TASSEL__ACLK` |
|-|-----------------|----------------|
| `ID__1` | 1 000 000 tick/sec | 32 000 tick/sec |
| `ID__2` | 500 000 tick/sec | 16 000 tick/sec |
| `ID__4` | 250 000 tick/sec | 8 000 tick/sec |
| `ID__8` | 125 000 tick/sec | 4 000 tick/sec |

#### Configurare la durata

Per configurare il timer, bisogna mettere il numero di tick per cui si vuole che conti nella variabile `TA1CCR0`.

Come calcolare il numero di tick? Basta fare `numero_secondi * frequenza_timer`, prendendo la frequenza timer dalla tabella sopra!

#### Esempio

Voglio che il timer duri mezzo secondo.

Decido di usare `TASSEL_ACLK` e `ID__8`, perchè è il più facile da usare.

Calcolo il numero di tick: `0.5 secondi * 4000 tick/sec = 2000 tick`.

Allora, scrivo questo:

```c
TA1CCR0 = 2000;
```





### Tante altre cose inutili

Il timer ha tante, tante, tante altre impostazioni che sono tanto, tanto, tanto inutili.

Queste vanno inserite nella variabile `TA1CTL` (**TA**imer **1** **C**on**T**ro**L**).

```c
/*
    clock_scelto: metti TASSEL__ACLK o TASSEL__SMCLK in base a cosa hai scelto di usare prima
    divisore_scelto: metti ID__1, ID__2, ID__4 o ID__8 in base a cosa hai scelto di usare prima
    MC__UP: fai in modo che il timer vada avanti (duh)
    TAIE: attiva/disattiva il timer (TAimer Interrupt Enable)
    TACLR & ~TAIFG: resetta il timer
*/
TA1CTL = clock_scelto | divisore_scelto | MC__UP | TAIE | TACLR & ~TAIFG;
```

### Funzione da chiamare quando finisce il timer

Per marcare una funzione come "quella da chiamare quando finisce il timer", bisogna aggiungere `__attribute__ (( interrupt(TIMER1_A1_VECTOR) ))` dopo `void`, ma prima del nome della funzione.

> Anche qui sì, sono due parentesi tonde.

Tutto il resto del codice verrà _interrotto_ mentre questa funzione è in esecuzione, e questa potrebbe essere chiamata mentre un'altra è a metà.

#### Esempio

```c
void __attribute__ (( interrupt(TIMER1_A1_VECTOR) )) timer1() {
    //Fai cose!
}
```

### **TL;DR**: Too long, didn't read

Per eseguire una funzione dopo X secondi, scrivi questo.

```c
void avviaTimer1(float secondi) {
    TA1CCR0 = (int) (secondi * 4000)
    TA1CTL = TASSEL__ACLK | ID__8 | TACLR | TAIE | MC__UP & ~TAIFG; 
}

void __attribute__ ((interrupt(TIMER1_A1_VECTOR))) timer1() {
    //Codice che vuoi che venga eseguito!
}
```