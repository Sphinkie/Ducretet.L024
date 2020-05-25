# DUCRETET L.024

## Préparation de la carte MP3

La carte MP3 peut être refaite chaque fois que l'on veut ajouter de nouveaux morceaux pour le player MP3.

La préparation nécessite l'application freeware MP3Tag "*The universal Tag Editor*" http://www.mp3tag.de

### Tags

Les tags MP3 doivent être bien renseignés, en particulier: **Titre, Artiste, Genre, Année**.
Ne pas hésiter à donner des étoiles (*Rating*) aux différents morceaux.

### Préparation

* Lancer **MP3Tag.exe**

   File → Change directory : choisir le répoertoire des musiques et cocher "subdirectories"
   Edit → Select all files
   Actions → Actions (Quick) → Export → OK
   **Export** → **radial_catalog**:  Génère un fichier **catalog.txt** (2 minutes).
   **Export** → **radial_transfert**: Génère un fichier **TransfertFiles.txt** (2 minutes).
   Note:  ces fichiers sont générés dans le dossier racine des musiques.

* Copier les deux fichiers générés dans le répertoire source vers **/Preparation**

* Lancer "**ConvertTransfertBatch.bat**" qui convertit le fichier texte ".txt" en un fichier de commandes ".bat". *S'il y a un message d'erreur, c'est probablement dû à des caractères non-latins (cyrilliques, par ex.): il faut les modifier dans le fichier texte ou dans les tags mp3, et relancher le batch de conversion.)*

* Insérer la carte SD du player MP3 dans le PC. S'assurer qu'elle est identifiée en tant que disque R:.

* Lancer "**TransfertFiles.bat**"

## Format du fichier Catalogue

| Année (4 chars) | Hash (8 chars) |  Rating (1 char) | Beat (3 chars) |Genre (variable)|
| --------------- | -------------- | ---------------- | --------------- | --------------- |
| 1878            | B2BC31C5       | 3       | 120          |Classique|
| 1918            | 080B6B84       | 2            | 097            |Jazz|

Exemple:

```
1878;B2BC31C5;3;120;Classique;
1918;080B6B84;2;097;Jazz;
```

0000 = Année inconnue
Le Catalogue est trié par Année. Séparateur = *point-virgule*.

## Fichiers du répertoire

* **Catalog.ndx** = catalogue final, à copier sur la carte SD
* **TransfertFiles.bat** = fichier de commande en CP850 pour copier les MP3 sur la carte SD
* **radial_catalog.mte** = fichier d'actions MP3Tag pour générer *Catalog.txt*
* **radial_transfert.mte** = fichier d'actions MP3Tag pour générer *radial_transfert.txt*
* **ConvertTransfertBatch.bat** = fichier de commande pour générer *TransfertFiles.bat*
* **radial_transfert.txt** = fichier UTF-16
* **Catalog.txt** = fichier intermédiaire, non trié par Année.
* **iconv** = utilitaire de conversion UTF-16 en CodePage CP850

