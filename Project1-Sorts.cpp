#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>


using namespace std;

//MERGE SORT
template<typename Typ>
void merge(Typ *tab, Typ *temp, int l, int m, int r);

template <typename Typ>
void merge_sort(Typ *tab, Typ *temp, int l, int r);

//QUICK SORT
template <typename Typ>
int partycja(Typ *tab, int l, int r);

template <typename Typ>
void quick_sort(Typ *tab, int l, int r);

//INTRO SORT
template <typename Typ>
void insert_sort(Typ* tab, int l, int r);

template <typename Typ>
void max_heap(Typ* tab, int rozm_kopca, int ind);

template <typename Typ>
void heap_sort(Typ* tab,int l, int r);

template <typename Typ>
void intro_sort(Typ* tab, int l, int r);

//REVERSE SORT
template <typename Typ>
void reverse_sort(Typ* tab, int l, int r);

//INNE FUNKCJE POMOCNICZE
template <typename Typ>
Typ *generuj_tablice(int rozmiar);

template <typename Typ>
void usun_tablice(Typ tablica);

template <typename Typ>
void wypelnij_tablice(Typ *tablica, int rozmiar);

template <typename Typ>
void wyswietl_tablice(Typ *tablica, int rozmiar);

template <typename Typ>
void sprawdz_sort(Typ *danet, int rozmiar);

chrono::duration<double> SumaCzasu(chrono::duration<double> tab_czasow[], int ilosc);
void dostepne_procenty();
void dostepne_rozmiary();

int main()
{
  const int ilosc_tablic=100;
  char wybor, wybor_rozmiaru, wybor_st_sortowania;
  int rozmiar_tablicy;
  double granica_sort;
  bool zly_wybor;
  char czy_wyswietlic;
  srand(time(NULL));
  //auto start_t=chrono::system_clock::now();
//  auto end_t=chrono::system_clock::now();
  chrono::duration<double> czas_sort[ilosc_tablic];
  while(1)
  {
    cout << "------MENU------" << endl;
    dostepne_rozmiary();
    do {
      cout << "Wybierz rozmiar tablicy: ";
      cin >> wybor_rozmiaru;
      switch(wybor_rozmiaru)
      {
        case '1':
        rozmiar_tablicy=10000;
        zly_wybor=false;
        break;
        case '2':
        rozmiar_tablicy=50000;
        zly_wybor=false;
        break;
        case '3':
        rozmiar_tablicy=100000;
        zly_wybor=false;
        break;
        case '4':
        rozmiar_tablicy=500000;
        zly_wybor=false;
        break;
        case '5':
        rozmiar_tablicy=1000000;
        zly_wybor=false;
        break;
        case 'q':
        return 0;
        break;
        default: cout << "Rozmiar niedostepny, sprobuj ponownie" << endl;
        zly_wybor=true;
        break;
      }
    } while(zly_wybor);

    int **dane1=new int* [ilosc_tablic];
    int **dane2=new int* [ilosc_tablic];
    int **dane3=new int* [ilosc_tablic];
    for(int i=0; i<ilosc_tablic;i++)
    {
      dane1[i]=generuj_tablice<int>(rozmiar_tablicy);
      dane2[i]=generuj_tablice<int>(rozmiar_tablicy);
      dane3[i]=generuj_tablice<int>(rozmiar_tablicy);
      wypelnij_tablice(dane1[i],rozmiar_tablicy);
    }
    for(int i=0;i<ilosc_tablic;i++)
    {
      for(int j=0;j<rozmiar_tablicy;j++)
      {
      dane2[i][j]=dane1[i][j];
      dane3[i][j]=dane1[i][j];
      }
    }
    int *temp=generuj_tablice<int>(rozmiar_tablicy);
    dostepne_procenty();

    do {
      cout << "Wybor: ";
      cin >> wybor_st_sortowania;
      switch(wybor_st_sortowania)
      {
        case '1':
        zly_wybor=false;
        break;
        case '2':
        granica_sort=(rozmiar_tablicy-1)*0.250;
        for(int i=0;i<ilosc_tablic;i++)
        {
          quick_sort(dane1[i],0,granica_sort);
          quick_sort(dane2[i],0,granica_sort);
          quick_sort(dane3[i],0,granica_sort);
        }
        zly_wybor=false;
        break;
        case '3':
        granica_sort=(rozmiar_tablicy-1)*0.500;
        for(int i=0;i<ilosc_tablic;i++)
        {
          quick_sort(dane1[i],0,granica_sort);
          quick_sort(dane2[i],0,granica_sort);
          quick_sort(dane3[i],0,granica_sort);
        }
        zly_wybor=false;
        break;
        case '4':
        granica_sort=(rozmiar_tablicy-1)*0.750;
        for(int i=0;i<ilosc_tablic;i++)
        {
          quick_sort(dane1[i],0,granica_sort);
          quick_sort(dane2[i],0,granica_sort);
          quick_sort(dane3[i],0,granica_sort);
        }
        zly_wybor=false;
        break;
        case '5':
        granica_sort=(rozmiar_tablicy-1)*0.950;
        for(int i=0;i<ilosc_tablic;i++)
        {
          quick_sort(dane1[i],0,granica_sort);
          quick_sort(dane2[i],0,granica_sort);
          quick_sort(dane3[i],0,granica_sort);
        }
        zly_wybor=false;
        break;
        case '6':
        granica_sort=(rozmiar_tablicy-1)*0.990;
        for(int i=0;i<ilosc_tablic;i++)
        {
          quick_sort(dane1[i],0,granica_sort);
          quick_sort(dane2[i],0,granica_sort);
          quick_sort(dane3[i],0,granica_sort);
        }
        zly_wybor=false;
        break;
        case '7':
        granica_sort=(rozmiar_tablicy-1)*0.997;
        for(int i=0;i<ilosc_tablic;i++)
        {
          quick_sort(dane1[i],0,granica_sort);
          quick_sort(dane2[i],0,granica_sort);
          quick_sort(dane3[i],0,granica_sort);
        }
        zly_wybor=false;
        break;
        case '8':
        for(int i=0;i<ilosc_tablic;i++)
        {
          reverse_sort(dane1[i],0,rozmiar_tablicy-1);
          reverse_sort(dane2[i],0,rozmiar_tablicy-1);
          reverse_sort(dane3[i],0,rozmiar_tablicy-1);
        }
        zly_wybor=false;
        break;
        default: cout << "Opcja nie istnieje" << endl;
        zly_wybor=true;
        break;
      }
    } while(zly_wybor);
    sprawdz_sort(dane1[4],rozmiar_tablicy);
    sprawdz_sort(dane2[4],rozmiar_tablicy);
    sprawdz_sort(dane3[4],rozmiar_tablicy);
    cout << "sortuje... " << endl;
    for(int i=0;i<ilosc_tablic;i++)
    {
      auto start_t=chrono::system_clock::now();
      merge_sort(dane1[i],temp,0,rozmiar_tablicy-1);
      auto end_t=chrono::system_clock::now();
      czas_sort[i]=(end_t-start_t);
    }
    sprawdz_sort(dane1[4],rozmiar_tablicy);
    cout << "Czas merge_sort: " << SumaCzasu(czas_sort,ilosc_tablic).count()*1000 << " ms " << endl;

    for(int i=0;i<ilosc_tablic;i++)
    {
      auto start_t=chrono::system_clock::now();
      quick_sort(dane2[i],0,rozmiar_tablicy-1);
      auto end_t=chrono::system_clock::now();
      czas_sort[i]=(end_t-start_t);
    }
    sprawdz_sort(dane2[4],rozmiar_tablicy);
    cout << "Czas quick_sort: " << SumaCzasu(czas_sort,ilosc_tablic).count()*1000 << " ms " << endl;

    for(int i=0;i<ilosc_tablic;i++)
    {
      auto start_t=chrono::system_clock::now();
      intro_sort(dane3[i],0,rozmiar_tablicy-1);
      auto end_t=chrono::system_clock::now();
      czas_sort[i]=(end_t-start_t);
    }
    sprawdz_sort(dane3[4],rozmiar_tablicy);
    cout << "Czas intro_sort: " << SumaCzasu(czas_sort,ilosc_tablic).count()*1000 << " ms " << endl;
    //cout << "Czas sortowania: " << SumaCzasu(czas_sort,ilosc_tablic).count()*1000 << " ms " << endl;
    /*cout << "Czy chcesz wyswietlic losowa tablice? (y/n) ";
    cin >> czy_wyswietlic;
    cout << endl;
    if(czy_wyswietlic=='y' || czy_wyswietlic=='Y')
    wyswietl_tablice(dane[4],rozmiar_tablicy);*/
    usun_tablice(temp);
    for(int j=0;j<ilosc_tablic;j++)
    {
      usun_tablice(dane1[j]);
      usun_tablice(dane2[j]);
      usun_tablice(dane3[j]);
    }
    usun_tablice(dane1);
    usun_tablice(dane2);
    usun_tablice(dane3);
  }
}




//MERGE SORT
template<typename Typ>
void merge(Typ *tab, Typ *temp, int l, int m, int r)
{
  for(int i = l;i<=r;i++)
  {
    temp[i]=tab[i];
  }
  int i=l;
  int j=m+1;
  int q=l;
  while(i<=m && j<=r)
  {
    if(temp[i]<temp[j]) tab[q++]=temp[i++];
    else tab[q++]=temp[j++];
  }
  while (i<=m)
  {
    tab[q++]=temp[i++];
  }
}

template <typename Typ>
void merge_sort(Typ *tab, Typ *temp, int l, int r)
{
  if(l<r)
  {
    int m=(l+r)/2;
    merge_sort(tab,temp,l,m);
    merge_sort(tab,temp,m+1,r);
    merge(tab,temp,l,m,r);
  }
}
//QUICK SORT
template <typename Typ>
int partycja(Typ *tab, int l, int r)
{
  int e_wyz=tab[(l+r)/2];

  int i=l;
  int j=r;
  while(1)
  {
    while(tab[j]>e_wyz) j--;
    while(tab[i]<e_wyz) i++;
    if(i<j) {
      swap(tab[i],tab[j]);
      i++;
      j--;
    }
    else return j;
  }
}

template <typename Typ>
void quick_sort(Typ *tab, int l, int r)
{
  if(l<r)
  {
    int q=partycja(tab,l,r);
    quick_sort(tab,l,q);
    quick_sort(tab,q+1,r);
  }
}
//INTRO SORT
template <typename Typ>
void insert_sort(Typ* tab, int l, int r)
{
  r++;
  int i=l;
  while(i<r)
  {
    int q=i;
    while(q>0 && tab[q-1]>tab[q])
    {
      swap(tab[q],tab[q-1]);
      q--;
    }
    i++;
  }
}
template <typename Typ>
void max_heap(Typ* tab, int rozm_kopca, int ind)
{
  int l=ind*2+1;
  int r=ind*2+2;
  int max=ind;
  if(l<rozm_kopca && tab[l]>tab[max]) max=l;
  if(r<rozm_kopca && tab[r]>tab[max]) max=r;
  if(max!=ind)
  {
    swap(tab[ind],tab[max]);
    max_heap(tab,rozm_kopca,max);
  }
}
template <typename Typ>
void heap_sort(Typ* tab,int l, int r)
{
  r++;
  int rozm_kopca=r-l;
  Typ* temp = new Typ [r-l];
  for(int i=0;i<rozm_kopca;i++)
  temp[i]=tab[i+l];
  for(int j=(rozm_kopca/2);j>=0;j--)
  max_heap(temp,rozm_kopca,j);
  for(int j=r-l-1;j>=0;j--)
  {
    swap(temp[0],temp[j]);
    max_heap(temp,j,0);
  }
  for(int i=0; i<rozm_kopca;i++)
  tab[i+1]=temp[i];

  delete [] temp;
}
template <typename Typ>
void intro_sort(Typ* tab, int l, int r)
{
  int c=r-l+1;
  int max_glebia=(int) log(c)*2;
  if(c<=16) insert_sort(tab,l,r);
  else if(max_glebia==0) heap_sort(tab,l,r);
  else if(l<r)
  {
    int e_wyz=partycja(tab,l,r);
    intro_sort(tab,l,e_wyz);
    intro_sort(tab,e_wyz+1,r);
  }

}
//REVERSE SORT
template <typename Typ>
void reverse_sort(Typ* tab, int l, int r)
{
  int i=l;
  int j=r;
  int sr=tab[(l+r)/2];
  do {
    while(tab[i]>sr) i++;
    while(tab[j]<sr) j--;
    if(i<=j)
    {
      swap(tab[i],tab[j]);
      i++;
      j--;
    }
  } while(i<=j);
  if(l<j) reverse_sort(tab,l,j);
  if(r>i) reverse_sort(tab,i,r);
}
//INNE FUNKCJE POMOCNICZE
template <typename Typ>
Typ *generuj_tablice(int rozmiar)
{
  return new Typ[rozmiar];
}
template <typename Typ>
void usun_tablice(Typ tablica)
{
  delete [] tablica;
}
template <typename Typ>
void wypelnij_tablice(Typ *tablica, int rozmiar)
{

  for(int i=0;i<rozmiar;i++)
  {
    tablica[i]=(rand());
  }
}
template <typename Typ>
void wyswietl_tablice(Typ *tablica, int rozmiar)
{
  for(int i=0;i<rozmiar;i++)
  {
    cout << tablica[i] << endl;
  }
}

template <typename Typ>
void sprawdz_sort(Typ *danet, int rozmiar)
{
  bool ifsorted;
  for(int i=0; i<rozmiar;i++)
  {
    if(i+1<rozmiar)
    {
    if(danet[i]<=danet[i+1]) ifsorted=true;
    else {
      ifsorted=false;
      i=rozmiar+1;
    }
    }
  }
  if(ifsorted==true) cout << "Tablica posortowana! :)" << endl;
  else cout << "Tablica nieposortowana! :("<<endl;
}

chrono::duration<double> SumaCzasu(chrono::duration<double> tab_czasow[], int ilosc)
{
  chrono::duration<double> Suma=(chrono::duration<double>)0;
  for(int i=0; i<ilosc;i++)
  {
    Suma+=tab_czasow[i];
  }
  return Suma;
}

void dostepne_procenty()
{
  cout << "W jakim stopniu tablice maja byc wstepnie posortowane?" << endl;
  cout << "1. 0%" << endl;
  cout << "2. 25%" << endl;
  cout << "3. 50%" << endl;
  cout << "4. 75%" << endl;
  cout << "5. 95%" << endl;
  cout << "6. 99%" << endl;
  cout << "7. 99.7%" << endl;
  cout << "8. W odwrotnej kolejnosci" << endl;
}
void dostepne_rozmiary()
{
  cout << "Wybierz rozmiar tablicy. Dostepne rozmiary:" << endl;
  cout << "1. 10000 " << endl;
  cout << "2. 50000 " << endl;
  cout << "3. 100000 " << endl;
  cout << "4. 500000 " << endl;
  cout << "5. 1000000 " << endl;
  cout << "q. Wyjscie " << endl;
}
