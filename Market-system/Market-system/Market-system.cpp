#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <ctime>
#include <io.h>
#include <fcntl.h>
#pragma warning(disable : 4996) // vypnutie 4996 aby fungovala funkcia getActualTime()

using namespace std;

string language = "sk"; // globalna premenna na jazyk
float money = 0; // globalna premnna na trzbu

// random kod zo stack overflow na output eura
void printEuroSign() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	std::wcout << L"€\n";
	_setmode(_fileno(stdout), _O_TEXT);
}

// ak sa subor nepodarilo otvorit vypise sa chybova hlaska a program sa vypne
void fileCouldntBeOpened() {
	system("CLS"); // vycistenie obrazovky
	if (language == "sk") // vypis v dvoch jazykoch
		cout << "Fatal error > Subor sa nepodarilo otvorit" << endl;
	else
		cout << "Fatal error > File could not be opened" << endl;
	exit(0); // vypnutie programu
}

// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
void fileCouldntBeClosed() {
	system("CLS"); // vycistenie obrazovky
	if (language == "sk") // vypis v dvoch jazykoch
		cout << "Fatal error > Subor sa nepodarilo zatvorit" << endl;
	else
		cout << "Fatal error > File could not be closed" << endl;
	exit(0); // vypnutie programu
}

// TODO
// vyber jazyka
void selectLanguage() {

}

// vyberie data zo suboru ktore si vypytam
string getReceiptData(string data) { // parameter pre hladanie dat
	ifstream fin; // ifstream na citanie suboru
	fin.open("Udaje.txt"); // otvorenie suboru
	if (!fin.is_open()) // overenie ci je subor otvoreny
	{
		fileCouldntBeOpened(); // volanie chybovej funkcie
	}
	string line; // uskladnenie riadku zo subora
	while (getline(fin, line)) // prechadzanie suboru po riadkoch
	{
		if (line.substr(0, (data + ":\t").size()) == (data + ":\t")) // kontrola ci riadok obsahuje data ktore potrebujem
			break; // opustenie cyklu
	}
	string extractedData = ""; // ulozenie extrahovanych dat z riadku
	for (int i = 0; i < line.length(); i++) // cyklus na prechadzanie riadkom
	{
		if (line[i] == '\t') // hladanie tabulatoru v riadku
		{
			int splitOn = i + 1; // bod na rozdelenie stringu
			extractedData = line.substr(splitOn); // vytvorenie substringu od bodu rozdelenia
			break; // opustenie cyklu
		}
	}
	fin.close(); // zatvorenie suboru
	if (fin.is_open()) // konrola ci je subor zatvoreny
	{
		fileCouldntBeClosed(); // chybova funkcia
	}
	return extractedData; // navratova hodnota 
}

void mainMenu() {
	// funkcia ktora vypise hlavne menu
	cout << ":::::::::::::::::::::::::::::::" << endl;
	cout << "::::::::  Hlavne menu  ::::::::" << endl;
	cout << ":::::::::::::::::::::::::::::::" << endl << endl;
	cout << "\t[1] Nablokovat produkty" << endl;
	cout << "\t[2] Pridat produkt" << endl;
	cout << "\t[3] Odstranit produkt" << endl;
	cout << "\t[4] Upravit produkt" << endl;
	cout << "\t[5] Zmenit udaje pre blocky" << endl;
	cout << "\t[6] Zmenit prihlasovacie udaje" << endl;
	cout << "\t[7] Exit" << endl << endl;
	cout << "Pre pokracovanie zvolte jedno z cisel v hranatych zatvorkach" << endl;
}

// random kod zo stack overflow na ziskanie systemoveho datumu a casu
string getActualTime() {
	// current date/time based on current system
	time_t now = time(0);
	// convert now to string form
	string dt = ctime(&now);
	return dt;
}

// funkcia ktora vypne program
void exitFunc() {
	system("CLS");
	if (language == "sk")
		cout << "/*--*/ Dovidenia /*--*/" << endl;
	else
		cout << "/*--*/ Goodbye /*--*/" << endl;
	exit(0);
}

// kontrola ci string obsahuje ciarku
string checkForComma(string s) { // parameter typu string
	for (int i = 0; i < s.length(); i++) // cyklus na prechadzanie stringom
	{
		if (s[i] == ',') // hladanie ciarky
			s[i] = '.'; // zamenenie ciarky za bodku
	}
	return s; // navratova hodnota
}

// skontrolovanie ci subor obsahuje produkt
bool containsProduct(string productName) { // parameter typu string
	ifstream fin; // ifstream na citanie suboru
	fin.open("Produkty.txt"); // otvorenie suboru
	if (!fin.is_open()) // kontrola ci je subor otvoreny
		fileCouldntBeOpened(); // chybova funkcia
	string line; // uskladnenie riadku 
	while (getline(fin, line)) // prechadzanie suboru po riadkoch
	{
		if (line.substr(0, (productName + "\t").size()) == (productName + "\t")) { // kontrola ci riadok obsahuje to co hladam
			fin.close(); // zatvorenie suboru
			if (fin.is_open()) // kontrola ci je subor zatvoreny
				fileCouldntBeClosed(); // chybova funkcia
			return true; // navratova hodnota, koniec funkcie ak je produkt najdeny
		}
	}
	fin.close(); // zatvorenie suboru ak sa produkt nenasiel
	if (fin.is_open()) // kontrola ci je subor zatvoreny
		fileCouldntBeClosed(); // chybova funkcia
	return false; // navratova hodnota
}

// blokovanie produktov
void checkout() {
	system("CLS"); // vycistenie obrazovky
	// vector == pole s vyuzitim dynamickej pamäte
	vector<string> listOfProducts; // vector na uskladnenie celeho zoznamu produktov aby sa zabranilo zbytocnemu repetitivnemu otvaraniu suboru
	vector<string> products; // vector na uskladnenie nablokovanych produktov
	float price = 0; // vysledna cena
	ifstream fin; // ifstream na citanie zo suboru
	fin.open("Produkty.txt"); // otvorenie suboru
	if (!fin.is_open()) // kontrola ci je subor otvoreny
		fileCouldntBeOpened(); // chybova funkcia
	string line; // uskladnenie riadku
	while (getline(fin, line)) // prechadzanie suboru po riadkoch
	{
		if (line != "" && line != "\n") // kontrola ci nie je riadok prazdny
			listOfProducts.push_back(line); // pridavanie produktov do vektoru
	}
	fin.close(); // zatvorenie suboru
	if (fin.is_open()) // kontrola ci je subor zatvoreny
		fileCouldntBeClosed(); // chybova funkcia
	if (language == "sk") // dve jazykove verzie
	{
		cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
		cout << "$ Nablokovanie produktov $" << endl;
		cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl << endl;
	}
	else
	{
		cout << "$$$$$$$$$$$$" << endl;
		cout << "$ Checkout $" << endl;
		cout << "$$$$$$$$$$$$" << endl << endl;
	}
	while (true) // cyklus na pripadne opakovanie v pripade zadania neplatneho produktu
	{
		cout << "Zoznam produktov: " << endl << endl;
		// syntactic sugar. Funguje podobne ako klasicky for cyklus
		for (string i: listOfProducts) // prechadzanie vectorom
		{
			cout << "\t" << i; // vypisovanie produktov z vectoru
			printEuroSign(); // output eura
		}
		cout << endl;
		string userProductName; // uskladnenie inputu
		cout << "Zadajte nazov produktu, ktory chcete nablokovat. Ak chcete skoncit s blokovanim napiste STOP" << endl; // info pre uzivatela
		cout << "-> ";
		cin >> userProductName; // input
		string s = userProductName; // duplikat premennej userProductName
		transform(s.begin(), s.end(), s.begin(), ::toupper); // pretransformovanie duplikatu na velke pismena
		if (s == "STOP") // overenie ci chce uzivatel prestat blokovat produkty
			break; // ukoncenie cyklu
		bool contains = false; 
		string containedLine; // buduce uskladnenie elementu z vectora
		for(int i = 0; i < listOfProducts.size(); i++) // prechadzanie vectorom
		{
			if (listOfProducts.at(i).substr(0, (userProductName + "\t").size()) == (userProductName + "\t")) // overenie ci riadok obsahuje zadany prdukt
			{
				contains = true; // zmena false na true
				containedLine = listOfProducts.at(i); // ulozenie riadku do premennej
				products.push_back(containedLine); // ulozenie riadku do druheho vectoru
				break; // ukonecenie cyklu
			}
		}
		if (contains) // kontrola ci bol produkt najdeny
		{
			regex rgx("(\t\\d+\\.\\d+)"); // regex pre extrahovanie ceny produktu
			smatch match; // zhoda
			if (regex_search(containedLine, match, rgx)) // prehladavanie riadku
			{
				containedLine = match[1]; // priradenie uz extrahovaneho stringu pomocou regexu do premennej. Zaroven konverzia zo smatch na string
				try // exception
				{
					float num = stof(containedLine.substr(1)); // konverzia stringu na float
					// vysvetlenie parametra funkcie stof - regex mi vrati tabulator a cenu. Tym ze urobim substring od indexu 1 sa zbavim tabulatoru
					price += num; // pripocitanie ceny produktu do vyslednej ceny
					system("CLS"); // vycistenie obrazovky
					continue; // vratenie sa na zaciato celeho cyklu
				}
				catch (...) // v pripade neuspesnej konverzie v bloku try
				{
					system("CLS"); // vycistenie obrazovky
					cout << "Neocakavana chyba pri cene produktu" << endl; // chybova hlaska
					exit(0); // vypnutie programu
				}
			}
		}
		else // ak sa produkt v zozname nenachadza spusti sa tato cast
		{
			cout << "Produkt, ktory ste zadali sa v zozname nenachadza" << endl; // info pre uzivatela 
			string yesOrNo; // uskladnenie inputu
			cout << "Chcete zadat produkt este raz? ano/nie" << endl; // info pre uzivatela
			cout << "-> ";
			cin >> yesOrNo; // input
			transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper); // transform stringu na velke pismena
			if (yesOrNo == "ANO") // overenie inputu
			{
				system("CLS"); // vycistenie obrazovky
				continue; // navrat na zaciatok cyklu
			}
			else 
				break; // koniec cyklu
		}
	}
	if (products.size() > 0) // ak vector produktov nie je prazdny
	{
		system("CLS"); // vycistenie obrazovky
		// vypisanie nakupneho dokladu
		string tabs = "\t\t\t\t"; // rovnaky pocet tabulatorov pre vsetko
		cout << tabs << getReceiptData("Nazov") << endl; // vypytanie si Nazvu zo suboru a vypisanie 
		cout << tabs << getReceiptData("Adresa") << endl; // vypytanie si Adresy zo suboru a vypisanie
		cout << tabs << "ICO: " << getReceiptData("ICO") << endl; // vypytanie si ICO zo suboru a vypisanie
		cout << tabs << getActualTime() << endl; // vypisanie aktualneho casu
		cout << tabs << "------------------------------------------------" << endl << endl; // GUI prvok
		for (string i: products) // prechadzanie vectorom
		{
			cout << tabs << i << endl; // vypisovanie jednotlivych produktov a ich cien
		}
		cout << endl;
		cout << tabs << "SUMA" << "\t\t\t" << price; // vypisanie vyslednej sumy
		printEuroSign(); // output eura
		cout << endl << tabs << "Suma bez DPH" << "\t\t" << price / 1.2; // vypocet ceny bez DPH
		printEuroSign(); // output eura
		cout << tabs << "Sadzba DPH:\t\t20%" << endl; // vypisanie sadzby DPH 
		cout << tabs << "DPH: " << price - (price / 1.2); // vypocet DPH
		printEuroSign(); // output eura
		cout << tabs << "------------------------------------------------" << endl; // GUI prvok
		cout << tabs << "Dakujeme za nakup" << endl << endl;
		while (true) // platba
		{
			cout << "Moznosti platby: " << endl << endl; // info pre uzivatela
			cout << "\t[1] Hotovost" << endl;
			cout << "\t[2] Platobna karta" << endl << endl;
			cout << "Prosim zvolte jedenu z moznosti zadanim cisla v hranatych zatvorkach" << endl;
			cout << "Vasa volba -> ";
			string s; // uskladnenie inputu
			cin >> s; // input
			if (s == "2") // platba kartou
			{
				cout << "Prebieha platba" << endl;
				cout << "Zadajte svoj PIN: " << endl; // info pre uzivatela
				cin >> s; // input PINu
				if (s.length() == 4) // kontrola ci ma PIN styri cislice
				{
					money += price; // pripocitanie penazi na nas ucet
					break; // ukoncenie cyklu
				}
				else // neplatny PIN
				{
					system("CLS"); // vycistenie obrazovky
					cout << "Zadali ste neplatny PIN. Skuste to este raz" << endl; // info pre pouzivatela
					continue; // navrat na zaciatok cyklu
				}
			}
			else if (s == "1") // platba v hotovosti
			{
				money += price; // pripocitanie penazi na nas ucet
				break; // opustenie cyklu
			}
			else // v pripade neplatnej moznosti
			{
				system("CLS"); // vycistenie obrazovky
				cout << "Zadali ste neplatnu moznost!!" << endl; // info pre uzivatela
				cout << "SUMA" << "\t\t" << price; // znovu vypisanie ceny
				printEuroSign(); // output eura
				cout << endl;
				continue; // navrat na zaciatok cyklu
			}
		}
	}
	else // ak je prazdny zoznam nablokovanych produktov spusti sa toto
	{
		system("CLS"); // vycistenie obrazovky
		cout << "Prazdny zoznam nablokovanych produktov!" << endl; // info pre uzivatela
		while (true) 
		{
			string oneOrZero; // uskladnenie inputu
			cout << "Ak sa chcete vratit na hlavne menu zadajte '1'. Ak chcete ukoncit program zadajte '0'." << endl; // info
			cout << "-> "; 
			cin >> oneOrZero; // input
			if (oneOrZero == "0")
			{
				exitFunc(); // opustenie programu
			}
			else if (oneOrZero == "1")
			{
				break; // navrat do hlavneho menu
			}
			else // neplatny vstup
			{
				cout << "Neplatný vstup. Skúste to znova" << endl; // info pre uzivatela
				continue; // navrat na zaciatok cyklu
			}
		}
	}
	system("CLS"); // vycistenie obrazovky
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
		bool isValid = true;
		cout << "Zadajte nazov produktu: ";
		cin >> userProductName;
		cout << "Zadajte cenu noveho produktu: ";
		cin >> userProductPrice;
		userProductPrice = checkForComma(userProductPrice);
		try
		{
			float checkFloat = stof(userProductPrice);
		}
		catch (...)
		{
			isValid = false;
		}
		fil.open("Produkty.txt", ios::in);
		if (!fil.is_open())
		{
			fileCouldntBeOpened();
		}
		bool isDuplicate = false;
		if (containsProduct(userProductName))
		{
			isDuplicate = true;
		}
		fil.close();
		if (fil.is_open())
		{
			fileCouldntBeClosed();
		}
		if (!isDuplicate && isValid)
		{
			fil.open("Produkty.txt", ios::app);
			if (!fil.is_open())
			{
				fileCouldntBeOpened();
			}
			string tabs;
			if (userProductName.length() <= 7)
			{
				string tabs = "\t\t\t";
				fil << userProductName << tabs << userProductPrice << endl;
			}
			else if(userProductName.length() <= 15)
			{
				string tabs = "\t\t";
				fil << userProductName << tabs << userProductPrice << endl;
			}
			else
			{
				string tabs = "\t";
				fil << userProductName << tabs << userProductPrice << endl;
			}
			fil.close();
			if (fil.is_open())
				fileCouldntBeClosed();
			break;
		}
		else
		{
			cout << "Produkt uz je v zozname alebo ste zadali neplatnu cenu" << endl;
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
				break;
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
		if (!fin.is_open()) // kontorola ci je subor otvoreny
			fileCouldntBeOpened();
		string line; // premenna pre funkciu getline
		while (getline(fin, line)) // getline zoberie riadok zo suboru a ulozi ho do premennej line
		{
			// vypisovanie jednotlivych riadkov
			cout << "\t" << line << endl;
		}
		fin.close(); // zatvorenie suboru
		if (fin.is_open()) // kontorola ci je subor zatvoreny
			fileCouldntBeClosed();
		cout << "Zadajte nazov produktu ktory chcete upravit: ";
		cin >> userProductName;
		fin.open("Produkty.txt");
		if (!fin.is_open()) // kontorola ci je subor otvoreny
			fileCouldntBeOpened();
		bool isValid = false;
		if (containsProduct(userProductName))
			isValid = true;
		fin.close();
		if (fin.is_open()) // kontorola ci je subor zatvoreny
			fileCouldntBeClosed();
		if (isValid)
		{
			ofstream temp;
			cout << "Zadajte novy nazov produktu: ";
			cin >> userProductName;
			cout << "Zadajte novu cenu produktu: ";
			cin >> userProductPrice;
			userProductPrice = checkForComma(userProductPrice);
			try
			{
				float checkFloat = stof(userProductPrice);
			}
			catch (...)
			{
				isValid = false;
			}
			if (isValid)
			{
				fin.open("Produkty.txt");
				if (!fin.is_open()) // kontorola ci je subor otvoreny
					fileCouldntBeOpened();
				temp.open("temp.txt");
				if (!temp.is_open()) // kontorola ci je subor otvoreny
					fin.close();
					fileCouldntBeOpened();
				while (getline(fin, line)) // getline zoberie riadok zo suboru a ulozi ho do premennej line
				{
					// podmienka ktora zisti ci riadok obsahuje alebo neobsahuje produkt ktory chce pouzivatel odstranit
					if (line.substr(0, (userProductName + "\t").size()) != userProductName + "\t")
						temp << line << endl; // zapisanie riadku do docasneho suboru temp.txt pokial neobsahuje produkt ktory chce uzivatel zmazat
				}
				fin.close(); // zatvorenie suboru Produkty.txt
				string tabs;
				if (userProductName.length() <= 7)
				{
					string tabs = "\t\t\t";
					temp << userProductName << tabs << userProductPrice << endl;
				}
				else if (userProductName.length() <= 15)
				{
					string tabs = "\t\t";
					temp << userProductName << tabs << userProductPrice << endl;
				}
				else 
				{
					string tabs = "\t";
					temp << userProductName << tabs << userProductPrice << endl;
				}
				temp.close(); // zatvorenie suboru temp.txt
				remove("Produkty.txt"); // odstranenie suboru
				rename("temp.txt", "Produkty.txt"); // premenovanie suboru temp.txt na Produkty.txt
				if (fin.is_open() || temp.is_open()) // kontrola ci je subor zatvoreny
					fileCouldntBeClosed();
				break;
			}
			else
			{
				string yesOrNo;
				cout << "Cena produktu je neplatna!" << endl;
				cout << "Chcete znova zadat nazov produktu? ano/nie " << endl;
				cout << "-> ";
				cin >> yesOrNo;
				transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper);
				if (yesOrNo == "ANO")
				{
					system("CLS");
					continue;
				}
				else
					break;
			}
		}
		else
		{
			string yesOrNo;
			cout << "Produkt ktory ste zadali sa v zozname nenachadza" << endl;
			cout << "Chcete znova zadat nazov produktu? ano/nie " << endl;
			cout << "-> ";
			cin >> yesOrNo;
			transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper);
			if (yesOrNo == "ANO")
			{
				system("CLS");
				continue;
			}
			else
				break;
		}
	}
	system("CLS");
}

void removeProduct() {
	system("CLS");
	ifstream fin; // ifstream na citanie suboru
	ofstream temp; // ofstream na zapisovanie do suboru
	fin.open("Produkty.txt"); // otvorenie suboru ifstreamom
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	cout << "!!!!!! Vymazanie produktu !!!!!!" << endl; // info pre uzivatela
	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
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
			fileCouldntBeClosed();
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
					fileCouldntBeClosed();
			}
			else
			{
				temp.close();
				fileCouldntBeOpened();
			}
		}
		else
			fileCouldntBeOpened();
	}
	else
		fileCouldntBeOpened();
	system("CLS");
}

void changeReceiptData(bool isEmpty) {
	system("CLS");
	ofstream temp;
	ifstream fin;
	if (!isEmpty)
	{
		cout << "****************************" << endl;
		cout << "* Zmena udajov pre blociky *" << endl;
		cout << "****************************" << endl << endl;
	}
	else
	{
		cout << "****************************************" << endl;
		cout << "* Nastavenie novych udajov pre blociky *" << endl;
		cout << "****************************************" << endl << endl;
	}
	fin.open("Udaje.txt");
	if (!fin.is_open())
		fileCouldntBeOpened();
	cout << "Aktulane udaje: " << endl << endl;
	string line; // premenna pre funkciu getline
	while (getline(fin, line)) // getline zoberie riadok zo suboru a ulozi ho do premennej line
	{
		// vypisovanie jednotlivych riadkov
		cout << "\t" << line << endl;
	}
	fin.close();
	if (fin.is_open())
		fileCouldntBeClosed();
	char userName[50];
	char userAdress[100];
	string userICO;
	cout << "Zadajte novy nazov obchodu: ";
	cin.getline(userName, 50);
	cout << "Zadajte novu adresu: ";
	cin.getline(userAdress, 100);
	string adress = userAdress;
	string name = userName;
	cout << "Zadajte nove ICO: ";
	while (true)
	{
		cin >> userICO;
		try
		{
			int userICOint = stoi(userICO);
			break;
		}
		catch (...)
		{
			cout << "Zadal si nespravne ICO, skus to znova: ";
			continue;
		}
	}
	temp.open("temp.txt");
	if (!temp.is_open())
		fileCouldntBeOpened();
	temp << "Nazov:\t" << name << endl;
	temp << "Adresa:\t" << adress << endl;
	temp << "ICO:\t" << userICO;
	temp.close();
	if (temp.is_open())
		fileCouldntBeClosed();
	remove("Udaje.txt");
	rename("temp.txt", "Udaje.txt");
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
			fileCouldntBeClosed();
		else
		{
			remove("AdminLogin.txt"); // odstranenie povodneho suboru AdminLogin.txt
			rename("temp.txt", "AdminLogin.txt"); // premenovanie docasneho suboru temp.txt na AdminLogin.txt
		}
	}
	else
		fileCouldntBeOpened();
	system("CLS");
}

void loginSystem() {
	system("CLS");
	cout << "**********************************************" << endl;
	cout << "* Najskor sa prihlaste, ak chcete pokracovat *" << endl;
	cout << "**********************************************" << endl << endl;
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
			fileCouldntBeClosed();
	}
	else
		fileCouldntBeOpened();
	// vycistenie konzoly v pripade spravneho loginu a hesla
	system("CLS");
}

int checkFileLines(string fileName) {
	ifstream fil; // ifstream na citanie suboru
	fil.open(fileName); // otvorenie suboru
	if (!fil.is_open()) // kontrola ci je subor otvoreny
		fileCouldntBeOpened();
	int counter = 0; // counter na pocitanie riadkov v subore
	string line; // premenna na ulozenie riadku 
	while (getline(fil, line))
	{
		if (line!="" && line!= "\n") // overenie ci nie je riadok prazdny
			counter++; // inkrementacia counteru
	}
	fil.close(); // zatvorenie suboru
	if (fil.is_open()) // kontrola ci je subor zatvoreny
		fileCouldntBeClosed();
	if (counter < 2) // kontrola ci ma subor 1 riadok a menej
		return 0;
	else if (counter == 2)
		return 1;
	else
		return 2;
}

int checkIfFileIsEmpty(string fileName) {
	ifstream fil;
	fil.open(fileName);
	if (!fil.is_open())
		fileCouldntBeOpened();
	fil.seekg(0, ios::end);
	int len = fil.tellg();
	fil.close();
	if (fil.is_open())
		fileCouldntBeClosed();
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
			cin.ignore();
			try // exception
			{
				intMenuInput = stoi(strmenuInput); // konverzia stringu na int  
				if (intMenuInput >= 0 && intMenuInput <= 7) // kontrola, ci je cislo v intervale od 1 po 7
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
				system("CLS");
				cout << "Nespravny input" << endl;
				cout << "Zadaj svoj vyber znova -> ";
				continue; // navrat na zaciatok cyklu
			}
		}
		if (intMenuInput == 1)
		{
			int isEmpty = checkIfFileIsEmpty("Udaje.txt");
			int lineCount = checkFileLines("Udaje.txt");
			if (isEmpty == 0 && lineCount == 0) // kontrola ci nie je subor prazdny
				changeReceiptData(true);
			checkout();
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
			loginSystem();
			int isEmpty = checkIfFileIsEmpty("Udaje.txt");
			int lineCount = checkFileLines("Udaje.txt");
			if (isEmpty != 0 && lineCount != 0 && lineCount != 1) // kontrola ci je subor prazdny
				changeReceiptData(false);
			else
				changeReceiptData(true);
			continue;
		}
		else if (intMenuInput == 6)
		{
			loginSystem();
			changeLoginData(false); // zavolanie funkcie na zmenu prihlasovacich udajov
			continue; // navrat na zaciatok cyklu
		}
		else if(intMenuInput == 7)
		{
			exitFunc(); // zavolanie funkcie na vypnutie programu
			break; // opustenie cyklu
		}
		else
		{
			getActualTime();
			break;
		}
	}
	return 0;
}