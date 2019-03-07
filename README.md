# HT_000
HT_000 is a hash table implementation written in C which supports multi-
threaded applications.  HT_000 supports atomic construction and destruction of
objects.  When a requesting thread obtains a hash object, HT_000 guarantees
that the object will not be freed until the thread has released this object.

The task here is to implement HT_000 as described below.  The implementation
must be done in C and must be accompanied with multi-threaded test program
which demonstrates the use of HT_000.

See README.todo.txt for more details.

# Project AOC

L'idée du projet est autant la démarche d'analyse et d'expérimentation que de codage.

A priori il y a dans ce code probablement 3 sources d'optimisation:

    la gestion des verrous, dans la version présente on utilise les pthread_mutex_lock/unlock,
    la fonction de hachage de la clef,
    la fonction de calcul du CRC associée à la valeur attachée à une clef.

Le code reste un prototype il vous est donc tout a fait possible de le modifier en vue de l'optimiser.

Les paramêtres à explorer: taille des valeurs associées au clef (de quelques Byte à quelques KB ou plus), degré de parallélisme (nombre de threads par rapport à la taille de la table), méthode de verrouillage, typologies de accès (insertion de clef-valeur, lecture de valeur associée à une clef, charge mixte d'insertions et de lectures)

Voici le code de CRC a associer à chaque valeur entrée dans la table de hachage. N'hésitez pas à faire des recherches pour en trouver une version plus optimisée (hint: il y en a 🙂 )

```
const unsigned char CRC7_POLY = 0x91 ;

/*
 * getCRC,
 * this function compute a checksum for a given I/O request.
 */

uint32_t
getCRC(char *message, uint32_t length)
{
    uint32_t i, j, crc = 0;

    for (i = 0; i < length; i++)
    {
        crc ^= message[i];
        for (j = 0; j < 8; j++)
        {
            if (crc & 1)
                crc ^= CRC7_POLY;
            crc >>= 1;
        }
    }

  return crc;
}
```
C'est une mini-application de taille réduite donc on attend des modifications de code et un protocole expérimental solide.
Soyez créatifs vos propositions seront toujours examinées avec bienveillance,
