#include <iostream> //test
#include <fstream>
#include <string>
#include <sstream> //istringstream::iss

using namespace std;

//Struktura ksi��ki
struct ksiazka_ele {
	string author;
	string title;
	ksiazka_ele *next;
	//ksiazka_ele() { next = nullptr; };
};
//Pocz�tek i koniec listy ksi��ek
struct ksiazka {
	ksiazka_ele *head;
	ksiazka_ele *tail;
};

//Lista wska�nik�w na ksi��ki
struct wsk_ksiazka_ele {
	ksiazka_ele * ptr_to_ksiazka;
	wsk_ksiazka_ele * next;
};
//Pocz�tek i koniec listy wska�nik�w na ksi��ki
struct wsk_ksiazka {
	wsk_ksiazka_ele * head;
	wsk_ksiazka_ele * tail;
};

//struktura etykiet
struct lista_ele {
	string label;
	wsk_ksiazka *list_ptr; //sortowana wedlug nazwiska
	lista_ele *next;
};
//Pocz�tek i koniec listy etykiet
struct lista {
	lista_ele *head;
	lista_ele *tail;
};

ksiazka_ele * dodajKsiazke(ksiazka & ksiazkaGIO, string & autor, string & tytul);
void przeszukajEtykiety(lista &listaGIO, string & etykieta, ksiazka_ele* ksiazka_do_dodania);
lista_ele* dodajEtykiete(lista &listaGIO, string & etykieta);
void dodajKsiazkeDoEtykiety(lista_ele* tu_dodaj, ksiazka_ele* ksiazka_do_dodania);
void wyswietlPomoc(string const & program);
void wypiszDoPliku(lista listaGIO, ofstream & plik);
void plikWyjsciowy(string const & wyjscie, ofstream & plik);
void plikWejsciowy(string const & wejscie, ifstream & plik);
string wyswietlKsiazke(ksiazka_ele* to_wyswietlic);


int main(int argc, char* argv[]) {
	//Sprawdzenie argumentow inicjalizujacych
	string zrodlo, wyjscie;
	if (argc != 5) {
		wyswietlPomoc(argv[0]);
	} else {
		bool in_flag = false;
		bool out_falg = false;
		short arg = 1;
		while (argv[arg]) {
			if (strcmp("-i", argv[arg]) == 0) {
				zrodlo = argv[arg + 1];
				arg += 2;
				in_flag = true;
			}
			else
				if (strcmp("-o", argv[arg]) == 0) {
					wyjscie = argv[arg + 1];
					arg += 2;
					out_falg = true;
				}
				else
					if (argv[arg] == "-h") {
						cout << "Sposob uzycia: " << argv[0] << " -i \"Plik_wejsciowy.txt\" -o \"Plik_wyjsciowy.txt\" " << endl;
						exit(68);
					}
					else wyswietlPomoc(argv[0]);
		}
		if (in_flag == false || out_falg == false) {
			wyswietlPomoc(argv[0]);
		}
	}

	//Wczytanie pliku
	ifstream plik_we;
	plikWejsciowy(zrodlo, plik_we);

	//Inicjalizacja struktur
	ksiazka wszystkieKsiazki;
		wszystkieKsiazki.head = nullptr;
		wszystkieKsiazki.tail = nullptr;
	lista wszystkieEtykiety;
		wszystkieEtykiety.head = nullptr;
		wszystkieEtykiety.tail = nullptr;

	//Wczytanie danych z pliku wejsciowego
	string rec, autor, tytul, etykieta;
	while (getline(plik_we, rec)) {
		//autor i tytul
		size_t pos = rec.find("; ");
		autor = rec.substr(0, pos);
		rec = rec.substr(pos + 2);
		pos = rec.find("; ");
		tytul = rec.substr(0, pos);
		rec = rec.substr(pos + 2);
		ksiazka_ele* to_dodac = dodajKsiazke(wszystkieKsiazki, autor, tytul); //dodanie ksiazki
		//etykiety
		istringstream etykiety_stream; //przerobienie etykiet na typ istringstream w celu �atwiejszego zap�tlenia
		etykiety_stream.str(rec);
		while (etykiety_stream >> etykieta) {
			pos = etykieta.find(",");
			etykieta = etykieta.substr(0, pos);
			przeszukajEtykiety(wszystkieEtykiety, etykieta, to_dodac);	//dodanie ksi��ki do etykiety i ewentualne jej stworzenie
		}
	}//Koniec wczytywania danych
	plik_we.close();
	
	//Utworzenie pliku wyjsciowego
	ofstream plik_wy;
	plikWyjsciowy(wyjscie, plik_wy);
	
	//Zapisanie wczytanych danych
	wypiszDoPliku(wszystkieEtykiety, plik_wy);



	cout << "DOBRZE PRAWIE DOBRZE!";
	return 0;
}
//DZIALA
	ksiazka_ele * dodajKsiazke(ksiazka & ksiazkaGIO, string & autor, string & tytul) {
	if (ksiazkaGIO.head == nullptr) {
		ksiazkaGIO.head = new ksiazka_ele{ autor,tytul,nullptr };
		ksiazkaGIO.tail = ksiazkaGIO.head;
		//ksiazkaGIO.tail->next = nullptr;
		return ksiazkaGIO.head;
	}
	else {
		ksiazkaGIO.tail->next = new ksiazka_ele;
		ksiazkaGIO.tail = ksiazkaGIO.tail->next;
		ksiazka_ele * tmp = ksiazkaGIO.tail;
		ksiazkaGIO.tail->author = autor;
		ksiazkaGIO.tail->title = tytul;
		ksiazkaGIO.tail->next = nullptr;
		//ksiazkaGIO.tail = ksiazkaGIO.tail->next;
		return tmp;
	}
}

//DZIA�A
void przeszukajEtykiety(lista &listaGIO, string & etykieta, ksiazka_ele* ksiazka_do_dodania) {
	//LISTA PUSTA
	if (listaGIO.head == nullptr) {
		lista_ele* tu_dodaj = dodajEtykiete(listaGIO, etykieta);
		dodajKsiazkeDoEtykiety(tu_dodaj, ksiazka_do_dodania);
	}
	else {
		//PRZESZUKIWANIE
		lista_ele * tmp_szukanie = listaGIO.head;
		while (tmp_szukanie->label != etykieta && tmp_szukanie->next != nullptr) {
			tmp_szukanie = tmp_szukanie->next;
			//if(tmp_szukanie->label)
		}
		//ZNALAZLO
		if (tmp_szukanie->label == etykieta) {
			dodajKsiazkeDoEtykiety(tmp_szukanie, ksiazka_do_dodania);
		}
		//NIE ZNALEZIONO
		else {
			lista_ele* tu_dodaj = dodajEtykiete(listaGIO, etykieta);
			dodajKsiazkeDoEtykiety(tu_dodaj, ksiazka_do_dodania);
		}
	}
}

//DZIA�A
lista_ele* dodajEtykiete(lista & listaGIO, string & etykieta) {
	//PUSTA LISTA
	if (listaGIO.head == nullptr) {
		listaGIO.head = new lista_ele{ etykieta,new wsk_ksiazka{ nullptr,nullptr } , nullptr };
		listaGIO.tail = listaGIO.head;
		//listaGIO.tail->next = nullptr;
		return listaGIO.head;
	}
	//DODANIE NA KONIEC
	else {
		listaGIO.tail->next = new lista_ele;
		listaGIO.tail = listaGIO.tail->next;
		lista_ele* tmp_tu_dodaj = listaGIO.tail;
		listaGIO.tail->label = etykieta;
		listaGIO.tail->list_ptr = new wsk_ksiazka{ nullptr,nullptr };
		//listaGIO.tail->next = new lista_ele;
		//listaGIO.tail = listaGIO.tail->next;
		listaGIO.tail->next = nullptr;
		return tmp_tu_dodaj;
	}

}

//DZIA�A bez sotrowania
void dodajKsiazkeDoEtykiety(lista_ele* tu_dodaj, ksiazka_ele* ksiazka_do_dodania) {
	//PIERWSZA KSIAZKA DO ETYKIETY
	if (tu_dodaj->list_ptr->head == nullptr) {
		tu_dodaj->list_ptr->head = new wsk_ksiazka_ele{ ksiazka_do_dodania, nullptr };
		tu_dodaj->list_ptr->tail = tu_dodaj->list_ptr->head;
	}
	//KOLEJNA KSIAZKA DO TEJ ETYKIETY
	else {
		tu_dodaj->list_ptr->tail->next = new wsk_ksiazka_ele;
		tu_dodaj->list_ptr->tail = tu_dodaj->list_ptr->tail->next;
		tu_dodaj->list_ptr->tail->ptr_to_ksiazka = ksiazka_do_dodania;
		tu_dodaj->list_ptr->tail->next = nullptr;
		//tu_dodaj->list_ptr->tail = tu_dodaj->list_ptr->tail->next;
	}
}

//DZIA�A
void wyswietlPomoc(string const & program)
{
	cerr << "Niepoprawne argumenty sprobuj: " << program << " -i \"Plik_wejsciowy.txt\" -o \"Plik_wyjsciowy.txt\" " << endl;
	exit(69);
}


//DZIWNIE DZIALA
void wypiszDoPliku(lista listaGIO, ofstream & plik) {
	lista_ele* etykiety_lista = listaGIO.head;
	while (etykiety_lista) {
		plik << etykiety_lista->label << ":\n";
		wsk_ksiazka_ele* to_zapisac = etykiety_lista->list_ptr->head;
		while (to_zapisac) {
			plik << wyswietlKsiazke(to_zapisac->ptr_to_ksiazka);
			to_zapisac = to_zapisac->next;
		}
		plik << "\n";
		etykiety_lista = etykiety_lista->next;
	}
}

//DZIA�A
void plikWyjsciowy(string const & wyjscie, ofstream & plik) {
	plik.open(wyjscie);
	if (!plik.good()) {
		cerr << "Blad zapisu pliku wyjsciowego";
		exit(404);
	}
}

//DZIA�A
void plikWejsciowy(string const & wejscie, ifstream & plik) {
	plik.open(wejscie,ios_base::app);
	if (!plik.good()) {
		cerr << "Blad odczytu pliku wejsciowego";
		exit(33);
	}
}

//NIE WIEM
string wyswietlKsiazke(ksiazka_ele* to_wyswietlic) {
	return "\t" + to_wyswietlic->author + "; " + to_wyswietlic->title +"\n";
}