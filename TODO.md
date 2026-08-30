# TODO

## Provisioning MQTT (casa/set) — debolezze di design note

Il meccanismo di provisioning (finestra di 30s dopo il boot in cui il dispositivo
ascolta su `casa/set` per ricevere un nuovo topic) funziona per l'uso attuale
(pochi sensori, riconfigurati a mano uno alla volta) ma ha limiti noti:

- **Esclusività solo temporale, non per dispositivo.** Se più sensori si riavviano
  nello stesso momento (es. dopo un blackout), tutti tornano in ascolto su
  `casa/set` insieme — il primo comando che arriva li riconfigura tutti, non solo
  quello voluto. Non c'è nessun identificativo per dispositivo che distingua i
  comandi.
- **Nessuna conferma/ack.** Chi manda il comando (a mano con `mosquitto_pub`, o
  poi Node-RED) non ha modo di sapere con certezza che il dispositivo giusto
  l'abbia ricevuto e applicato, oltre a rileggere l'annuncio periodico.
- **Nessuna validazione del payload.** Il dispositivo scrive come nuovo topic
  qualunque cosa arrivi su `casa/set`, senza controllare che sia un topic
  sensato (lunghezza, formato).

Accettato come compromesso per ora — da rivedere quando il numero di sensori
cresce o la riconfigurazione smette di essere un'operazione manuale sorvegliata.
