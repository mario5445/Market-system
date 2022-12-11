#include <iostream>
#include <fstream>

using namespace std;


void mainMenu() {
	// funkcia ktora vypise hlavne menu
	cout << "•••••••••••Hlavne menu•••••••••••" << endl;
	cout << "\t[1] Nablokovat produkty" << endl;
	cout << "\t[2] Pridat produkt" << endl;
	cout << "\t[3] Odstranit produkt" << endl;
	cout << "\t[4] Upravit produkt" << endl;
	cout << "\t[5] Zmenit udaje pre nakupne doklady" << endl;
	cout << "\t[6] Zmenit prihlasovacie udaje" << endl;
	cout << "\t[7] Exit" << endl << endl;
	cout << "Pre pokracovanie zvolte jedno z cisel v hranatych zatvorkach" << endl;
}


void exitFunc() {
	// funkcia ktora vypne program
	cout << "/*--*/ Dovidenia /*--*/";
	exit(0);
}

void loginSystem() {
	cout << "**Najskor sa prihlas, ak chces pokracovat**" << endl;
	int counter = 0; // pomocou counteru program vie ci ma overit login alebo heslo
	ifstream fin; // ifstream - subor je len na citanie
	fin.open("AdminLogin.txt");
	// Premenne na ulozenie inputov
	string loginstr;
	string passwordstr;
	// Login form
	cout << "Login: ";
	cin >> loginstr;
	cout << "Heslo: ";
	cin >> passwordstr;
	if (fin.is_open()) // overenie ci sa subor podarilo otvorit
	{
		while (!fin.eof()) // cyklus, ktory ide po jednotlivych riadkoch textoveho suboru. podmienka je splnena ak nie je este koniec suboru
		{
			char arr[50]; // funkcia getline uklada riadok do pola typu char
			fin.getline(arr, 50); // prvy argument hovori, kde sa ma riadok ulozit a druhy hovori o jeho dlzke, ktora je relativna
			string arra = arr; // konverzia pola typu char na string. Toto je mozne lebo string je v podstate pole znakov
			if (counter == 0) // program sa rozhodne na zaklade counteru, co skontroluje
			{
				if (arra == loginstr) // kedze ideme v textovom subore po riadkoch tak najskor musi skontrolovat login
				{
					counter++; // inkrementacia counteru
				}
				else
				{
					/*
					tato cast sa spusti len ked je nespravny login
					vypise sa chybova hlaska, zatvori sa textovy subor a vypne sa program
					*/
					system("CLS");
					cout << "Zistili sme neopravneny pristup" << endl;
					fin.close();
					exit(0);
				}
			}
			else if(counter == 1)
			{
				if (arra == passwordstr) // program skontroluje heslo
				{
					break; // counter sa uz neinkrementuje ale pomocou break sa dostaneme von z cyklu
				}
				else
				{
					// tato cast sa spusti ak je nespravne heslo
					system("CLS");
					cout << "Zistili sme neopravneny pristup" << endl;
					fin.close();
					exit(0);
				}
			}
		}
		// zatvorenie suboru
		fin.close();
		if (fin.is_open()) // overenie ci je subor skutocne zatvoreny
		{
			// ak subor nie je zatvoreny vypise sa chybova hlaska a program sa hned vypne
			system("CLS");
			cout << "Fatal error > Subor sa nepodarilo zatvorit" << endl;
			exit(0);
		}
	}
	else
	{
		// ak sa subor nepodarilo otvorit vypise sa chybova hlaska a program sa vypne
		system("CLS");
		cout << "Fatal error > Subor sa nepodarilo otvorit" << endl;
		exit(0);
	}
	// vycistenie konzoly v pripade spravneho loginu a hesla
	system("CLS");
}

int main() {
	// zavolanie funkcie na prihlasenie do systemu
	loginSystem();
	// uvitanie pouzivatela
	cout << "~~~~~~~Vitajte v nasom market systeme~~~~~~~" << endl;
	cout << ":::::::Vyberte prosim, co chete robit:::::::" << endl << endl << "---------------------------------------------" << endl << endl;
	// zavolanie hlavneho menu
	mainMenu();
	return 0;
}
