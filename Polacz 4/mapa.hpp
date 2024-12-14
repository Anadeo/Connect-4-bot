class mapa {
private:
	struct plansza {
		unsigned long long pozycje = 0;
		unsigned long long maska = 0;
	};
	plansza plansza;
	unsigned long long dolna_maska(short kolumna) {
		return (unsigned long long)1 << 7 * kolumna;
	}
	unsigned long long gorna_maska(short kolumna) {
		return (unsigned long long)1 << 7 * kolumna + 5;
	}
	unsigned long long mozliwosci() {
		return (plansza.maska + 4432676798593) & 279258638311359;
	}
	unsigned long long wygrane_miejsca_przeciwnika(unsigned long long mapa) {
		//pionowo
		unsigned long long wygrane = mapa << 1 & mapa << 2 & mapa << 3;
		//poziomo
		unsigned long long pomoc = mapa << 7 & mapa << 14;
		wygrane |= pomoc & mapa << 21;
		wygrane |= pomoc & mapa >> 7;
		pomoc = mapa >> 7 & mapa >> 14;
		wygrane |= pomoc & mapa >> 21;
		wygrane |= pomoc & mapa << 7;
		//ukos w prawo
		pomoc = mapa << 6 & mapa << 12;
		wygrane |= pomoc & mapa << 18;
		wygrane |= pomoc & mapa >> 6;
		pomoc = mapa >> 6 & mapa >> 12;
		wygrane |= pomoc & mapa >> 18;
		wygrane |= pomoc & mapa << 6;
		//ukos w lewo
		pomoc = mapa << 8 & mapa << 16;
		wygrane |= pomoc & mapa << 24;
		wygrane |= pomoc & mapa >> 8;
		pomoc = mapa >> 8 & mapa >> 16;
		wygrane |= pomoc & mapa >> 24;
		wygrane |= pomoc & mapa << 8;
		return wygrane;
	}
	unsigned long long moje_wygrane_miejsca() {
		return wygrane_miejsca_przeciwnika(plansza.pozycje ^ plansza.maska);
	}
	short policz_bity(unsigned long long liczba) {
		short bity = 0;
		for (; liczba; bity++) {
			liczba &= liczba - 1;
		}
		return bity;
	}
	bool get_custom(unsigned long long mapa, short x, short y) {
		short miejsce = x * 7 + 5 - y;
		return (mapa >> miejsce) % 2;
	}
public:
	short krazki_gracza = 0;
	short krazki_bota = 0;
	/*void zaktualizuj_liczniki_krozkow() {
		krazki_gracza = 0;
		krazki_bota = 0;
		for (short i = 0; i < 7; i++) {
			for (short j = 0; j < 6; j++) {
				if (plansza[i][j] == 1) {
					krazki_gracza++;
				}
				if (plansza[i][j] == 2) {
					krazki_bota++;
				}
			}
		}
	}*/
	bool get_pozycje(short x, short y) {
		short miejsce = x * 7 + 5 - y;;
		return (plansza.pozycje >> miejsce) % 2;
	}
	bool get_maska(short x, short y) {
		short miejsce = x * 7 + 5 - y;
		return (plansza.maska >> miejsce) % 2;
	}
	void zagraj_ze_zmiana(unsigned long long ruch, bool przeciwnik) {
		plansza.maska |= ruch;
		plansza.pozycje ^= plansza.maska;
		if (przeciwnik) {
			krazki_gracza++;
		}
		else {
			krazki_bota++;
		}
	}
	void zagraj_bez_zmiany(short kolumna, bool przeciwnik) {
		if (przeciwnik) {
			plansza.pozycje ^= plansza.maska;
			plansza.maska |= plansza.maska + dolna_maska(kolumna);
			plansza.pozycje ^= plansza.maska;
			krazki_gracza++;
		}
		else {
			plansza.maska |= plansza.maska + dolna_maska(kolumna);
			krazki_bota++;
		}
	}
	bool czy_mozna_zagrac(short kolumna) {
		return !(plansza.maska & gorna_maska(kolumna));
	}
	bool czy_wygrana() {
		//pionowo
		unsigned long long m = plansza.pozycje & plansza.pozycje >> 7;
		if (m & m >> 14) {
			return true;
		}
		//poziomo
		m = plansza.pozycje & plansza.pozycje >> 1;
		if (m & m >> 2) {
			return true;
		}
		//ukos w prawo
		m = plansza.pozycje & plansza.pozycje >> 6;
		if (m & m >> 12) {
			return true;
		}
		//usok w lewo
		m = plansza.pozycje & plansza.pozycje >> 8;
		if (m & m >> 16) {
			return true;
		}
		return false;
	}
	short ocen(bool przeciwnik) {
		if (przeciwnik) {
			return krazki_gracza - 22;
		}
		else {
			return 22 - krazki_bota;
		}
	}
	short czy_ktos_wygral() {
		if (czy_wygrana()) {
			return -1;
		}
		plansza.pozycje ^= plansza.maska;
		if (czy_wygrana()) {
			plansza.pozycje ^= plansza.maska;
			return 1;
		}
		plansza.pozycje ^= plansza.maska;
		if (krazki_gracza + krazki_bota == 42) {
			return 0;
		}
		return 2;
	}
	bool czy_remis() {
		return krazki_bota + krazki_gracza == 42;
	}
	void wypisz_pozycje() {
		for (short i = 0; i < 6; i++) {
			for (short j = 0; j < 7; j++) {
				std::cout << get_pozycje(j, i) << " ";
			}
			std::cout << std::endl;
		}
	}
	void wypisz_maske() {
		for (short i = 0; i < 6; i++) {
			for (short j = 0; j < 7; j++) {
				std::cout << get_maska(j, i) << " ";
			}
			std::cout << std::endl;
		}
	}
	void wypisz(unsigned long long mapa) {
		for (short i = 0; i < 6; i++) {
			for (short j = 0; j < 7; j++) {
				std::cout << get_custom(mapa, j, i) << " ";
			}
			std::cout << std::endl;
		}
	}
	unsigned long long klucz() {
		return plansza.maska + plansza.pozycje;
	}
	unsigned long long mozliwe_zagrania() {
		unsigned long long mozliwe_polozenia_krozkow = mozliwosci();
		unsigned long long moje_wygrane = moje_wygrane_miejsca();
		unsigned long long wymuszone_ruchy = moje_wygrane & mozliwe_polozenia_krozkow;
		if (wymuszone_ruchy) {
			return wymuszone_ruchy;
		}
		unsigned long long wygrane_przeciwnika = wygrane_miejsca_przeciwnika(plansza.pozycje);
		wymuszone_ruchy = mozliwe_polozenia_krozkow & wygrane_przeciwnika;
		if (wymuszone_ruchy) {
			if (wymuszone_ruchy & wymuszone_ruchy - 1) {
				return 0;
			}
			mozliwe_polozenia_krozkow = wymuszone_ruchy;
		}
		return mozliwe_polozenia_krozkow & ~(wygrane_przeciwnika >> 1);
	}
	unsigned long long maska_kolumny(short kolumna) {
		return ((unsigned long long)1 << 6) - 1 << kolumna * 7;
	}
	short ocen_ruch(unsigned long long ruch) {
		return policz_bity(~plansza.maska & wygrane_miejsca_przeciwnika(plansza.pozycje ^ (plansza.maska | ruch)));
	}

};