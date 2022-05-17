#include <iostream>
#include <cmath>
class filmy
{
private:
  std::string tytul;
  int ranking;
public:
  filmy();
  filmy(filmy &film);
  filmy(filmy &&film);
  ~filmy();
  int filmy_add(std::string wiersz);
  void show();
  bool operator < (filmy film);
  bool operator > (filmy film);
  bool operator <= (filmy film);
  filmy& operator = (const filmy &film);
  filmy& operator = (const filmy &&film);
};

filmy::filmy()
{
}

filmy::filmy(filmy &film)
{
  this->tytul = film.tytul;
  this->ranking = film.ranking;
}

filmy::filmy(filmy &&film)
{
  this->tytul = film.tytul;
  this->ranking = film.ranking;
}

filmy::~filmy()
{
}
int filmy::filmy_add(std::string wiersz)
{
  int od1,od2,x;
  
  std::string tytulod1,rankingod2;
  od1=wiersz.find_first_of(",")+1;
  od2=wiersz.find_last_of(",")+1;
  
  if (od2<wiersz.length()){
    tytulod1=wiersz[od1];
    rankingod2=wiersz[od2];
    
    for(;od1<od2-1;){
      
      tytulod1.push_back(wiersz[od1+1]);
      od1=od1+1;
    }
    
    for(int i=od2;od2<i+2;++od2)
      rankingod2.push_back(wiersz[od2+1]);

    this->ranking=std::stoi(rankingod2);
    this->tytul=tytulod1;
    return 0;
  }
  return 1;
}

void filmy::show()
{
  std::cout << this->tytul  << this->ranking << std::endl; 
}

bool filmy::operator < (filmy film)
{
  if(this->ranking<film.ranking)
  {
    return true;
  }
  return false;
}

bool filmy::operator > (filmy film)
{
  if(this->ranking>film.ranking)
  {
    return true;
  }
  return false;
}

bool filmy::operator <= (filmy film)
{
  if(this->ranking<=film.ranking)
  {
    return true;
  }
  return false;
}

filmy& filmy::operator = (const filmy &film)
{
  this->tytul = film.tytul;
  this->ranking = film.ranking;
  return *this;
}

filmy& filmy::operator = (const filmy &&film)
{
  this->tytul = film.tytul;
  this->ranking = film.ranking;
  return *this;
}

template<typename T>
int podziel_tablice(T *tab, int i0, int i1){
  int i = i0-1, j = i1+1;
  //T os = tab[(i0+i1)>>1];
  T os = tab[i0];

  while(true){
    do{++i;}while(tab[i] < os);
    do{--j;}while(tab[j] > os);
    if(i >= j) return j;
    std::swap(tab[i], tab[j]);
  }
  
}

template<typename T>
void szybkie_sortowanie(T *tab, unsigned int i0, unsigned int i1){
  /*int l,p,podzial;
  
  stos->push(i0); stos->push(i1);

  while(!stos->is_empty()){
    stos->pop(p); stos->pop(l);

    while(l < p){
      podzial = podziel_tablice(tab, l, p);

      if(podzial-l <= p-podzial){
	stos->push(podzial+1); stos->push(p);
	p = podzial;
      } else{
	stos->push(l); stos->push(podzial);
	l = podzial+1;
      }
    }
    }*/

  int podzial;

  while(i1 > i0){
    podzial = podziel_tablice(tab, i0, i1);

    if(podzial-i0 <= i1-podzial){
      szybkie_sortowanie(tab, i0, podzial);
      i0 = podzial+1;
    } else{
      szybkie_sortowanie(tab, podzial+1, i1);
      i1 = podzial;
    }
  }

  
}

template<typename T>
void scal(T *tab, unsigned i0, unsigned srodek, unsigned  i1, T *tab_lewa, T *tab_prawa){
  unsigned Nl = srodek-i0+1, Np = i1-srodek;

  for(int i = 0; i < Nl; ++i)
    tab_lewa[i] = tab[i0+i];
  for(int i = 0; i < Np; ++i)
    tab_prawa[i] = tab[srodek+i+1];

  int il = 0, ip = 0, i=i0;

  while(il < Nl && ip < Np){
    if(tab_lewa[il] <= tab_prawa[ip]){
      tab[i] = tab_lewa[il];
      ++i; ++il;
    }
    else{
      tab[i] = tab_prawa[ip];
      ++i; ++ip;
    }
  }

  while(il < Nl){
    tab[i] = tab_lewa[il];
    ++i; ++il;
  }
  while(ip < Np){
    tab[i] = tab_prawa[ip];
    ++i; ++ip;
  }
}

template<typename T>
void sortowanie_przez_scalanie(T *tab, unsigned i0, unsigned i1, T *tab_lewa, T *tab_prawa){

  if(i0 >= i1) return;

  int srodek = (i0+i1)>>1;

  sortowanie_przez_scalanie(tab, i0, srodek, tab_lewa, tab_prawa);
  sortowanie_przez_scalanie(tab, srodek+1, i1, tab_lewa, tab_prawa);
  scal(tab, i0, srodek, i1, tab_lewa, tab_prawa);
}

template<typename T>
void uporzadkuj_podkopiec(T *tab, unsigned rozmiar, unsigned i){
  int rodzic, aktualny=i;
  while(true){
    rodzic = aktualny;
    int lewe_dziecko = 2*rodzic+1, prawe_dziecko = lewe_dziecko+1;
    
    if(lewe_dziecko < rozmiar && tab[lewe_dziecko] > tab[aktualny])
      aktualny = lewe_dziecko;
    if(prawe_dziecko < rozmiar && tab[prawe_dziecko] > tab[aktualny])
      aktualny = prawe_dziecko;
    
    if(aktualny == rodzic)
      break;
    
    std::swap(tab[aktualny], tab[rodzic]);
  }
}

template<typename T>
void uporzadkuj_kopiec(T *tab, unsigned int N){
  for(int i = N/2-1; i >= 0; --i)
    uporzadkuj_podkopiec(tab, N, i);
}

template<typename T>
void sortowanie_przez_kopcowanie(T *tab, unsigned int N){
  uporzadkuj_kopiec(tab, N);

  for(int i=N-1; i > 0; --i){
    std::swap(tab[0], tab[i]);
    uporzadkuj_podkopiec(tab, i, 0);
  }
}

template<typename T>
void sortowanie_przez_wstawianie(T *tab, unsigned int N){
  T wstawiany_elem;
  int ind_porownywanego;
  
  for(int i = 1; i < N; ++i){
    wstawiany_elem = tab[i];
    ind_porownywanego = i-1;
    
    while(ind_porownywanego >= 0 && tab[ind_porownywanego] > wstawiany_elem){
      tab[ind_porownywanego + 1] = tab[ind_porownywanego];
      --ind_porownywanego;
    }
    
    tab[ind_porownywanego+1] = wstawiany_elem;
  }
 } 

/*template<typename T>
void introsort(T *tab, unsigned int N, int glebokosc_rekurencji){  
  /*int l, p, podzial, glebokosc=0;
  
  stos->push(0); stos->push(N-1);

  while(!stos->is_empty()){
    stos->pop(p); stos->pop(l);
    ++glebokosc;
    if(glebokosc_rekurencji >= glebokosc_rekurencji){
    sortowanie_przez_kopcowanie(tab, N);
    return;
    }
    while(l < p){
      podzial = podziel_tablice(tab, l, p);
	  
      if(podzial-l <= p-podzial){
	if(p-podzial > 9){
	  stos->push(podzial+1); stos->push(p);
	}
	if(podzial-l > 9)
	  p = podzial;
	else
	  break;
      } else{
	if(podzial-l> 9){
	  stos->push(l); stos->push(podzial);
	}
	if(p-podzial > 9)
	  l = podzial+1;
	else
	  break;
      }
    }
    }* /

  if(N <=1)
    return;
  if(N <= 20){
    sortowanie_przez_wstawianie(tab, N);
    return;
    }
  if(glebokosc_rekurencji <= 0)
    sortowanie_przez_kopcowanie(tab, N);

  else{
  int podzial;

  while(N > 1){
    podzial = podziel_tablice(tab, 0, N-1);

    if(podzial <= N-1-podzial){
      introsort(tab, podzial+1, glebokosc_rekurencji-1);
      tab = tab+podzial;
    } else{
      introsort(tab+podzial+1, N-podzial-1, glebokosc_rekurencji-1);
      N = podzial;
    }
  }
  }
  }*/


template<typename T>
void introsort(T *tab, unsigned i0, unsigned i1, int glebokosc_rekurencji){
  if(i0 >= i1)
    return;
  if(i1-i0 <= 20){
    sortowanie_przez_wstawianie(tab+i0, i1-i0+1);
    return;
    }
  if(glebokosc_rekurencji <= 0)
    sortowanie_przez_kopcowanie(tab+i0, i1-i0+1);
  else{
    int podzial = podziel_tablice(tab, i0, i1);
    
    introsort(tab, i0, podzial, glebokosc_rekurencji-1);
    introsort(tab, podzial+1, i1, glebokosc_rekurencji-1);
  }
}


template<typename T>
void sortowanie_introspektywne(T *tab, unsigned int N){
  introsort(tab, 0, N-1, 2*floor(log2(N)));
}

template<typename T>
bool sprawdz_sortowanie(T *tab, unsigned int N){
  for(int i = 1; i < N; ++i)
    if(tab[i] < tab[i-1])
      return false;

  return true;
}

template<typename T>
void odwroc_tablice(T *tab, unsigned int N){
  for(int i = 0; i < N/2; ++i)
    std::swap(tab[i], tab[N-1-i]);
}

template<typename T>
void przepisz_tablice(T *tab1, T *tab2, unsigned int N){
  for(int i = 0; i < N; ++i)
    tab2[i] = tab1[i];
}

template<typename T>
void wyswietl_tablice(T *tab, unsigned int N){
  for(int i = 0; i < N; ++i)
    std::cout << tab[i] << " ";
  std::cout << "\n";
}
