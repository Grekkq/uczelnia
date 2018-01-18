#include <iostream> //test
#include <fstream>
#include <string>

using namespace std;

//Struktura ksi¹¿ki
struct ksiazka_ele {
	string author;
	string title;
	ksiazka_ele *next;
	//ksiazka_ele() { next = nullptr; };
};
//Pocz¹tek i koniec listy ksi¹¿ek
struct ksiazka {
	ksiazka_ele *head;
	ksiazka_ele *tail;
};

//Lista wskaŸników na ksi¹¿ki
struct wsk_ksiazka_ele {
	ksiazka_ele * ptr_to_ksiazka;
	wsk_ksiazka_ele * next;
};
//Pocz¹tek i koniec listy wskaŸników na ksi¹¿ki
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
//Pocz¹tek i koniec listy etykiet
struct lista {
	lista_ele *head;
	lista_ele *tail;
};

ksiazka_ele* dodajKsiazke(ksiazka &ksiazkaGIO, string & autor, string & tytul);
void przeszukajEtykiety(lista &listaGIO, string & etykieta, ksiazka_ele* ksiazka_do_dodania);
lista_ele* dodajEtykiete(lista &listaGIO, string & etykieta);
void dodajKsiazkeDoEtykiety(lista_ele* tu_dodaj, ksiazka_ele* ksiazka_do_dodania);


int main() {
	//Sprawdzenie argumentow inicjalizujacych

	//Wczytanie pliku
	string nazwa = "PPK_projekt.txt";
	ifstream plik;
	plik.open(nazwa);
	if (!plik.good()) {
		cerr << "Blad odczytu pliku wejsciowego";
		exit(1);
	}

	//inicjalizacja struktur?
	ksiazka wszystkieKsiazki;
	wszystkieKsiazki.head = nullptr;
	wszystkieKsiazki.tail = nullptr;
	lista wszystkieEtykiety;
	wszystkieEtykiety.head = nullptr;
	wszystkieEtykiety.tail = nullptr;


	//Wczytanie danych
	string rec;
	string autor, tytul, etykieta;
	while (getline(plik, rec)) {
		//autor i tytul
		size_t pos = rec.find("; ");
		autor = rec.substr(0, pos);
		rec = rec.substr(pos + 2);
		pos = rec.find("; ");
		tytul = rec.substr(0, pos);
		rec = rec.substr(pos + 2);
		ksiazka_ele* to_dodac = dodajKsiazke(wszystkieKsiazki, autor, tytul);
		//etykiety
		bool znalazl = true;
		pos = rec.find(", ");
		if (pos == string::npos)
			etykieta = rec; else {
			etykieta = rec.substr(0, pos);
			znalazl = false;
		}
		rec = rec.substr(pos + 2);
		do {
			//wymyœliæ jak przeczytaæ tylko jedn¹ etykiete i ja wyizolowaæ i nie zapomnieæ reszty bo póŸniej do niej przejœæ
			przeszukajEtykiety(wszystkieEtykiety, etykieta, to_dodac);
			if (znalazl) {
				break;
			}
			else {
				etykieta = rec;
				przeszukajEtykiety(wszystkieEtykiety, etykieta, to_dodac);
			}
			pos = rec.find(", ");
			if (pos == std::string::npos)
				break;
			etykieta = rec.substr(0, pos);
			rec = rec.substr(pos + 2);


			//rec = rec.substr(pos + 2);
			//pos = rec.find(", ");
		} while (etykieta.length());
	}


	//for(int i = 0; i <5; i++) nie dziala bo to nie string w zmiennej tylko sam a przekazuje przez wartosc
	/*
	string test1 = "Ja", test2 = "Ja2", test3 = "Ja3", test = "Bo nie ty";
	dodajKsiazke(wszystkieKsiazki, test1, test);
	dodajKsiazke(wszystkieKsiazki, test2, test);
	dodajKsiazke(wszystkieKsiazki, test3, test);
	*/

	cout << "DOBRZE PRAWIE DOBRZE!";
	return 0;
}
//TO DZIALA
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

//DZIA£A
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

//DZIA£A
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

//DZIA£A
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