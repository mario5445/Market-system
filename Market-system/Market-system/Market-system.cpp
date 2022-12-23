#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void mainMenu() {
	// funkcia ktora vypise hlavne menu
	cout << "++++++++  Hlavne menu  ++++++++" << endl;
	cout << "\t[1] Nablokovat produkty" << endl;
	cout << "\t[2] Pridat produkt" << endl;
	cout << "\t[3] Odstranit produkt" << endl;
	cout << "\t[4] Upravit produkt" << endl;
	cout << "\t[5] Zmenit udaje pre blocky" << endl;
	cout << "\t[6] Zmenit prihlasovacie udaje" << endl;
	cout << "\t[7] Exit" << endl << endl;
	cout << "Pre pokracovanie zvolte jedno z cisel v hranatych zatvorkach" << endl;
}

void exitFunc() {
	// funkcia ktora vypne program
	system("CLS");
	cout << "/*--*/ Dovidenia /*--*/" << endl;
	exit(0);
}


void addProduct() {
	system("CLS");
	fstream fil;
	string userProductName;
	string userProductPrice;
	
	cout << "++++++++++++++++++++++++++++++" << endl;
	cout << "|| Pridanie noveho produktu ||" << endl;
	cout << "++++++++++++++++++++++++++++++" << endl << endl;

	while (true)
	{
		cout << "Zadajte nazov produktu: ";
		cin >> userProductName;
		cout << "Zadajte cenu noveho produktu: ";
		cin >> userProductPrice;

		fil.open("Produkty.txt", ios::in);
		if (!fil.is_open())
		{
			// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
			system("CLS");
			cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
			exit(0);
		}
		string line;
		bool isDuplicate = false;

		while (getline(fil, line))
		{
			if (line.substr(0, (userProductName + "\t").size()) == (userProductName + "\t"))
			{
				isDuplicate = true;

				break;
			}
		}
		fil.close();
		if (fil.is_open())
		{
			// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
			system("CLS");
			cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
			exit(0);
		}

		if (!isDuplicate)
		{
			fil.open("Produkty.txt", ios::app);
			if (!fil.is_open())
			{
				// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
				system("CLS");
				cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
				exit(0);
			}
			fil << userProductName << "\t\t\t" << userProductPrice << endl;
			fil.close();

			if (fil.is_open())
			{
				// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
				system("CLS");
				cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
				exit(0);

			}
			break;
		}
		else
		{
			cout << "Produkt uz je v zozname" << endl;
			string yesOrNo;
			cout << "Chcete zadat produkt este raz? ano/nie" << endl;
			cout << "-> ";
			cin >> yesOrNo;
			if (yesOrNo == "ano" || yesOrNo == "ANO")
			{
				system("CLS");
				continue;
			}
			else
			{
				break;
			}

		}
	}

	
	system("CLS");
}


void editProduct() {
	system("CLS");
	ifstream fin;
	string userProductName;
	string userProductPrice;

	cout << "**********************" << endl;
	cout << "* Upravenie produktu *" << endl;
	cout << "**********************" << endl << endl;
	while (true)
	{
		cout << "Zoznam produktov: " << endl;
		fin.open("Produkty.txt");
		if (!fin.is_open()) // kontorola ci je subor zatvoreny
		{
			// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
			system("CLS");
			cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
			exit(0);
		}
		string line; // premenna pre funkciu getline
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
			cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
			exit(0);
		}
		cout << "Zadajte nazov produktu ktory chcete upravit: ";
		cin >> userProductName;
		fin.open("Produkty.txt");
		if (!fin.is_open()) // kontorola ci je subor zatvoreny
		{
			// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
			system("CLS");
			cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
			exit(0);
		}
		bool isValid = false;

		while (getline(fin, line))
		{
			if (line.substr(0, (userProductName + "\t").size()) == (userProductName + "\t"))
			{
				isValid = true;
				break;
			}
		}

		fin.close();
		if (fin.is_open()) // kontorola ci je subor zatvoreny
		{
			// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
			system("CLS");
			cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
			exit(0);
		}

		if (isValid)
		{
			ofstream temp;

			cout << "Zadajte novy nazov produktu: ";
			cin >> userProductName;
			cout << "Zadajte novu cenu produktu: ";
			cin >> userProductPrice;
			fin.open("Produkty.txt");
			if (!fin.is_open()) // kontorola ci je subor zatvoreny
			{
				// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
				system("CLS");
				cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
				exit(0);
			}
			temp.open("temp.txt");
			if (!temp.is_open()) // kontorola ci je subor zatvoreny
			{
				// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
				system("CLS");
				cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
				fin.close();
				exit(0);
			}
			while (getline(fin, line)) // getline zoberie riadok zo suboru a ulozi ho do premennej line
			{
				// podmienka ktora zisti ci riadok obsahuje alebo neobsahuje produkt ktory chce pouzivatel odstranit
				if (line.substr(0, (userProductName + "\t").size()) != userProductName + "\t")
					temp << line << endl; // zapisanie riadku do docasneho suboru temp.txt pokial neobsahuje produkt ktory chce uzivatel zmazat
			}
			fin.close(); // zatvorenie suboru Produkty.txt
			temp << userProductName << "\t\t\t" << userProductPrice << endl;
			temp.close(); // zatvorenie suboru temp.txt
			remove("Produkty.txt"); // odstranenie suboru
			rename("temp.txt", "Produkty.txt"); // premenovanie suboru temp.txt na Produkty.txt
			if (fin.is_open() || temp.is_open()) // kontrola ci je subor zatvoreny
			{
				// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
				system("CLS");
				cout << "Fatal error > Subor Produkty.txt alebo temp.txt sa nepodarilo zatvorit" << endl;
				exit(0);
			}
			break;
		}
		else
		{
			string yesOrNo;
			cout << "Produkt ktory ste zadali sa v zozname nenachadza" << endl;
			cout << "Chcete znova zadat nazov produktu? ano/nie " << endl;
			cout << "-> ";
			cin >> yesOrNo;
			if (yesOrNo == "ano" || yesOrNo == "ANO")
			{
				system("CLS");
				continue;
			}
			else
			{
				break;
			}
		}
	}
	system("CLS");
}




void removeProduct() {
	system("CLS");
	ifstream fin; // ifstream na citanie suboru
	ofstream temp; // ofstream na zapisovanie do suboru
	fin.open("Produkty.txt"); // otvorenie suboru ifstreamom
	cout << "!!!!!! Vymazanie produktu !!!!!!" << endl << endl; // info pre uzivatela
	cout << "Zoznam produktov: " << endl << endl; // vypisanie produktov
	if(fin.is_open()) // kontrola, ci je subor otvoreny
	{
		string line; // premenna pre funkciu getline
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
			cout << "Fatal error > Subor Produkty.txt sa nepodarilo zatvorit" << endl;
			exit(0);
		}
		string userInput; // vytvorenie premennej typu string na uskladnenie inputu od pouzivatela
		cout << endl << "Zadajte nazov produktu: "; // info pre pouzivatela
		cin >> userInput; // ulozenie inputu do premennej
		temp.open("temp.txt"); // otvorenie suboru ofstreamom
		if (temp.is_open()) // kontrola ci je subor otvoreny
		{
			// otvorenie suboru druhym ifstreamom
			fin.open("Produkty.txt");
			if (fin.is_open()) // kontrola ci je subor otvoreny
			{	
				while (getline(fin, line)) // getline zoberie riadok zo suboru a ulozi ho do premennej line
				{
					// podmienka ktora zisti ci riadok obsahuje alebo neobsahuje produkt ktory chce pouzivatel odstranit
					if (line.substr(0, (userInput + "\t").size()) != userInput + "\t") 
						temp << line << endl; // zapisanie riadku do docasneho suboru temp.txt pokial neobsahuje produkt ktory chce uzivatel zmazat
				}
				fin.close(); // zatvorenie suboru Produkty.txt
				temp.close(); // zatvorenie suboru temp.txt
				remove("Produkty.txt"); // odstranenie suboru
				rename("temp.txt", "Produkty.txt"); // premenovanie suboru temp.txt na Produkty.txt
				if (fin.is_open() || temp.is_open()) // kontrola ci je subor zatvoreny
				{
					// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
					system("CLS");
					cout << "Fatal error > Subor Produkty.txt alebo temp.txt sa nepodarilo zatvorit" << endl;
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
			cout << "Fatal error > Subor temp.txt sa nepodarilo otvorit" << endl;
			exit(0);
		}
	}
	else
	{
		// ak sa subor nepodarilo otvorit vypise sa chybova hlaska a program sa vypne
		system("CLS");
		cout << "Fatal error > Subor Produkty.txt sa nepodarilo otvorit" << endl;
		exit(0);
	}
	system("CLS");
}

void changeLoginData(bool isEmpty) {
	system("CLS"); 
	
	if (isEmpty != true)
	{
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl; // maly GUI prvok
		cout << "!!!!!!! Zmena prihlasovacich udajov !!!!!!!" << endl; // upozornenie pre uzivatela
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
	}
	else
	{
		cout << " =============================================" << endl;
		cout << "||  Nastavenie novych prihlasovacich udajov  ||" << endl;
		cout << " =============================================" << endl << endl;
	}
	ofstream temp; // vytvorenie noveho ostreamu
	string loginData; // vytvorenie dvoch premennych na uskladnenie inputu od uzivatela
	string passwordData;
	cout << "Zadajte novy login: "; // vypytanie si loginu od uzivatela
	cin >> loginData;  // ulozenie inputu do jednej z premennych
	cout << "Zadajte nove heslo: "; // vypytanie si hesla od uzivatela
	cin >> passwordData; // uskladnenie inputu do jednej z premennych
	temp.open("temp.txt"); // otvorenie suboru ofstreamom
	if (temp.is_open()) // kontrola ci je subor otvoreny
	{
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
	cout << "*** Najskor sa prihlaste, ak chcete pokracovat ***" << endl << endl;
	int counter = 0; // pomocou counteru program vie ci ma overit login alebo heslo
	ifstream fin; // ifstream - subor je len na citanie
	// Premenne na ulozenie inputov
	string loginstr;
	string passwordstr;
	// Login form
	cout << "Login: ";
	cin >> loginstr;
	cout << "Heslo: ";
	cin >> passwordstr;
	fin.open("AdminLogin.txt");
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

int checkFileLines(string fileName) {
	ifstream fil; // ifstream na citanie suboru
	fil.open(fileName); // otvorenie suboru
	if (!fil.is_open()) // kontrola ci je subor otvoreny
	{
		// chybova hlaska
		system("CLS");
		cout << "Fatal error > Subor sa nepodarilo otvorit" << endl;
		exit(0);
	}
	int counter = 0; // counter na pocitanie riadkov v subore
	string line; // premenna na ulozenie riadku 
	while (getline(fil, line))
	{
		if (line!="" || line!= "\n") // overenie ci nie je riadok prazdny
		{
			counter++; // inkrementacia counteru
		}
	}
	fil.close(); // zatvorenie suboru
	if (fil.is_open()) // kontrola ci je subor zatvoreny
	{
		// chybova hlaska
		system("CLS");
		cout << "Fatal error > Subor sa nepodarilo otvorit" << endl;
		exit(0);
	}
	if (counter < 2) // kontrola ci ma subor 1 riadok a menej
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int checkIfFileIsEmpty(string fileName) {
	ifstream fil;
	fil.open(fileName);
	if (!fil.is_open())
	{
		// chybova hlaska
		system("CLS");
		cout << "Fatal error > Subor sa nepodarilo otvorit" << endl;
		exit(0);
	}
	fil.seekg(0, ios::end);
	int len = fil.tellg();
	fil.close();
	if (fil.is_open())
	{
		// chybova hlaska
		system("CLS");
		cout << "Fatal error > Subor sa nepodarilo otvorit" << endl;
		exit(0);
	}
	return len;
}

int main() {
	int isFileEmpty = checkIfFileIsEmpty("AdminLogin.txt");
	int filLineCount = checkFileLines("AdminLogin.txt");
	if (isFileEmpty != 0 && filLineCount != 0) // kontrola ci je subor prazdny
		loginSystem();
	else
		changeLoginData(true);

	// uvitanie pouzivatela
	cout << "~~~~~~~Vitajte v nasom market systeme~~~~~~~" << endl;
	cout << ":::::::Vyberte prosim, co chete robit:::::::" << endl << endl << "--------------------------------------------" << endl << endl;
	int intMenuInput; // premenna na uskladnenie inputu vo forme intu
	string strmenuInput; // premenna na uskladnenie inputu vo forme stringu
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
		if (intMenuInput == 1)
		{
			continue;
		}
		else if (intMenuInput == 2)
		{
			addProduct();
			continue;

		}
		else if (intMenuInput == 3)
		{
			removeProduct(); // zavloanie funkcie na vymazanie produktu
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 4)
		{
			editProduct();
			continue;
		}
		else if (intMenuInput == 5)
		{
			continue;
		}
		else if (intMenuInput == 6)
		{
			changeLoginData(false); // zavolanie funkcie na zmenu prihlasovacich udajov
			continue; // navrat na zaciatok cyklu
		}
		else if(intMenuInput == 7)
		{
			exitFunc(); // zavolanie funkcie na vypnutie programu
			break; // opustenie cyklu
		}
	}
	return 0;
}