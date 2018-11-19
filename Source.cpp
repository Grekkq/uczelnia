#include "Class.h"

using namespace std;

int main(void)
{

	cout << "zadanie 1" << endl << endl;
	string nazwa_pliku="plik.txt";
	CountNumbers(nazwa_pliku);
	cout << endl << "zadanie 2" << endl << endl;
	nazwa_pliku = "plik2.txt";
	PrepareQueue(nazwa_pliku);
	cout << endl << "zadanie 3" << endl<<endl;
	nazwa_pliku = "plik3.txt";
	RouteMap map(nazwa_pliku);
	system("pause");
}