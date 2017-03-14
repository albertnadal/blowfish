# Blowfish (cipher) C++ implementation

Implementation of the Blowfish symmetric-key block cipher written in C++.

### Build
```
g++ -c blowfish.cpp
g++ -c xifrar_fitxer.cpp
g++ -o blowfish xifrar_fitxer.o blowfish.o
```

### Usage
In order to encrypt a plain file use the executable file as described below and enter a password.

```
./blowfish -x <plain_file> <output_encrypted_file>
```

In order to decrypt an encrypted file use the executable file as described below and enter the password.
```
./blowfish -d <encrypted_file> <output_plain_file>
```

### The Blowfish algorithm (in Catalan)

Blowfish usa un llarg nombre de subclaus. Aquestes claus han de sér precomputades abans de qualsevol xifratge o desxifratge. El vector P consisteix en 18 subclaus de 32 bits. S’usen quatre S-boxes amb 256 valors de 32 bits cada una. Blowfish és una xarxa Feistel consistent en 16 rondes iterades. L’entrada de l’algorisme és un bloc x de 64 bits. El procés per encriptar és el següent:

```
Dividir x en dos parts de 32 bits: xL i xR
For i = 1 to 16
{
	xL = xL XOR P[i]
	xR = F(xL) XOR xR
	Intercanviar xL i xR
}
Intercanviar xL i xR (Desfer l’últim intercanvi)
xR = xR XOR P[17]
xL = xL XOR P[18]
Recombinar xL i xR
```

![](http://albertnadal.cat/wp-content/uploads/2013/02/blowfish_diagrama.png)

La funció F (funció Feistel) es mostra a continuació:


Dividir xL en quatre parts de vuit bits: a, b, c i d
F(xL) = (((S1[a]+S2[b]) XOR S3[c]) + S4[d])

![](http://albertnadal.cat/wp-content/uploads/2013/02/blowfish_feistel_function.png)

El desxifratge és fa exactament igual que el xifratge, excepte que P[1], P[2], … ,P[18] són usats en ordre invers.
La generació de les subclaus es realitza inicialitzant primer de tot el vector P i les quatre S-Boxes, ordenadament, amb una cadena de caràcters fixada. Aquesta cadena consisteix en els dígits del valor pi(excepte el tres inicial) en hexadecimal: P[1] = 0x243f6a88, P[2] = 0x85a308d3, P[3] = 0x13198a2e, etc… Després s’ha de fer una operació XOR de P[1] amb els primers 32 bits de la clau, després fer una operació XOR de P[2] amb els següents 32 bits de la clau, i així amb tota la resta d’elements del vector P i de la clau.
En aquesta implementació he tingut en compte el tamany de la clau, és a dir, si la longitud de la clau és menor que el nombre d’elements del vector P, aleshores es continua fent el mateix procés actualizant la resta de valors del vector P amb la clau.


El mètode XifrarText(char text[], int longitud) xifra un fragment de memòria. El xifratge s’efectua en blocs de 8 bytes, d’aquesta manera s’ha d’aplicar l’algorisme de xifratge a cada bloc. El resultat de xifrar un bloc genera un altre bloc xifrat de 8 bytes en un altre fragment de memòria. El fragment de memòria que ha de contenir les dades xifrades haurà de sér múltiple de vuit, això no implica que el fragment que es vol xifrar també ho hagi de sér.

![](http://albertnadal.cat/wp-content/uploads/2013/02/blowfish_farciment1.png)

DesxifrarText(char text[], int longitud, int NoHiHaGuarnicio) desxifra un fragment un fragment de memòria de longitud fixada. El desxifratge també s’efectua en blocs de 8 bytes, d’aquesta manera s’ha d’aplicar l’algorisme de desxifratge descrit a cada bloc. El resultat de desxifrar un bloc genera un altre bloc de 8 bytes amb les dades en clar que s’afegeixen a un fragment de memòria generat dinàmicament. El fragment de memòria que ha de contenir les dades desxifrades ha de tenir la mateixa longitud que el text original en clar, per fer possible això s’han d’eliminar les dades de farciment inserides al final. El farciment només s’usa quan el text que es vol xifrar no és múltiple de vuit i té la principal funcionalitat d’indicar el final de les dades en clar.

![](http://albertnadal.cat/wp-content/uploads/2013/02/blowfish_farciment_desxifratge.png)

Al moment de xifrar es xifra bloc a bloc fins a arrivar a l’últim bloc del fragment. En aquest últim bloc en clar, s’insereix la suma en mòdul 255 dels últims bytes del missatge, al següent byte, just després de l’últim caràcter del missatge. La resta de bytes del bloc contenen valors aleatòris que no són la suma en mòdul 255 dels primers bytes. Finalment es xifra el bloc.

![](http://albertnadal.cat/wp-content/uploads/2013/02/blowfish_farciment_impl.png)


### License
MIT

