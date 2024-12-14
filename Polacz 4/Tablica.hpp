class Tablica {
private:
	unsigned long long wielkosc = 99999999;
	struct pozycja {
		unsigned long long klucz;
		short wartosc;
	};
	pozycja* pozycje = new pozycja[wielkosc];
	unsigned long long index(unsigned long long klucz) {
		return klucz % wielkosc;
	}
public:
	Tablica() {
		reset();
	}
	void reset() {
		memset(pozycje, 23, wielkosc);
	}
	void put(unsigned long long klucz, short wartosc) {
		unsigned long long i = index(klucz);
		pozycje[i].klucz = klucz;
		pozycje[i].wartosc = wartosc;
	}
	short get(unsigned long long klucz) {
		unsigned long long i = index(klucz);
		if (pozycje[i].klucz == klucz) {
			return pozycje[i].wartosc;
		}
		else {
			return 23;
		}
	}
};