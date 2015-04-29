/**
 *
 * @file   Projet Puissance 4
 *
 * @author Carbonne Loic, Martinez Lucas, Soumille Lucas, Wident Cedric
 *
 * @date   21/12/12
 *
 * @brief  Puissance 4
 *
**/
#include <string>
#include <iostream>
#include <vector>
#include <iomanip> // setw()
#include <ctime> // clock()
#include <cctype> // toupper()
#include <cstdlib> // rand()

using namespace std;
#define classdef typedef

namespace
{
    typedef vector <bool> CVBool;
    typedef vector <char> CVLigne;
    typedef vector <CVLigne> CVMatrice;
    typedef vector <unsigned> CVUInt;
    const string KReset   ("0");
    const string KNoir    ("30");
    const string KRouge   ("31");
    const string KVert    ("32");
    const string KJaune   ("33");
    const string KBleu    ("34");
    const string KMAgenta ("35");
    const string KCyan    ("36");
    const char KJoueur1 ('X');
    const char KJoueur2 ('O');
    const char KVide ('.');
    const string KNomsCol ("ABCDEFG");
    const float KVitesse(0.07);

    void Attendre(float temps)
    {
        clock_t FinAttente = clock() + (temps*CLOCKS_PER_SEC);

        while(clock() < FinAttente);
    }//Attendre

    void Couleur (const string & coul)
    {
        cout << "\033[" << coul.c_str () << "m";
    }//Couleur	

    void ClearScreen ()
    {
        cout << "\033[H\033[2J";
    }//ClearScreen

    void AfficheLigne (const CVLigne Li)
    {
        for(unsigned i(0); i < Li.size() ; ++i)
        {
            if (Li[i] == KJoueur1) Couleur(KBleu);
            if (Li[i] == KJoueur2) Couleur(KRouge);
            cout << setw(3) << Li[i];
            Couleur(KReset);
        }
    }//AfficheLigne

    void AfficheMatrice (const CVMatrice Mat)
    {
        ClearScreen ();
        Couleur (KReset);
        for(unsigned i(0) ; i < Mat.size() ; ++i)
        {
            AfficheLigne(Mat[i]);
            cout << endl;
        }
    }//AfficheMatrice

    void AffichePuissance4 (const CVMatrice Mat)
    {
        AfficheMatrice(Mat);
        cout << "-----------------------" << endl;
        for (unsigned i(0) ; i < 7 ; ++i)
        {
            cout << setw(3) << KNomsCol[i];
        }
        cout << endl;
    }//AffichePuissance4


    void InitMat (CVMatrice & Mat)
    {
        Mat = CVMatrice(7, CVLigne(7, KVide));
    }//InitMat

    void PositionneJeton (CVMatrice & Mat, const unsigned NumCol, 
                          unsigned & NumLi, const bool Joueur1, bool Visuel)
    {
        char Jeton;
        if (Joueur1)
        {
            Jeton = KJoueur1;
        }
        else
        {
            Jeton = KJoueur2;
        }
        for(NumLi = 0 ; NumLi < 7 ; ++NumLi)
        {
            if (Mat[NumLi][NumCol] != KVide) break;
            if (Visuel == true)
            {
                Mat[NumLi][NumCol] = Jeton;
                AfficheMatrice(Mat);
                Attendre(KVitesse);
                Mat[NumLi][NumCol] = KVide;
            }
        }
        --NumLi;
        Mat[NumLi][NumCol] = Jeton;
    }//PositionneJeton

    bool TestVictoireColonne (const CVMatrice Mat, const unsigned NumLi,
                              const unsigned NumCol)
    {
        if (NumLi < 4)
        {
            unsigned i (NumLi + 1);
            while (i < (NumLi + 4) && Mat[i][NumCol] == Mat[NumLi][NumCol])
            {
                ++i;
            }
            if ( i == NumLi + 4 ) return true;
        }
        return false;
    }//TestVictoireColonne

    bool TestVictoireLigne (const CVMatrice Mat, const unsigned NumLi,
                            const unsigned NumCol)
    {
        unsigned BorneInf(NumCol);
        unsigned BorneSup(NumCol);
        while (BorneInf > 0 && Mat[NumLi][BorneInf-1] == Mat[NumLi][NumCol])
        {
        	--BorneInf;
        }
        while (BorneSup < 6 && Mat[NumLi][BorneSup+1] == Mat[NumLi][NumCol])
        {
            ++BorneSup;
        }
        if (BorneSup - BorneInf >= 3) return true;
        return false;
    }//TestVictoireLigne

    bool TestVictoireDiagonale (const CVMatrice Mat, const unsigned NumLi,
                            	const unsigned NumCol)
    {
        // Diagonale 1
        unsigned BorneInf(NumCol);
        unsigned BorneSup(NumCol);
        unsigned JInf(NumLi);
        unsigned JSup(NumLi);
        while (BorneInf > 0 && JInf > 0 
               && Mat[JInf-1][BorneInf-1] == Mat[NumLi][NumCol])
        {
            --BorneInf;
            --JInf;
        }
        while (BorneSup < 6 && JSup < 6 && Mat[JSup+1][BorneSup+1] == Mat[NumLi][NumCol])
        {
            ++BorneSup;
            ++JSup;
        }
        if ( BorneSup - BorneInf >= 3 ) return true;
        // Diagonale 2
        BorneInf = NumCol;
        BorneSup = NumCol;
        JInf = NumLi;
        JSup = NumLi;
        while (BorneInf > 0 && JInf < 6 
               && Mat[JInf+1][BorneInf-1] == Mat[NumLi][NumCol])
        {
            --BorneInf;
            ++JInf;
        }
        while (BorneSup < 6 && JSup > 0 && Mat[JSup-1][BorneSup+1] == Mat[NumLi][NumCol])
        {
            ++BorneSup;
            --JSup;
        }
        if ( BorneSup - BorneInf >= 3 ) return true;
        return false;
    }//TestVictoireDiagonale

    unsigned Alentours (CVMatrice Mat,unsigned NumCol)
    {
        unsigned NbAlentours(0);
        unsigned Ligne;
        PositionneJeton(Mat,NumCol,Ligne,false,false);
        Mat[Ligne][NumCol] = KVide;
        CVBool Alentours (7,true);
        if ( NumCol == 0 )
        {
            Alentours[0]=false;
            Alentours[1]=false;
            Alentours[2]=false;
        }
        if ( NumCol == 6 )
        {
            Alentours[4]=false;
            Alentours[5]=false;
            Alentours[6]=false;
        }
        if ( Ligne == 6 )
        {
            Alentours[2]=false;
            Alentours[3]=false;
            Alentours[4]=false;
        }
        if ( Ligne == 0 )
        {
            Alentours[0]=false;
            Alentours[6]=false;
        }

        if ( Alentours[0] == true && Mat[Ligne - 1][NumCol - 1] == KJoueur2 ) ++NbAlentours;
        if ( Alentours[1] == true && Mat[Ligne][NumCol - 1] == KJoueur2 ) ++NbAlentours;
        if ( Alentours[2] == true && Mat[Ligne + 1][NumCol - 1] == KJoueur2 ) ++NbAlentours;
        if ( Alentours[3] == true && Mat[Ligne + 1][NumCol] == KJoueur2 ) ++NbAlentours;
        if ( Alentours[4] == true && Mat[Ligne + 1][NumCol + 1] == KJoueur2 ) ++NbAlentours;
        if ( Alentours[5] == true && Mat[Ligne][NumCol + 1] == KJoueur2 ) ++NbAlentours;
        if ( Alentours[6] == true && Mat[Ligne - 1][NumCol + 1] == KJoueur2 ) ++NbAlentours;
        return NbAlentours;

    } //Alentours

    unsigned Bot(CVMatrice Mat, CVUInt & TabCol)
    {
		// Etape 1 Chercher ou il faut obligatoirement jouer

        bool VictoireAnticipe(false);
        unsigned LiAnticipe;
        unsigned i (0);
        while(i < TabCol.size())
        {
            // Pour gagner
            PositionneJeton(Mat,TabCol[i],LiAnticipe,false,false);
            VictoireAnticipe += TestVictoireColonne (Mat, LiAnticipe, 
						                             TabCol[i]);
            VictoireAnticipe += TestVictoireLigne (Mat, LiAnticipe, 
						                           TabCol[i]);
            VictoireAnticipe += TestVictoireDiagonale (Mat, LiAnticipe, 
						                               TabCol[i]);
            if ( VictoireAnticipe == true ) return TabCol[i];
            Mat[LiAnticipe][TabCol[i]] = KVide;
            ++i;
        }
        i = 0;
        while(i < TabCol.size())
        {
            // Pour empecher la victoire de joueur 1
            PositionneJeton(Mat,TabCol[i],LiAnticipe,true,false);
            VictoireAnticipe += TestVictoireColonne (Mat, LiAnticipe, 
						                             TabCol[i]);
            VictoireAnticipe += TestVictoireLigne (Mat, LiAnticipe, 
						                           TabCol[i]);
            VictoireAnticipe += TestVictoireDiagonale (Mat, LiAnticipe, 
						                               TabCol[i]);
            if ( VictoireAnticipe == true ) return TabCol[i];
            Mat[LiAnticipe][TabCol[i]] = KVide;
            ++i;
        }
        
        // Etape 2 Chercher ou ne pas jouer (si lorsque l'on joue, 
	    //				                    joueur 1 gagne)
        CVUInt TabBotTmp(0);        
        i = 0;
        while(i < TabCol.size())
        {
            VictoireAnticipe = false;
            PositionneJeton(Mat,TabCol[i],LiAnticipe,false,false);
            if (LiAnticipe > 0)
            {
                PositionneJeton(Mat,TabCol[i],LiAnticipe,true,false);
                VictoireAnticipe += TestVictoireColonne (Mat, LiAnticipe, 
							                             TabCol[i]);
                VictoireAnticipe += TestVictoireLigne (Mat, LiAnticipe, 
						                               TabCol[i]);
                VictoireAnticipe += TestVictoireDiagonale (Mat, LiAnticipe, 
							                               TabCol[i]);
                Mat[LiAnticipe][TabCol[i]] = KVide;
                Mat[LiAnticipe+1][TabCol[i]] = KVide;
            }
            else
            {
                Mat[LiAnticipe][TabCol[i]] = KVide;
            }
            if ( VictoireAnticipe == false ) TabBotTmp.push_back(TabCol[i]);
            ++i;
        }
        
        // Etape 3 Chercher ou jouer
        if ( TabBotTmp.size() == 0 ) return TabCol[0];
        if ( TabBotTmp.size() == 1 ) return TabBotTmp[0];
        CVUInt TabStats(0);
        for(unsigned j(0) ; j < TabBotTmp.size() ; ++j)
        {
            TabStats.push_back(Alentours(Mat,TabBotTmp[j]));
        }
        unsigned ValMax(0);
        CVUInt TabBotFinal(0);
        for(unsigned j(0) ; j < TabStats.size() ; ++j)
        {
            if ( TabStats[j] > ValMax )
            {
            	ValMax = TabStats[j];
            }
        }
        for(unsigned j(0) ; j < TabStats.size() ; ++j)
        {
            if ( TabStats[j] == ValMax )
            { 
            	TabBotFinal.push_back(TabBotTmp[j]);
            }
        }


        int Tirage;
        Tirage = rand() % TabBotFinal.size();


        return TabBotFinal[Tirage];

    } //Bot

    bool SaisirChar(char &Char)
        {
            string StrTemp;

            for( ; ; )
            {
                cin >> StrTemp;
                if(cin.fail() || StrTemp.length() != 1)
                {
                    cout << "Saisie incorrecte, veuillez rejouer :" << endl;
                    cin.clear();
                    continue;
                }
                cin.clear();
                Char = StrTemp.at(0); // Transforme StrTemp en char
                return true;
            }
        return false;
        } // SaisirChar

    int ppal ()
    {
        srand ( time(NULL) );
        bool Jouer (true);
        bool Victoire(false);
        bool Joueur1(true);
        unsigned DemandeBot;
        unsigned PtsJoueur1(0);
        unsigned PtsJoueur2(0);
        string NomJoueur2("2");
        ClearScreen ();
        cout << "  ____        _                                "
             << "_  _   " << endl;
        cout << " |  _ \\ _   _(_)___ ___  __ _ _ __   "
             << "___ ___  | || |  " << endl;
        cout << " | |_) | | | | / __/ __|/ _` | '_ \\ "
             << "/ __/ _ \\ | || |_ " << endl;
        cout << " |  __/| |_| | \\__ \\__ \\ (_| | | | | "
             << "(_|  __/ |__   _|" << endl;
        cout << " |_|    \\__,_|_|___/___/\\__,_|_| |_|\\___\\_"
             << "__|    |_|  " << "\n\n";
        cout << "Bonjour, contre qui desirez vous jouer ?" << endl;
        Couleur(KCyan);
        cout << "Tapez 0 pour jouer contre un ami" << endl;
        Couleur(KJaune);
        cout << "Tapez 1 pour jouer contre l'ordinateur" << endl;
        Couleur(KReset);
        for ( ; ; )
        {
        	cin >> DemandeBot;
        	if ( DemandeBot == 0 ) break;
        	if ( DemandeBot == 1 )
        	{
        		NomJoueur2 = "Ordinateur";
        		break;
        	}
        	else 
        	{
        		cout << "Vous avez fait une erreur, recommencez "<<endl;
        		cin.clear();
        	}
        }
        for ( ; ; )
        {
            CVUInt TabCol;
    		for (unsigned i(0) ; i < 7 ; ++i)
            {
                TabCol.push_back(i);
            }
            string Joueur;
            char NomCol;
            unsigned NumLi;
            CVMatrice Mat;
            InitMat(Mat);
            AffichePuissance4(Mat);
            unsigned CoupsJ = 1;
            for( ; Jouer == true ; ++CoupsJ)
            {
                if (Joueur1 == true)
                {
                    Joueur = "1";
                }
                else
                {
                    Joueur = NomJoueur2;
                }
                cout << "C'est au joueur " << Joueur
                     << " de jouer :" << endl;
                unsigned NumCol(0);
                if ( DemandeBot == 0  | Joueur1 == true)
                {
                    for( ; ; )
                    {
                        SaisirChar(NomCol);
                        NomCol = toupper(NomCol);
                        while ( NumCol < 6 )
                        {
                            if( KNomsCol[NumCol] == NomCol ) break;
                            ++NumCol;
                        }
                        bool ExistenceCol(false);
                        for ( unsigned i(0) ; i < TabCol.size() ; ++i )
                        {
                            if ( TabCol[i] == NumCol ) ExistenceCol = true;
                        }
                        if ( ExistenceCol == true && NomCol <= 'G') break;
                        cout << "Colonne inexistante ou pleine"
                             << ", veuillez rejouer :" << endl;
                        NumCol = 0;
                    }
                }
                else
                {
                    NumCol = Bot(Mat, TabCol);
                }
                PositionneJeton(Mat,NumCol,NumLi,Joueur1,true);
                AffichePuissance4(Mat);
                Victoire += TestVictoireColonne (Mat, NumLi, NumCol);
                Victoire += TestVictoireLigne (Mat, NumLi, NumCol);
                Victoire += TestVictoireDiagonale (Mat, NumLi, NumCol);
                if ((CoupsJ == 49) | (Victoire == true)) break;
                Joueur1 = !Joueur1;
                //Supprimer de TabCol les colonnes pleines si on a jouer tout en haut
                if ( NumLi == 0 )
                {
                    unsigned NbSuppressions(0);
                    for(unsigned i(0) ; i < TabCol.size() ; ++i)
                    {
                        if ( Mat[0][TabCol[i]] != KVide )
                        {
                            for (unsigned j(i) ; j < TabCol.size()-1 ; ++j)
                            {
                                TabCol[j] = TabCol[j+1];
                            }
                            ++NbSuppressions;
                        }
                    }
                    for (unsigned i(0) ; i < NbSuppressions ; ++i)
                    {
                        TabCol.pop_back();
                    }
                }
            }
            Victoire = false;
            Joueur1 = !Joueur1;
            if (CoupsJ == 49)
            {
                Couleur(KVert);
                cout << "Match nul !" << endl;
                Couleur(KReset);
            }
            else
            {
                if ( Joueur == "1" )
                {
                	++PtsJoueur1;
                }
                else ++PtsJoueur2;
                {
                	Couleur(KMAgenta);
                	cout << "Le Joueur " << Joueur << " a gagné\n\n" ;
                	cout << "Scores :\n Joueur 1 : " << PtsJoueur1
                 		 << "\n Joueur " << NomJoueur2 << " : " << PtsJoueur2 << endl;
                 }
            }




            char rejouer;
            cout << "Voulez vous rejouer ? appuyer sur 'y'" << endl;
            cin >> rejouer;
            Couleur(KReset);
            if ( rejouer != 'y' ) break;
        }
        cout << "Merci d'avoir joué !" << endl;
        return 0;
    } //ppal

} // namespace 
int main ()
{
    ppal();
    return 0;
} // main()
		
