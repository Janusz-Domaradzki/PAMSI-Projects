#include <iostream>
#include <cmath>
#include <vector>
#include <memory>
#include <unistd.h>
#include <SFML/Graphics.hpp>

const int rozmiarr=850;


using namespace std;

struct Wspolrzedne
{
  int x1 : 4;
  int y1 : 4;
  int x2 : 4;
  int y2 : 4;
  Wspolrzedne(int x1_, int y1_, int x2_, int y2_)
  {
    x1=x1_;
    y1=y1_;
    x2=x2_;
    y2=y2_;
  }
  bool operator != (const Wspolrzedne &B)
  {
    if(this->x1!=B.x1 || this->y1!=B.y1 || this->x2!=B.x2 || this->y2!=B.y2) return true;
    return false;
  }
};

struct Ruch
{
  vector<Wspolrzedne> wspolrzedne;
  Ruch(){}
  Ruch(int x1, int y1, int x2, int y2);
  Ruch(Wspolrzedne c);
  void dodaj(Wspolrzedne c);
  bool operator == (const Ruch &B);
};

enum Pionki{PUSTY, BIALY_PION, CZARNY_PION, BIALA_DAMA, CZARNA_DAMA};

class Plansza
{
public:
  Pionki plansza[8][8];
  int licznikRemisu;
  int bialyP, czarnyP, bialaD, czarnaD;
  Plansza()
  {
    restart();
  }
  void restart();
  bool sprawdzZbicie(int x, int y);
  bool sprawdzZbicie(bool kolor);
  vector<Ruch> SekwencjaRuchu(Ruch ruch, Plansza p);
  vector<Ruch> dostepneRuchy(bool kolor);
  vector<Ruch> dostepneRuchy(int x, int y);
  void Bicie(int x1, int y1, int x2, int y2);

  int ruch(Wspolrzedne m);
  int ruch(Ruch m);
  int ruch(int x1, int y1, int x2, int y2, bool kolor);
  int SprawdzStan(bool kolor);

  int OcenPole(int x, int y);
  int OcenPlansze(bool kolor, bool strona);
};

class Wezel
{
public:
  bool kolor;
  Plansza plansza;
  int wartosc;
  bool strona;
  Ruch ruch;
  vector<Wezel> pochodne{};

  Wezel(int glebia, Plansza plansza_, bool strona_, bool kolor_, Ruch ruch_=Ruch(0,0,0,0));

};

int max(int a, int b)
{
  return a<b ? b:a;
}
int min(int a, int b)
{
  return a<b ? a:b;
}

int minmax(Wezel& wezel, int glebia, bool MaxGracza, int alfa, int beta);

class AI
{
  AI(){}
public:
  static Ruch znajdzNajlepszyRuch(int max_glebia, Plansza plansza, bool strona)
  {
    auto zrodlo = make_unique<Wezel>(max_glebia,plansza,strona,strona);
    minmax(*zrodlo,0,true,-100000,100000);
    return zrodlo->ruch;
  }
};

class Warcaby
{
  // 0 - czlowiek, 1 - komputer(AI)
  bool gracz;
  bool strona;

  sf::RenderWindow okno;
  sf::Texture tekstury[6];
  sf::Sprite obiekty[6];

  Plansza plansza;

public:
  Warcaby();
  void graj();
  bool StartGry();
  int koniecGry(int status);
  void ruchGracz();
  void ruchKomp(bool drugi=false);
  bool Dzialanie();
  int mysz();
  void rysuj();
};

int main()
{
  Warcaby Gra;
  Gra.graj();
  return 0;
}



Ruch::Ruch(int x1, int y1, int x2, int y2)
{
  wspolrzedne.emplace_back(x1,y1,x2,y2);
}
Ruch::Ruch(Wspolrzedne c)
{
  wspolrzedne.push_back(c);
}
void Ruch::dodaj(Wspolrzedne c)
{
  wspolrzedne.insert(wspolrzedne.begin(),c);
}
bool Ruch::operator == (const Ruch &B)
{
  if (this->wspolrzedne.size()!=B.wspolrzedne.size()) return false;
  else
  {
    for(int i=0; i<wspolrzedne.size();i++)
    {
      if(this->wspolrzedne[i]!=B.wspolrzedne[i]) return false;
    }
  }
  return true;
}

void Plansza::restart()
{
  licznikRemisu==0;
  for(int i=0;i<8;i++)
  {
    for(int j=0;j<8;j++)
    {
      if(i%2!=j%2)
      {
        if(i<3) plansza[i][j]=BIALY_PION;
        else if(i>4) plansza[i][j]=CZARNY_PION;
        else plansza[i][j]=PUSTY;
      }
      else plansza[i][j]=PUSTY;
    }
  }
}

bool Plansza::sprawdzZbicie(int x, int y)
{
  // biale 0, czarne 1
  bool kolor = (plansza[x][y]==BIALY_PION || plansza[x][y]==BIALA_DAMA)?0:1;

  if(plansza[x][y]==BIALY_PION || plansza[x][y]==CZARNY_PION)
  {
    for(int a:{-1,1})
    {
      for(int b:{-1,1})
      {
        int i=x+a;
        int j=y+b;
        if(i+a<8 && i+a>=0 && j+b<8 && j+b>=0)
        {
          if(kolor)
          {
            if(plansza[i][j]==BIALY_PION || plansza[i][j]==BIALA_DAMA)
            {
              if(plansza[i+a][j+b]==PUSTY) return true;
            }
          }
          else
          {
            if(plansza[i][j]==CZARNY_PION || plansza[i][j]==CZARNA_DAMA)
            {
              if(plansza[i+a][j+b]==PUSTY) return true;
            }
          }

        }
      }
    }
  }
  else if(plansza[x][y]==BIALA_DAMA || plansza[x][y]==CZARNA_DAMA)
  {
    for(int a:{-1,1})
    {
      for(int b:{-1,1})
      {
        int i=x+a;
        int j=y+b;
        while(i+a<8 && i+a>=0 && j+b<8 && j+b>=0)
        {
          if(kolor)
          {
            if(plansza[i][j]==CZARNA_DAMA || plansza[i][j]==CZARNY_PION) break;
            if(plansza[i][j]==BIALY_PION || plansza[i][j]==BIALA_DAMA)
            {
              if(plansza[i+a][j+b]==PUSTY) return true;
              else break;
            }
          }
          else
          {
            if(plansza[i][j]==BIALY_PION || plansza[i][j]==BIALA_DAMA) break;
            if(plansza[i][j]==CZARNY_PION || plansza[i][j]==CZARNA_DAMA)
            {
              if(plansza[i+a][j+b]==PUSTY) return true;
              else break;
            }
          }
          i+=a;
          j+=b;
        }
      }
    }
  }
  return false;
}

bool Plansza::sprawdzZbicie(bool kolor)
{
  for(int i=0;i<8;i++)
  {
    for(int j=0;j<8;j++)
    {
      if(kolor)
      {
        if(plansza[i][j]==CZARNA_DAMA || plansza[i][j]==CZARNY_PION)
        {
          if(sprawdzZbicie(i,j)) return true;
        }
      }
      else
      {
        if(plansza[i][j]==BIALY_PION || plansza[i][j]==BIALA_DAMA)
        {
          if(sprawdzZbicie(i,j)) return true;
        }
      }
    }
  }
  return false;
}

vector<Ruch> Plansza::SekwencjaRuchu(Ruch ruch, Plansza p)
{
  vector<Ruch> ruchy;
  if(p.ruch(ruch.wspolrzedne[0])==1)
  {
    ruchy.emplace_back(ruch.wspolrzedne[0]);
  }
  else
  {
    for(auto& m: p.dostepneRuchy(ruch.wspolrzedne[0].x2,ruch.wspolrzedne[0].y2))
    {
      vector<Ruch> tmp=SekwencjaRuchu(m,p);
      ruchy.insert(ruchy.end(), tmp.begin(), tmp.end());
    }
    for(auto& m: ruchy)
    {
      m.dodaj(ruch.wspolrzedne[0]);
    }
    return ruchy;
  }
}
vector<Ruch> Plansza::dostepneRuchy(bool kolor)
{
  vector<Ruch> ruchy;
  for(int i=0;i<8;i++)
  {
    for(int j=0;j<8;j++)
    {
      if(kolor)
      {
        if(plansza[i][j]==CZARNA_DAMA || plansza[i][j]==CZARNY_PION)
        {
          auto tmp=dostepneRuchy(i,j);
          for(auto& m: tmp)
          {
            auto tmp2=SekwencjaRuchu(m, *this);
            ruchy.insert(ruchy.end(),tmp2.begin(),tmp2.end());
          }
        }
      }
      else
      {
        if(plansza[i][j]==BIALA_DAMA || plansza[i][j]==BIALY_PION)
        {
          auto tmp=dostepneRuchy(i,j);
          for(auto& m: tmp)
          {
            auto tmp2=SekwencjaRuchu(m, *this);
            ruchy.insert(ruchy.end(),tmp2.begin(),tmp2.end());
          }
        }
      }
    }
  }
  return ruchy;
}
vector<Ruch> Plansza::dostepneRuchy(int x, int y)
{
  bool kolor;
  vector<Ruch> ruchy;
  if(plansza[x][y]==PUSTY) return ruchy;
  if(plansza[x][y]==BIALA_DAMA || plansza[x][y]==BIALY_PION) kolor=0;
  else kolor=1;

  if(sprawdzZbicie(kolor)!=sprawdzZbicie(x,y))
  return ruchy;

  if(plansza[x][y]==BIALY_PION || plansza[x][y]==CZARNY_PION)
  {
    if(!sprawdzZbicie(x,y))
    {
      int xdir=kolor? -1:1;
      if(y<7)
      {
        if(plansza[x+xdir][y+1]==PUSTY) ruchy.push_back(Ruch(x,y,x+xdir,y+1));
      }
      if(y>0)
      {
        if(plansza[x+xdir][y-1]==PUSTY) ruchy.push_back(Ruch(x,y,x+xdir,y-1));
      }
    }
    else
    {
      for(int a : {-1,1})
      {
        for(int b: {-1,1})
        {
          int i=x+a;
          int j=y+b;
          if(i+a<8 && i+a>=0 && j+b<8 && j+b>=0)
          {
            if(kolor)
            {
              if(plansza[i][j]==BIALY_PION || plansza[i][j]==BIALA_DAMA)
              {
                if(plansza[i+a][j+b]==PUSTY) ruchy.push_back(Ruch(x,y,i+a,j+b));
              }
            }
            else
            {
              if(plansza[i][j]==CZARNY_PION || plansza[i][j]==CZARNA_DAMA)
              {
                if(plansza[i+a][j+b]==PUSTY) ruchy.push_back(Ruch(x,y,i+a,j+b));
              }
            }
          }
        }
      }
    }
  }
  else
  {
    bool Fzbicia = sprawdzZbicie(x,y);
    for(int a : {-1,1})
    {
      for(int b : {-1,1})
      {
        int i=x+a;
        int j=y+b;
        while(i>=0 && j>=0 && i<=7 && j<=7)
        {
          if(plansza[i][j]==PUSTY)
          {
            if(!Fzbicia) ruchy.push_back(Ruch(x,y,i,j));
          }
          else if(kolor)
          {
            if(plansza[i][j]==CZARNY_PION || plansza[i][j]==CZARNA_DAMA) break;
            else if((plansza[i][j]==BIALY_PION || plansza[i][j]==BIALA_DAMA) && Fzbicia)
            {
              do{
                i+=a;
                j+=b;
                if(i>=0 && j>=0 && i<=7 && j<=7)
                {
                  if(plansza[i][j]==PUSTY) ruchy.push_back(Ruch(x,y,i,j));
                  else break;
                }
                else break;
              } while(true);
              break;
            }
            else break;
          }
          else
          {
            if(plansza[i][j]==BIALY_PION || plansza[i][j]==BIALA_DAMA) break;
            if((plansza[i][j]==CZARNY_PION || plansza[i][j]==CZARNA_DAMA) && Fzbicia)
            {
              do{
                i+=a;
                j+=b;
                if(i>=0 && j>=0 && i<=7 && j<=7)
                {
                  if(plansza[i][j]==PUSTY) ruchy.push_back(Ruch(x,y,i,j));
                  else break;
                }
                else break;
              } while(true);
              break;
            }
            else break;
          }
          i+=a;
          j+=b;
        }

      }
    }
  }
  return ruchy;
}

void Plansza::Bicie(int x1, int y1, int x2, int y2)
{
  int a=(x1<x2)? 1:-1;
  int b=(y1<y2)? 1:-1;
  for(int i=x1+a, j=y1+b; i!=x2, j!=y2; i+=a, j+=b)
  {
    plansza[i][j]=PUSTY;
  }
}

int Plansza::ruch(Wspolrzedne m)
{
  bool kolor = (plansza[m.x1][m.y1]==BIALA_DAMA || plansza[m.x1][m.y1]==BIALY_PION) ? 0 : 1;
  return ruch(m.x1, m.y1, m.x2, m.y2, kolor);
}
int Plansza::ruch(Ruch m)
{
  bool kolor = (plansza[m.wspolrzedne[0].x1][m.wspolrzedne[0].y1] == BIALA_DAMA || plansza[m.wspolrzedne[0].x1][m.wspolrzedne[0].y1] ==BIALY_PION) ? 0 : 1;
  int i{};
  while(ruch(m.wspolrzedne[i]) == 2)
  {
    i++;
  }
  return 1;
}
int Plansza::ruch(int x1, int y1, int x2, int y2, bool kolor)
{
  if((kolor && (plansza[x1][y1]==BIALA_DAMA || plansza[x1][y1]==BIALY_PION)) || (!kolor && (plansza[x1][y1]==CZARNA_DAMA || plansza[x1][y1]==CZARNY_PION)))
  return 0;

  bool FZbicia = false;
  for(auto& ruch : dostepneRuchy(x1,y1))
  {
    if(ruch==Ruch(x1,y1,x2,y2))
    {
      if(sprawdzZbicie(kolor))
      {
        Bicie(x1,y1,x2,y2);
        FZbicia = true;
      }
      plansza[x2][y2]=plansza[x1][y1];
      plansza[x1][y1]=PUSTY;
      if(FZbicia && sprawdzZbicie(x2,y2)) return 2;
      else if((!kolor && x2 == 7 ) || (kolor && x2 ==0)) plansza[x2][y2]=kolor ? CZARNA_DAMA : BIALA_DAMA;
      return 1;
    }
  }
  return 0;
}
/* 0 - kontynuacja gry
   1 - biale wygraly
   2 - czarne wygraly
   -1 - remis */
int Plansza::SprawdzStan(bool kolor)
{
  int BialePiony{}, CzarnePiony{}, BialeDamy{}, CzarneDamy{};
  bool ruchyBialych{}, ruchyCzarnych{};
  for(int i=0;i<8;i++)
  {
    for(int j=0;j<8;j++)
    {
      switch(plansza[i][j])
      {
        case PUSTY:
        break;
        case BIALY_PION:
        if(!ruchyBialych)
        {
        if(dostepneRuchy(i,j).size()!=0) ruchyBialych=true;
        }
        BialePiony++;
        break;
        case CZARNY_PION:
        if(!ruchyCzarnych)
        {
          if(dostepneRuchy(i,j).size()!=0) ruchyCzarnych=true;
        }
        CzarnePiony++;
        break;
        case BIALA_DAMA:
        if(!ruchyBialych)
        {
          if(dostepneRuchy(i,j).size()!=0) ruchyBialych=true;
        }
        BialeDamy++;
        break;
        case CZARNA_DAMA:
        if(!ruchyCzarnych)
        {
          if(dostepneRuchy(i,j).size()!=0) ruchyCzarnych=true;
        }
        CzarneDamy++;
        break;
      }
    }
  }
  if((CzarnePiony || CzarneDamy) && (BialePiony || BialeDamy) && ((kolor && ruchyCzarnych) || (!kolor && ruchyBialych)))
  {
    if(CzarneDamy && BialeDamy && BialePiony == bialyP && CzarnePiony==czarnyP && CzarneDamy == czarnaD && BialeDamy == bialaD)
    {
      if(++licznikRemisu==30) return -1;
      else licznikRemisu=0;
    }
    czarnyP=CzarnePiony;
    bialyP=BialePiony;
    czarnaD=CzarneDamy;
    bialaD=BialeDamy;
    return 0;
  }
  else if((!CzarnePiony && !CzarneDamy) || (kolor && !ruchyCzarnych)) return 1;
  else if((!BialePiony && !BialeDamy) || (!kolor && !ruchyBialych)) return 2;
}

int Plansza::OcenPole(int x, int y)
{
  int punkty{};
  switch(plansza[x][y])
  {
    case PUSTY:
    break;
    case BIALY_PION:
    punkty+=7;
    if(x>0)
    {
      punkty+=2;
      if(x>1)
      {
        punkty++;
        if(x>2) punkty++;
      }
    }
    if(y<2 || y>5)
    {
      punkty++;
      if(y<1 || y>6) punkty++;
    }
    punkty+=15;
    break;
    case CZARNY_PION:
    punkty+=7;
    if(x<7)
    {
      punkty+=2;
      if(x<6)
      {
        punkty++;
        if(x<5) punkty++;
      }
    }
    if(y<2 || y>5)
    {
      punkty++;
      if(y<1 || y>6) punkty++;
    }
    punkty+=15;
    break;
    case BIALA_DAMA:
    punkty+=25;
    if(y<2 || y>5)
    {
      punkty++;
      if(y<1 || y>6) punkty++;
    }
    punkty+=15;
    break;
    case CZARNA_DAMA:
    punkty+=25;
    if(y<2 || y>5)
    {
      punkty++;
      if(y<1 || y>6) punkty++;
    }
    punkty+=15;
    break;
  }
  return punkty;
}
int Plansza::OcenPlansze(bool kolor, bool strona)
{
  switch(SprawdzStan(kolor))
  {
    case -1:
    return 0;
    case 0:
    break;
    case 1:
    return strona ? -10000 : 10000;
    case 2:
    return strona ? 10000 : -10000;
  }
  int czarne{}, biale{};
  for(int i=0; i<8; i++)
  {
    for(int j=0; j<8; j++)
    {
      if(plansza[i][j]==BIALA_DAMA || plansza[i][j]==BIALY_PION)
      biale+=OcenPole(i,j);
      else if(plansza[i][j]!=PUSTY)
      czarne+=OcenPole(i,j);
    }
  }
  if(strona) return czarne-biale;
  else return biale-czarne;
}


Wezel::Wezel(int glebia, Plansza plansza_, bool strona_, bool kolor_, Ruch ruch_)
{
  kolor=kolor_;
  plansza=plansza_;
  strona=strona_;
  ruch=ruch_;
  wartosc=0;
  if(glebia==0 || plansza.SprawdzStan(strona)!=0) wartosc=plansza.OcenPlansze(strona,kolor);
  else
  {
    for(auto& m : plansza.dostepneRuchy(strona))
    {
      Plansza temp(plansza);
      temp.ruch(m);
      pochodne.emplace_back(glebia-1,temp,!strona,kolor,m);
    }
  }
}

int minmax(Wezel& wezel, int glebia, bool MaxGracza, int alfa, int beta)
{
  if(wezel.pochodne.size()==0) return wezel.wartosc;

  int wart, najlepsza_wart;

  if(MaxGracza)
  {
    najlepsza_wart=-100000;
    for(auto& pochodna : wezel.pochodne)
    {
      wart=minmax(pochodna, glebia+1, false, alfa, beta);
      najlepsza_wart = max(najlepsza_wart, wart);
      if(najlepsza_wart==wart && glebia==0) wezel.ruch = pochodna.ruch;
      alfa = max(alfa, najlepsza_wart);
      if(beta <= alfa) break;
    }
    return najlepsza_wart;
  }
  else
  {
    najlepsza_wart=100000;
    for(auto& pochodna : wezel.pochodne)
    {
      wart=minmax(pochodna, glebia+1, true, alfa, beta);
      najlepsza_wart = min(najlepsza_wart, wart);
      if(najlepsza_wart==wart && glebia==0) wezel.ruch = pochodna.ruch;
      beta = min(beta, najlepsza_wart);
      if(beta <= alfa) break;
      return najlepsza_wart;
    }
  }
}

Warcaby::Warcaby()
{
  okno.create(sf::VideoMode(rozmiarr,rozmiarr), "WARCABY");

  tekstury[0].loadFromFile("PLANSZA.png");
  tekstury[1].loadFromFile("BIALY_PION.png");
  tekstury[2].loadFromFile("BIALA_DAMA.png");
  tekstury[3].loadFromFile("CZARNY_PION.png");
  tekstury[4].loadFromFile("CZARNA_DAMA.png");
  tekstury[5].loadFromFile("Wiadomosci.png");

  obiekty[0].setTexture(tekstury[0]);
  obiekty[1].setTexture(tekstury[1]);
  obiekty[2].setTexture(tekstury[2]);
  obiekty[3].setTexture(tekstury[3]);
  obiekty[4].setTexture(tekstury[4]);
  obiekty[5].setTexture(tekstury[5]);

  strona = false;
  gracz = !strona;
}

void Warcaby::graj()
{
  if(StartGry())
  {
    while(okno.isOpen())
    {
      rysuj();
      okno.display();
      if(gracz)
      {
        ruchGracz();
        usleep(100000);
      }
      else ruchKomp();
      gracz =!gracz;
      if(koniecGry(plansza.SprawdzStan(gracz))) return;
    }
  }
}
bool Warcaby::StartGry()
{
  obiekty[5].setPosition(62, 330);
  if(strona) obiekty[5].setTextureRect(sf::IntRect(0,0,725,200));
  else obiekty[5].setTextureRect(sf::IntRect(0,200,725,200));
  rysuj();
  okno.draw(obiekty[5]);
  okno.display();
  sf::Event dzialaniee;
  while(true)
  {
    if(okno.pollEvent(dzialaniee))
    {
      switch(dzialaniee.type)
      {
        case sf::Event::Closed:
        return 0;
        case sf::Event::KeyPressed:
        if(dzialaniee.key.code == sf::Keyboard::Space) return 1;
        else if(dzialaniee.key.code == sf::Keyboard::Return) strona = !strona;
        gracz=!gracz;
        if(strona) obiekty[5].setTextureRect(sf::IntRect(0,0,725,200));
        else obiekty[5].setTextureRect(sf::IntRect(0,200,725,200));
        rysuj();
        okno.draw(obiekty[5]);
        okno.display();
        break;
      }
    }
  }
}
int Warcaby::koniecGry(int status)
{
  switch(status)
  {
    case 0:
    return 0;
    case -1:
    obiekty[5].setTextureRect(sf::IntRect(0,800,725,200));
    break;
    case 1:
    obiekty[5].setTextureRect(sf::IntRect(0,400,725,200));
    break;
    case 2:
    obiekty[5].setTextureRect(sf::IntRect(0,400,725,200));
    break;
  }
  obiekty[5].setPosition(62,330);
  rysuj();
  okno.draw(obiekty[5]);
  okno.display();
  sf::Event dzialanie;
  while(true)
  {
    if(okno.pollEvent(dzialanie))
    {
      switch(dzialanie.type)
      {
        case sf::Event::Closed:
        return 1;
        case sf::Event::KeyPressed:
        if(dzialanie.key.code == sf::Keyboard::Space)
        {
          plansza.restart();
          gracz = !strona;
          return !StartGry();
        }
      }
    }
  }
}
void Warcaby::ruchGracz()
{
  int pierwszy, drugi, rezultatRuchu{};
  do{
    if(rezultatRuchu == 0)
    {
      if((pierwszy=mysz())==-1) return;
    }
    do{
      if((drugi=mysz())==-1) return;
    } while(drugi == pierwszy);
    rezultatRuchu=plansza.ruch(pierwszy/10,pierwszy%10,drugi/10,drugi%10,strona);
    if(rezultatRuchu==2)
    {
      pierwszy=drugi;
      rysuj();
      okno.display();
    }
  } while(rezultatRuchu!=1);
}

void Warcaby::ruchKomp(bool drugi)
{
  auto ruch = AI::znajdzNajlepszyRuch(7,plansza,drugi? strona : !strona);
  for(auto& wspolrzednee : ruch.wspolrzedne)
  {
    plansza.ruch(wspolrzednee);
    rysuj();
    okno.display();
    usleep(100000);
  }
}

bool Warcaby::Dzialanie()
{
  sf::Event wydarzenie;
  while(okno.pollEvent(wydarzenie))
  {
    if(wydarzenie.type == sf::Event::Closed)
    {
      okno.close();
      return true;
    }
  }
  return false;
}

int Warcaby::mysz()
{
  sf::Vector2i pozycja;
  while(true)
  {
    if(Dzialanie()) return -1;
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
      pozycja=sf::Mouse::getPosition(okno);
      if(pozycja.x>25 && pozycja.x<825 && pozycja.y>25 && pozycja.y<825)
      {
        pozycja.y=pozycja.y-25;
        pozycja.x=pozycja.x-25;
        return (pozycja.y - pozycja.y%100)/10+(pozycja.x/100);
      }
    }
  }
}

void Warcaby::rysuj()
{
  okno.clear(sf::Color(41,41,41));
  okno.draw(obiekty[0]);
  for(int i=0; i<8; i++)
  {
    for(int j=0; j<8; j++)
    {
      switch(plansza.plansza[i][j])
      {
        case BIALY_PION:
        obiekty[1].setPosition(35+j*100, 35+i*100);
        okno.draw(obiekty[1]);
        break;
        case BIALA_DAMA:
        obiekty[2].setPosition(35+j*100, 35+i*100);
        okno.draw(obiekty[2]);
        break;
        case CZARNY_PION:
        obiekty[3].setPosition(35+j*100, 35+i*100);
        okno.draw(obiekty[3]);
        break;
        case CZARNA_DAMA:
        obiekty[4].setPosition(35+j*100, 35+i*100);
        okno.draw(obiekty[4]);
        break;
      }
    }
  }
}
