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
int numOfCheckouts = 0;

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
	cin.ignore();
	cin.get();
	exit(0); // vypnutie programu
}

// ak sa subor nepodarilo zatvorit vypise sa chybova hlaska a program sa vypne
void fileCouldntBeClosed() {
	system("CLS"); // vycistenie obrazovky
	if (language == "sk") // vypis v dvoch jazykoch
		cout << "Fatal error > Subor sa nepodarilo zatvorit" << endl;
	else
		cout << "Fatal error > File could not be closed" << endl;
	cin.ignore();
	cin.get();
	exit(0); // vypnutie programu
}

// vyber jazyka
void selectLanguage() {
	system("CLS"); // vycistenie obrazovky
	string tabs = "\t\t"; // tabulatory
	string input; // uskladnenie inputu
	cout << tabs << "Vyberte jazyk/Select language" << endl << endl;
	while (true)
	{
		cout << tabs << "\t[1] Slovencina" << endl;
		cout << tabs << "\t[2] English" << endl << endl; // info pre pouzivatela
		cout << tabs << "Vyberte jednu z moznosti/Select one option" << endl;
		cout << tabs << "-> ";
		cin >> input; // input
		if (input == "1") // overenie inputu
		{
			language = "sk";
			break;
		}
		else if (input == "2")
		{
			language = "en";
			break;
		}
		else
		{
			cout << "Zadali ste neplatnu moznost/You have selected invalid option" << endl;
			continue;
		}
	}
	system("CLS"); // vycistenie obrazovky
}
// hladanie bodky a desatinnych miest v cisle
string checkForDotAndDecimals(string price) { // parameter 
	bool containsDot = false;
	bool containsMoreDots = false;
	int counter = 0;
	int dotCounter = 0;
	for (int i = 0; i < price.length(); i++) // prechadzanie stringom
	{	
		if (price[i] == '.') // hladanie bodky
		{
			containsDot = true; // prepisanie false na true
			dotCounter++;
		}
		if (containsDot)
		{
			counter++;
		}
	}
	if (dotCounter > 1)
	{
		containsMoreDots = true;
		price.pop_back();
		price = checkForDotAndDecimals(price);
	}
	if (containsMoreDots)
	{
		price = price + "00";
	}
	if (!containsDot) // kontrola ci neobsahuje bodku
	{
		price = price + ".00"; // pridanie bodky a des. miest k cislu
	}
	if (counter > 3)
	{
		price = ceil(stoi(price) * 100) / 100;
	}
	return price; // navratova hodnota
}

// vypisanie obsahu zo suboru
void printFileContent(string fileName) { // parameter
	ifstream fil; // ifstream na citanie suboru
	fil.open(fileName);
	if (!fil.is_open()) // kontrola ci je subor otvoreny
	{
		fileCouldntBeOpened(); // chybova hlaska
	}
	if (fileName == "Produkty.txt") // kontrola nazvu suboru
	{
		string line; // premenna pre funkciu getline
		while (getline(fil, line)) // prechadzanie suborom
		{
			cout << "\t\t\t" << line; printEuroSign(); // vypisovanie riadkov zo suboru
		}
	}
	else if (fileName == "Products.txt") 
	{
		string line;
		while (getline(fil, line))
		{
			cout << "\t\t\t" << line + "$" << endl;
		}
	}
	else
	{
		string line;
		while (getline(fil,line))
		{
			cout << "\t\t\t" << line << endl;
		}
	}
	fil.close(); 
	if (fil.is_open()) // kontrola ci je subor zatvoreny
	{
		fileCouldntBeClosed(); // chybova hlaska
	}
}

// vyberie data zo suboru ktore si vypytam
string getData(string data, string typeOfData) { // parameter pre hladanie dat
	ifstream fin; // ifstream na citanie suboru
	string fileName; // nazov suboru
	if (typeOfData == "receipt")
	{
		if (language == "sk") // vyber suboru na zakladne jazyka
		{
			fileName = "Udaje.txt";
		}
		else
		{
			fileName = "Data.txt";
		}
	}
	else if (typeOfData == "login")
	{
		fileName = "AdminLogin.txt";
	}
	else
	{
		cout << "Chyba pri type pozadovanych dat";
		cin.ignore();
		cin.get();
		exit(0);
	}
	fin.open(fileName); // otvorenie suboru
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
		fileCouldntBeClosed(); // chybova funkcia
	return extractedData; // navratova hodnota 
}

// vypocet delitela na ziskanie ceny bez DPH
float countTaxDivisor() { 
	string DPH; // premenna pre hodnotu dane
	if (language == "sk") // overenie jazyka
	{
		DPH = getData("DPH", "receipt"); // ziskanie hodnoty zo suboru
	}
	else
	{
		DPH = getData("TAX", "receipt");
	}
	string dphNum = DPH; // kopia premennej DPH
	float dphDivisor; // delitel na vypocet DPH
	dphNum.pop_back(); // odtranenie posledneho znaku v stringu
	try // exception
	{
		dphDivisor = stof(dphNum); // konverzia stringu na float
	}
	catch (...) // pri neuspesnej konverzii
	{
		system("CLS"); // vycistenie obrazovky
		if (language == "sk") // overenie jazyka
		{
			cout << "Neocakavana chyba pri konverzii" << endl; // chybova hlaska
		}
		else
		{
			cout << "Unexpected error occured" << endl;
		}
		cin.ignore();
		cin.get(); // pockanie na ENTER
		exit(0); // opustenie programu
	}
	dphDivisor = (dphDivisor / 100 ) + 1; // vypocet delitela na vypocet DPH
	return dphDivisor; // navratova hodnota
}

// zobrazenie stavu financii
void viewStatus() {
	system("CLS"); // vycistenie obrazovky
	string tabs = "\t\t"; // tabulatory
	if (language == "sk") // overenie jazyka
	{
		cout << tabs << "$$$$$$$$$$$$$$$$$" << endl;
		cout << tabs << "$ Prehlad trzby $" << endl;
		cout << tabs << "$$$$$$$$$$$$$$$$$" << endl << endl;
		cout << tabs << "Pocet nakupov: " << numOfCheckouts << endl;
		cout << tabs << "Celkova trzba: " << money; printEuroSign(); // celkova trzba
		// funkcia ceil - zaokruhlenie
		cout << tabs << "Celkova trzba bez DPH: " << ceil((money / countTaxDivisor()) * 100) / 100; printEuroSign(); // vypocet ceny bez DPH
		cout << tabs << "Sadzba DPH: " << getData("DPH", "receipt") << endl; // dph sadzba
		cout << tabs << "Celkova DPH: " << ceil((money - (money / countTaxDivisor())) * 100) / 100; printEuroSign(); // vypocet celkovej DPH
		cout << tabs << "Pre opustenie stlacte ENTER -> "; // info pre uzivatela
	}
	else
	{
		cout << tabs << "$$$$$$$$$$$$$$$$$$" << endl;
		cout << tabs << "$ Sales overview $" << endl;
		cout << tabs << "$$$$$$$$$$$$$$$$$$" << endl << endl;
		cout << tabs << "Number of checkouts: " << numOfCheckouts << endl;
		cout << tabs << "Total sales: " << money << "$" << endl; // celkova trzba
		// funkcia ceil - zaokruhlenie
		cout << tabs << "Total sales without taxes: " << ceil((money / countTaxDivisor()) * 100) / 100 << "$" << endl; // vypocet ceny bez DPH
		cout << tabs << "Tax: " << getData("TAX", "receipt") << endl; // dph sadzba
		cout << tabs << "Total taxes: " << ceil((money - (money / countTaxDivisor())) * 100) / 100 << "$" << endl; // vypocet celkovej DPH
		cout << tabs << "Press ENTER to exit -> "; // info pre uzivatela
	}
	cin.get(); // narozdiel od klasickeho cin zoberie aj ENTER
	system("CLS"); // vycistenie obrazovky
}

// vypisanie hlavneho menu
void mainMenu() {
	string tabs = "\t\t"; // tabulatory
	if (language == "sk") // overenie jazyka
	{
		// vypisanie hlavneo menu
		cout << tabs << tabs << "\t" << ":::::::::::::::::::::::::::::::" << endl;
		cout << tabs << tabs << "\t" << "::::::::  Hlavne menu  ::::::::" << endl;
		cout << tabs << tabs << "\t" << ":::::::::::::::::::::::::::::::" << endl << endl;
		cout << tabs << "\t[1] Nablokovat produkty" << endl;
		cout << tabs << "\t[2] Pridat produkt" << endl;
		cout << tabs << "\t[3] Odstranit produkt" << endl;
		cout << tabs << "\t[4] Upravit produkt" << endl;
		cout << tabs << "\t[5] Zmenit udaje pre nakupne doklady" << endl;
		cout << tabs << "\t[6] Zmenit prihlasovacie udaje" << endl;
		cout << tabs << "\t[7] Prehlad trzby" << endl;
		cout << tabs << "\t[8] Zmena jazyka" << endl;
		cout << tabs << "\t[9] Exit" << endl << endl;
		cout << tabs << "\tPre pokracovanie zvolte jedno z cisel v hranatych zatvorkach" << endl;
	}
	else
	{
		cout << tabs << tabs << "\t" << ":::::::::::::::::::::::::::::" << endl;
		cout << tabs << tabs << "\t" << "::::::::  Main menu  ::::::::" << endl;
		cout << tabs << tabs << "\t" << ":::::::::::::::::::::::::::::" << endl << endl;
		cout << tabs << "\t[1] Checkout" << endl;
		cout << tabs << "\t[2] Add product" << endl;
		cout << tabs << "\t[3] Remove product" << endl;
		cout << tabs << "\t[4] Edit product" << endl;
		cout << tabs << "\t[5] Change receipt data" << endl;
		cout << tabs << "\t[6] Change login data" << endl;
		cout << tabs << "\t[7] Sales overview" << endl;
		cout << tabs << "\t[8] Change language" << endl;
		cout << tabs << "\t[9] Exit" << endl << endl;
		cout << tabs << "\tTo continue select one of the numbers in square brackets" << endl;
	}
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
	system("CLS"); // vycistenie obrazovky
	if (language == "sk") // dve jazykove varianty
		cout << "/*--*/ Dovidenia /*--*/" << endl; // rozlucenie
	else
		cout << "/*--*/ Goodbye /*--*/" << endl; // rozlucenie
	cin.get();
	exit(0); // vypnutie programu
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
	string fileName; // nazov suboru
	if (language == "sk") // overenie jazyka
	{
		fileName = "Produkty.txt";
	}
	else
	{
		fileName = "Products.txt";
	}
	fin.open(fileName); // otvorenie suboru
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
	string tabs = "\t\t";
	system("CLS"); // vycistenie obrazovky
	// vector == pole s vyuzitim dynamickej pamäte
	vector<string> listOfProducts; // vector na uskladnenie celeho zoznamu produktov aby sa zabranilo zbytocnemu repetitivnemu otvaraniu suboru
	vector<string> products; // vector na uskladnenie nablokovanych produktov
	float price = 0; // vysledna cena
	ifstream fin; // ifstream na citanie zo suboru
	string fileName; // nazov suboru
	if (language == "sk") // overenie jazyka
	{
		fileName = "Produkty.txt";
	}
	else
	{
		fileName = "Products.txt";
	}
	fin.open(fileName); // otvorenie suboru
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
		cout << tabs << "$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
		cout << tabs << "$ Nablokovanie produktov $" << endl; // GUI prvok
		cout << tabs << "$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl << endl;
	}
	else
	{
		cout << tabs << "$$$$$$$$$$$$" << endl;
		cout << tabs << "$ Checkout $" << endl; // GUI prvok
		cout << tabs << "$$$$$$$$$$$$" << endl << endl;
	}
	while (true) // cyklus na pripadne opakovanie v pripade zadania neplatneho produktu
	{
		if (language == "sk")
		{
			cout << tabs << "Zoznam produktov: " << endl << endl;
		}
		else
		{
			cout << tabs << "List of products: " << endl << endl;
		}
		// syntactic sugar. Funguje podobne ako klasicky for cyklus
		for (string i: listOfProducts) // prechadzanie vectorom
		{
			cout << tabs << "\t" << i; // vypisovanie produktov z vectoru
			if (language == "sk")
			{
				printEuroSign(); // output eura
			}
			else
			{
				cout << "$" << endl;
			}
		}
		cout << endl;
		string userProductName; // uskladnenie inputu
		if (language == "sk")
		{
			cout << tabs << "Zadajte nazov produktu, ktory chcete nablokovat. Ak chcete skoncit s blokovanim napiste STOP" << endl; // info pre uzivatela
			cout << tabs << "-> ";
		}
		else
		{
			cout << tabs << "Enter the name of product you want to checkout. If you want to finish the checkout type STOP" << endl; // info pre uzivatela
			cout << tabs << "-> ";
		}
		getline(cin, userProductName); // input
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
					if (language == "sk") // overenie jazyka
					{
						cout << "Neocakavana chyba pri cene produktu" << endl; // chybova hlaska
					}
					else
					{
						cout << "Unexpected error has occured" << endl;
					}
					cin.get();
					exit(0); // vypnutie programu
				}
			}
		}
		else // ak sa produkt v zozname nenachadza spusti sa tato cast
		{
			string yesOrNo; // uskladnenie inputu
			if (language == "sk")
			{
				cout << tabs << "Produkt, ktory ste zadali sa v zozname nenachadza" << endl; // info pre uzivatela 
				cout << tabs << "Chcete zadat produkt este raz? ano/nie" << endl; // info pre uzivatela
				cout << tabs << "-> ";
			}
			else
			{
				cout << tabs << "Product you have entered is not listed" << endl; // info pre uzivatela 
				cout << tabs << "Do you want to enter the product's name again? yes/no " << endl; // info pre uzivatela
				cout << tabs << "-> ";
			}
			cin >> yesOrNo; // input
			transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper); // transform stringu na velke pismena
			if (yesOrNo == "ANO" || yesOrNo == "YES") // overenie inputu
			{
				system("CLS"); // vycistenie obrazovky
				cin.ignore();
				continue; // navrat na zaciatok cyklu
			}
			else 
				break; // koniec cyklu
		}
	}
	if (products.size() > 0) // ak vector produktov nie je prazdny
	{
		system("CLS"); // vycistenie obrazovky
		// TODO
		// vypisanie nakupneho dokladu
		string tab = "\t\t\t\t"; // rovnaky pocet tabulatorov pre vsetko
		if (language == "sk")
		{
			cout << tab << "----------------------------------------------" << endl; // GUI prvok
			cout << tab << getData("Nazov", "receipt") << endl << endl; // vypytanie si Nazvu zo suboru a vypisanie 
			cout << tab << getData("Adresa", "receipt") << endl; // vypytanie si Adresy zo suboru a vypisanie
			cout << tab << "ICO: " << getData("ICO", "receipt") << endl; // vypytanie si ICO zo suboru a vypisanie
			cout << tab << getActualTime(); // vypisanie aktualneho casu
			cout << tab << "----------------------------------------------" << endl; // GUI prvok
		}
		else
		{
			cout << tab << "----------------------------------------------" << endl; // GUI prvok
			cout << tab << getData("Name", "receipt") << endl << endl; // vypytanie si Nazvu zo suboru a vypisanie 
			cout << tab << getData("Adress", "receipt") << endl; // vypytanie si Adresy zo suboru a vypisanie
			cout << tab << "ID: " << getData("ID", "receipt") << endl; // vypytanie si ICO zo suboru a vypisanie
			cout << tab << getActualTime(); // vypisanie aktualneho casu
			cout << tab << "----------------------------------------------" << endl; // GUI prvok
		}
		for (string i: products) // prechadzanie vectorom
		{
			cout << tab << i; // vypisovanie jednotlivych produktov a ich cien
			if (language == "sk") // oeverenie jazyka
			{
				printEuroSign(); // output eura
			}
			else
			{
				cout << "$" << endl;
			}
		}
		cout << endl;
		if (language == "sk")
		{
			cout << tab << "SUMA" << "\t\t\t" << price; // vypisanie vyslednej sumy
			printEuroSign(); // output eura
			cout << endl << tab << "Suma bez DPH" << "\t\t" << ceil((price / countTaxDivisor()) * 100) / 100; // vypocet ceny bez DPH
			printEuroSign(); // output eura
			cout << tab << "Sadzba DPH:\t\t" << getData("DPH", "receipt") << endl; // vypisanie sadzby DPH 
			cout << tab << "DPH:\t\t\t" << ceil((price - (price / countTaxDivisor())) * 100) / 100; // vypocet DPH
			printEuroSign(); // output eura
			cout << tab << "----------------------------------------------" << endl; // GUI prvok
			cout << tab << "\t\tDakujeme za nakup" << endl;
			cout << tab << "----------------------------------------------" << endl; // GUI prvok
		}
		else
		{
			cout << tab << "SUM" << "\t\t\t" << price; // vypisanie vyslednej sumy
			cout << "$" << endl; // output eura
			cout << endl << tab << "SUM without TAX" << "\t\t" << ceil((price / countTaxDivisor()) * 100) / 100; // vypocet ceny bez DPH
			cout << "$" << endl; // output eura
			cout << tab << "TAX:\t\t\t" << getData("TAX", "receipt") << endl; // vypisanie sadzby DPH 
			cout << tab << "Total TAX:\t\t" << ceil((price - (price / countTaxDivisor())) * 100) / 100; // vypocet DPH
			cout << "$" << endl; // output eura
			cout << tab << "----------------------------------------------" << endl; // GUI prvok
			cout << tab << "\tThank you for your purchase" << endl;
			cout << tab << "----------------------------------------------" << endl; // GUI prvok
		}
		while (true) // platba
		{
			if (language == "sk")
			{
				cout << endl << tabs << "Moznosti platby: " << endl << endl; // info pre uzivatela
				cout << tabs << "\t[1] Hotovost" << endl;
				cout << tabs << "\t[2] Platobna karta" << endl << endl;
				cout << tabs << "Prosim zvolte jedenu z moznosti zadanim cisla v hranatych zatvorkach" << endl;
				cout << tabs << "Vasa volba -> ";
			}
			else
			{
				cout << endl << tabs << "Payment methods: " << endl << endl; // info pre uzivatela
				cout << tabs << "\t[1] Cash" << endl;
				cout << tabs << "\t[2] Card" << endl << endl;
				cout << tabs << "Please select one of the options by selecting number from square brackets" << endl;
				cout << tabs << "Your selection -> ";
			}
			string s; // uskladnenie inputu
			cin >> s; // input
			if (s == "2") // platba kartou
			{
				if (language == "sk")
				{
					cout << tabs << "Prebieha platba" << endl;
					cout << tabs << "Zadajte svoj PIN: "; // info pre uzivatela
				}
				else
				{
					cout << tabs << "Payment in progress" << endl;
					cout << tabs << "Enter your PIN code: "; // info pre uzivatela
				}
				cin >> s; // input PINu
				if (s.length() == 4) // kontrola ci ma PIN styri cislice
				{
					money += price; // pripocitanie penazi na nas ucet
					numOfCheckouts++;
					break; // ukoncenie cyklu
				}
				else // neplatny PIN
				{
					system("CLS"); // vycistenie obrazovky
					if (language == "sk")
					{
						cout << tabs << "Zadali ste neplatny PIN. Skuste to este raz" << endl; // info pre pouzivatela
					}
					else
					{
						cout << tabs << "Invalid PIN code. Try again" << endl; // info pre pouzivatela
					}
					continue; // navrat na zaciatok cyklu
				}
			}
			else if (s == "1") // platba v hotovosti
			{
				string userCash; // uskladnenie inputu
				float userCashFloat = 0; // input vo forme float
				while (true)
				{
					if (language == "sk") // overenie jazyka
					{
						cout << tabs <<  "Obdrzana suma: "; // info pre uzivatela
					}
					else
					{
						cout << tabs <<  "Received cash: ";
					}
					cin >> userCash; // input 
					try // exception
					{
						userCash = checkForComma(userCash); // kontrola ciarok v inpute
						userCashFloat = stof(userCash); // konverzia stringu na float
						break;
					}
					catch (...) // pri neuspesnej konverzii
					{
						if (language == "sk")
						{
							cout << tabs << "Zadali ste neplatnu cenu, skuste to znova" << endl;
						}
						else
						{
							cout << tabs << "You have entered invalid value, try again" << endl;
						}
						continue;
					}
				}
				float vydavok = userCashFloat - price; // vypocet vydavku
				if (vydavok >= 0) // overenie ci je vydavok kladny
				{
					if (language == "sk")
					{
						cout << tabs << "Vydavok: " << vydavok; printEuroSign();
						cout << tabs << "-> ";
					}
					else
					{
						cout << tabs << "Change: " << vydavok << "$" << endl;
						cout << tabs << "-> ";
					}
					cin.ignore(); // bez toho to nejde
					cin.get(); // pockanie na ENTER
					money += price; // pripocitanie penazi na nas ucet
					numOfCheckouts++; // pripocitanie hodnoty do celkoveho poctu nakupov
					break; // opustenie cyklu
				}
				else
				{
					system("CLS"); 
					if (language == "sk")
					{
						cout << tabs << "Zakaznik dal malo penazi. Skuste inu moznost platby" << endl;
						cout << tabs << "SUMA: " << price; printEuroSign(); cout << endl;
					}
					else
					{
						cout << tabs << "Customer hasn't paid enough. Try different payment method" << endl;
						cout << tabs << "SUM: " << price; cout << "$" << endl << endl;
					}
					continue;
				}
			}
			else // v pripade neplatnej moznosti
			{
				system("CLS"); // vycistenie obrazovky
				if (language == "sk")
				{
					cout << tabs << "Zadali ste neplatnu moznost!!" << endl; // info pre uzivatela
					cout << tabs << "SUMA" << "\t\t" << price; // znovu vypisanie ceny
					printEuroSign(); // output eura
				}
				else
				{
					cout << tabs << "You have entered invalid option!!" << endl; // info pre uzivatela
					cout << tabs << "SUM" << "\t\t" << price << "$" << endl; // znovu vypisanie ceny
				}
				cout << endl;
				continue; // navrat na zaciatok cyklu
			}
		}
	}
	else // ak je prazdny zoznam nablokovanych produktov spusti sa toto
	{
		system("CLS"); // vycistenie obrazovky
		if (language == "sk")
		{
			cout << tabs << "Prazdny zoznam nablokovanych produktov!" << endl; // info pre uzivatela
		}
		else
		{
			cout << tabs << "Empty list of checkout products!" << endl; // info pre uzivatela
		}
		while (true) 
		{
			string oneOrZero; // uskladnenie inputu
			if (language == "sk")
			{
				cout << tabs << "Ak sa chcete vratit na hlavne menu zadajte '1'. Ak chcete ukoncit program zadajte '0'." << endl; // info
				cout << tabs << "-> ";
			}
			else
			{
				cout << tabs << "Press '1' if you want to return to main menu. Press '0' if you want to quit program." << endl; // info
				cout << tabs << "-> ";
			}
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
				if (language == "sk")
				{
					cout << tabs << "Neplatný vstup. Skúste to znova" << endl; // info pre uzivatela
				}
				else
				{
					cout << tabs << "Invalid input. Try again" << endl; // info pre uzivatela
				}
				continue; // navrat na zaciatok cyklu
			}
		}
	}
	system("CLS"); // vycistenie obrazovky
}


// pridanie produktu
void addProduct() {
	string tabs = "\t\t";
	system("CLS"); // vycistenie obrazovky
	// fstream - multifunkcny. Citanie, pisanie, pridavanie
	fstream fil; // fstream na citanie a aj na pridavanie do suboru
	string fileName; // nazov suboru
	if (language == "sk")
	{
		fileName = "Produkty.txt";
	}
	else
	{
		fileName = "Products.txt";
	}
	string userProductName; // uskladnenie inputu 
	string userProductPrice; // uskladnenie inputu
	if (language == "sk")
	{
		cout << tabs << "++++++++++++++++++++++++++++++" << endl;
		cout << tabs << "|| Pridanie noveho produktu ||" << endl; // GUI prvok
		cout << tabs << "++++++++++++++++++++++++++++++" << endl << endl;
	}
	else
	{
		cout << tabs << "+++++++++++++++++++++" << endl;
		cout << tabs << "|| Add new product ||" << endl; // GUI prvok
		cout << tabs << "+++++++++++++++++++++" << endl << endl;
	}
	while (true)
	{
		if (language == "sk")
		{
			cout << tabs << "Zoznam produktov: " << endl << endl;
		}
		else
		{
			cout << tabs << "List of products: " << endl << endl;
		}
		printFileContent(fileName); // vypisanie produktov zo suboru
		cout << endl;
		bool isValid = true; 
		if (language == "sk")
		{
			cout << tabs << "Zadajte nazov noveho produktu: "; // info pre uzivatela
			getline(cin, userProductName);
			cout << tabs << "Zadajte cenu noveho produktu: "; // info pre uzivatela
		}
		else
		{
			cout << tabs << "Enter new product's name: "; // info pre uzivatela
			getline(cin, userProductName);
			cout << tabs << "Enter the price of new product: "; // info pre uzivatela
		}
		cin >> userProductPrice; // input
		userProductPrice = checkForComma(userProductPrice); // hladanie ciarky v cene a jej nahradenie
		try // exception
		{
			float checkFloat = stof(userProductPrice); // konverzia na float aby sme zistili ci uzivatel zadal platnu cenu
		}
		catch (...) // v pripade neuspesnej konverzie v bloku try
		{
			isValid = false; // prepisanie true na false
		}
		fil.open(fileName, ios::in); // otvorenie suboru na citanie - ios::in
		if (!fil.is_open()) // kontrola ci je subor otvoreny
			fileCouldntBeOpened(); // chybova funkcia
		bool isDuplicate = false;
		if (containsProduct(userProductName)) // kontrola duplikatu
			isDuplicate = true; // prepiasnie false na true
		fil.close(); // zatvorenie suboru
		if (fil.is_open()) // kontrola ci je subor zatvoreny
			fileCouldntBeClosed(); // chybova funkcia
		if (!isDuplicate && isValid) // ak nie je duplikat a ak je validny
		{
			userProductPrice = checkForDotAndDecimals(userProductPrice);
			fil.open(fileName, ios::app); // otvorenie suboru na pridavanie - ios::app - append
			if (!fil.is_open()) // kontrola ci je subor otvoreny
				fileCouldntBeOpened(); // chybova funkcia
			string tabs; // uskladnenie poctu tabulatorov
			if (userProductName.length() <= 7) // kontrola dlzky zadaneho nazvu produktu
			{
				string tabs = "\t\t\t"; // cim mensia dlzka tym viac tabulatorov
				fil << userProductName << tabs << userProductPrice << endl; // zapisanie produktu do suboru
			}
			else if(userProductName.length() < 11) 
			{
				string tabs = "\t\t";
				fil << userProductName << tabs << userProductPrice << endl; // zapisanie produktu do suboru
			}
			else
			{
				string tabs = "\t";
				fil << userProductName << tabs << userProductPrice << endl; // zapisanie produktu do suboru
			}
			fil.close(); // zatvorenie suboru
			if (fil.is_open()) // kontrola ci je subor zatvoreny
				fileCouldntBeClosed(); // chybova funkcia
			break; // koniec cyklu
		}
		else // ak je duplikat alebo nie je validny
		{
			string yesOrNo; // uskladnenie inputu
			if (language == "sk")
			{
				cout << tabs << "Produkt uz je v zozname alebo ste zadali neplatnu cenu" << endl; // info pre uzivatela
				cout << tabs << "Chcete zadat produkt este raz? ano/nie" << endl; // info pre uzivatela
				cout << tabs << "-> ";
			}
			else
			{
				cout << tabs << "Product has been already listed or you have entered invalid price" << endl; // info pre uzivatela
				cout << tabs << "Do you want to enter product again? yes/no " << endl; // info pre uzivatela
				cout << tabs << "-> ";
			}
			cin >> yesOrNo; // input
			transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper); // pretransformovanie inputu na velke pismena
			if (yesOrNo == "ANO" || yesOrNo == "YES") // kontrola inputu
			{
				system("CLS"); // vycistenie obrazovky
				cin.ignore();
				continue; // navrat na zaciatok cyklu
			}
			else
				break; // navrat do hlavneho menu
		}
	}
	system("CLS"); // vycistenie obrazovky
}

// upravenie produktu
void editProduct() {
	string tabs = "\t\t";
	system("CLS"); // vycistenie obrazovky
	ifstream fin; // ifstream na citanie suboru
	string fileName; // nazov suboru
	if (language == "sk")
	{
		fileName = "Produkty.txt";
	}
	else
	{
		fileName = "Products.txt";
	}
	string userProductName; // uskladnenie inputu
	string userProductPrice; // uskladnenie ceny
	if (language == "sk")
	{
		cout << tabs << "**********************" << endl;
		cout << tabs << "* Upravenie produktu *" << endl; // GUI prvok
		cout << tabs << "**********************" << endl << endl;
	}
	else
	{
		cout << tabs << "****************" << endl;
		cout << tabs << "* Edit product *" << endl; // GUI prvok
		cout << tabs << "****************" << endl << endl;
	}
	while (true) 
	{
		string line;
		if (language == "sk")
		{
			cout << tabs << "Zoznam produktov: " << endl << endl; // info pre uzivatela
			printFileContent(fileName);
			cout << endl << tabs << "Zadajte nazov produktu ktory chcete upravit: "; // info pre uzivatela
			getline(cin, userProductName); // input
		}
		else
		{
			cout << tabs << "List of products: " << endl << endl; // info pre uzivatela
			printFileContent(fileName);
			cout << endl << tabs << "Enter the name of product you want to change: "; // info pre uzivatela
			getline(cin, userProductName); // input
		}
		fin.open(fileName); // otvorenie suboru
		if (!fin.is_open()) // kontorola ci je subor otvoreny
			fileCouldntBeOpened(); // chybova funkcia
		bool isValid = false;
		if (containsProduct(userProductName)) // kontrola ci subor obsahuje dany produkt
			isValid = true; // prepisanie false na true
		fin.close(); // zatvorenie suboru
		if (fin.is_open()) // kontorola ci je subor zatvoreny
			fileCouldntBeClosed(); // chybova funkcia
		if (isValid) // kontrola ci je produkt validny
		{
			string editeProductName;
			ofstream temp; // ofstream na zapisaovanie do suboru
			if (language == "sk")
			{
				cout << tabs << "Zadajte novy nazov produktu: "; // info pre uzivatela
				getline(cin, editeProductName); // input
				cout << tabs << "Zadajte novu cenu produktu: "; // info pre uzivatela
			}
			else
			{
				cout << tabs << "Enter new product's name: "; // info pre uzivatela
				getline(cin, editeProductName); // input
				cout << tabs << "Enter new product's price: "; // info pre uzivatela
			}
			cin >> userProductPrice; // input
			userProductPrice = checkForComma(userProductPrice); // kontrola ci zadana cena obsahuje ciarku a jej nahradenie za bodku
			try // exception
			{
				float checkFloat = stof(userProductPrice); // konverzia stringu na float pre overenie ci uzivatel zadal validnu cenu
			}
			catch (...) // pri neuspesnej koverzii
			{
				isValid = false; // prepisane true na false
			}
			
			if (isValid) // ak je input validny
			{
				userProductPrice = checkForDotAndDecimals(userProductPrice); // kontrola ci ma cena desatinne miesta a bodku pre lepsie zobrazenie
				fin.open(fileName); // otvorenie suboru
				if (!fin.is_open()) // kontorola ci je subor otvoreny
					fileCouldntBeOpened(); // chybova funkcia
				temp.open("temp.txt"); // otvorenie suboru
				if (!temp.is_open()) { // kontorola ci je subor otvoreny
					fin.close(); // zatvorenie suboru
					fileCouldntBeOpened(); // chybova funkcia
				} 					
				while (getline(fin, line)) // getline zoberie riadok zo suboru a ulozi ho do premennej line
				{
					// podmienka ktora zisti ci riadok obsahuje alebo neobsahuje produkt ktory chce pouzivatel odstranit
					if (line.substr(0, (userProductName + "\t").size()) != userProductName + "\t")
						temp << line << endl; // zapisanie riadku do docasneho suboru temp.txt pokial neobsahuje produkt ktory chce uzivatel zmazat
				}
				fin.close(); // zatvorenie suboru 
				string tabs; // pocet tabulatorov
				if (editeProductName.length() <= 7) // kontrola dlzky zadaneho nazvu produktu
				{
					string tabs = "\t\t\t"; // cim kratsi nazov tym viac tabulatorov
					temp << editeProductName << tabs << userProductPrice << endl; // zapisanie produktu do suboru
				}
				else if (editeProductName.length() <= 15) 
				{
					string tabs = "\t\t";
					temp << editeProductName << tabs << userProductPrice << endl; // zapisanie produktu do suboru
				}
				else 
				{
					string tabs = "\t";
					temp << editeProductName << tabs << userProductPrice << endl; // zapisanie produktu do suboru
				}
				temp.close(); // zatvorenie suboru
				if (language == "sk")
				{
					remove("Produkty.txt"); // odstranenie suboru
					rename("temp.txt", "Produkty.txt"); // premenovanie suboru temp.txt na Produkty.txt
				}
				else
				{
					remove("Products.txt"); // odstranenie suboru
					rename("temp.txt", "Products.txt"); // premenovanie suboru temp.txt na Produkty.txt
				}
				if (fin.is_open() || temp.is_open()) // kontrola ci je subor zatvoreny
					fileCouldntBeClosed(); // chybova funkcia
				break; // opustenie cyklu
			}
			else // ak nie je validna cena
			{
				string yesOrNo; // uskladnenie inputu
				if (language == "sk")
				{
					cout << tabs << "Cena produktu je neplatna!" << endl; // info pre pouzivatela
					cout << tabs << "Chcete znova zadat produkt? ano/nie " << endl; // info pre pouzivatela
					cout << tabs << "-> ";
				}
				else
				{
					cout << tabs << "Invalid product's price!" << endl; // info pre pouzivatela
					cout << tabs << "Do you want to enter product again? yes/no " << endl; // info pre pouzivatela
					cout << tabs << "-> ";
				}
				cin >> yesOrNo; // input
				transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper); // transform inputu na velke pismena
				if (yesOrNo == "ANO" || yesOrNo == "YES") // overnie inputu
				{
					system("CLS"); // vycistenie obrazovky
					cin.ignore();
					continue; // navrat na zaciatok cyklu
				}
				else 
					break; // opustenie cyklu
			}
		}
		else // ak sa produkt v zozname nenachadza
		{
			string yesOrNo; // uskladnenie inputu
			if (language == "sk")
			{
				cout << tabs << "Produkt ktory ste zadali sa v zozname nenachadza" << endl; // info pre uzivatela
				cout << tabs << "Chcete znova zadat produkt? ano/nie " << endl; // info pre uzivatella
				cout << tabs << "-> ";
			}
			else
			{
				cout << tabs << "Product you have entered is not listed" << endl; // info pre uzivatela
				cout << tabs << "Do you want to enter product again? yes/no " << endl; // info pre uzivatella
				cout << tabs << "-> ";
			}
			cin >> yesOrNo; // innput
			transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper); // transform inputu na velke pismena
			if (yesOrNo == "ANO" || yesOrNo == "YES") // overenie inputu
			{
				system("CLS"); // vycistenie obrazovky
				cin.ignore();
				continue; // navrat na zaciatok cyklu
			}
			else
				break; // opustenie cyklu
		}
	}
	system("CLS"); // vycistenie obrazovky
}

// odstranenie produktu
void removeProduct() {
	string tabs = "\t\t";
	system("CLS"); // vycistenie obrazovky
	ifstream fin; // ifstream na citanie suboru
	string fileName; // nazov suboru
	if (language == "sk")
	{
		fileName = "Produkty.txt";
	}
	else
	{
		fileName = "Products.txt";
	}
	ofstream temp; // ofstream na zapisovanie do suboru
	fin.open(fileName); // otvorenie suboru ifstreamom
	if (language == "sk")
	{
		cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << tabs << "!!!!!! Vymazanie produktu !!!!!!" << endl; // info pre uzivatela
		cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << tabs << "Zoznam produktov: " << endl << endl; // vypisanie produktov
	}
	else
	{
		cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << tabs << "!!!!!! Remove product !!!!!!" << endl; // info pre uzivatela
		cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << tabs << "List of products: " << endl << endl; // vypisanie produktov
	}
	if(fin.is_open()) // kontrola, ci je subor otvoreny
	{
		string line; // premenna pre funkciu getline
		printFileContent(fileName); // vypisanie produktov zo suboru
		fin.close(); // zatvorenie suboru
		if (fin.is_open()) // kontorola ci je subor zatvoreny
			fileCouldntBeClosed();
		string userInput; // vytvorenie premennej typu string na uskladnenie inputu od pouzivatela
		
		if (language == "sk")
		{
			cout << endl << tabs << "Zadajte nazov produktu: "; // info pre pouzivatela
		}
		else
		{
			cout << endl << tabs << "Enter product's name: "; // info pre pouzivatela
		}
		getline(cin, userInput); // ulozenie inputu do premennej
		temp.open("temp.txt"); // otvorenie suboru ofstreamom
		if (temp.is_open()) // kontrola ci je subor otvoreny
		{
			// otvorenie suboru druhym ifstreamom
			fin.open(fileName);
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
				if (language == "sk")
				{
					remove("Produkty.txt"); // odstranenie suboru
					rename("temp.txt", "Produkty.txt"); // premenovanie suboru temp.txt na Produkty.txt
				}
				else
				{
					remove("Products.txt"); // odstranenie suboru
					rename("temp.txt", "Products.txt"); // premenovanie suboru temp.txt na Produkty.txt
				}
				if (fin.is_open() || temp.is_open()) // kontrola ci je subor zatvoreny
					fileCouldntBeClosed(); // chybova funkcia
			}
			else
			{
				temp.close(); // zatvorenie suboru
				fileCouldntBeOpened(); // chybova funkcia
			}
		}
		else
			fileCouldntBeOpened(); // chybova funkcia
	}
	else
		fileCouldntBeOpened(); // chybova funkcia
	system("CLS"); // vycistenie obrazovky
}

// zmena udajov pre blociky
void changeReceiptData(bool isEmpty) { // parameter funkcie
	string tabs = "\t\t"; // tabulatory
	system("CLS"); // vycistenie obrazovky
	ofstream temp; // ofstream na zapisovanie do suboru
	ifstream fin; // ifstream na citanie suboru
	string fileName; // nazov suboru
	if (language == "sk")
	{
		fileName = "Udaje.txt";
	}
	else
	{
		fileName = "Data.txt";
	}
	if (!isEmpty) // ak subor nie je prazdny
	{
		if (language == "sk")
		{
			cout << tabs << "************************************" << endl;
			cout << tabs << "* Zmena udajov pre nakupne doklady *" << endl; // GUI prvok
			cout << tabs << "************************************" << endl << endl;
		}
		else
		{
			cout << tabs << "***********************" << endl;
			cout << tabs << "* Change receipt data *" << endl; // GUI prvok
			cout << tabs << "***********************" << endl << endl;
		}
	}
	else // ak je subor prazdny
	{
		if (language == "sk")
		{
			cout << tabs << "************************************************" << endl;
			cout << tabs << "* Nastavenie novych udajov pre nakupne doklady *" << endl; // GUI prvok
			cout << tabs << "************************************************" << endl << endl;
		}
		else
		{
			cout << tabs << "*****************************" << endl;
			cout << tabs << "* Set new data for receipts *" << endl; // GUI prvok
			cout << tabs << "*****************************" << endl << endl;
		}
	}
	fin.open(fileName); // otvorenie suboru
	if (!fin.is_open()) // kontrola ci je subor otvoreny
		fileCouldntBeOpened(); // chybova funkcia
	if (language == "sk")
	{
		cout << tabs << "Aktulane udaje: " << endl << endl; // info pre uzivatela
	}
	else
	{
		cout << tabs << "Current data: " << endl << endl; // info pre uzivatela
	}
	printFileContent(fileName); // vypisanie produktov zo suboru
	cout << endl; 
	fin.close(); // zatvorenie suboru
	if (fin.is_open()) // kontrola ci je subor zatvoreny
		fileCouldntBeClosed(); // chybova funkcia
	string userName; // uskladnenie inputu
	string userAdress; // uskladnenie inputu
	string userICO; // uskladnenie inputu
	string userDPH; // uskladnenie inputu
	if (language == "sk")
	{
		cout << tabs << "Zadajte novy nazov obchodu: "; // info pre uzivatela
		cin.ignore(); // na ignorovanie noveho riadku. Program by bez toho nefungoval
		getline(cin, userName); // input celeho riadku nie len jedneho slova
		cout << tabs << "Zadajte novu adresu: "; // info pre uzivatela
		getline(cin, userAdress); // input celeho riadku nie len jedneho slova
		cout << tabs << "Zadajte nove ICO: "; // info pre uzivatela
	}
	else
	{
		cout << tabs << "Enter new name of your store: "; // info pre uzivatela
		cin.ignore(); // na ignorovanie noveho riadku. Program by bez toho nefungoval
		getline(cin, userName); // input celeho riadku nie len jedneho slova
		cout << tabs << "Enter new adress: "; // info pre uzivatela
		getline(cin, userAdress); // input celeho riadku nie len jedneho slova
		cout << tabs << "Enter new ID: "; // info pre uzivatela
	}
	while (true) 
	{
		try // exception
		{
			cin >> userICO; // input
			int userICOint = stoi(userICO); // konverzia stringu na int
			break; // opustenie cyklu
		}
		catch (...) // ak je neuspesna konverzia
		{
			if (language == "sk")
			{
				cout << tabs << "Zadali ste nespravne ICO, skuste to znova: "; // info pre uzivatela
			}
			else
			{
				cout << tabs << "You have entered invalid ID, try again: "; // info pre uzivatela
			}
			continue; // navrat na zaciatok cyklu
		}
	}
	while (true)
	{
		if (language == "sk")
		{
			cout << tabs << "Zadajte novu hodnotu DPH bez znaku %: "; // info pre uzivatela
		}
		else
		{
			cout << tabs << "Enter new tax value without % character: "; // info pre uzivatela
		}
		try // exception
		{
			cin >> userDPH; // input
			float userDPHfloat = stof(userDPH); // konverzia stringu na float. Tymto viem zabranit tomu aby nahodou uzivatel nezadal nieco co nechcem
			break; // opustenie cyklu
		}
		catch (...) // ak je neuspesna konverzia
		{
			if (language == "sk")
			{
				cout << tabs << "Zadali ste nespravnu DPH, skuste to znova" << endl; // info pre uzivatela
			}
			else
			{
				cout << tabs << "You have entered invalid TAX, try again" << endl; // info pre uzivatela
			}
			continue; // navrat na zaciatok cyklu
		}
	}
	temp.open("temp.txt"); // otvorenie suboru
	if (!temp.is_open()) // kontrola ci je subor otvoreny
		fileCouldntBeOpened(); // chybova funkcia
	if (language == "sk")
	{
		temp << "Nazov:\t" << userName << endl; // zapisanie do suboru
		temp << "Adresa:\t" << userAdress << endl;  // zapisanie do suboru
		temp << "ICO:\t" << userICO << endl; // zapisanie do suboru
		temp << "DPH:\t" << userDPH + "%";
	}
	else
	{
		temp << "Name:\t" << userName << endl; // zapisanie do suboru
		temp << "Adress:\t" << userAdress << endl;  // zapisanie do suboru
		temp << "ID:\t" << userICO << endl; // zapisanie do suboru
		temp << "TAX:\t" << userDPH + "%";
	}
	temp.close(); // zatvorenie suboru
	if (temp.is_open()) // kontrola ci je subor zatvoreny
		fileCouldntBeClosed(); // chybova funkcia
	if (language == "sk")
	{
		remove("Udaje.txt"); // odstranenie suboru
		rename("temp.txt", "Udaje.txt"); // premenovanie suboru temp.txt na Produkty.txt
	}
	else
	{
		remove("Data.txt"); // odstranenie suboru
		rename("temp.txt", "Data.txt"); // premenovanie suboru temp.txt na Produkty.txt
	}
	system("CLS"); // vycistenie obrazovky
} 

// zmena prihlasovacich udajov
void changeLoginData(bool isEmpty) { // parameter
	string tabs = "\t\t"; // tabulatory
	system("CLS"); // vycistenie obrazovky
	if (!isEmpty)  // ak subor nie je prazdny 
	{
		if (language == "sk")
		{
			cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl; // maly GUI prvok
			cout << tabs << "!!!!!!! Zmena prihlasovacich udajov !!!!!!!" << endl; // upozornenie pre uzivatela
			cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
		}
		else
		{
			cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl; // maly GUI prvok
			cout << tabs << "!!!!!!! Change login data !!!!!!!" << endl; // upozornenie pre uzivatela
			cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
		}
	}
	else // ak je subor prazdny
	{
		if (language == "sk")
		{
			cout << tabs << " =============================================" << endl;
			cout << tabs << "||  Nastavenie novych prihlasovacich udajov  ||" << endl; // GUI prvok
			cout << tabs << " =============================================" << endl << endl;
		}
		else
		{
			cout << tabs << " ========================" << endl;
			cout << tabs << "||  Set new login data  ||" << endl; // GUI prvok
			cout << tabs << " ========================" << endl << endl;
		}
	}
	ofstream temp; // ofstream na zapisovanie do suboru
	string loginData; // uskladnenie inputu
	string passwordData; // uskladnenie inputu
	if (language == "sk")
	{
		cout << tabs << "Zadajte novy login: "; // vypytanie si loginu od uzivatela
		cin >> loginData;  // ulozenie inputu do jednej z premennych
		cout << tabs << "Zadajte nove heslo: "; // vypytanie si hesla od uzivatela
	}
	else
	{
		cout << tabs << "Enter new login: "; // vypytanie si loginu od uzivatela
		cin >> loginData;  // ulozenie inputu do jednej z premennych
		cout << tabs << "Enter new password: "; // vypytanie si hesla od uzivatela
	}
	cin >> passwordData; // uskladnenie inputu do jednej z premennych
	temp.open("AdminLogin.txt"); // otvorenie suboru ofstreamom
	if (temp.is_open()) // kontrola ci je subor otvoreny
	{
		temp << "Login:\t" << loginData << endl; // zapisanie loginu do suboru temp.txt
		temp << "Password:\t" << passwordData; // zapisanie hesla do suboru temp.txt
		temp.close(); // zatvorenie suboru
		if (temp.is_open()) // kontrola ci je subor zatvoreny
			fileCouldntBeClosed(); // chybova funkcia
	}
	else
		fileCouldntBeOpened(); // chybova funkcia
	system("CLS"); // vycistenie obrazovky
}

void loginSystem() {
	string tabs = "\t\t"; // tabulatory
	system("CLS"); // vycistenie obraovky
	string loginstr; // uskladnenie inputu
	string passwordstr; // uskladnenie inputu
	string correctLogin = getData("Login", "login");
	string correctPassword = getData("Password", "login");
	for (int i = 0; i < 3; i++)
	{
		if (language == "sk")
		{
			cout << tabs << "**********************************************" << endl;
			cout << tabs << "* Najskor sa prihlaste, ak chcete pokracovat *" << endl; // GUI prvok
			cout << tabs << "**********************************************" << endl << endl;
			cout << tabs << "Login: "; // info pre uzivatela
			cin >> loginstr; // input
			cout << tabs << "Heslo: "; // info pre uzivatela
		}
		else
		{
			cout << tabs << "*****************************" << endl;
			cout << tabs << "* Login before you continue *" << endl; // GUI prvok
			cout << tabs << "*****************************" << endl << endl;
			cout << tabs << "Login: "; // info pre uzivatela
			cin >> loginstr; // input
			cout << tabs << "Password: "; // info pre uzivatela
		}
		cin >> passwordstr; // input
		if (loginstr == correctLogin && passwordstr == correctPassword)
		{
			system("CLS");
			return;
		}
		system("CLS");
	}
	// tato cast sa spusti ak je nespravny login alebo heslo
	system("CLS"); // vycistenie obrazovky
	if (language == "sk")
	{
		cout << "Zistili sme neopravneny pristup" << endl; // info pre uzivatela
	}
	else
	{
		cout << "Unauthorized access detected" << endl; // info pre uzivatela
	}
	cin.ignore();
	cin.get();
	exit(0); // vypnutie programu
} 

// skontroluje kolko ma subor riadkov. Vrati int
int checkFileLines(string fileName) { // paramter funkcie
	ifstream fil; // ifstream na citanie suboru
	fil.open(fileName); // otvorenie suboru
	if (!fil.is_open()) // kontrola ci je subor otvoreny
		fileCouldntBeOpened(); // chybova funkcia
	int counter = 0; // counter na pocitanie riadkov v subore
	string line; // premenna na ulozenie riadku 
	while (getline(fil, line)) // prechadzanie suboru po riadkoch
	{
		if (line!="" && line!= "\n") // overenie ci nie je riadok prazdny
			counter++; // inkrementacia counteru
	}
	fil.close(); // zatvorenie suboru
	if (fil.is_open()) // kontrola ci je subor zatvoreny
		fileCouldntBeClosed(); // chybova funkcia
	if (counter < 2) // kontrola ci ma subor 1 riadok a menej. Pre loginSystem()
		return 0; // navratova hodnota
	else if (counter <= 3) // pre changeReceiptData()
		return 1; // navratova hodnota
	else
		return 2; // navratova hodnota
}

// kontrola ci je subor prazdny. Vrati int
int checkIfFileIsEmpty(string fileName) { // parameter funkcie
	ifstream fil; // ifstream na citanie suboru
	fil.open(fileName); // otvorenie suboru
	if (!fil.is_open()) // kontrola ci je subor otvoreny
		fileCouldntBeOpened(); // chybova funkcia
	fil.seekg(0, ios::end); // umiestnenie "kurzora"  na koniec suboru
	int len = fil.tellg(); // pozicia konca suboru
	fil.close(); // zatvorenie suboru
	if (fil.is_open()) // kontrola ci je subor zatvoreny
		fileCouldntBeClosed(); // chybova funkcia
	return len; // navratova hodnota
}

int main() {
	selectLanguage(); // vyber jazyka
	string tabs = "\t\t";
	if (checkIfFileIsEmpty("AdminLogin.txt") != 0 && checkFileLines("AdminLogin.txt") != 0) // kontrola ci je subor prazdny alebo ma malo riadkov
		loginSystem();
	else
		changeLoginData(true); // zmena prihlasovacich udajov
	// uvitanie pouzivatela
	if (language == "sk")
	{
		cout << tabs << tabs << "~~~~~~~Vitajte v nasom market systeme~~~~~~~" << endl;
		cout << tabs << tabs << ":::::::Vyberte prosim, co chete robit:::::::" << endl << endl << tabs << tabs << "--------------------------------------------" << endl << endl;
	}
	else
	{
		cout << tabs << tabs << "~~~~~~~Welcome to our market system~~~~~~~" << endl;
		cout << tabs << tabs << ":::::::Select what you need to do:::::::::" << endl << endl << tabs << tabs << "--------------------------------------------" << endl << endl;
	}
	
	int intMenuInput; // premenna na uskladnenie inputu vo forme intu
	string strmenuInput; // premenna na uskladnenie inputu vo forme stringu
	while (true)
	{
		mainMenu(); // zavolanie hlavneho menu
		if (language == "sk")
		{
			cout << tabs << "\tVas vyber -> ";
		}
		else
		{
			cout << tabs << "\tYour selection -> ";
		}
		while (true) // cyklus na kontrolu exception
		{
			cin >> strmenuInput; // input od uzivatela
			cin.ignore(); // Bez tohto by to nefungovalo
			try // exception
			{
				intMenuInput = stoi(strmenuInput); // konverzia stringu na int  
				if (intMenuInput > 0 && intMenuInput <= 9) // kontrola, ci je cislo v intervale od 1 po 9
					break; // opustenie cyklu
				else
				{
					// v pripade nesplnenej podmienky
					// chybova hlaska
					if (language == "sk")
					{
						cout << tabs << "Nespravny input" << endl; // info pre uzivatela
						cout << tabs << "Zadaj svoj vyber znova -> ";
					}
					else
					{
						cout << tabs << "Invalid input" << endl; // info pre uzivatela
						cout << tabs << "Enter your choice again -> ";
					}
					continue; // navrat na zaciatok cyklu
				}
			}
			catch (...) // zachytenie errorov napriklad pri konverzii
			{
				// chybova hlaska
				system("CLS");
				if (language == "sk")
				{
					cout << tabs << "Nespravny input" << endl; // info pre uzivatela
					cout << tabs << "Zadaj svoj vyber znova -> ";
				}
				else
				{
					cout << tabs << "Invalid input" << endl; // info pre uzivatela
					cout << tabs << "Enter your choice again -> ";
				}
				continue; // navrat na zaciatok cyklu
			}
		}
		if (intMenuInput == 1) // kontrola co uzivatel zadal
		{
			string fileName;
			if (language == "sk")
			{
				fileName = "Udaje.txt";
			}
			else
			{
				fileName = "Data.txt";
			}
			int isEmpty = checkIfFileIsEmpty(fileName); // kontrola suboru ci je prazdny
			int lineCount = checkFileLines(fileName); // kontrola poctu riadkov v subore
			if (isEmpty == 0 || (lineCount == 0 || lineCount == 1)) // kontrola ci nie je subor prazdny alebo ma malo riadkov
				changeReceiptData(true); // zmena udajov pre nakupny doklad
			checkout(); // blokovanie produktov
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 2)
		{
			addProduct(); // pridavanie produktov
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 3)
		{
			string fileName; // nazov suboru
			if (language == "sk")
			{
				fileName = "Produkty.txt";
			}
			else
			{
				fileName = "Products.txt";
			}
			if (checkIfFileIsEmpty(fileName) == 0)
			{
				addProduct();
				continue;
			}
			removeProduct(); // zavloanie funkcie na vymazanie produktu
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 4)
		{
			string fileName; // nazov suboru
			if (language == "sk")
			{
				fileName = "Produkty.txt";
			}
			else
			{
				fileName = "Products.txt";
			}
			if (checkIfFileIsEmpty(fileName) == 0) // kontrola ci je subor prazdny
			{
				addProduct(); // pridanie produktu
				continue; // navrat na zaciatok cyklu
			}
			editProduct(); // upravenie produktu
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 5)
		{
			loginSystem(); // autentifikacia
			string fileName; // nazov suboru
			if (language == "sk")
			{
				fileName = "Udaje.txt";
			}
			else
			{
				fileName = "Data.txt";
			}
			int isEmpty = checkIfFileIsEmpty(fileName); // kontrola ci je subor prazdny
			int lineCount = checkFileLines(fileName); // kontrola poctu riadkov subora
			if (isEmpty != 0 && lineCount != 0 && lineCount != 1) // kontrola ci je subor prazdny alebo ma malo riadkov
				changeReceiptData(false); // zmena udajov pre nakupne doklady s parametrom false
			else
				changeReceiptData(true); // zmena udajov pre nakupne doklady s parametrom true
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 6) 
		{
			loginSystem(); // autentifikacia
			changeLoginData(false); // zavolanie funkcie na zmenu prihlasovacich udajov
			continue; // navrat na zaciatok cyklu
		}
		else if(intMenuInput == 7)
		{
			viewStatus(); // zavolanie funkcie na zobrazenie informacii o stave financii
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 8) 
		{
			selectLanguage(); // zavolanie funkcie na zmenu jazyka
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 9)
		{
			exitFunc(); // zavolanie funkcie na vypnutie programu
			break; // opustenie cyklu
		}
	}
	return 0;
}