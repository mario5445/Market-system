#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void mainMenu() {
	// funkcia ktora vypise hlavne menu
	cout << "+++++++  Hlavne menu  +++++++" << endl;
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
	system("CLS");
	cout << "/*--*/ Dovidenia /*--*/" << endl << endl;
	exit(0);
}

void removeProduct() {
	system("CLS");
	ifstream fin; // vytvorenie dvoch istreamov na citanie suborov
	ifstream fil;
	ofstream temp; // vytvorenie ofstreamu na zapisovanie do suboru
	fin.open("Produkty.txt"); // otvorenie suboru ifstreamom
	temp.open("temp.txt"); // otvorenie suboru ofstreamom
	cout << "!!!!!! Vymazanie produktu !!!!!!" << endl << endl; // info pre uzivatela
	cout << "Zoznam produktov: " << endl << endl; // vypisanie produktov
	if(fin.is_open()) // kontrola, ci je subor otvoreny
	{
		if (temp.is_open()) // kontrola ci je subor otvoreny
		{
			string line; // vytvorenie pomocnej premennej specialne pre funkciu getline
			while (getline(fin, line)) // getline zoberie riadok zo suboru a ulozi ho do premennej line
			{
				// vypisovanie jednotlivych riadkov
				cout << "\t" << line << endl;
			}
			fin.close(); // zatvorenie suboru
			if (fin.is_open()) // kontorola ci je subor zatvoreny
			{
				// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
				system("CLS");
				temp.close();
				cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
				exit(0);
			}
			// otvorenie suboru druhym ifstreamom
			fil.open("Produkty.txt");
			if (fil.is_open()) // kontrola ci je subor otvoreny
			{
				string userInput; // vytvorenie premennej typu string na uskladnenie inputu od pouzivatela
				cout << endl << "Zadajte nazov produktu: "; // info pre pouzivatela
				cin >> userInput; // ulozenie inputu do premennej
				while (getline(fil, line)) // getline zoberie riadok zo suboru a ulozi ho do premennej line
				{
					// podmienka ktora zisti ci riadok obsahuje alebo neobsahuje produkt ktory chce pouzivatel odstranit
					if (line.substr(0, (userInput + "\t").size()) != userInput + "\t") 
						temp << line << endl; // zapisanie riadku do docasneho suboru temp.txt pokial neobsahuje produkt ktory chce uzivatel zmazat
				}
				fil.close(); // zatvorenie suboru Produkty.txt
				temp.close(); // zatvorenie suboru temp.txt
				remove("Produkty.txt"); // odstranenie suboru
				rename("temp.txt", "Produkty.txt"); // premenovanie suboru temp.txt na Produkty.txt
				if (fil.is_open()) // kontrola ci je subor zatvoreny
				{
					// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
					system("CLS");
					cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
					exit(0);
				}
				if (temp.is_open()) // kontrola ci je subor zatvoreny
				{
					// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
					system("CLS");
					cout << "Fatal error > Subor temp.txt sa nepodarilo zatvorit" << endl;
					exit(0);
				}
			}
			else
			{
				// ak sa subor nepodarilo otvorit vypise sa chybova hlaska a program sa vypne
				system("CLS");
				cout << "Fatal error > Subor Produkty.txt sa nepodarilo otvorit" << endl;
				temp.close();
				exit(0);
			}
		}
		else
		{
			// ak sa subor nepodarilo otvorit vypise sa chybova hlaska a program sa vypne
			system("CLS");
			fin.close();
			cout << "Fatal error > Subor temp.txt sa nepodarilo otvorit" << endl;
			exit(0);
		}
	}
	else
	{
		// ak sa subor nepodarilo otvorit vypise sa chybova hlaska a program sa vypne
		system("CLS");
		temp.close();
		cout << "Fatal error > Subor Produkty.txt sa nepodarilo otvorit" << endl;
		exit(0);
	}
	system("CLS");
}

void changeLoginData() {
	system("CLS"); 
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl; // maly GUI prvok
	cout << "!!!!!!! Zmena prihlasovacich udajov !!!!!!!" << endl; // upozornenie pre uzivatela
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
	ofstream temp; // vytvorenie noveho ostreamu
	temp.open("temp.txt"); // otvorenie suboru ofstreamom
	if (temp.is_open()) // kontrola ci je subor otvoreny
	{
		string loginData; // vytvorenie dvoch premenny na uskladnenie inputu od uzivatela
		string passwordData;
		cout << "Zadajte novy login: "; // vypytanie si loginu od uzivatela
		cin >> loginData;  // ulozenie inputu do jednej z premennych
		cout << "Zadajte nove heslo: "; // vypytanie si hesla od uzivatela
		cin >> passwordData; // uskladnenie inputu do jednej z premennych
		temp << loginData << endl; // zapisanie loginu do suboru temp.txt
		temp << passwordData; // zapisanie hesla do suboru temp.txt
		temp.close(); // zatvorenie suboru
		if (temp.is_open()) // kontrola ci je subor zatvoreny
		{
			// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
			system("CLS");
			cout << "Fatal error > Subor sa nepodarilo zatvorit" << endl;
			cout << "Udaje sa nepodarilo zmenit" << endl;
			exit(0);
		}
		else
		{
			remove("AdminLogin.txt"); // odstranenie povodneho suboru AdminLogin.txt
			rename("temp.txt", "AdminLogin.txt"); // premenovanie docasneho suboru temp.txt na AdminLogin.txt
		}
	}
	else
	{
		// ak sa subor nepodarilo otvorit vypise sa chybova hlaska a program sa vypne
		system("CLS");
		cout << "Fatal error > Subor sa nepodarilo otvorit" << endl;
		exit(0);
	}
	system("CLS");
}


void loginSystem() {
	cout << "*** Najskor sa prihlaste, ak chcete pokracovat ***" << endl;
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
					counter++; // inkrementacia counteru
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
					break; // counter sa uz neinkrementuje ale pomocou break sa dostaneme von z cyklu
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
	cout << ":::::::Vyberte prosim, co chete robit:::::::" << endl << endl << "--------------------------------------------" << endl << endl;
	int intMenuInput; // vytvorenie premennej na uskladnenie inputu vo forme int
	string strmenuInput; // vytvorenie premennej na uskladnenie inputu vo forme stringu
	while (true)
	{
		mainMenu(); // zavolanie hlavneho menu
		cout << "Vas vyber -> ";
		while (true) // cyklus na kontrolu exception
		{
			cin >> strmenuInput; // input od uzivatela
			try // exception
			{
				intMenuInput = stoi(strmenuInput); // konverzia stringu na int  
				if (intMenuInput >= 1 && intMenuInput <= 7) // kontrola, ci je cislo v intervale od 1 po 7
					break; // opustenie cyklu
				else
				{
					// v pripade nesplnenej podmienky
					// chybova hlaska
					cout << "Nespravny input" << endl;
					cout << "Zadaj svoj vyber znova -> ";
					continue; // navrat na zaciatok cyklu
				}
			}
			catch (...) // zachytenie errorov napriklad pri konverzii
			{
				// chybova hlaska
				cout << "Nespravny input" << endl;
				cout << "Zadaj svoj vyber znova -> ";
				continue; // navrat na zaciatok cyklu
			}
		}
		if (intMenuInput == 3)
		{
			removeProduct(); // zavloanie funkcie na vymazanie produktu
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 6)
		{
			changeLoginData(); // zavolanie funkcie na zmenu prihlasovacich udajov
		}
		else if(intMenuInput == 7)
		{
			exitFunc(); // zavolanie funkcie na vypnutie programu
			break;
		}
	}
	return 0;
}
