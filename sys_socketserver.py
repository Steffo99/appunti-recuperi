import socket

# Crea un nuovo socket: per ora potrebbe essere sia server sia client
server = socket.socket()

# Trasforma il socket in un server:
#   Crea una tupla con l'indirizzo locale a cui associare il server
#   Il primo elemento è una str, e indica l'unica interfaccia da cui altri si possono connettere
#   Il secondo elemento è un int, ed è la porta del server
indirizzo_locale = ('127.0.0.1', 9999)
#   Associa il socket all'indirizzo
server.bind(indirizzo_locale)
#   Inizia ad accettare connessioni; massimo 1 contemporaneamente, 
#   visto che questo il codice ne può gestire una sola alla volta
server.listen(1)

# Ciclo principale del server
while True:
    # Aspetta che si connetta qualcuno;
    # quando si connette, metti il suo oggetto socket corrispondente nella variabile client
    # e il suo indirizzo nella variabile indirizzo_remoto (è una tupla, come indirizzo locale!)
    client, indirizzo_remoto = server.accept()
    print("Connection from: " + str(indirizzo_remoto))
    # Aspetta che quel qualcuno ti invii dei dati;
    # Quando li ricevi, mettili nella variabile data (attenzione, è del tipo bytes!)
    data = client.recv(1024)
    # Se ricevi una riga vuota, ignorala
    if not data:
        continue
    # Converti in una stringa i dati che hai ricevuto
    data_str = str(data, encoding="ascii")
    print("Received: " + data_str)
    # Invia un "addio" in risposta al messaggio
    # Nota la b davanti alle prime virgolette:
    # non è una stringa, sono i bytes che se convertiti in ascii scrivono "addio"
    client.send(b"addio")
    # Chiudi la connessione con il client; nessuno dei due sarà più in grado di mandare messaggi all'altro...
    # almeno finchè non ne viene stabilita una nuova
    client.close()
