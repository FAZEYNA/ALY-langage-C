# ALY-langage-C
MINI PROJET DE GESTION DES VENTES DANS UNE BOUTIQUE D'HABILLEMENT

La boutique de vente d’habillement GOOR FALL SHOP, souhaite mettre en place une
application de gestion de ses ventes et de son stock. Il fait appel a vous en vous soumettant
un exemple d’application sous forme de vidéo et les informations suivantes :
Pour un Article donné, on a besoin de savoir sa désignation, son prix, sa quantité en stock
(qui se met à jour à chaque vente), son code (composé de 7 caractères), sa catégorie et sa
date d’ajout.
Une catégorie est caractérisée par son libellé et son son id (auto).
Pour accéder à l’application, on nous demande de se connecter. Il y a deux types
d’utilisateurs :
§ Les Admin : qui peuvent ajouter, modifier, supprimer les produits. Ils peuvent aussi mettre
à jour les stocks en cas de nouvelle arrivage. Ils ont aussi le rôle d’ajouter les utilisateurs
l’applications. Ces derniers peuvent être bloqué par un Admin ou être débloqué.
§ Les utilisateurs : qui peuvent utiliser l’interface de vente de produits et celle de la caisse et
peuvent aussi imprimer l’état de la caisse.
Un admin peut faire tout se qu’un utilisateur peut faire.
Lors de la création d’un utilisateur par l’admin, il lui donne un mot de passe par défaut :
passer123. Pour la première fois que l’utilisateur se connecte, il doit changer son mot de
passe.
Un utilisateur/ admin est caractérisé par son id (auto), son nom, son prénom, son téléphone,
son login et son mot de passe. Le login de l’utilisateur ne peut pas être changer, est fixé par
l’admin et comporte exactement 5 caractère alphabétique en majuscules.
Les informations des utilisateurs ou admins sont stockées dans un fichier binaire nommé
USERS. Dans ces fichiers les mots de passe doivent être crypté par un algorithme que vous
aurez créé.
Les catégories sont stockées dans un fichier binaire nommé CATEGORIES.
Les produits sont stockés dans un fichier binaire nommé PRODUCTS. Impossible d’avoir un
produit dont sa catégorie n’est pas dans le fichier CATEGORIES.
Les ventes sont dans un dossiers nommé BILLS, dans lequel chaque vente est un fichier texte
sous forme de facture nommé RECU_ AAAAMMDDHHmmSS_II_LL.txt .
Une vente est caractérisée par son id (auto), son numéro, les produits vendus et leur
quantités vendues, le montant total de la vente et les informations de l’utilisateur qui a
enregistré cette vente (son nom, prénom et son numéro de téléphone : l’utilisateur
connecté).
Le numéro de vente est sous le format : AAAAMMDDHHmmSS.
Convention de nommage :
- AAAA : l’année actuelle
- MM : le mois actuel
- DD : le jour actuel
- HH : heure actuelle
- mm : le nombre de minutes actuel
- SS : le nombre de second actuel
- II : l’id de la vente actuelle
- LL : le login de l’utilisateur connecté
Les utilisateurs peuvent imprimer (générer un fichier texte) l’état des ventes par jour. C’est
un fichier texte nommé ETAT_AAAAMMDD dans lequel on inscrit le montant total des
ventes ainsi que le nombre d’article vendu.
Pour vendre un article, l’utilisateur doit ajouter des articles dans la commande du client.
L’utilisateur recherche (A) l’article par son code et si le code existe, on lui demande de
mettre la quantité à vendre (vérifier si la quantité demandée existe dans le stock, sinon on
lui demande s’il veut prendre la quantité qui reste dans le stock). Si le code recherché existe
mais sa quantité est égale à zéro (0) dans ce cas on lui affiche que l’article est en rupture de
stock. Après la recherche du code et la saisie de la quantité, le produit est ajouté dans la
commande et l’application propose à l’utilisateur s’il veut ajouter un nouveau produit dans
la commande : si oui, on reprend le même procèdes (A), si non, on enregistre la vente et
automatiquement la facture (reçu) est générée.
L’application propose un menu avec plusieurs choix après la connexion de l’utilisateur. Parmi
ces choix, on peut avoir, la liste des utilisateurs, l’ajout d’utilisateur, l’ajout de catégorie,
l’ajout de produit, effectuer une vente, imprimer l’état d’aujourd’hui, lister les produits, liste
les catégories.
Bonne chance.
