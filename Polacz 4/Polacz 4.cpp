#include<SDL.h>
#include<iostream>
#include<time.h>
#include "mapa.hpp"
#include "Tablica.hpp"
#include "Sortownik.hpp"
Tablica tablica;
using namespace std;
unsigned long long rekurecie;
short odleglosc(short x1, short y1, short x2, short y2) {
	return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
}
void kolo(SDL_Renderer* renderer, short x, short y, short r) {
	for (short i = x - r; i < x + r; i++) {
		for (short j = y - r; j < y + r; j++) {
			if (odleglosc(x, y, i, j) < r * r) {
				SDL_RenderDrawPoint(renderer, i, j);
			}
		}
	}
}
short przypuszczenie(mapa mapa, unsigned long long ruch, bool przeciwnik, short iteracje, short alfa, short beta, short kolejnosc[]) {
	rekurecie++;
	if (iteracje) {
		mapa.zagraj_ze_zmiana(ruch, przeciwnik);
		if (mapa.czy_remis()) {
			return 0;
		}
		if (mapa.czy_wygrana()) {
			return mapa.ocen(przeciwnik);
		}
		short wartosc = tablica.get(mapa.klucz());
		if (wartosc != 23) {
			return wartosc;
		}
		unsigned long long mozliwosci = mapa.mozliwe_zagrania();
		if (!mozliwosci) {
			return mapa.ocen(przeciwnik);
		}
		Sortownik sortownik;
		for (short i = 0; i < 7; i++) {
			ruch = mozliwosci & mapa.maska_kolumny(kolejnosc[i]);
			if (ruch) {
				sortownik.put(ruch, mapa.ocen_ruch(ruch));
			}
		}
		short ekstremum_punkty, punkty;
		bool ekstremum_niepobrane = true;
		while ((ruch = sortownik.get()) != -1) {
			if (alfa < beta) {
				if (ruch) {
					punkty = przypuszczenie(mapa, ruch, !przeciwnik, iteracje - 1, alfa, beta, kolejnosc);
					if (przeciwnik) {
						if (ekstremum_punkty < punkty || ekstremum_niepobrane) {
							ekstremum_punkty = punkty;
							alfa = ekstremum_punkty;
							ekstremum_niepobrane = false;
						}
					}
					else {
						if (ekstremum_punkty > punkty || ekstremum_niepobrane) {
							ekstremum_punkty = punkty;
							beta = ekstremum_punkty;
							ekstremum_niepobrane = false;
						}
					}
				}
			}
			else {
				break;
			}
		}
		tablica.put(mapa.klucz(), ekstremum_punkty);
		return ekstremum_punkty;
	}
	return 0;
}
short solver(mapa mapa, short alfa, short beta) {
	short iteracje = 25, punkty[7], max_punkty = -22, kolejnosc[7], kolejnosc_szukania[7], licznik = 0;
	kolejnosc[0] = 3;
	kolejnosc[1] = 2;
	kolejnosc[2] = 4;
	kolejnosc[3] = 1;
	kolejnosc[4] = 5;
	kolejnosc[5] = 0;
	kolejnosc[6] = 6;
	unsigned long long mozliwosci = mapa.mozliwe_zagrania(), ruch;
	Sortownik sortownik;
	for (short i = 0; i < 7; i++) {
		ruch = mozliwosci & mapa.maska_kolumny(kolejnosc[i]);
		if (ruch) {
			sortownik.put(ruch, mapa.ocen_ruch(ruch));
		}
	}
	while ((ruch = sortownik.get()) != -1) {
		if (alfa < beta) {
			if (ruch) {
				//zapisanie kolejnosci ruchow
				if (ruch & mapa.maska_kolumny(0)) {
					kolejnosc_szukania[licznik] = 0;
				}
				else if (ruch & mapa.maska_kolumny(1)) {
					kolejnosc_szukania[licznik] = 1;
				}
				else if (ruch & mapa.maska_kolumny(2)) {
					kolejnosc_szukania[licznik] = 2;
				}
				else if (ruch & mapa.maska_kolumny(3)) {
					kolejnosc_szukania[licznik] = 3;
				}
				else if (ruch & mapa.maska_kolumny(4)) {
					kolejnosc_szukania[licznik] = 4;
				}
				else if (ruch & mapa.maska_kolumny(5)) {
					kolejnosc_szukania[licznik] = 5;
				}
				else if (ruch & mapa.maska_kolumny(6)) {
					kolejnosc_szukania[licznik] = 6;
				}
				punkty[licznik] = przypuszczenie(mapa, ruch, false, iteracje, alfa, beta, kolejnosc);
				cout << "Punkty dla " << kolejnosc_szukania[licznik] << " pozycji: " << punkty[licznik] << endl;
				if (max_punkty < punkty[licznik]) {
					max_punkty = punkty[licznik];
					alfa = max_punkty;
				}
				licznik++;
			}
		}
		else {
			break;
		}
	}
	cout << "-------------------" << endl;
	for (short i = 0; i < licznik; i++) {
		if (max_punkty == punkty[i]) {
			return kolejnosc_szukania[i];
		}
	}
}
short AI(mapa mapa) {
	short min = (mapa.krazki_bota + mapa.krazki_gracza) / 2 - 22, max = 22 - (mapa.krazki_bota + mapa.krazki_gracza) / 2, srodek, r;
	while (min < max) {
		srodek = (min + max) / 2;
		if (srodek <= 0 && min / 2 < srodek) {
			srodek = min / 2;
		}
		else if (srodek >= 0 && max / 2 > srodek) {
			srodek = max / 2;
		}
		r = solver(mapa, srodek, srodek + 1);
		if (r <= srodek) {
			max = r;
		}
		else {
			min = r;
		}
	}
	return min;
}
//-------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
	// bot = 0
	// gracz = 1
	SDL_Window* window = SDL_CreateWindow("Polacz 4", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1260, 1000, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Rect prostokat;
	SDL_Event event;
	mapa mapa;
	//mapa.zagraj_bez_zmiany(3, true);
	//mapa.zagraj_bez_zmiany(5, true);
	//mapa.zagraj_bez_zmiany(3, true);
	//mapa.zagraj_bez_zmiany(3, false);
	//mapa.zagraj_bez_zmiany(5, true);
	time_t start;
	short podswietlenie = 8, wybor_AI, stan_gry = 2, wybor_gracz = -1;
	bool kolej = false; //<-- czy bot zaczyna pierwszy?
	while (stan_gry == 2) {
		//wejście
		SDL_PollEvent(&event);
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				stan_gry = 3;
			}
		}
		else if (event.type == SDL_MOUSEMOTION) {
			podswietlenie = event.button.x / 180;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			wybor_gracz = podswietlenie;
		}
		//logika gry
		if (kolej) {
			start = clock();
			wybor_AI = AI(mapa);
			cout << "Bot dokonal wyboru w " << clock() - start << " milisekund" << endl;
			cout << "Ilosc wywolania rekurecji: " << rekurecie << endl;
			rekurecie = 0;
			mapa.zagraj_bez_zmiany(wybor_AI, false);
			kolej = false;
		}
		else if (wybor_gracz != -1) {
			if (mapa.czy_mozna_zagrac(wybor_gracz)) {
				mapa.zagraj_bez_zmiany(wybor_gracz, true);
				kolej = true;
			}
			wybor_gracz = -1;
		}
		stan_gry = mapa.czy_ktos_wygral();
		//render klatki
		//czyszczenie
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		//render
		//render podświetlenia
		if (kolej) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 64, 255);
		}
		else {
			SDL_SetRenderDrawColor(renderer, 64, 0, 0, 255);
		}
		prostokat.x = 180 * podswietlenie;
		prostokat.y = 0;
		prostokat.w = 180;
		prostokat.h = 1080;
		SDL_RenderFillRect(renderer, &prostokat);
		//render kułek
		for (short i = 0; i < 7; i++) {
			for (short j = 0; j < 6; j++) {
				if (!mapa.get_maska(i, j)) {
					SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
				}
				else if (mapa.get_pozycje(i, j)) {
					SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
				}
				else {
					SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
				}
				kolo(renderer, i * 180 + 90, j * 180 + 90, 70);
			}
		}
		//wyświetlenie
		SDL_RenderPresent(renderer);
	}
	switch (stan_gry) {
	case -1:
		cout << "Czerwony wygrywa!!!" << endl;
		break;
	case 1:
		cout << "Niebieski wygrywa!!!" << endl;
		break;
	case 0:
		cout << "Remis" << endl;
		break;
	}
	system("pause");
	return 0;
}