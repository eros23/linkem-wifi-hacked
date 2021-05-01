# linkem-wifi-hacked

## Descrizione Vulnerabilità
Linkem S.p.A. è una società italiana che opera nel settore delle telecomunicazioni, in particolare nel mercato della 
connessione a banda larga in modalità wireless.

Se conosciamo il numero di serie del router, possiamo generare la password predefinita del router con il programma linkem_pass.c
Ad esempio per il router con numero di serie "GMK170210001234" ed ESSID "Linkem2.4GHz_A1B285" possiamo generare la password come descritto sotto.

## Esecuzione del codice

Compila con il comando:
`gcc linkem_pass.c -lcrypto -o linkem_pass`

Esegui con:
`./a.out GMK170709001234 A1B285`
