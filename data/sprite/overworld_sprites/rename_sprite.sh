#!/bin/bash

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 directory_name"
  exit 1
fi

# Définir le nom de base pour les nouveaux fichiers
new_name_front="${1}_front_"
new_name_back="${1}_back_"
new_name_right="${1}_right_"
new_name_left="${1}_left_"



# Vérifier si le dossier existe
if [ ! -d "$1" ]; then
  echo "Le dossier spécifié n'existe pas"
  exit 1
fi

# Se déplacer vers le dossier spécifié
cd "$1"


# Boucle pour renommer les fichiers
for i in {0..2}
do
    # Renommer les fichiers avec le nouveau nom
    mv "row-1-column-$((i+1)).png" "${new_name_front}${i}.png"
    mv "row-1-column-$((i+4)).png" "${new_name_back}${i}.png"
    mv "row-1-column-$((i+7)).png" "${new_name_right}${i}.png"
    mv "row-1-column-$((i+10)).png" "${new_name_left}${i}.png"
done

echo "Renommage terminé!"
