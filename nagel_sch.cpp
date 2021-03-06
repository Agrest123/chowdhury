#include "stdafx.h"

using namespace std;
static int W = 100;	//Dlugosc drogi
bool isChecked = false; //flaga , sprawdzamy czy dany pojazd byl aktualizowany podczas zmiany pasa
class pojazd
{
public:
	int V; //aktualna predkosc pojazdu
	int Vmax;// maksymalna predkosc pojazdu 
	bool checked;// flaga sprawdzamy czy dany pojazd byl aktualizowany podczas zmiany pasa 
	pojazd() 
	{
		Vmax = rand() % 2 + 1; // losowanie max predkosc w przedziale od 1 - 2
		this->V = rand() % Vmax + 1; //losowanie aktualnej predkosci
		this->checked = !isChecked;
	}
	void wyswietl() 
	{
		cout<<V;
	}
	void speedup() {
		if (V < Vmax ) {
			V++;
		}
	}
	void speeddown() {
		if (V > 1) {
			V--;
		}
	}
};

void generator( pojazd ***&d)
{
	d[1][0] = new pojazd;
	d[0][0] = new pojazd;
}

void wyswietlanie(pojazd ***&d) {
	system("cls");
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < W; j++) {
			if (d[i][j] == NULL)
			{
				cout << '-';
			}
			else {
				d[i][j]->wyswietl();
			}
		}
		cout << endl;
	}

}
// Zlicza gapy - odleglosc luki przed pojazdem
int gap(pojazd ***d, int i, int j) {
	pojazd *t = d[i][j];
	int g = 0;
	while (d[i][j + 1 + g] == NULL && j + g + 1 < W) { g++; }
	return g;
}
// Zlicza gapy na drugim pasie - odleglosc luki do tylu
bool gap_back(pojazd ***d, int i, int j) {
	pojazd *t = d[i][j];
	int g = 0;
	while (j - g - 1 >= 0 && d[i][j - g - 1 ] == NULL) { g++; }
	if (j == g) return 0;// brak pojazdow na pasie
	if (d[i][j - g - 1]->Vmax < g) return true;
	return false;
}

void odswiezanie(pojazd ***&d)
{
	pojazd *t;
	
	int x, y;
	/*zmiana pasa*/
	for (int i = 0, l = 1; i < 2; i++, l--) {
		for (int j = 0; j < W; j++) {
			t = d[i][j];
			x = y = 0;
			if (t != NULL && t->checked != isChecked)
			{
				t->checked = isChecked; //zmiana co cykl flagi
				x = gap(d, i, j); // x - dlugosc luki
				if (x < t->V + 1 && d[l][j] == NULL /*&& t->V>1*/) {
					d[l][j] = t;
					if (gap_back(d, l, j)) {
						d[i][j] = NULL;
					}
					else {
						d[l][j] = NULL;
					}
				}
			}
		}
	}
	isChecked = !isChecked;

	/**/
	pojazd ***tmp = new pojazd**[2];
	tmp[0] = new pojazd*[W];
	tmp[1] = new pojazd*[W];
	for (int i = 0; i < 2; i++) { for (int j = 0; j < W; j++) { tmp[i][j] = NULL; } }
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < W; j++)
		{
			t = d[i][j];
			if (t != NULL)
			{
				y = gap(d, i, j);// y - dlugosc luki
				if (y > t->V) { t->speedup(); }
				else if (y > 0) { t->V = y; }
				else {	t->V = 1; }
				if (rand() % 100 < 25) { t->speeddown(); }//prawdopodobienstwo hamowania
				x = j + t->V; //przyszla pozycja
				if (x < W) { tmp[i][x] = t; }
				else { delete t; }
				d[i][j] = NULL;
			}
		}
	}
	delete[]d;
	d = tmp;
	tmp = NULL;
}


int main()
{
	srand(time(NULL));
	pojazd ***d = new pojazd**[2];
	d[0] = new pojazd*[W];
	d[1] = new pojazd*[W];
	for (int i = 0; i < 2; i++) { for (int j = 0; j < W; j++) { d[i][j] = NULL; } }

	int nr = 0;
	int flaga = 0;

	while (nr<50) {
		if (flaga == 0)
		{
			flaga = 4;
			nr++;
			generator(d);
		}
		else flaga--;
		wyswietlanie(d);
		odswiezanie(d);
		Sleep(300);
		
		
	}

	
	cout << endl;

	system("PAUSE");
	delete []d;

    return 0;
}

