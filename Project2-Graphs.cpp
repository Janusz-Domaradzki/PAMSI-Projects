#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <cstdlib>
#include <memory>
#include <chrono>
#include <string>

#define inft 1000000
#define notinft -1000000
#define wsp_balans -1


using namespace std;

class Graf {
protected:
  int lW, lK;
  double gestosc;
public:
  const int& getlW() const { return lW; }
  const int& getlK() const { return lK; }
  //----
  virtual void wypelnijG(const bool L) const=0; //Metoda wypeniajaca losowo graf
  virtual void wyswietlG() const=0; //Metoda wyswietlajaca reprezentacje grafu
  virtual const int WczytajPlik()=0; //Metoda Metoda wypelniajaca graf na podstawie podanego pliku
  virtual void ZapiszPlik(const int start) const=0; //Tworzy plik, w ktorym zostanie zapisana reprezentacja grafu
virtual ~Graf() {};
explicit Graf(int tW, int tK, double tgestosc) : lW(tW), lK(tK), gestosc(tgestosc) {}
Graf() {};

};

struct Krawedz
{
  int zrodlo, cel, wart;
};
//Graf w postaci listy
class GrafL:public Graf
{
  Krawedz* krawedz;
public:
  void wypelnijG(const bool L) const;
  void wyswietlG() const;
  const int WczytajPlik();
  void ZapiszPlik(const int start) const;

  const Krawedz* zbuduj() const {return krawedz;}
  void dodajKrawedz(int nbez,int n, int nwart, int flg) const //dodaje krawedz we wskazanym miejscu (flg)
  {
    krawedz[flg].zrodlo=nbez;
    krawedz[flg].cel=n;
    krawedz[flg].wart=nwart;
  }
  bool sprIstnienie(int nbez, int nwierz) const //Sprawdza, czy polaczenie wierzcholkow istnieje
  {
    for(int i=0;i<lK;++i)
    {
      if(krawedz[i].cel==nwierz && krawedz[i].zrodlo==nbez) return true;
    }
    return false;
  }
  explicit GrafL(int tW, double tgestosc)
  :Graf(tW,static_cast<int>(tgestosc* tW* (tW-1)),tgestosc),
  krawedz(new Krawedz[static_cast<int>(tgestosc* tW* (tW-1))]) {}
  GrafL():Graf() {};
  ~GrafL() {delete[] krawedz;}

};

//Graf w postaci Macierzy
class GrafM:public Graf{
  unique_ptr<unique_ptr<int[]>[]> macierz;
public:
  void wypelnijG(const bool L) const;
  void wyswietlG() const;
  const int WczytajPlik();
  void ZapiszPlik(const int start) const;

  const int& wyswietlKom(int rzad,int kolumna) const {return macierz[rzad][kolumna];}
  GrafM():Graf() {};
  explicit GrafM(int tW, double tgestosc)
  : Graf(tW,static_cast<int>(tgestosc* tW* (tW-1)),tgestosc),
  macierz(make_unique<unique_ptr<int []>[]> (tW))
  {
    for(int i=0;i<lW;i++)
    {
      macierz[i] = make_unique<int[]>(tW);
      for(int j=0;j<lW;j++)
      {
        if(i==j) macierz[i][j]=0;
        else macierz[i][j]=inft;
      }
    }
  }
};

void RezultatAlgorytmu(string sciezka[], int dyst[], int t, int start);
//Algorytm Bellmana-Forda dla listy
double bellmanFord(shared_ptr<GrafL> graff, int pktstart, bool rozw);
//Algorytm Bellmana-Forda dla macierzy
double bellmanFord(shared_ptr<GrafM> graff, int pktstart, bool rozw);

//Funkcja sluzaca do testowania grafow (w zaleznosci od podanej ilosci)
template<typename T>
ostream& testy(ostream& file, int (&Tabwierz)[5], double (&Tabgest)[4], int ilosc_g);


int main()
{
  srand(static_cast<unsigned int>(time(NULL)));
  bool strefaTestu = true; //Jesli chce sie uniknac wykonywania testow do sprawozdania, wpisac "false"

  //TESTY//
  if(strefaTestu)
  {
    int TabWierzcholkow[5]={5,15,30,50,100};
    double gestosciG[4]={0.25,0.5,0.75,1};
    int ilosc_grafow=100;

    ofstream file("Czasy.txt");
    if(!file.is_open())
    {
      cerr << "Brak pliku!" << flush;
      return 1;
    }

    testy<GrafL>(file,TabWierzcholkow,gestosciG,ilosc_grafow);
    cout << "Reprezentacja przy pomocy listy zrobiona" << endl << endl;
    testy<GrafM>(file,TabWierzcholkow,gestosciG,ilosc_grafow);
    cout << "Reprezentacja przy pomocy macierzy zrobiona" << endl << endl;

    file.close();
    return 0;
  }
  //TWORZENIE WLASNEGO GRAFU//
  //Wykonuje sie tylko wtedy, gdy streafa testow "TESTY" jest wylaczona (strefaTestu=false;)
  //Ponizsze dane mozna zmieniac
  typedef GrafM GrafTestowany; //reprezentacja grafu
  bool uzyjPliku=false; //czy graf ma byc wczytany z pliku
  bool dozwolPowt=true; //czy krawedz ma zaczynac sie i konczyc w tym samym wezle
  int wierzcholki=7; //ilosc wezlow
  double gestosc=0.5; //gestosc grafu
  int pktStartowy=3; //punkt startowy dla algorytmu
  //
  double czas_wykonania; //w tej zmiennej zostanie zapisany czas wykonania algorytmu

  shared_ptr<GrafTestowany> grafx;
  if(uzyjPliku)
  {
    shared_ptr<GrafTestowany> tmp=make_shared<GrafTestowany>();
    grafx=tmp;
    try
    {
      pktStartowy=tmp->WczytajPlik();
    }
    catch(const char* msg)
    {
      cerr << msg << flush;
      return 1;
    }
  }
  else
  {
    shared_ptr<GrafTestowany> tmp = make_shared<GrafTestowany>(wierzcholki,gestosc);
    grafx=tmp;
    tmp->wypelnijG(dozwolPowt);
  }
  grafx->wyswietlG();
  try
  {
    if(!uzyjPliku) grafx->ZapiszPlik(pktStartowy);
  }
  catch(const char* msg)
  {
    cerr << msg << flush;
    return 1;
  }
  czas_wykonania=bellmanFord(move(grafx),pktStartowy,true);
  cout << endl << "Czas algorytmu: " << czas_wykonania << " ms" << endl;
  return 0;
}

//Metody

//Metody Grafu reprezentowanego na liscie
void GrafL::wypelnijG(const bool L) const
{
  if(gestosc==1)
  {
    int flg=0;
    for(int i=0; i<lW;i++)
    {
      for(int j=0; j<lW;j++)
      {
        if (i!=j)
        {
          int wart = rand()%20+wsp_balans;
          while(wart==0)
          {
            wart=rand()%20+wsp_balans;
          }
          dodajKrawedz(i,j,wart,flg++);
        }
      }
    }
  }
  else
  {
    int krw=0;
    while(krw<lK)
    {
      int grd = rand()%lW;
      int pkt = rand()%lW;
      if(!sprIstnienie(grd,pkt))
      {
        int wart=rand()%20+wsp_balans;
        while(wart==0)
        {
          wart=rand()%20+wsp_balans;
        }
        if(grd!=pkt) dodajKrawedz(grd,pkt,wart,krw++);
        else if(L) dodajKrawedz(grd,pkt,wart,krw++);
      }
    }
  }
}

void GrafL::wyswietlG() const
{
  cout << string(30, '-') << endl << "Reprezentacja listy" << endl;

  for(int i=0;i<lW;++i)
  {
    cout << i;
    for(int j=0;j<lK;++j)
    {
      if(krawedz[j].zrodlo==i)
      cout << "->" << "[" << krawedz[j].cel << "|" << krawedz[j].wart << "]";
    }
    cout << endl;
  }
  cout << endl;
}

const int GrafL::WczytajPlik()
{
  ifstream file("Plikwejsciowy.txt");
  if(!file.is_open())
  {
    cerr << "Brak pliku!";
    return 1;
  }
  int start, zrodl, celz, wartx;
  file >> lK >> lW >> start;
  krawedz=new Krawedz[lK];

  for(int i=0;i<lK;++i)
  {
    file >> zrodl >> celz >> wartx;
    dodajKrawedz(zrodl, celz, wartx, i);
  }
  file.close();
  return start;
}

void GrafL::ZapiszPlik(const int start) const
{
  ofstream file("ZapisanyGraf.txt");
  if(!file.is_open())
  {
    cerr << "Brak pliku!";
    return;
  }
  file << lK <<" "<< lW << " " << start << endl;
  for(int i=0;i<lK;++i)
  {
    file << krawedz[i].zrodlo << " ";
    file << krawedz[i].cel << " ";
    file << krawedz[i].wart << " ";
    file << endl;
  }
  file.close();
}


//Metody Grafu reprezentowanego na macierzy
void GrafM::wypelnijG(const bool L) const
{
  if(gestosc==1)
  {
    for(int i=0; i<lW;i++)
    {
      for(int j=0; j<lW;j++)
      {
        if (i!=j)
        {
          int wart = rand()%20+wsp_balans;
          while(wart==0)
          {
            wart=rand()%20+wsp_balans;
          }
          macierz[i][j]=wart;
        }
      }
    }
  }
  else
  {
    int krw=lK;
    while(krw)
    {
      int rzd = rand()%lW;
      int col = rand()%lW;
      if(macierz[rzd][col]==0 || macierz[rzd][col]==inft)
      {
        int wart=rand()%20+wsp_balans;
        while(wart==0)
        {
          wart=rand()%20+wsp_balans;
        }
        if(rzd!=col)
        {
          macierz[rzd][col]=wart;
          krw--;
        }
        else if (L)
        {
          macierz[rzd][col]=wart;
          krw--;
        }
      }
    }
  }
}

void GrafM::wyswietlG() const
{
  cout << string(33, '-') << endl << "Reprezentacja macierzy" << endl;

  for(int i=0;i<lW;++i)
  {
    if(i<=10) cout << "    " << i;
    else if(i<=100) cout << "   " << i;
    else cout << "  " << i;
  }
  cout << endl << endl;
  for(int j=0;j<lW;++j)
  {
    if(j<10) cout << j << "   |";
    else if(j<100) cout << j << "  |";
    else cout << j << " |";

    for(int k=0;k<lW;++k)
    {
      int wart = macierz[j][k];
      if(wart==inft) cout << "*";
      else cout << wart;

      if(abs(wart)<10 || abs(wart==inft)) cout << "    ";
      else if (abs(wart)<100) cout << "   ";
      else cout << "  ";
      if(wart<0) cout << '\b';
    }
    cout << "|" << endl;
  }
  cout << endl;
}


const int GrafM::WczytajPlik()
{
  ifstream file("Plikwejsciowy.txt");
  if(!file.is_open())
  {
    cerr << "Brak pliku!";
    return 1;
  }
  int start, zrodlo, cel, waga;
  file >> lK >> lW >> start;
  macierz = make_unique<unique_ptr<int[]>[]>(lW);

  for(int i=0;i<lW;++i)
  {
    macierz[i]=move(make_unique<int[]>(lW));
    for(int j=0;j<lW;++j)
    {
      if(i==j) macierz[i][j]=0;
      else macierz[i][j]=inft;
    }
  }
  for(int i=0; i<lK;++i)
  {
    file >> zrodlo >> cel >> waga;
    macierz[zrodlo][cel]=waga;
  }
  file.close();
  return start;
}

void GrafM::ZapiszPlik(const int start) const
{
  ofstream file("ZapisanyGraf.txt");
  if(!file.is_open())
  {
    cerr << "Plik zamkniety badz nie istnieje!";
    return;
  }
  file << lK << " " << lW << " " << start << endl;
  for(int i=0;i<lW;++i)
  {
    for(int j=0;j<lW;++j)
    {
      if(macierz[i][j]!=inft && macierz[i][j]!=0)
      {
        file << i << " ";
        file << j << " ";
        file << macierz[i][j] << endl;
      }
    }
    cout << endl;
  }
  file.close();
}

//Pozostałe Metody

void RezultatAlgorytmu(string sciezka[], int dyst[], int t, int start)
{
  ofstream file("Sciezki.txt");
  cout << string(20,'-') << endl << "Rezultat" << endl;
  cout << "Wierzcholek poczatkowy -> " << start << endl;

  file << string(20,'-') << endl << "Rezultat" << endl;
  file << "Wierzcholek poczatkowy -> " << start << endl;

  for(int i=0;i<t;++i)
  {
    if(dyst[i]==notinft)
    {
      cout << i << "->" << "-inf" << endl;
      file << i << "->" << "-inf" << endl;
      continue;
    }
    else if(dyst[i]==inft)
    {
      cout << i << "->" << "inf" << endl;
      file << i << "->" << "inf" << endl;
      continue;
    }
    else
    {
      cout << i << "->" << dyst[i];
      file << i << "->" << dyst[i];
    }

    if(i<10)
    {
      cout << "   ";
      file << "   ";
    }
    else if(i<100)
    {
      cout << "  ";
      file << "  ";
    }
    else
    {
      cout << " ";
      file << " ";
    }

    if((dyst[i] >= 100 && dyst[i] <1000) || (-100>dyst[i] && dyst[i] <=-10))
    {
      cout << " Najkrotsza sciezka: " << sciezka[i] << i;
      file << " Najkrotsza sciezka: " << sciezka[i] << i;
    }
    else if(0<=dyst[i] && dyst[i]<10)
    {
      cout << "   Najkrotsza sciezka: " << sciezka[i] << i;
      file << "   Najkrotsza sciezka: " << sciezka[i] << i;
    }
    else if((dyst[i]>=10 && dyst[i]<100)||(-10<dyst[i] && dyst[i]<0))
    {
      cout << "  Najkrotsza sciezka: " << sciezka[i] << i;
      file << "  Najkrotsza sciezka: " << sciezka[i] << i;
    }
    else
    {
      cout << "Najkrotsza sciezka: " << sciezka[i] << i;
      file << "Najkrotsza sciezka: " << sciezka[i] << i;
    }
    cout << endl;
    file << endl;
  }
  cout << endl;
  file.close();
}

//Algorytm Bellmana-Forda dla grafu reprezentowanego lista

double bellmanFord(shared_ptr<GrafL> graff, int pktstart, bool rozw)
{
  string* magazynSciezki = new string[graff->getlW()];

  auto czas_start=chrono::high_resolution_clock::now();

  int* magazynDystans = new int[graff->getlW()];

  for(int i=0;i<graff->getlW();++i)
  {
    magazynDystans[i]=inft;
  }
  magazynDystans[pktstart]=0;

  for(int i=1; i<graff->getlW();++i)
  {
    for(int j=0;j<graff->getlK();++j)
    {
      int u=graff->zbuduj()[j].zrodlo;
      int v=graff->zbuduj()[j].cel;
      int wart=graff->zbuduj()[j].wart;

      if(magazynDystans[u]+wart <magazynDystans[v])
      {
        magazynDystans[v]=magazynDystans[u]+wart;
        if(rozw)
        {
          magazynSciezki[v].clear();
          magazynSciezki[v].append(magazynSciezki[u]+to_string(u)+"->");
        }
      }
    }
  }
  for(int i=1; i<graff->getlW();++i)
  {
    for(int j=0;j<graff->getlK();++j)
    {
      int u=graff->zbuduj()[j].zrodlo;
      int v=graff->zbuduj()[j].cel;
      int wart=graff->zbuduj()[j].wart;
      if(magazynDystans[u]+wart<magazynDystans[v])
      {
        if(magazynDystans[u]>inft/2) magazynDystans[u]=inft;
        else magazynDystans[v]=notinft;
      }
      else if(magazynDystans[u]>inft/2) magazynDystans[u]=inft;
    }
  }
  auto czas_end=chrono::high_resolution_clock::now();

  if(rozw) RezultatAlgorytmu(move(magazynSciezki),move(magazynDystans),graff->getlW(), pktstart);
  delete[] magazynDystans;
  delete[] magazynSciezki;
  return chrono::duration<double,milli>(czas_end-czas_start).count();
}
//Algorytm Bellmana-Forda dla grafu reprezentowanego macierza
double bellmanFord(shared_ptr<GrafM> graff, int pktstart, bool rozw)
{
  string* magazynSciezki = new string[graff->getlW()];
  auto czas_start=chrono::high_resolution_clock::now();
  int* magazynDystans = new int[graff->getlW()];
  for(int i=0; i<graff->getlW();++i)
  {
    magazynDystans[i]=inft;
  }
  magazynDystans[pktstart]=0;

  for(int i=1;i<graff->getlW();++i)
  {
    for(int j=0;j<graff->getlW();++j)
    {
      for(int w=0;w<graff->getlW();++w)
      {
        int u=j;
        int v=w;
        int wart=graff->wyswietlKom(j,w);
        if(magazynDystans[u]+wart<magazynDystans[v])
        {
          magazynDystans[v]=magazynDystans[u]+wart;
          if(rozw)
          {
            magazynSciezki[v].clear();
            magazynSciezki[v].append(magazynSciezki[u]+to_string(u)+"->");
          }
        }
      }
    }
  }
  for(int i=1;i<graff->getlW();++i)
  {
    for(int j=0; j<graff->getlW();++j)
    {
      for(int w=0; w<graff->getlW();++w)
      {
        int u=j;
        int v=w;
        int wart=graff->wyswietlKom(j,w);
        if(magazynDystans[u]+wart<magazynDystans[v])
        {
          if(magazynDystans[u]>inft/2) magazynDystans[u]=inft;
          else if(wart==inft) continue;
          else magazynDystans[v]=notinft;
        }
        else if(magazynDystans[u]>inft/2) magazynDystans[u]=inft;
      }
    }
  }
  auto czas_end=chrono::high_resolution_clock::now();

  if(rozw) RezultatAlgorytmu(move(magazynSciezki),move(magazynDystans),graff->getlW(),pktstart);
  delete[] magazynDystans;
  delete[] magazynSciezki;
  return chrono::duration<double, milli>(czas_end-czas_start).count();
}

template<typename T>
ostream& testy(ostream& file, int (&Tabwierz)[5], double (&Tabgest)[4], int ilosc_g)
{
  for(double gestsc:Tabgest)
  {
    for(int wierzcholki:Tabwierz)
    {
      double suma=0;
      for(int i=0;i<ilosc_g;++i)
      {
        shared_ptr<T> graff = make_shared<T>(wierzcholki,gestsc);
        int pktstart=rand()%wierzcholki;
        graff->wypelnijG(true);
        suma+=bellmanFord(move(graff),pktstart,false);
        cout << (i*100/ilosc_g)+1 << " % " <<"\r" <<flush;
      }
      file << suma/ilosc_g<<"  ";
      cout << "Grafy o ilosci wierzcholkow: "<< wierzcholki << " sprawdzone!" << endl;
      cout << "Sredni czas dzialania algorytmu: "<< suma/ilosc_g << " ms" << endl;

    }
    cout << "Grafy o gestosci "<< gestsc << " ukonczone!" << endl << endl;
  }
  file << endl;
  return file;
}
