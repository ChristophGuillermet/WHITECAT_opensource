//FICHIER D EXAMPLE. A VOUS DE CHANGER LES NOMS DE MY_WINDOW .... 
//my_window est un exemple.

bool index_my_window=0; //pour vos fenetres vous cr�erez un autre index_bool�en

bool my_window_variable_boolean=0;
int my_window_fader_value[6];

// c est dans ce fichier que vous mettrez les variables globales n�cessaires � vos modules

///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////CREER UNE FENETRE C EST LE PLUS .... NEBULEUX ////////////////////////////////////
////////////////////PETIT PAS A PAS EN MODE TUNNEL/////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

/*Dans un premier temps vous pouvez directement modifier My_window_file_sample.cpp . 
C est un gabarit actif dans whitecat. Passer � ce moment directement au point 5 pour essayer. 
Par contre vous aurez besoin de transform my_window en mon_module, ou d'ajouter une fen�tre, et l� il faudra passer
par la longue manip des points 1 � 4
*/


/////////////////////CREER UNE FENETRE VIERGE/////////////////////////////////////////////////////

/*
1. DEFINIR UNE NOUVELLE ID DE FENETRE

Chaque fen�tre a une ID, d�fin�e dans whitecat.h  dans enum WINDOW{} . 
L'ID de My_Window est 927.

Si vous rajoutez plus d'une fenetre, modifier int max_window_identity_is=927; et dire quelle est l'iD max.

2. CREER L OUVERTURE FERMETURE DE FENETRE PAR L INDEX BOOLEEN. MODIFIER LES FONCTIONS
D OUVERTURE FERMETURES DE FENETRES 

Il y a un index bool�en d ouverture ou de fermeture de la fenetre pour activer la partie affichage,
et la partie logique (clicks de souris). Ici: index_my_window

Ouverture de fen�tre ( et mettre des choses logiques si besoin  d'initialisation des donn�es par ex ) 
dans gestionnaire_fenetres2.cpp fonction add_a_window(int id) index_my_window=1

Fermeture de fenetre dans CORE_6.cpp > fonction close_all_windows(): index_my_window=0 

toujours dans CORE_6.cpp ajouter dans les switch case de 
write_window_indexes_from_list_of_windows()  le case W_MY_WINDOW
et dans substract_a_window(int id) le case W_MY_WINDOW


3. CREER UN BOUTON D'APPEL DANS LE MENU

Le menu est constitu� de 2 boucles enchass�es l'une dans l'autre: colonnes et lignes.
Si vous rajoutez un autre onglet, il faudra changer la boucle de la colonne correspondante  
dans les fonctions l'utilisant ( logique , et affichage:  la logique est s�par�e de l'affichage pour la performance ).
for(int cl=0;cl<5;cl++) deviendra for(int cl=0;cl<6;cl++)

dans Call_everybody_5.cpp > fonctions do_logical_menus pour le click souris. 
L'identifiant midi est mis � 2047 mais il faudra lui attribuer une r�f�rence unique. il faut regarder ce 
qui n est pas attribu� dans midi_13.cpp  > fonction do_midi_call_order(int control) ou control est l 'identifiant 
de la commande.
voir la liste affectation_midi.txt dans les sources

tout le travail de logique au click est fait dans la fonction command_button_logical dans gui_boutons_rebuild1.cpp
le case 15: est my window. c est la boucle des boutons colonne 1= 0 colonne 2= dizaine colonne 3= vingtaine
case 15: 5�me bouton de la deuxieme colonne

l'affichage des boutons du menu est d�fini dans Call_everybody_5.cpp: fonction int Menus(
il faudra aller modifier la bonne colonne du for(int cl=0;cl<6;cl++)

toujours dans gui_boutons_rebuild1.cpp les affichages des boutons : c est dans la fonction  command_button_view
par contre (shame on me) c est l'id midi qui est prise en compte comme identifiant. car je checkais des cas sp�cifiques.
(les commandes du haut notamment)

noter que dans Menus et dans do_logical_menus stae est notre bool�en index_my_window

Si vous avez besoin d'agrandir la fen�tre des menus, notamment en hauteur, vous trouverez dans whitecat.h
int size_x_mainmenu=365;
int size_y_mainmenu=200;


/////////////////////DESSINER LA FENETRE VIERGE////////////////////////////////////////

Ok, on arrive au bout de la purge.

On va avoir besoin de variables globales pour avoir la position de la fenetre et sa taille:
d�clarer dans le fichier d'include:
*/

//la position
int my_window_x=100;//pixels
int my_window_y=100;
//la taille
int largeur_my_window=400;//pixels
int hauteur_my_window=250;

/*
4. MODIFIER LES FICHIERS DEDIEZ  AU TRAITEMENT DE LA LOGIQUE et A L AFFICHAGE:
            
 fichier proc_visuels_rebuild1.cpp  
 > fonction detection_over_window()       
     
 > fonction move_window(int idwindowis)
 case W_MY_WINDOW:
     do_logical_my_window_Box(my_window_x, my_window_y);     
     do_logical_MoveCloseBox( my_window_x+20,my_window_y+20,W_MY_WINDOW);      
 break;

 dans  check_graphics_mouse_handling() se passe tous les appels au click souris dans votre fen�tre.
 regarder et modifier 
 case W_MY_WINDOW:
     do_logical_my_window_Box(my_window_x, my_window_y);     //ca c est votre fonction pour definir ce qui se passe au click souris
     do_logical_MoveCloseBox( my_window_x+20,my_window_y+20,W_GRID);      //ca c est la fonction de fermeture bougeage de fen�tre
 break;
 
 Vous verrez que dans les sources de whitecat il ya un fichier _visu et un fichier _core. 
 Pour faire simple les fonctions appel�es vont aller dans votre fichier my_window_sample.cpp
 
ENFIN dans graphic_rebuild1, fonction Boxes , c est l� ou est fait l'affichage

        case W_MY_WINDOW:
        MyWindow_Box(my_window_x, my_window_y);
        MoveCloseBox( my_window_x+20,my_window_y+20,W_MY_WINDOW);              
        break;
        
5. ENFIN ECRIRE VOTRE FENETRE dans my_window_file_sample.cpp

Ok. Comme s�parer la logique de l'affichage c est un peu tordu, vous pouvez aussi ne rien mettre dans do_logical_my_window_Box(int mw_x, int mw_y)
et �crire logique et visu d'abord dans my_window_Box(int mw_x, int mw_y) puis nettoyer, en ne laissant que la logique dans la logique, et la visu dans la visu

vous pouvez aller dans  my_window_file_sample.cpp
la doc d open layer (la lib graphique opengl) est dans le dossier borderless06 dans /sources

banger_visu et banger_core sont des bons exemples d affichage, logique
faders_visu faders_core aussi
gridplayer aussi
*/
