//getch et putc pour cacher le mot de passe
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#define F fflush(stdin);
const char nomCat[]= "CATEGORIES";
const char nomProd[] = "PRODUCTS";
const char nomSupp[] = "change";
const char nomUser[] = "USERS";
int id_auto = 0;
int id_cat = 0;
int id_vente = 0;

// MES STRUCTURES
typedef struct
{
    int s, mn, h, j, m, a;
}DATE;
typedef struct
{
    char libelle[25];
    int idCat; //UNIQUE
}CATEGORIE;
typedef struct
{
    char cat[25], nom[15], code[7];
    DATE d;
    int prix;
    int qte; //qui se met à jour à chaque vente;
}ARTICLE;
typedef struct
{
    int id;
    char etat[6], nom[10], prenom[20], tel[10], login[6], mdp[15], statut[7];
}USER;
typedef struct
{
    int idVente;
    DATE heureVente; //numéro de vente est sous le format : AAAAMMDDHHmmSS. SPRINTF
    ARTICLE articleVente;
    int montantVente;
    char nomV[15], prenomV[20], telV[10], loginV[6], num[15]/*num numero vente*/;
}VENTE;
typedef struct
{
    int nbrV, montV;
}ETAT;
// MES MODULES
void afficherMenuUser(); //AFFICHAGE DU MENU DE L'UTILISATEUR
void afficherMenuAdmin(); //AFFICHAGE DU MENU DE L'ADMIN
void menuConnexion(char nom[], FILE *f, char nomfP[], FILE *fP, char nomfC[], FILE *fC, char nomfS[], FILE *fS, FILE *f5, FILE *f6);//C'EST LA QUE TOUT SE PASSSE C'EST LUNIQUE MODULE APPELE DANS LE MAIN()
void modifUser(char nom[], FILE *f); //C'EST UN MENU QUI PROPOSE L'AJOUT ET LE BLOCAGE D'UTILISATEUR MAIS AUSSI LE DEBLOCAGE
FILE* blockUser(char [],char nomf[], FILE *f); // BLOCAGE D'UTILISATEUR
FILE* unblockUser(char [], char nomf[], FILE *f); // DEBLOCAGE D'UTILISATEUR
void listeUser(char nom[],FILE*); // LISTE DES UTILISATEURS ET ADMINS
FILE* addUser(char nom[]); // AJOUT D'UTILISATEURS
int idUser(char nom[], FILE*); //CE MODULE PERMET DE COMPTER LE NOMBRE DUTILISATEUR DANS LE FICHIER USERS ET RETOURNE LE NOMBRE DUTILISATEURS TOTAL,
//AINSI LE PROCHAIN UTILISATEUR AJOUTE AURA L'ID DU NOMBRE TOTAL D'UTILISATEURS + 1, ON EN A BESOIN POUR EVITER LA REDONDANCE D'ID
FILE* passer123(char [], char[], FILE*); //ON LUI PASSE LE LOGIN DE L'UTILISATEUR CONNECTE SI LUTILISATEUR A POUR MOT DE PASSE CELA SIGNIFIE QUE C'EST SA PREMIERE CONNEXION
//DONC ON LUI FAIT CHANGER SON MOT DE PASSE ET LE SAUVEGARDE DANS LE FICHIER USERS
USER saveInfosUser(char login[], char nom[], FILE *f);
int searchLogin(char nom[], char nomf[], FILE *f);
void modifProd(char nom[], FILE *f, char nomf2[], FILE *f2, char nomf3[], FILE *f3); //C'EST UN MENU QUI PROPOSE L'AJOUT, LA SUPPRESSION ET LA MODIFICATION DE STOCK DE PRODUIT
FILE* modifStock(char code[], char nom[], FILE*); //MODULE QUI PERMET DE CHANGER LA QUANTITE EN STOCK DU PRODUIT ON PASSE PAR LE CODE DU PRODUIT
FILE* supProd(char code[], char nomf[], FILE *f, char nomf2[], FILE *f2); // MODULE QUI SUPPRIME LE PRODUIT ON LUI PASSE LE CODE
FILE* addProd(char nom[], int N, char nomCat[], FILE *f2); //AJOUT DE PRODUIT
int searchProd(char nom[], char nomf[], FILE *f); //RECHERCHE DE PRODUITS PAR LE CODE
void listeProd(char nom[], FILE*); //LISTE DE PRODUIT
FILE* addCat(char nom[], int N); // AJOUT DE CATEGORIE
void listeCat(char nom[],FILE*); //LISTE DE CATEGORIES
int searchCat(char nom[], char nomf[], FILE *f); //RECHERCHE DU LIBELLE DE LA CATEGORIE DANS LE FICHIER CATEGORIES
int verifnumber(char[]); //VERIFICATION DU NUMERO DE TELEPHONE
DATE hora(); // MODULE RECUPERANT LA DATE ACTUELLE
int idCat(char nom[],FILE*); //CE MODULE PERMET DE COMPTER LE NOMBRE DE CATEGORIES DANS LE FICHIER CATEGORIES ET RETOURNE LE NOMBRE TOTAL DE CATEGORIES, AINSI LA PROCHAINE
//CATEGORIE AJOUTEE AURA L'ID DU NOMBRE TOTAL DE CATEGORIES + 1, ON EN A BESOIN POUR EVITER LA REDONDANCE D'ID
int saisiePositive(char[]);
FILE* vente(USER, char nom[], FILE*, FILE *f2, FILE *f3); //ARTICLE/RECU/ETAT
FILE* vente(USER, char nom[], FILE*, FILE *f2, FILE *f3); //ARTICLE/RECU/ETAT
FILE* addArticle(FILE *f, char nom[], char code[], char cat[], char nomV[], int prix, int qte, int monV); //RECU
FILE* addTotal(FILE *f, char nom[], int monT); //RECU
FILE* addEtat(FILE *f, char nom[], int monT, int nbrV); //ETAT TODAY

main()
{
    FILE *f, *f2, *f3, *f4, *f5, *f6;
    menuConnexion(nomUser, f, nomProd, f2, nomCat, f3, nomSupp, f4, f5, f6);
}

void menuConnexion(char nom[], FILE *f, char nomfP[], FILE *fP, char nomfC[], FILE *fC, char nomfS[], FILE *fS, FILE *f5, FILE *f6)// TU ENTRES TON LOGIN ET TON MOT DE PASSE SILS CORRESPONDENT A UN ENREGISTREMENT DANS LE FICHIER USERS, ON AFFICHE LE MENU ADMIN
//SI CEST UN ADMIN EN LUI PASSANT LES INFOS DE LADMIN CONNECTE, OU ALORS LE MENU DE LUTILISATEUR SI CEST UN USER NON BLOQUE EN LUI PASSANT LES INFOS
//DE LUSER
{
    f = fopen(nom,"rb");
    USER u;
    char login[6], mdp[15], choix;
    int cpt,i;
        F //MENU DE CONNEXION
        printf("\t\t\t\t\t====================================================================================\n");
        printf("\t\t\t\t\t============================BIENVENUE CHER USER !===================================\n");
        printf("\t\t\t\t\t====================================================================================\n");
        puts("");
        printf("\t\t\t\t\t====================================================================================\n");
        printf("\t\t\t\t\t============================ENTRER LE LOGIN SVP :===================================\n");
        printf("\t\t\t\t\t====================================================================================\n");
        gets(login);puts("");
        printf("\t\t\t\t\t====================================================================================\n");
        printf("\t\t\t\t\t============================ENTRER LE MOT DE PASSE :================================\n");
        printf("\t\t\t\t\t====================================================================================\n");
        gets(mdp);puts("");
        if(strcmp("passer123", mdp)!=0) //PASSER123 NA PAS ETE CRYPTE
        {
             for(i=0;i<strlen(mdp);i++)
            {
                if(mdp[i]!='/0')
                {
                    mdp[i]+=45;
                }
            }
        }
        system("PAUSE");
        system("CLS");
        if(f)
        {
            rewind(f);
            while(fread(&u, sizeof(u), 1, f) != 0) //ON PARCOURT LE FICHIER DUTILISATEUR ET ON REGARDE SI LE LOGIN ET LE MOT DE PASSE CORRESPONDENT A UN ENREGISTREMENT DANS LE FICHIER
            {
                if(strcmp(u.login, login) == 0 && strcmp(u.mdp, mdp) == 0) // TEST DE VALIDITE DU MOT DE PASSE ET LOGIN
                {
                    if(strcmp(u.etat,"user")==0 && strcmp(u.statut,"bloque")!=0) //MDP ET LOGIN VALIDES ON REGARDE SI CEST UN USER NON BLOQUE OU SI CEST UN ADMIN
                    {
                        printf("\n\t\t\t\t\t\t\t\t   AUTHENTIFICATION REUSSIE !\n");
                        passer123(login, nom, f); // TEST DE LA PREMIERE CONNEXION SI LE MOT DE PASSE EST EGAL A PASSER123 ON LE MODIFIE
                        COMEBABY:
                        printf("\t\t\t\t\t====================================================================================\n");
                        printf("\t\t\t\t\t===========================BIENVENUE CHER %s!====================================\n",u.login);
                        printf("\t\t\t\t\t====================================================================================\n");
                        printf("\t\t\t\t\t==========================1-EFFECTUER UNE VENTE=====================================\n");
                        printf("\t\t\t\t\t====================================================================================\n");
                        printf("\t\t\t\t\t==========================2-LISTE DES ARTICLES======================================\n");
                        printf("\t\t\t\t\t====================================================================================\n");
                        printf("\t\t\t\t\t=================================3-QUITTER==========================================\n");
                        printf("\t\t\t\t\t====================================================================================\n");
                        do{
                            F
                            printf("FAITES VOTRES CHOIX SVP : \n");
                            scanf("%c",&choix);puts("");
                            if(choix!='1' && choix !='2' && choix!='3')
                            {
                                printf("\n\t\t\t\t\tCHOIX INDISPONIBLE !\n");
                            }
                        }while(choix!='1' && choix !='2' && choix!='3');
                         system("CLS");
                        switch(choix)
                        {
                            case '1':
                                vente(saveInfosUser(login,nom,f),nomfP,fP,f5,f6);
                                system("PAUSE");
                                system("CLS");
                                goto COMEBABY;
                            break;
                            case '2':
                                listeProd(nomfP,fP);
                                system("PAUSE");
                                system("CLS");
                                goto COMEBABY;
                            break;
                            case '3':
                            break;
                        }
                        cpt = 1;
                    }
                    if(strcmp(u.etat,"admin")==0)
                    {
                        printf("\n\t\t\t\t\t\t\t\t    AUTHENTIFICATION REUSSIE !\n");
                        HERE :
                        printf("\t\t\t\t\t====================================================================================\n");
                        printf("\t\t\t\t\t===========================BIENVENUE CHER %s!====================================\n",u.login);
                        printf("\t\t\t\t\t====================================================================================\n");
                        printf("\t\t\t\t\t==========================1-LISTE DES UTILISATEURS==================================\n");
                        printf("\t\t\t\t\t==========================2-MODIFICATION D'UTILISATEUR==============================\n");
                        printf("\t\t\t\t\t====================================================================================\n");
                        printf("\t\t\t\t\t==========================3-LISTE DES PRODUITS======================================\n");
                        printf("\t\t\t\t\t==========================4-MODIFICATION DE PRODUIT=================================\n");
                        printf("\t\t\t\t\t====================================================================================\n");
                        printf("\t\t\t\t\t==========================5-LISTE DES CATEGORIES====================================\n");
                        printf("\t\t\t\t\t==========================6-AJOUT DE CATEGORIE======================================\n");
                        printf("\t\t\t\t\t====================================================================================\n");
                        printf("\t\t\t\t\t==========================7-EFFECTUER UNE VENTE=====================================\n");
                        printf("\t\t\t\t\t====================================================================================\n");
                        printf("\t\t\t\t\t================================8-QUITTER===========================================\n");
                        printf("\t\t\t\t\t====================================================================================\n");
                        do{
                            F
                            printf("FAITES VOTRES CHOIX SVP : \n");
                            scanf("%c",&choix);
                            if(choix!='1' && choix !='2' && choix!='3' && choix!='4' && choix !='5' && choix!='6' && choix!='7' && choix !='8')
                            {
                                printf("\n\t\t\t\t\tCHOIX INDISPONIBLE !\n");
                            }
                        }while(choix!='1' && choix !='2' && choix!='3' && choix!='4' && choix !='5' && choix!='6' && choix!='7' && choix !='8');
                        system("CLS");
                        switch(choix)
                        {
                            case '1':
                                listeUser(nom, f);
                                system("PAUSE");
                                system("CLS");
                                goto HERE;
                            break;
                            case '2':
                                modifUser(nom, f);
                                system("PAUSE");
                                system("CLS");
                                goto HERE;
                            break;
                            case '3':
                                listeProd(nomfP,fP);
                                system("PAUSE");
                                system("CLS");
                                goto HERE;
                            break;
                            case '4':
                                modifProd(nomfP, fP, nomfC, fC, nomfS, fS);
                                system("PAUSE");
                                system("CLS");
                                goto HERE;
                            break;
                            case '5':
                                listeCat(nomfC, fC);
                                system("PAUSE");
                                system("CLS");
                                goto HERE;
                            break;
                            case '6':
                                addCat(nomfC,saisiePositive("CATEGORIES"));
                                system("PAUSE");
                                system("CLS");
                                goto HERE;
                            break;
                            case '7':
                                vente(saveInfosUser(login,nom,f),nomfP,fP,f5,f6);
                                system("PAUSE");
                                system("CLS");
                                goto HERE;
                            break;
                            case '8':
                                break;
                        }
                        cpt = 1;
                        break;
                    }
                }
                else
                {
                    cpt = 0;
                }
            }
            fclose(f);
        }
        if(cpt==0)
        {
            printf("\n\t\t\t\t\t\t\t\tUTILISATEUR INEXISTANT!\n");
        }
}

//   switch(choix)
//   {
//      case '1':
//        vente();
//      break;
////    case '2':
////       etaToday();
////    break;
//        case '3':
//        break;
//   }


void modifUser(char nom[], FILE *f)
{
    char choix, login[6];
    COMEHERE:
    printf("\t\t\t\t\t====================================================================================\n");
    printf("\t\t\t\t\t============================BIENVENUE CHER ADMIN !==================================\n");
    printf("\t\t\t\t\t====================================================================================\n");
    printf("\t\t\t\t\t==========================1-AJOUT D'UTILISATEUR=====================================\n");
    printf("\t\t\t\t\t====================================================================================\n");
    printf("\t\t\t\t\t==========================2-BLOCAGE D'UTILISATEUR===================================\n");
    printf("\t\t\t\t\t====================================================================================\n");
    printf("\t\t\t\t\t==========================3-DEBLOCAGE D'UTILISATEUR=================================\n");
    printf("\t\t\t\t\t====================================================================================\n");
    printf("\t\t\t\t\t=================================4-QUITTER==========================================\n");
    printf("\t\t\t\t\t====================================================================================\n");
    do{
        F
        printf("FAITES VOTRES CHOIX SVP : \n");
        scanf("%c",&choix);puts("");puts("");
        if(choix!='1' && choix !='2' && choix !='3' && choix !='4')
        {
            printf("\n\t\t\t\t\tCHOIX INDISPONIBLE !\n");
        }
    }while(choix!='1' && choix !='2' && choix!='3'  && choix !='4');
    switch(choix)
    {
        case '1':
            addUser(nom);
            system("PAUSE");
            system("CLS");
            goto COMEHERE;
        break;
        case '2':
            printf("Entrer le login de l'utilisateur a bloquer : \n");
            scanf("%s",&login);
            blockUser(login, nom, f);
            system("PAUSE");
            system("CLS");
            goto COMEHERE;
        break;
        case '3':
            printf("Entrer le login de l'utilisateur a debloquer : \n");
            scanf("%s",&login);
            unblockUser(login, nom, f);
            system("PAUSE");
            system("CLS");
            goto COMEHERE;
        break;
        case '4':
        break;
    }
}

void modifProd(char nom[], FILE *f, char nomf2[], FILE *f2, char nomf3[], FILE *f3)
{
    char choix , code[7];
    IWANTUHERE:
    printf("\t\t\t\t\t====================================================================================\n");
    printf("\t\t\t\t\t============================BIENVENUE CHER ADMIN !==================================\n");
    printf("\t\t\t\t\t====================================================================================\n");
    printf("\t\t\t\t\t==========================1-AJOUT DE PRODUIT(S)=====================================\n");
    printf("\t\t\t\t\t====================================================================================\n");
    printf("\t\t\t\t\t==========================2-MISE A JOUR DE STOCK(S)=================================\n");
    printf("\t\t\t\t\t====================================================================================\n");
    printf("\t\t\t\t\t==========================3-SUPPRESSION DE PRODUIT(S)===============================\n");
    printf("\t\t\t\t\t====================================================================================\n");
    printf("\t\t\t\t\t=================================4-QUITTER==========================================\n");
    printf("\t\t\t\t\t====================================================================================\n");
    do{
        F
        printf("FAITES VOTRES CHOIX SVP : \n");
        scanf("%c",&choix);puts("");
        if(choix!='1' && choix !='2' && choix!='3' && choix != '4')
        {
            printf("\n\t\t\t\t\tCHOIX INDISPONIBLE !\n");
        }
    }while(choix!='1' && choix !='2' && choix!='3'&& choix != '4');
    switch(choix)
    {
        case '1':
            addProd(nom, saisiePositive("PRODUITS"), nomf2, f2);
            system("PAUSE");
            system("CLS");
            goto IWANTUHERE;
        break;
        case '2':
            printf("ENTRER LE CODE DU PRODUIT DONT LA QUANTITE EN STOCK EST A MODIFIER : \n");
            scanf("%s",&code);
            modifStock(code, nom, f);
            system("PAUSE");
            system("CLS");
            goto IWANTUHERE;
        break;
        case '3':
            printf("ENTRER LE CODE DU PRODUIT A SUPPRIMER : \n");
            scanf("%s",&code);
            supProd(code, nom, f, nomf3, f3);
            system("PAUSE");
            system("CLS");
            goto IWANTUHERE;
        break;
        case '4':
        break;
    }
}

FILE* addCat(char nomf2[], int N) // AJOUT DE CATEGORIE
{
    FILE *f = fopen(nomf2, "a");
    CATEGORIE cat;
    F
    int i, id_cat = idCat(nomf2,f); //RECUPERATION DU NOMBRE TOTAL DE CATEGORIES CONTENUES DANS LE FICHIER
    printf("\t\t\t\t\t==========================1-AJOUT DE CATEGORIE======================================\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    if(f)
    {
        for(i=0; i<N; i++) //
        {
            printf("CATEGORIE %d------------------------------------------------------------------------------------------------------------------------------------------------------------\n",i+1);
            do{
                printf("Entrer le libelle de la categorie : \n");
                gets(cat.libelle);puts("");
                strupr(cat.libelle);
                if(searchCat(cat.libelle,nomf2,f)==1)
                {
                    printf("\n\t\t\t\t\t\tLIBELLE DEJA EXISTANT ! \n");
                }
                else
                {
                    cat.idCat = ++id_cat;
                    fwrite(&cat, sizeof(cat), 1, f);
                }
            }while(searchCat(cat.libelle,nomf2,f)==1);
        }
         fclose(f);
    }
    return f;
}
void listeCat(char nomf2[], FILE *f) //LISTE DE CATEGORIES
{
    f = fopen(nomf2, "rb");
    CATEGORIE cat;
    printf("\t\t\t\t\t==========================5-LISTE DES CATEGORIES====================================\n");
    if(f)
    {
        while(fread(&cat, sizeof(cat), 1, f) != 0)
        {
            printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("ID : %d\n",cat.idCat);
            printf("Libelle : %s\n",cat.libelle);
        }
        fclose(f);
    }
}
int idCat(char nomf2[], FILE *f) //OUVERTURE DU FICHIER BINAIRE EN MODE LECTURE, PUIS DECOMPTE ET RETOUR DU NOMBRE DE CATEGORIE
{
    f = fopen(nomf2,"rb");
    CATEGORIE cat;
    int cpt=0;
    if(f)
    {
        while(fread(&cat, sizeof(cat), 1, f) != 0)
        {
            cpt++;
        }
        fclose(f);
    }
    return cpt;
}

int verifnumber(char tel[]) //VERIFICATION DU NUMERO DE TELEPHONE SI LE CPT=0 NUMERO INVALIDE SINON VALIDE,
{
    int cpt=1, i;
    if(strlen(tel)!=9)
    {
        cpt=0;
    }else{
        if((tel[0]!='7' || (tel[1]!='0' && tel[1]!='6' && tel[1]!='7' && tel[1]!='8'))) //TEST DES 2 PREMIERS CHIFFRES
        {
            cpt=0;
        }
        else
        {
            for(i=2; i<strlen(tel); i++)
            {
                if(tel[i]!='9' && tel[i]!='8' && tel[i]!='7' && tel[i]!='6' && tel[i]!='5' && tel[i]!='4' && tel[i]!='3' && tel[i]!='2' && tel[i]!='1' && tel[i]!='0')
                {
                    cpt=0; //TEST DES 7 DERNIERS CHIFFRES
                }
            }
        }
    }
    return cpt;
}

DATE hora() //RECUPERATION DE LA DATE ET L'HEURE
{
    DATE h;
    time_t now;
    time(&now);
    struct tm *local = localtime(&now);
    h.j = local ->tm_mday;
    h.m = local ->tm_mon+1;
    h.a = local->tm_year+1900;
    h.h = local->tm_hour;
    h.mn = local->tm_min;
    h.s = local->tm_sec;
    return h;
}

int saisiePositive(char msg[]) //RECOIT UN MESSAGE ET RETOURNE UN NOMBRE POSITIF
{
    int N;
    do{
        printf("COMBIEN DE %s VOULEZ-VOUS SAISIR ? \n", msg);
        scanf("%d",&N);
    }while(N<=0);
    return N;
}

int searchCat(char nom[], char nomf[], FILE *f) //VERIFIE SI LE LIBELLE PASSE EN PARAMETRES EST CONTENU DANS LE FICHIER DE CATEGORIES SI OUI CPT=1
{
    f = fopen(nomf, "rb");
    int cpt=0;
    CATEGORIE cat;
    if(f)
    {
        while(fread(&cat, sizeof(cat), 1, f) != 0)
        {
            if(strcmp(nom,cat.libelle)==0)
            {
                cpt=1;
                break;
            }
        }
        fclose(f);
    }
    return cpt;
}

int searchProd(char nom[], char nomf[], FILE *f)//VERIFIE SI LE CODE PASSE EN PARAMETRES EST CONTENU DANS LE FICHIER D'ARTICLE SI OUI CPT=1
{
    f = fopen(nomf, "rb");
    int cpt=0;
    ARTICLE a;
    if(f)
    {
        while(fread(&a, sizeof(a), 1, f) != 0)
        {
            if(strcmp(nom,a.code)==0)
            {
                cpt=1;
                break;
            }
        }
        fclose(f);
    }
    return cpt;
}
int searchLogin(char nom[], char nomf[], FILE *f) //VERIFIE SI LE LOGIN PASSE EN PARAMETRES EST CONTENU DANS LE FICHIER DE USERS SI OUI CPT=1
{
    f = fopen(nomf, "rb");
    int cpt=0;
    USER u;
    if(f)
    {
        while(fread(&u, sizeof(u), 1, f) != 0)
        {
            if(strcmp(nom,u.login)==0)
            {
                cpt=1;
                break;
            }
        }
        fclose(f);
    }
    return cpt;
}
void listeProd(char nom[], FILE *f) //PARCOURS DU FICHERS DARTICLES ET AFFICHAGE DES DONNEES DES ARTICLES
{
    f = fopen(nom,"rb");
    ARTICLE a;
    printf("\t\t\t\t\t==========================3-LISTE DES PRODUITS======================================\n");
    if(f)
    {
        while(fread(&a, sizeof(a), 1, f) != 0)
        {
            printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
            printf("Libelle : %s\n",a.nom);
            printf("Code : %s\n",a.code);
            printf("Categorie : %s\n",a.cat);
            printf("Prix : %d FCFA\n",a.prix);
            printf("Quantite en stock : %d\n",a.qte);
            printf("Ajoute le %d-%d-%4d a %d:%d:%d\n",a.d.j,a.d.m,a.d.a,a.d.h,a.d.mn,a.d.s);
        }
        fclose(f);
    }
}

FILE* addProd(char nom[], int N, char nomf2[], FILE *f2)//DANS CE MODULE ON DOIT AJOUTER UN PRODUIT, DABORD ON ENTRE LA CATEGORIE DE LARTICLE SEULEMENT
//SI ELLE EXISTE PEUT ON AJOUTER LE PRODUIT ENSUITE ON VERIFIE SI LE CODE ENTRE NEST PAS DEJA EXISTANT SIL NE LEST PAS ON CONTINUE LAJOUT
{
    FILE *f = fopen(nom,"ab");
    f2 = fopen(nomf2,"rb");
    ARTICLE a;
    CATEGORIE cat;
    int i;
    printf("\t\t\t\t\t==========================1-AJOUT DE PRODUIT======================================\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    if(f)
    {
         for(i=0; i<N; i++)
         {
            printf("ARTICLE %d--------------------------------------------------------------------------------------------------------------------------------------------------------------\n",i+1);
             do{
                F
                printf("Entrer la categorie de l'article : \n");
                gets(a.cat);puts("");
                strupr(a.cat);
                if(searchCat(a.cat,nomf2,f2)!=1)
                {
                    printf("\n\t\t\t\t\t\t\t\t\tCATEGORIE INEXISTANTE !\n");
                }
            }while(searchCat(a.cat,nomf2,f2)!=1);

            do{
                do{
                    printf("Entrer le code de l'article (7 caracteres): \n");
                    gets(a.code);puts("");
                }while(strlen(a.code)!=7);
                if(searchProd(a.code,nom,f)==1)
                {
                    printf("\n\t\t\t\t\t\t\t\t\tCODE DEJA EXISTANT ! \n");
                }
            }while(searchProd(a.code,nom,f)==1);

            printf("Entrer le libelle de l'article : \n");
            gets(a.nom);puts("");
            do{
                printf("Entrer le prix de l'article : \n");
                scanf("%d",&a.prix);puts("");
            }while(a.prix<=0);
            do{
                printf("Entrer la quantite en stock de l'article : \n");
                scanf("%d",&a.qte);puts("");
            }while(a.qte<=0);
            a.d = hora();
            fwrite(&a, sizeof(a), 1, f);
         }
        fclose(f);
    }
    return f;
}

FILE* modifStock(char code[], char nom[], FILE *f)
//N OUVRE LE FICHIER EM MODE LECTURE ECRITURE, SI LE CODE ENTRE NEST PAS CONTENU DANS LE FICHIER DE PRODUITS ON AFFICHE CODE INEXISTANT,
//SINON ON ENTRE LA NEW QUANTITE DU PRODUIT
//ON REECRIT LA NOUVELLE QUANTITE DANS LE FICHIER ET ENSUITE ON FAIT UN BREAK POUR QUITTER UNE FOIS LA MODIFICATION FAITE
{
    f = fopen(nom,"rb+");
    ARTICLE a;
    int qte;
    F
    printf("\t\t\t\t\t==========================2-MISE A JOUR DE STOCK(S)=================================\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    if(f)
    {
        if(searchProd(code,nom,f)!=1)
        {
            printf("\n\t\t\t\t\t\t\t\tCODE INEXISTANT ! \n");
        }
        else
        {
            do{
                F
                printf("Entrer la nouvelle quantite : \n");
                scanf("%d",&qte);puts("");
            }while(qte<0);
            rewind(f);
            while(fread(&a, sizeof(a), 1, f) != 0)
            {
                if(strcmp(a.code, code) == 0)
                {
                    a.qte = qte;
                    fseek(f, -sizeof(a), 1);
                    fwrite(&a, sizeof(a), 1, f);
                    break;
                }
            }
            printf("\n\t\t\t\t\t\t\t\t  MODIFICATION REUSSIE !\n");
        }
        fclose(f);
    }
    return f;
}

FILE* supProd(char code[], char nomf[], FILE *f, char nomf2[], FILE *f2) // ICI ON VA UTILISER DEUX FICHIERS,LE PREMIER EST CELUI
//DES PRODUITS LAUTRE UN FICHIER VIDE, SI LE CODE ENTRE EST TROUVE DANS LE FICHIER DE PRODUITS ON COPIE TOUS LES ENREGISTREMENTS DONT
//LES CODES SONT DIFFERENTS DE CELUI ENTRE, ON SUPPRIME LE FICHIER DE PRODUITS PUIS ON RENOMME LE DEUXIEME FICHIER, SINON ON AFFICHE
//ARTICLE INEXISTANT
{
    f = fopen(nomf, "rb"), f2 = fopen(nomf2, "wb");
    ARTICLE a;
    if(f)
    {
        if(f2)
        {
            if(searchProd(code,nomf,f)==1)
            {
                rewind(f);
                while(fread(&a, sizeof(a), 1, f) != 0)
                {
                    if(strcmp(code,a.code)!=0)
                    {
                       fwrite(&a, sizeof(a), 1, f2);
                    }
                }
                printf("\n\t\t\t\t\t\t\t\t  SUPPRESSION REUSSIE !\n");
            }
            else{
                printf("\n\t\t\t\t\t\t\t\t  ARTICLE INEXISTANT !\n");
            }
            fclose(f2);
        }
        fclose(f);
    }
    remove(nomf);
    rename(nomf2,nomf);
    return f;
}

FILE* addUser(char nom[])//ON VERIFIE DABORD SI LE LOGIN NEST PAS DEJA EXISTANT SIL NE LEST PAS ON AJOUTE LUTILISATEUR OU LADMIN, SI CEST UN ADMIN IL MET SON MOT
// DE PASSE MAIS SI CEST UN USER IL A POUR MOT DE PASSE passer123
{
    FILE *f = fopen(nom,"ab");
    USER u;
    char rep[3];
    int cpt, k;
    printf("\t\t\t\t\t==========================1-AJOUT D'UTILISATEUR=====================================\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    if(f)
    {
        do{
            F
            printf("Voulez-vous ajouter un utilisateur ou un administrateur ? \n");
            printf("Tapez 'user' pour utilisateur ou 'admin' pour administrateur. \n");
            gets(u.etat);
            strlwr(u.etat);puts("");
        }while(strcmp(u.etat,"user")!=0 && strcmp(u.etat,"admin")!=0);
        do{
            do{
                printf("Entrer le login : \n");
                gets(u.login); puts("");
                strupr(u.login);
            }while(strlen(u.login)!=5);
            if(searchLogin(u.login,nom,f)==1)
            {
                printf("\n\t\t\t\t\t\t\t\tLOGIN DEJA EXISTANT \n");
            }
        }while(searchLogin(u.login,nom,f)==1);
            printf("Entrer le nom : \n");
            gets(u.nom);puts("");
            printf("Entrer le(s) prenom(s) : \n");
            gets(u.prenom);puts("");
            do{
                printf("Entrer le numero de telephone: \n");
                gets(u.tel);puts("");
            }while(verifnumber(u.tel)!=1);
            strcpy(u.statut,"actif");
            if(strcmp(u.etat,"user")==0)
            {
                strcpy(u.mdp,"passer123");
            }
            else
            {
                if(strcmp(u.etat,"admin")==0)
                {
                    printf("Entrer le mot de passe : \n");
                    gets(u.mdp);puts("");
                    for(k=0; k<strlen(u.mdp); k++) //CRYPTAGE DU MOT DE PASSE
                    {
                        if(u.mdp[k] != '\0')
                        {
                            u.mdp[k]+=45;
                        }
                    }
                }
            }
//                u.id = ++id_auto;
            u.id = idUser(nom,f)+1;
            fwrite(&u, sizeof(u), 1, f);
            printf("\n\t\t\t\t\t\t\t\t  ENREGISTREMENT EFFECTUE !\n");
            fclose(f);
        }
    return f;
}

void listeUser(char nom[],FILE *f)
{
    f = fopen(nom,"rb");
    USER u;
    int k;
    printf("\t\t\t\t\t==========================1-LISTE DES UTILISATEURS==================================\n");
    if(f)
    {
        while(fread(&u, sizeof(u), 1, f)!= 0)
        {
            {
                printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
                printf("ID : %d\n",u.id);
                printf("Statut : %s\n",u.etat);
                printf("Nom : %s\n",u.nom);
                printf("Prenom(s) : %s\n",u.prenom);
                printf("Telephone : %s\n",u.tel);
                printf("Login : %s\n",u.login);
                for(k=0; k<strlen(u.mdp); k++)
                {
                    if(u.mdp[k] != '\0')
                    {
                        u.mdp[k]-=45; //DECRYPTAGE DU MOT DE PASSE
                    }
                }
                printf("Mot de passe : %s\n",u.mdp);
                printf("Etat : %s\n",u.statut);
            }
        }
        fclose(f);
    }
}

int idUser(char nom[], FILE *f)  //OUVERTURE DU FICHIER BINAIRE EN MODE LECTURE, PUIS DECOMPTE ET RETOUR DU NOMBRE D'UTILISATEUR
{
    f = fopen(nom,"rb");
    USER u;
    int cpt=0;
    if(f)
    {
        while(fread(&u, sizeof(u), 1, f) != 0)
        {
            cpt++;
        }
        fclose(f);
    }
    return cpt;
}

FILE* blockUser(char login[],char nomf[], FILE *f)
//ON OUVRE LE FICHIER EM MODE LECTURE ECRITURE ON CHERCHE LE LOGIN DANS LE FICHIER, SI ON LE TROUVE LE LOGIN ET QUE CE NEST PAS UN ADMIN
//ON MET LE STATUT DE LUTILISATEUR A BLOQUE ET ON REECRIT LENREGISTREMENT DANS LE FICHIER ET ENSUITE ON FAIT UN BREAK POUR QUITTER UNE
//FOIS LA MODIFICATION FAITE, SI ON NE TROUVE PAS LE LOGIN DANS LE FICHIER OU SI CEST UN ADMIN LE COMPTEUR EST A 0, ET A LA SORTIE DU PARCOURS DU FICHIER
//ON LUI AFFICHE QUE LUTILISATEUR EST INEXISTANT SI cpt=0 OU SI cpt=1 LE BLOCAGE A ETE UN SUCCES.
{
    f = fopen(nomf,"rb+");
    USER u;
    F
    printf("\t\t\t\t\t==========================2-BLOCAGE D'UTILISATEUR(S)================================\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    if(f)
    {
        if(searchLogin(login, nomf, f)==1)
        {
            rewind(f);
            while(fread(&u, sizeof(u), 1, f) !=0)
            {
                if(strcmp(u.login, login) == 0 && strcmp(u.etat,"user")==0)
                {
                    strcpy(u.statut,"bloque");
                    fseek(f, -sizeof(u), 1);
                    fwrite(&u, sizeof(u), 1, f);
                    break;
                }
            }
            printf("\n\t\t\t\t\t\t\t\tUtilisateur bloque avec succes ! \n");
        }
        else
        {
            printf("\n\t\t\t\t\t\t\t\tUtilisateur non existant ! \n");
        }
        fclose(f);
    }
    return f;
}

FILE* unblockUser(char login[],char nomf[], FILE *f)
//ON OUVRE LE FICHIER EM MODE LECTURE ECRITURE ON CHERCHE LE LOGIN DANS LE FICHIER, SI ON LE TROUVE ET QUE CE NEST PAS UN ADMIN
//LE COMPTEUR EST A 1 ON MET LE STATUT DE LUTILISATEUR A DEBLOQUE ET ON REECRIT LENREGISTREMENT DANS LE FICHIER ET ENSUITE ON FAIT UN BREAK POUR QUITTER UNE
//FOIS LA MODIFICATION FAITE, SI ON NE TROUVE PAS LE LOGIN DANS LE FICHIER OU SI CEST UN ADMIN LE COMPTEUR EST A 0, ET A LA SORTIE DU PARCOURS DU FICHIER
//ON LUI AFFICHE QUE LUTILISATEUR EST INEXISTANT SI cpt=0 OU SI cpt=1 LE DEBLOCAGE A ETE UN SUCCES.
{
    f = fopen(nomf,"rb+");
    USER u;
    F
    printf("\t\t\t\t\t==========================3-BLOCAGE D'UTILISATEUR(S)================================\n");
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    if(f)
    {
        if(searchLogin(login, nomf, f)==1)
        {
            rewind(f);
            while(fread(&u, sizeof(u), 1, f) !=0)
            {
                if(strcmp(u.login, login) == 0 && strcmp(u.etat,"user")==0)
                {
                    strcpy(u.statut,"actif");
                    fseek(f, -sizeof(u), 1);
                    fwrite(&u, sizeof(u), 1, f);
                    break;
                }
            }
            printf("\n\t\t\t\t\t\t\t\tUtilisateur debloque avec succes ! \n");
        }
        else
        {
            printf("\n\t\t\t\t\t\t\t\tUtilisateur non existant ! \n");
        }
        fclose(f);
    }
    return f;
}

FILE* passer123(char login[], char nom[], FILE *f) //SI CEST LA PREMIERE CONNEXION DE LUTILISATEUR IL DOIT CHANGER SON MOT DE PASSE
{
    f = fopen(nom,"rb+");
    USER u;
    int k;
    char mdp[15];
    if(f)
    {
        rewind(f);
        while(fread(&u, sizeof(u), 1, f) != 0)
        {
           if(strcmp(login,u.login)==0)
            {
                if(strcmp(u.mdp, "passer123") == 0)
                {
                    printf("CHER %s, VEUILLEZ ENTRER VOTRE NOUVEAU MOT DE PASSE : \n",u.login);
                    gets(mdp);
                    for(k=0; k<strlen(mdp); k++) //CRYPTAGE DU MOT DE PASSE
                    {
                        if(mdp[k] != '\0')
                        {
                            mdp[k]+=45;
                        }
                    }
                    strcpy(u.mdp,mdp);
                    fseek(f, -sizeof(u), SEEK_CUR); //REECRIT LE NEW PASSWORD DANS LE FICHIER
                    fwrite(&u, sizeof(u), 1, f);
                    printf("\n\t\t\t\t\t\t\t\t  MODIFICATION DU MOT DE PASSE REUSSIE !\n");
                    break;
                }
            }
        }
        fclose(f);
    }
    return f;
}

USER saveInfosUser(char login[], char nom[], FILE *f) //RECHERCHES LES INFORMATIONS DE LUSER DONT LE LOGIN EST PASSE EN PARAMETRES
{
    USER user, u;
    f = fopen(nom,"rb");
    if(f)
    {
        rewind(f);
        while(fread(&u,sizeof(u),1,f) != 0)
        {
            if(strcmp(u.login, login) == 0){
                user = u;
                break;
            }
        }
        fclose(f);
    }
    return user;
}

FILE* vente(USER u, char nom[], FILE *f, FILE *f2, FILE *f3)
{
    FILE *f4;
    VENTE v;
    ARTICLE a;
    int cpt, qte, nbrV=-1, monV=0, nbrArt=0; //SI LE NBR DE VENTE EST EGAL A 0 ALORS ON OUVRE LE FICHIER RECU EN ECRITURE SI NBV>0 ON LOUVRE EN MODE AJOUT
    char rep[3], rep2[3], nomfich[100], saveNomfich[100], code[7], etat[100];
    f = fopen(nom, "rb+");
    if(f)
    {
        do{
            do{
                F
                printf("Souhaitez-vous effectuer une vente ?\n");
                gets(rep);puts("");
                strlwr(rep);
                if(strcmp(rep,"oui")!=0 && strcmp(rep,"non")!=0)
                {
                    printf("\nVEUILLEZ REPONDRE PAR 'OUI' OU 'NON' ! \n");
                }
            }while(strcmp(rep,"oui")!=0 && strcmp(rep,"non")!=0);
            if(strcmp(rep,"oui")==0)
            {
                printf("ENTRER LE CODE DU PRODUIT :\n");
                gets(code);puts("");
                rewind(f);
                while(fread(&a, sizeof(a), 1, f) != 0)
                {
                    if(strcmp(a.code,code)==0)
                    {
                        cpt = 1;
                        do{
                            printf("ENTRER LA QUANTITE A ACHETER : \n");
                            scanf("%d",&qte);
                        }while(qte<=0);
                        if(a.qte == 0) //LA QUANTITE DE LARTICLE EST EGALE A ZERO
                        {
                            printf("\t\t\t\t\t\t\t\t\tSTOCK EPUISE !\n");
                        }
                        else
                        {
                            if(qte>a.qte) //LA DEMANDE SUPERIEURE A LOFFRE
                            {
                                do{
                                    F
                                    printf("\nVoulez-vous prendre la quantite en stock restante : %d ? \n",a.qte);
                                    gets(rep2);puts("");
                                    strlwr(rep2);
                                    if(strcmp(rep2,"oui")!=0 && strcmp(rep2,"non")!=0)
                                    {
                                        printf("\nVEUILLEZ REPONDRE PAR 'OUI' ou 'NON' ! \n");
                                    }
                                }while(strcmp(rep2,"oui")!=0 && strcmp(rep2,"non")!=0);
                                if(strcmp(rep2,"oui")==0)
                                {
                                    printf("\t\t\t\t\t\t\t\t\tACHAT VALIDE ! \n");
                                    nbrV++;
                                    strcpy(v.loginV,u.login);
                                    strcpy(v.nomV,u.nom);
                                    strcpy(v.prenomV,u.prenom);
                                    strcpy(v.telV,u.tel);
                                    strcpy(v.articleVente.cat,a.cat);
                                    strcpy(v.articleVente.code,code);
                                    strcpy(v.articleVente.nom,a.nom);
                                    v.articleVente.prix = a.prix;
                                    v.articleVente.qte = a.qte;
                                    v.heureVente = hora();
                                    v.montantVente = v.articleVente.prix * v.articleVente.qte;
                                    monV += v.montantVente;
                                    nbrArt += v.articleVente.qte;
                                    a.qte = 0;
                                    fseek(f, -sizeof(a), 1);
                                    fwrite(&a, sizeof(a), 1, f);
                                    if(nbrV==0) //PREMIERE VENTE
                                    {
                                        v.idVente = ++id_vente;
                                        sprintf(v.num, "%d%d%d%d%d%d", v.heureVente.j, v.heureVente.m, v.heureVente.a, v.heureVente.h, v.heureVente.mn, v.heureVente.s);
                                        sprintf(nomfich,"C:\\Users\\compaq\\Desktop\\COURS ISI\\LANGAGE C\\MON EXAM\\BILLS\\%s_%s_%d_%s.txt","RECU",v.num,v.idVente,v.loginV);
                                        sprintf(etat,"C:\\Users\\compaq\\Desktop\\COURS ISI\\LANGAGE C\\MON EXAM\\BILLS\\ETAT\\%s_%d%d%d.txt","ETAT_", v.heureVente.a, v.heureVente.m, v.heureVente.j);
                                        f2=fopen(nomfich,"w");
                                        if(f2)
                                        {
                                            fprintf(f2,"%d--------------------%s\n", v.idVente, v.num);
                                            fprintf(f2,"%s %s        00221%s\n", v.prenomV,v.nomV,v.telV);
                                            fprintf(f2,"%s       le %d/%d/%d a %d:%d:%d\n",  v.loginV,v.heureVente.j, v.heureVente.m, v.heureVente.a, v.heureVente.h, v.heureVente.mn, v.heureVente.s);
                                            fprintf(f2,"---------%s---------\n","LISTE DES ACHATS");
                                            fprintf(f2,"%s--------%s\n", v.articleVente.code, v.articleVente.cat);
                                            fprintf(f2,"-%s\n", v.articleVente.nom);
                                            fprintf(f2," %d FCFA * %d = %dFCFA\n", v.articleVente.prix ,v.articleVente.qte,v.montantVente);
                                            fprintf(f2,"%s","----------------------------------\n");
                                            fclose(f2);
                                        }
//
                                    }
                                    else
                                    {
                                        if(nbrV>0) //PLUS DUNE VENTE
                                        {
                                            addArticle(f3,nomfich,v.articleVente.code, v.articleVente.cat,v.articleVente.nom,v.articleVente.prix ,v.articleVente.qte,v.montantVente);
                                        }
                                    }
                                }
                                else
                                {
                                   break;
                                }
                            }
                            else //LA QUANTITE DEMANDEE EST DISPO
                            {
                                printf("\t\t\t\t\t\t\t\t\tACHAT VALIDE ! \n");
                                nbrV++;
                                strcpy(v.loginV,u.login);
                                strcpy(v.nomV,u.nom);
                                strcpy(v.prenomV,u.prenom);
                                strcpy(v.telV,u.tel);
                                strcpy(v.articleVente.cat,a.cat);
                                strcpy(v.articleVente.code,code);
                                strcpy(v.articleVente.nom,a.nom);
                                v.articleVente.prix = a.prix;
                                v.articleVente.qte = qte;
                                v.heureVente = hora();
                                v.montantVente = v.articleVente.prix * v.articleVente.qte;
                                monV += v.montantVente;
                                nbrArt += v.articleVente.qte;
                                a.qte = a.qte - qte;
                                fseek(f, -sizeof(a), 1);
                                fwrite(&a, sizeof(a), 1, f);
                                if(nbrV==0) //PREMIERE VENTE
                                {
                                    v.idVente = ++id_vente;
                                    sprintf(v.num, "%d%d%d%d%d%d", v.heureVente.j, v.heureVente.m, v.heureVente.a, v.heureVente.h, v.heureVente.mn, v.heureVente.s);
                                    sprintf(nomfich,"C:\\Users\\compaq\\Desktop\\COURS ISI\\LANGAGE C\\MON EXAM\\BILLS\\%s_%s_%d_%s.txt","RECU",v.num,v.idVente,v.loginV);
                                    sprintf(etat,"C:\\Users\\compaq\\Desktop\\COURS ISI\\LANGAGE C\\MON EXAM\\BILLS\\ETAT\\%s_%d%d%d.txt","ETAT_", v.heureVente.a, v.heureVente.m, v.heureVente.j);
                                    f2=fopen(nomfich,"w");
                                    if(f2)
                                    {
                                        fprintf(f2,"%d--------------------%s\n", v.idVente, v.num);
                                        fprintf(f2,"%s %s      00221%s\n", v.prenomV,v.nomV,v.telV);
                                        fprintf(f2,"%s      le %d/%d/%d a %d:%d:%d\n",  v.loginV,v.heureVente.j, v.heureVente.m, v.heureVente.a, v.heureVente.h, v.heureVente.mn, v.heureVente.s);
                                        fprintf(f2,"---------%s---------\n","LISTE DES ACHATS");
                                        fprintf(f2,"%s--------%s\n", v.articleVente.code, v.articleVente.cat);
                                        fprintf(f2,"-%s\n", v.articleVente.nom);
                                        fprintf(f2," %d FCFA * %d = %d FCFA\n", v.articleVente.prix ,v.articleVente.qte,v.montantVente);
                                        fprintf(f2,"%s","----------------------------------\n");
                                        fclose(f2);
                                    }
                                }
                                else
                                {
                                    if(nbrV>0) //PLUS DUNE VENTE
                                    {
                                         addArticle(f3,nomfich,v.articleVente.code, v.articleVente.cat,v.articleVente.nom,v.articleVente.prix ,v.articleVente.qte,v.montantVente);
                                    }
                                }
                            }
                        }
                        break;
                    }
                    else
                    {
                        cpt =  0;
                    }
                }
                if(cpt==0)
                {
                    printf("\t\t\t\t\t\t\t\t\tCODE INEXISTANT !\n");
                }
            }
            else
            {
                addTotal(f3,nomfich,monV);
                addEtat(f4,etat,monV,nbrArt);
                break;
            }
        }while(1);
        fclose(f);
    }
    return f;
}

FILE* addArticle(FILE *f, char nom[], char code[], char cat[], char nomV[], int prix, int qte, int monV)
{
    f = fopen(nom, "a");
    if(f)
    {
        fprintf(f,"%s--------%s\n", code, cat);
        fprintf(f,"-%s\n",nomV);
        fprintf(f," %d FCFA * %d = %d FCFA\n", prix ,qte,monV);
        fprintf(f,"%s","----------------------------------\n");
        fclose(f);
    }
    return f;
}
FILE* addTotal(FILE *f, char nom[], int monT)
{
    f = fopen(nom, "a");
    if(f)
    {
        fprintf(f,"\tTOTAL :  %d FCFA\n", monT);
        fprintf(f,"%s","----------------------------------\n");
        fclose(f);
    }
    return f;
}

FILE* addEtat(FILE *f, char nom[], int monT, int nbrV)
{
    ETAT e;
    f = fopen(nom, "a");
    e.montV = monT;
    e.nbrV = nbrV;
    if(f)
    {
        fprintf(f,"%d articles ---> %d FCFA\n", e.nbrV, e.montV);
        fclose(f);
    }
    return f;
}
//void afficheEtat(FILE *f, char nom[])
//{
//    ETAT e;
//    f = fopen(nom,"r");
//    int m=0, n=0;
//    while(!feof(f))
//    {
//        fscanf(f,"%d articles ---> %d FCFA", e.nbrV, e.montV);
//        m+= e.nbrV;
//        n+= e.montV;
//    }
//    printf("%d %dFCFA",n,m);
//}
