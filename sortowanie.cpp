#include <ctime>
#include "sortowanie.hh"
#include <fstream>
#include <string>


int main(){
  
  srand(time(nullptr));

  int liczba_tablic = 20;
  int rozmiary_tablic[] = {10000, 50000, 100000, 500000, 1000000};
  float proc_posort[] = {0, 0.25, 0.5, 0.75, 0.95, 0.99, 0.997};

  int u=0;
  filmy *tabds = new filmy[1010292],*tabdsl = new filmy[505146],*tabdsp = new filmy[505146];
  std::string dane,dane1,dane2;
  std::ifstream plik_do_sortowania("projekt2_dane.csv");
  std::ofstream plik_z_sortowaniem("sortowanie_csv");

  std::ofstream plik_przez_scalanie("sortowanie_przez_scalanie");
  std::ofstream plik_szybkie("sortowanie_szybkie");
  std::ofstream plik_introspektywne("sortowanie_introspektywne");
  int *tab1 = new int[1000000], *tab2 = new int[1000000], *tabl = new int[500000], *tabp = new int[500000];

  plik_przez_scalanie << "nr rozm ods_posort czas[s]\n";
  plik_szybkie        << "nr rozm ods_posort czas[s]\n";
  plik_introspektywne << "nr rozm ods_posort czas[s]\n";

  getline(plik_do_sortowania,dane);
  while(!plik_do_sortowania.eof()){
	  	
		getline(plik_do_sortowania,dane);
  		if(tabds[u].filmy_add(dane)==0)
		  {
			tabds[u].show();
			u=u+1;
		  }
			
    }
	std::cout << u << "\n";
	int rozmior=u;
	clock_t poczatek, koniec;
  for(unsigned a=0; a<3; ++a){
	  switch(a){
	  case 0:
	  
	    poczatek = clock();
	    sortowanie_przez_scalanie(tabds, 0, rozmior-1, tabdsl, tabdsp);
	    koniec = clock();
	    plik_z_sortowaniem << "Dla sortowania przez scalanie\n" << "Czas to: ";
	    plik_z_sortowaniem << double(koniec-poczatek)/CLOCKS_PER_SEC << std::endl;
		/*for(int i=0;i<700000;i+=10000)
		{
			tabds[i].show();
		}*/
	    break;

	  case 1:
	    poczatek = clock();
	    szybkie_sortowanie(tabds, 0, rozmior-1);
	    koniec = clock();
	    plik_z_sortowaniem << "Dla quicksorta\n" << "Czas to: ";
	    plik_z_sortowaniem << double(koniec-poczatek)/CLOCKS_PER_SEC << std::endl;
		/*for(int i=0;i<700000;i+=10000)
		{
			tabds[i].show();
		}*/
	    break;

	  case 2:
	    poczatek = clock();
	    sortowanie_introspektywne(tabds, rozmior);
	    koniec = clock();
	    plik_z_sortowaniem << "Dla sortowania introspektywnego\n" << "Czas to: ";
	    plik_z_sortowaniem << double(koniec-poczatek)/CLOCKS_PER_SEC << std::endl;
		/*for(int i=0;i<700000;i+=10000)
		{
			tabds[i].show();
		}*/
	    break;
	  }
	  
	}

  for(unsigned r=0; r<sizeof(rozmiary_tablic)/sizeof(*rozmiary_tablic); ++r){
    unsigned rozmiar = rozmiary_tablic[r];
    
    for(unsigned nr=0; nr<liczba_tablic; ++nr){
      clock_t poczatek, koniec;
      
      for(unsigned i=0; i<rozmiar; ++i)
	    tab1[i] = rand() % int(60e6) - 30e6;
      
      for(unsigned p=0; p<sizeof(proc_posort)/sizeof(*proc_posort); ++p){
	szybkie_sortowanie(tab1, 0, unsigned(proc_posort[p]*rozmiar));	
	
	if(!sprawdz_sortowanie(tab1, unsigned(proc_posort[p]*rozmiar)))
	  std::cerr << "uUuU1\n";

	przepisz_tablice(tab1, tab2, rozmiar);

	if(sprawdz_sortowanie(tab1, rozmiar))
	    std::cerr << "uUuU4\n";
        
	for(unsigned a=0; a<3; ++a){
	  przepisz_tablice(tab2, tab1, rozmiar);
	  if(sprawdz_sortowanie(tab1, rozmiar))
	    std::cerr << "uUuU3\n";
	  switch(a){
	  case 0:
	    poczatek = clock();
	    sortowanie_przez_scalanie(tab1, 0, rozmiar-1, tabl, tabp);
	    koniec = clock();
	    plik_przez_scalanie << nr << " " << rozmiar << " " << proc_posort[p] << " ";
	    plik_przez_scalanie << double(koniec-poczatek)/CLOCKS_PER_SEC << std::endl;

	    break;

	  case 1:
	    poczatek = clock();
	    szybkie_sortowanie(tab1, 0, rozmiar-1);
	    koniec = clock();
	    plik_szybkie << nr << " " << rozmiar << " " << proc_posort[p] << " ";
	    plik_szybkie << double(koniec-poczatek)/CLOCKS_PER_SEC << std::endl;
	    break;

	  case 2:
	    poczatek = clock();
	    sortowanie_introspektywne(tab1, rozmiar);
	    koniec = clock();
	    plik_introspektywne << nr << " " << rozmiar << " " << proc_posort[p] << " ";
	    plik_introspektywne << double(koniec-poczatek)/CLOCKS_PER_SEC << std::endl;
	    break;
	  }
	  if(!sprawdz_sortowanie(tab1, rozmiar)){
	    std::cerr << "uUuU2\n " << a << " " << p << std::endl;
	  }
	}
	
	przepisz_tablice(tab2, tab1, rozmiar);
      }

      szybkie_sortowanie(tab1, 0, rozmiar-1);
      for(unsigned a=0; a<3; ++a){
	if(!sprawdz_sortowanie(tab1, rozmiar))
	  std::cerr << "uUuUa\n";
	odwroc_tablice(tab1, rozmiar);
	switch(a){
	case 0:
	  poczatek = clock();
	  sortowanie_przez_scalanie(tab1, 0, rozmiar-1, tabl, tabp);
	  koniec = clock();
	  plik_przez_scalanie << nr << " " << rozmiar << " " << -1 << " ";
	  plik_przez_scalanie << double(koniec-poczatek)/CLOCKS_PER_SEC << std::endl;
	  break;

	case 1:
	  poczatek = clock();
	  szybkie_sortowanie(tab1, 0, rozmiar-1);
	  koniec = clock();
	  plik_szybkie << nr << " " << rozmiar << " " << -1 << " ";
	  plik_szybkie << double(koniec-poczatek)/CLOCKS_PER_SEC << std::endl;
	  break;

	case 2:
	  poczatek = clock();
	  sortowanie_introspektywne(tab1, rozmiar);
	  koniec = clock();
	  plik_introspektywne << nr << " " << rozmiar << " " << -1 << " ";
	  plik_introspektywne << double(koniec-poczatek)/CLOCKS_PER_SEC << std::endl;
	  break;
	}
	if(!sprawdz_sortowanie(tab1, rozmiar))
	  std::cerr << "uUuU3\n";
	  }
      
      przepisz_tablice(tab2, tab1, rozmiar);
    }
  }

  delete[] tab1;
  delete[] tab2;
  delete[] tabl;
  delete[] tabp;
  
  return 0;
  
}
