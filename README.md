(EL YOUBI MONCEF - 2G2TD1TP2 - TP2 - Prog Sys)

Compte Rendu de TP : TFTP

1. Utilisation des arguments en ligne de commande pour gettftp et puttftp
Ce que j'ai fait : J'ai implémenté la gestion des arguments en ligne de commande pour les programmes gettftp et puttftp. Chaque programme prend en entrée l'adresse du serveur et le fichier à transférer, comme spécifié dans le TP. Pour ce faire, j'ai utilisé la fonction getopt pour analyser les arguments de la ligne de commande.

Difficultés rencontrées et solution : La difficulté principale ici était d'assurer que les arguments étaient correctement traités et validés avant de poursuivre. Une erreur dans l'analyse des arguments aurait conduit à une mauvaise configuration de la connexion TFTP. J'ai ajouté des vérifications pour garantir que l'adresse du serveur et le fichier étaient bien spécifiés et formatés.

2. Utilisation de getaddrinfo pour obtenir l'adresse du serveur
Ce que j'ai fait : J'ai utilisé getaddrinfo pour obtenir l'adresse du serveur à partir de la chaîne de caractères fournie par l'utilisateur. Cela permet de gérer à la fois les adresses IPv4 et IPv6.

Difficultés rencontrées et solution : Le principal défi était d'assurer que getaddrinfo renvoie la bonne structure de données, surtout en ce qui concerne la gestion des erreurs liées aux résolutions d'adresses. J'ai ajouté des vérifications de retour pour m'assurer que l'adresse a bien été récupérée avant de tenter de se connecter au serveur.

3. Réservation d'un socket pour la connexion au serveur
Ce que j'ai fait : Après avoir obtenu l'adresse du serveur, j'ai réservé un socket en utilisant la fonction socket(). Puis, j'ai utilisé connect() pour établir une connexion avec le serveur TFTP.

Difficultés rencontrées et solution : La difficulté résidait dans le fait qu'il était nécessaire de s'assurer que le socket était correctement ouvert et que la connexion au serveur était réussie. L'une des erreurs courantes que j'ai rencontrées était l'oubli de spécifier le bon type de socket (UDP dans ce cas), ce qui entraînait des échecs de connexion. Une fois cette erreur corrigée, la connexion s'est bien établie.

4. Construction et envoi de la Read Request (RRQ) pour gettftp
Ce que j'ai fait : J'ai correctement construit la RRQ (Read Request) en respectant les spécifications du RFC1350, en envoyant l'adresse du fichier et en choisissant le mode de transfert (octet ou texte). La structure du paquet a été soigneusement créée pour respecter l'ordre des octets et l'ajout de caractères nuls.

Difficultés rencontrées et solution : La plus grande difficulté a été de bien découper la RRQ en respectant les formats spécifiés par les RFCs, notamment la gestion de l'ordre des octets et la présence du caractère nul. Après plusieurs tentatives et débogages avec Wireshark pour observer les paquets envoyés, j'ai pu résoudre cette difficulté et envoyer correctement la RRQ.

5. Réception et gestion des Data (DAT) et ACK pour gettftp
Ce que j'ai fait : J'ai géré la réception des paquets de données (DAT) envoyés par le serveur. J'ai utilisé un processus de boucle pour récupérer plusieurs paquets si nécessaire, en m'assurant d'envoyer un paquet d'ACK pour chaque bloc de données reçu.

Difficultés rencontrées et solution : La plus grande difficulté résidait dans le découpage et l'assemblage des paquets DAT multiples. Comme le fichier était divisé en plusieurs paquets, j'ai dû gérer correctement les numéros de bloc et les accusés de réception (ACK) pour chaque paquet. Pour éviter la perte de données, j'ai mis en place une stratégie pour envoyer un ACK à chaque bloc reçu, garantissant ainsi la transmission correcte du fichier.

Conclusion
Ce TP m'a permis de renforcer ma compréhension du fonctionnement du protocole TFTP, ainsi que des méthodes de gestion des paquets et des erreurs dans les protocoles réseau. La décomposition de la RRQ pour gettftp a été la partie la plus complexe du TP, mais grâce aux captures Wireshark et à une gestion minutieuse des paquets, j'ai pu surmonter cette difficulté. L'utilisation des outils comme getaddrinfo et la gestion des sockets m'ont permis de mieux comprendre les bases de la communication réseau.
