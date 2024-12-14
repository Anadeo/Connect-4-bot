class Sortownik {
private:
	struct Rekord {
		unsigned long long ruch;
		short punkty;
	};
	short rozmiar = 0;
	Rekord rekord[7];
public:
	void put(unsigned long long ruch, short punkty) {
		short i = rozmiar;
		rozmiar++;
		for (;i && rekord[i - 1].punkty >= punkty; i--) {
			rekord[i] = rekord[i - 1];
		}
		rekord[i].punkty = punkty;
		rekord[i].ruch = ruch;
	}
	unsigned long long get() {
		rozmiar--;
		if (rozmiar < 0) {
			return -1;
		}
		return rekord[rozmiar].ruch;
	}
};