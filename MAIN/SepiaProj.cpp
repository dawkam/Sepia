// SepiaProj.cpp : Defines the entry point for the console application.
//
#include <fstream>
#include <thread>
#include<windows.h>
#include<string>
#include <iostream>
#include <chrono> 
using namespace std::chrono;

using namespace std;

#define DLUGOSC_NAGLOWKA  54
#define INDEX_SZEROKOSCI 18
#define ZAKRES_SZEROKOSCI 4
#define INDEX_WYSOKOSC 22
#define ZAKRES_WYSOKOSCI 4
#define CONWERTER_16 256


int main(int argc, char* agrv[]) {

	if (argc != 4) {
		cout << "Bledne dane" << endl;
		return 0;
	}

	ifstream wej(agrv[1], ios::binary);
	//ifstream wej("2.bmp", ios::binary);
	if (!wej)
	{
		cout << "Nie mozna otworzyc pliku" << endl;
		wej.close();
		return 0;
	}
	ofstream wyj("ObrazWynikowy.bmp", ios::binary);

	wej.seekg(0, wej.end);
	int dlugosc_pliku = wej.tellg();
	wej.seekg(0, wej.beg);

	unsigned char* naglowek = new unsigned char[DLUGOSC_NAGLOWKA];
	int dlugosc_tab = dlugosc_pliku - DLUGOSC_NAGLOWKA;
	unsigned char* tab = new unsigned char[dlugosc_tab];
	char znak_przechwycony;																	// zmienna pomocnicza do rzutowania

	int szerokosc = 0, dopelnienie_bajtow = 0 ,wysokosc = 0, mnoznik = 0;

	//rdzenie
	unsigned concurentThreadsSupported = std::thread::hardware_concurrency();
	cout << "Liczba dostepnych rdzeni: " << concurentThreadsSupported << endl;

	//plik wej
	for (int i = 0; i < DLUGOSC_NAGLOWKA; i++)
	{
			wej.get(znak_przechwycony);
			wyj.put(znak_przechwycony);
			naglowek[i] = static_cast<unsigned char>(znak_przechwycony);
	}
	mnoznik = 1;								//reset mno¿nika potrzebnego do konwersji na liczbê dziesiêtn¹ 

	for (int i = 0; i < ZAKRES_SZEROKOSCI; i++) 
	{
		szerokosc += naglowek[INDEX_SZEROKOSCI + i] * mnoznik;
		mnoznik *= CONWERTER_16;
	}
	mnoznik = 1;
	for (int i = 0; i < ZAKRES_WYSOKOSCI; i++)
	{
		wysokosc += naglowek[INDEX_WYSOKOSC + i] * mnoznik;
		mnoznik *= CONWERTER_16;
	}

	dopelnienie_bajtow = (4 - ((szerokosc * 3) % 4)) % 4;		
	// do tablicy nie sa zapisywane zera dopeleniajace do podzielnosci przez 4
	for (int i = 0; i < (szerokosc*wysokosc*3); )
	{
		for (int j = 0; j < szerokosc*3; j++)
		{
			wej.get(znak_przechwycony);
			tab[i] = static_cast<unsigned char>(znak_przechwycony);
			i++;
		}
		for (int j = 0; j < dopelnienie_bajtow; j++) 
		{
			wej.get(znak_przechwycony);
		}
	}
	wej.close();

	int ilosc_watkow=1;

	ilosc_watkow=atoi(agrv[2]);
	//ilosc_watkow = 4;
	//w¹tki
	thread * watki = new thread[ilosc_watkow];
	string tmp=agrv[3];
	if (tmp=="cpp") {
		//dll cpp
		typedef void(_fastcall *zmiana_na_sepie_c)(unsigned char*&, int, int);
		HINSTANCE dllHandle1 = NULL;

		zmiana_na_sepie_c procedura = NULL;
		dllHandle1 = LoadLibrary(L"ProjCpp.dll");
		if (dllHandle1 != NULL)
		{
			procedura = (zmiana_na_sepie_c)GetProcAddress(dllHandle1, "zmiana_na_sepie_c");
			int zakres = (dlugosc_tab / ilosc_watkow)
				- (dlugosc_tab / ilosc_watkow) % 3;

			auto start = high_resolution_clock::now();
			
			for (int i = 0; i < ilosc_watkow - 1; i++) {
				watki[i] = thread(procedura, ref(tab), zakres * i, zakres);
			}
			watki[ilosc_watkow - 1] = thread(procedura, ref(tab), zakres * (ilosc_watkow - 1),
				dlugosc_tab - (ilosc_watkow - 1) * zakres);

			for (int i = 0; i < ilosc_watkow; i++) {
				watki[i].join();
			}
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Time taken by function: " << duration.count() << " microseconds" << endl;


		}
	}else if (tmp=="asm" ){
		//dll asm
		typedef void(_fastcall *zmiana_na_sepie_asm)(unsigned char*, int, int);

		HINSTANCE dllHandle = NULL;

		zmiana_na_sepie_asm procedura1 = NULL;
		dllHandle = LoadLibrary(L"ProjAsm.dll");
		if (dllHandle != NULL)
		{
			procedura1 = (zmiana_na_sepie_asm)GetProcAddress(dllHandle, "zmiana_na_sepie_asm");
			int zakres = (dlugosc_tab / ilosc_watkow)
				- (dlugosc_tab / ilosc_watkow) % 3;

			auto start = high_resolution_clock::now();
			for (int i = 0; i < ilosc_watkow - 1; i++) {
				watki[i] = thread(procedura1, ref(tab), zakres * i, zakres);
			}
			watki[ilosc_watkow - 1] = thread(procedura1, ref(tab), zakres * (ilosc_watkow - 1),dlugosc_tab - (ilosc_watkow - 1) * zakres);
									
			for (int i = 0; i < ilosc_watkow; i++) {
				watki[i].join();
			}
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<microseconds>(stop - start);
			cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
		}
	}else{
				cout << "Error" << endl;
				return 0;

	}

		dopelnienie_bajtow = szerokosc % 4;
		for (int i = 0; i < (szerokosc*wysokosc * 3); )
		{
			for (int j = 0; j < szerokosc * 3; j++)
			{
				wyj.put(tab[i]);
				i++;
			}
			for (int j =  0; j < dopelnienie_bajtow; j++)
			{
				wyj.put(0);		//dopelnienie do 4
			}
		}

	wyj.close();
	return 0;
}


