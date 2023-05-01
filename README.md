# Dragon quest monster SDL

Dragon Quest Monster SDL est un projet C++ basé sur la série de jeux Dragon Quest Monster. Le projet utilise la librairie graphique SDL. Le joueur est plongé dans un monde en 2D en vue du dessus et peut se déplacer librement pour explorer les différents environnements.

Le but du jeu est de former l'équipe la plus puissante. Pour se faire le joueur peut partir à la rencontre de monstres sauvages pour les affronter. Il peut aussi les capturer pour créer une équipe plus forte. Les monstres nouvellements capturés peuvent par la suite être fusionnés pour créer des monstres encore plus puissants qui récupère les traits de ses parents.

Le jeu implémente toutes les fonctionnalités de base de la série Dragon Quest Monster, y compris la capture de monstres, la gestion de son équipe, la fusion de monstres et bien plus encore. Le joueur doit être stratégique dans la composition de son équipe de monstres et dans la manière dont il les utilise lors des combats.

## Utilisation

Pour compiler le projet dans une console, placez-vous dans le dossier principale et entrez la ligne de commande:

```
make
```

Pour lancer le jeu lancez la commande:

```
./bin/game
```

## Fonctionnalités

-Les monstres (implémenté complètement)
    Un Monstre peut être considéré comme un combattant il possède plusieurs caractéristiques:
        - Un nom
        - Un type : On peut voir ça comme son espece
        - Un niveau : Le niveau du monstre détermine la croissance de ses statistiques
        - Des statistiques : Les statistiques du monstre déterminent sa puissance en combat
        - Des sets de compétences : Les sets de compétences du monstre déterminent les actions qu'il peut effectuer en combat
        - Une croissance : qui détermine comment vont évoluer les statistiques du monstre lorsqu'il gagne un niveau
        - Des statistiques Max : Les statistiques max du monstre déterminent la valeur limite qu'il peut atteindre dans chaque statistiques. Elles dépendent de son type
    
    Les monstres possèdent 6 statistiques : 
        - PV : Les points de vie du monstre, si ils tombent à 0 le monstre est KO
        - PM : Les points de mana du monstre, ils sont consommés lorsqu'il utilise une compétence
        - Attaque : Détermine la puissance des compétences physiques du monstre
        - Défense : Détermine la résistance aux compétences adverses du monstre
        - Agilité : Détermine la vitesse d'attaque du monstre plus la valeur est élevée plus le monstre attaquera en premier
        - Sagesse : Détermine la puissance des compétences magiques du monstre


-Combat en tour par tour (implémenté partiellement)
    Le joueur affronte des monstres sauvages dans des combats en tour par tour par équipe allant de 1 à 3 monstres par équipe. Le joueur peut choisir les actions de ses monstres où les laisser agir de manière autonome en suivant une tactique prédéfinie que le joueur peut choisir:
    - Sans pitié : Le monstre utilise seulement des compétences infligeant des dégâts sur les ennemis
    - Soins avant tout : Le monstre utilise seulement des compétences de soin pour aider ses alliés
    - Agir avec sagesse : Le monstre utilise des compétences offensive pouvant infliger des dégâts sur les ennemis ainsi qu'alterer leur statut ou des compétences de soutien pour aider ses alliés
    - Pas de magie : Le monstre n'utilise que des compétences qui ne consomment pas de PM
    Si le joueur lui donne un ordre, il peut alors choisir une compétence à utiliser parmi celles qu'il possède.
    Lorsque le joueur clique sur combattre les actions s'executent alors dans l'ordre d'agilité de chaque monstre. Plus un monstre a une grande stat d'agilité plus souvent il attaquera en premier. Si un monstre est KO il ne peut plus agir. Le combat se termine lorsque une des deux équipes est décimés complètement.
    Les monstres peuvent subir des altérations de status et de statistiques. Pour les altérations de status, il en existe plusieurs:
    - Empoisonné : Le monstre perd des PV à chaque tour
    - Paralysé : Le monstre ne peut plus agir pendant un certain nombre de tour
    - Endormi : Le monstre ne peut plus agir pendant un certain nombre de tour, il se réveille lorsqu'il est attaqué
    - Confus : Le monstre agit de manière aléatoire pendant un certain nombre de tour (pas implémenté)
    Les status s'applique à chaque fois que le monstre effectue une action.
    Pour les altérations de stats, on applique un facteur multiplicateur sur la stat du monstre pendant un certain nombre de tour. Elles peuvent être aussi bien positives que négatives. Par exemple si un monstre est affaibli, sa stat d'attaque sera divisé par 2.
    Le joueur peut aussi tenter de dresser un monstre. Dans ce cas ses monstres passent en mode dressage, tour à tour ils attaquent le monstre voulant être capturé, sans lui infliger de véritable dégât le pourcentage de dégâts qu'il lui aurait infligé indique la chance qu'ils soit capturé. Si le monstre est capturé, le combat s'arrête. Mais s'il refuse d'être capturé, il a une chance sur deux d'être vexé, dans ce cas le monstre ne pourra plus être capturé du combat. À l'échec d'un dressage le combat continue sans que les monstres du joueur puissent attaquer.
    Enfin le joueur peut décider de fuir le combat. La probabiliter de fuite dépend de la statistique d'agilité de chaque monstre. Si la fuite échoue le combat continue sans que les monstres du joueur puissent attaquer. Si la fuite réussi le combat s'arrête sans que le joueur n'est reçu aucune récompense.



## Auteurs

Projet créé par Célian VASSON p2103016 et Léandro PANEPINTO THOLLOT p2109482 dans le cadre de l'UE LIFAPCDA 

année: 2022/2023 
