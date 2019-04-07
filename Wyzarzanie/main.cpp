#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <random>
#include <ctime>
#include <math.h>
#include <time.h>


using namespace std;
int il_maszyn;
int il_zadan;
class Zadania {

private:
    int nr_zadania;
    int nr_maszyny;
    int czas_wykonania;
    
public:
    Zadania(int nr_zadania, int nr_maszyny, int czas_wykonania )
    {
        this->nr_zadania = nr_zadania;
        this->nr_maszyny = nr_maszyny;
        this->czas_wykonania = czas_wykonania;
        
    }


    int wez_zadanie() { return nr_zadania; }
    int wez_maszyne() { return nr_maszyny; }
    int wez_czas() { return czas_wykonania; }
    int ustaw_zadanie(int nr_zadania) { this->nr_zadania = nr_zadania; }
    int ustaw_czas(int czas_wykonania) { this->czas_wykonania = czas_wykonania; }
    void pokaz_zadanie() {
        cout << "Nr zadania: " << nr_zadania
             << " czas wykonania: " << czas_wykonania
             << " nr maszyny: " << nr_maszyny<< endl;
    }
};


vector<Zadania> zaladuj_zadania() {
    int  k;


    vector<Zadania> kontener_zadan;
    ifstream plik;
    plik.open("dane.txt");
    plik >> il_zadan >> il_maszyn;

    //Dla kazdego wiersza (zadanie)
    for (int i = 0; i < il_zadan; i++)
    {
        //Dla kazdej kolumny (maszyny)
        for (int j = 0; j < il_maszyn; ++j)
        {
            plik >> k;											//Wczytuj czas wykonywania sie zadania
            kontener_zadan.push_back(Zadania(i + 1, j + 1, k)); //I odpowiednio zappisz wszystkie wartosci
        }
    }
    plik.close();

    return kontener_zadan;
}


vector<vector<Zadania>> podziel_na_maszyny(vector<Zadania> kontener){
    vector<vector<Zadania>> podzielone(il_maszyn);

    for(int j=0;j<il_maszyn;j++) {
        for ( int i = 0 ; i < kontener.size() ; i ++ ) {
            if ( kontener[ i ].wez_maszyne() == j+1 )
                podzielone[j].push_back(kontener[i]);
        }

    }

    return  podzielone;
}


void wypisz_maszyny(vector<vector<Zadania>> maszyna){
    for(int i=0; i<maszyna.size();i++) {
        cout<<endl<<"MASZYNA NR:"<<i+1<<endl;
        for ( int j = 0 ; j < maszyna[ i ].size() ; j ++ ) {
            maszyna[ i ][ j ].pokaz_zadanie();
        }

    }
}


int policz_Cmax(vector<vector<Zadania>> maszyna, vector<int>kolejnosc){
    vector<int> czas_maszyny(maszyna.size(),0);




    // Ustawiamy w maszynach odpowiednia kolejnosc zadan
    for(int i=0; i<maszyna.size();i++) {

        for ( int j = 0 ; j < kolejnosc.size() ; j ++ ) {
            for ( int k = 0 ; k < maszyna[i].size() ; k ++ ) {
                if ( maszyna[ i ][k].wez_zadanie() == kolejnosc[j] )
                {
                    Zadania tmp=maszyna[i][k];
                    maszyna[i].erase(maszyna[i].begin()+k);
                    maszyna[i].insert(maszyna[i].end(),tmp);
                }
            }
        }
    }


    //Obliczamy Cmax

    for(int j=0;j<maszyna[0].size();j++) {

        czas_maszyny[0]+=maszyna[0][j].wez_czas();
        if(j==0) {
            for ( int k = 1 ; k < maszyna.size() ; k ++ ) {
                if ( j == 0 ) {
                    czas_maszyny[ k ] += czas_maszyny[ k - 1 ] + maszyna[ k ][ j ].wez_czas();
                }
            }
        }

        for(int h=0;h<maszyna.size()-1;h++) {
            if ( j != 0 && czas_maszyny[h] <= czas_maszyny[h+1] ) {
                czas_maszyny[h+1] = czas_maszyny[h+1] + maszyna[ h + 1 ][ j ].wez_czas();
            } else if ( j != 0 && czas_maszyny[h] > czas_maszyny[h+1]) {
                czas_maszyny[h+1] += (czas_maszyny[h] - czas_maszyny[h+1]) + maszyna[h+1][j].wez_czas();
            }
        }
    }


/*
    for(int i=0; i<czas_maszyny.size();i++){
        cout<<"CZAS NA MASZYNIE NR: "<<i+1<<" = "<<czas_maszyny[i]<<" "<<endl;
    }
*/


    // wypisz_maszyny(maszyna);
    return czas_maszyny[maszyna.size()-1];
}

void wypisz_zadanka(vector<Zadania> kontener){
    for(auto it: kontener){
        it.pokaz_zadanie();
        cout<<endl;
    }
}
int losuj_liczbe(){
    int liczba = (rand()%il_zadan);
    return liczba;
}

void insert(vector<int> &vec,int elem,int miejsce){
    int tmp=vec[elem];
    vec.erase(vec.begin()+elem);
    vec.insert(vec.begin()+miejsce,tmp);

}


void wyzarzanie(vector<Zadania> kontener){
srand(time(NULL));
double u=0.80;
double T=100;
int liczba1=0,liczba2=0;
int aktualny_wynik, nowy_wynik;
kontener=zaladuj_zadania();
vector<int> aktualna_kolejnosc(il_zadan);
for(int i=0; i<il_zadan;i++){
    aktualna_kolejnosc[i]=i+1;
}

/* INICJALIZACJA */
//LOSOWANIE POCZATKOWE
random_device rd;
mt19937 g(rd());
shuffle(aktualna_kolejnosc.begin(),aktualna_kolejnosc.end(),g);
aktualny_wynik = policz_Cmax(podziel_na_maszyny(kontener) , aktualna_kolejnosc);
vector<int> nowa_kolejnosc = aktualna_kolejnosc;
/*/////END//////*/

double e;
double ra;

for(int i=0; i< 40;i++) {

    while(liczba1==liczba2) {
        liczba1 = losuj_liczbe();
        liczba2 = losuj_liczbe();
    }



	/* Wybór ruchu */
    //swap(nowa_kolejnosc[liczba1],nowa_kolejnosc[liczba2]);

    insert(nowa_kolejnosc,liczba1,liczba2);




	/* PRAWDOPODOBIENSTWO */
    nowy_wynik=policz_Cmax(podziel_na_maszyny(kontener),nowa_kolejnosc);
    if(nowy_wynik<aktualny_wynik)
    {
        aktualny_wynik=nowy_wynik;
        aktualna_kolejnosc=nowa_kolejnosc;

    }
    else{
        e=exp((aktualny_wynik-nowy_wynik)/T);
        ra=(rand()%1000);
        ra=ra/1000;
        if(e>ra) {
            aktualny_wynik = nowy_wynik;
            aktualna_kolejnosc = nowa_kolejnosc;

        }

    }







    liczba1=liczba2;
    // ZMNIEJSZANIE TEMPERATURY

    /*cout<<"Po drodze wynik to: "<<aktualny_wynik<<" Dla T= "<<T<<" Dla kolejnosci: ";
    for(auto it: aktualna_kolejnosc)
        cout<<it<<" ";
    cout<<endl;*/
    T=u*T;
}
cout<<aktualny_wynik<<endl;
/*cout<<"Top wynik to: "<<aktualny_wynik<<" Dla kolejnosci: ";
for(auto it: aktualna_kolejnosc)
    cout<<it<<" ";*/

}





int main() {
    vector<Zadania> vec;
    clock_t zegar;
    double czas_trwania;

    for(int i=0;i<100;i++) {
        zegar = clock();
        wyzarzanie(vec);
        czas_trwania = (clock() - zegar) / (double) CLOCKS_PER_SEC;
       // cout <<  czas_trwania << endl;

    }

    return 0;
}

