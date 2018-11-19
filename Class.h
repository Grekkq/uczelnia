#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <list>
struct ascendingOrdering
{
	bool operator()(int a, int b) const 
	{
		return a > b;
	}
};
void CountNumbers(const std::string& file_name)
{
	std::ifstream File_in;
	File_in.open(file_name);
	if (File_in.good() == false)
	{
		std::cout << "Nie mozna otworzyc pliku " << file_name << std::endl;
		return;
	}
	std::map<int, int, ascendingOrdering> stats;
	int sign;
	while (File_in >> sign)
	{
			stats[sign] += 1;
	}
	File_in.close();
	for (auto e = stats.begin(); e != stats.end(); ++e) 
	{
		std::cout << e->first << " " << e->second << std::endl;
	}
	std::cout << std::endl;
	for (auto e = stats.rbegin(); e != stats.rend(); ++e) 
	{
		std::cout << e->first << " " << e->second << std::endl;
	}
}
class Patient
{
public:
	std::string Surname;
	int Age;
	Patient(const std::string& surname, int& age) : Surname(surname), Age(age) {};
};
struct ascendingOrdering1
{
	bool operator()(Patient a, Patient b) const {
		if (a.Age == b.Age)
		{
			return a.Surname[0] > b.Surname[0];
		}
		else
		{
			return a.Age < b.Age;
		}
	}
};
void PrepareQueue(const std::string& file_name)
{
	std::ifstream File_in;
	File_in.open(file_name);
	if (File_in.good() == false)
	{
		std::cout << "Nie mozna otworzyc pliku " << file_name << std::endl;
		return;
	}
	std::priority_queue<Patient, std::vector<Patient>, ascendingOrdering1> Kolejka;
	std::string text;
	int age;
	while (File_in >> text >> age)
	{
		Patient tmp(text, age);
		Kolejka.push(tmp);
	}
	File_in.close();
	while (!Kolejka.empty())
	{
		std::cout << Kolejka.top().Surname << " " << Kolejka.top().Age << std::endl;
		Kolejka.pop();
	}
}
class RouteMap
{
protected:
	std::vector<std::list<std::pair<int,int>>> wektor;
public:
	RouteMap(std::string& file_name)
	{
		std::ifstream File_in;
		File_in.open(file_name);
		if (File_in.good() == false)
		{
			std::cout << "Nie mozna otworzyc pliku " << file_name << std::endl;
			return;
		}
		int a,b,c,n,i=0;
		File_in >> n;
		wektor.resize(n);
		while (File_in >> a >> b >>c)
		{
			wektor[a].emplace_back(b, c);
			wektor[b].emplace_back(a, c);
		}
		for (int a = 0; a < wektor.size(); a++)
		{
			if (wektor[a].size() != 0)
			{
				std::cout << a << ": ";
				auto tmp = wektor[a].begin();
				for (int b = 0; b < wektor[a].size(); b++)
				{
					std::cout << tmp->first << "(" << tmp->second << ") ";
					tmp++;
				}
				std::cout << std::endl;
			}
		}
		File_in.close();
	};
};