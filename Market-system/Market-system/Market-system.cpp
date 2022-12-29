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

void printFileContent(string fileName) {
	ifstream fil;
	fil.open(fileName);
	if (!fil.is_open())
	{
		fileCouldntBeOpened();
	}
	if (fileName == "Produkty.txt")
	{
		string line;
		while (getline(fil, line))
		{
			cout << "\t\t\t" << line; printEuroSign();
		}
	}
	else
	{
		string line;
		while (getline(fil,line))
		{
			cout << "\t\t\t" << line;
		}
	}
	fil.close();
	if (fil.is_open())
	{
		fileCouldntBeClosed();
	}
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

// vypocet delitela na ziskanie ceny bez DPH
float countTaxDivisor() {
	string DPH = getReceiptData("DPH");
	string dphNum = DPH;
	float dphDivisor;
	dphNum.pop_back();
	try
	{
		dphDivisor = stof(dphNum);
	}
	catch (...)
	{
		system("CLS");
		cout << "Neocakavana chyba pri konverzii" << endl;
		exit(0);
	}
	dphDivisor = dphDivisor / 100;
	dphDivisor += 1;
	return dphDivisor;
}

// zobrazenie stavu financii
void viewStatus() {
	system("CLS"); // vycistenie obrazovky
	cout << "Celkova trzba: " << money; printEuroSign(); // celkova trzba
	// funkcia ceil - zaokruhlenie
	cout << "Celkova trzba bez DPH: " << ceil((money / countTaxDivisor()) * 100) / 100; printEuroSign(); // vypocet ceny bez DPH
	cout << "Sadzba DPH: " << getReceiptData("DPH") << endl; // dph sadzba
	cout << "Celkova DPH: " << ceil((money - (money / countTaxDivisor())) * 100) / 100; printEuroSign(); // vypocet celkovej DPH
	cout << "Pre opustenie stlacte ENTER -> "; // info pre uzivatela
	cin.get(); // narozdiel od klasickeho cin zoberie aj ENTER
	system("CLS"); // vycistenie obrazovky
}

// vypisanie hlavneho menu
void mainMenu() {
	string tabs = "\t\t";
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
	cout << tabs << "\t[8] Exit" << endl << endl;
	cout << tabs << "\tPre pokracovanie zvolte jedno z cisel v hranatych zatvorkach" << endl;
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
	string tabs = "\t\t";
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
		cout << tabs << "Zoznam produktov: " << endl << endl;
		// syntactic sugar. Funguje podobne ako klasicky for cyklus
		for (string i: listOfProducts) // prechadzanie vectorom
		{
			cout << tabs << "\t" << i; // vypisovanie produktov z vectoru
			printEuroSign(); // output eura
		}
		cout << endl;
		string userProductName; // uskladnenie inputu
		cout << tabs << "Zadajte nazov produktu, ktory chcete nablokovat. Ak chcete skoncit s blokovanim napiste STOP" << endl; // info pre uzivatela
		cout << tabs << "-> ";
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
					cout << "Neocakavana chyba pri cene produktu" << endl; // chybova hlaska
					exit(0); // vypnutie programu
				}
			}
		}
		else // ak sa produkt v zozname nenachadza spusti sa tato cast
		{
			cout << tabs << "Produkt, ktory ste zadali sa v zozname nenachadza" << endl; // info pre uzivatela 
			string yesOrNo; // uskladnenie inputu
			cout << tabs << "Chcete zadat produkt este raz? ano/nie" << endl; // info pre uzivatela
			cout << tabs << "-> ";
			cin >> yesOrNo; // input
			transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper); // transform stringu na velke pismena
			if (yesOrNo == "ANO") // overenie inputu
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
		// vypisanie nakupneho dokladu
		string tab = "\t\t\t\t"; // rovnaky pocet tabulatorov pre vsetko
		cout << tab << "----------------------------------------------" << endl; // GUI prvok
		cout << tab << getReceiptData("Nazov") << endl << endl; // vypytanie si Nazvu zo suboru a vypisanie 
		cout << tab << getReceiptData("Adresa") << endl; // vypytanie si Adresy zo suboru a vypisanie
		cout << tab << "ICO: " << getReceiptData("ICO") << endl; // vypytanie si ICO zo suboru a vypisanie
		cout << tab << getActualTime(); // vypisanie aktualneho casu
		cout << tab << "----------------------------------------------" << endl; // GUI prvok
		for (string i: products) // prechadzanie vectorom
		{
			cout << tab << i; // vypisovanie jednotlivych produktov a ich cien
			printEuroSign(); // output eura
		}
		cout << endl;
		cout << tab << "SUMA" << "\t\t\t" << price; // vypisanie vyslednej sumy
		printEuroSign(); // output eura
		cout << endl << tab << "Suma bez DPH" << "\t\t" << ceil((price / countTaxDivisor()) * 100) / 100; // vypocet ceny bez DPH
		printEuroSign(); // output eura
		cout << tab << "Sadzba DPH:\t\t" << getReceiptData("DPH") << endl; // vypisanie sadzby DPH 
		cout << tab << "DPH:\t\t\t" << ceil((price - (price / countTaxDivisor())) * 100) / 100; // vypocet DPH
		printEuroSign(); // output eura
		cout << tab << "----------------------------------------------" << endl; // GUI prvok
		cout << tab << "\t\tDakujeme za nakup" << endl;
		cout << tab << "----------------------------------------------" << endl; // GUI prvok
		while (true) // platba
		{
			cout << endl << tabs << "Moznosti platby: " << endl << endl; // info pre uzivatela
			cout << tabs << "\t[1] Hotovost" << endl;
			cout << tabs << "\t[2] Platobna karta" << endl << endl;
			cout << tabs << "Prosim zvolte jedenu z moznosti zadanim cisla v hranatych zatvorkach" << endl;
			cout << tabs << "Vasa volba -> ";
			string s; // uskladnenie inputu
			cin >> s; // input
			if (s == "2") // platba kartou
			{
				cout << tabs << "Prebieha platba" << endl;
				cout << tabs << "Zadajte svoj PIN: "; // info pre uzivatela
				cin >> s; // input PINu
				if (s.length() == 4) // kontrola ci ma PIN styri cislice
				{
					money += price; // pripocitanie penazi na nas ucet
					break; // ukoncenie cyklu
				}
				else // neplatny PIN
				{
					system("CLS"); // vycistenie obrazovky
					cout << tabs << "Zadali ste neplatny PIN. Skuste to este raz" << endl; // info pre pouzivatela
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
				cout << tabs << "Zadali ste neplatnu moznost!!" << endl; // info pre uzivatela
				cout << tabs << "SUMA" << "\t\t" << price; // znovu vypisanie ceny
				printEuroSign(); // output eura
				cout << endl;
				continue; // navrat na zaciatok cyklu
			}
		}
	}
	else // ak je prazdny zoznam nablokovanych produktov spusti sa toto
	{
		system("CLS"); // vycistenie obrazovky
		cout << tabs << "Prazdny zoznam nablokovanych produktov!" << endl; // info pre uzivatela
		while (true) 
		{
			string oneOrZero; // uskladnenie inputu
			cout << tabs << "Ak sa chcete vratit na hlavne menu zadajte '1'. Ak chcete ukoncit program zadajte '0'." << endl; // info
			cout << tabs << "-> "; 
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
				cout << tabs << "Neplatný vstup. Skúste to znova" << endl; // info pre uzivatela
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
	string userProductName; // uskladnenie inputu 
	string userProductPrice; // uskladnenie inputu
	cout << tabs << "++++++++++++++++++++++++++++++" << endl;
	cout << tabs << "|| Pridanie noveho produktu ||" << endl; // GUI prvok
	cout << tabs << "++++++++++++++++++++++++++++++" << endl << endl;
	while (true)
	{
		cout << tabs << "Zoznam produktov: " << endl << endl;
		printFileContent("Produkty.txt");
		cout << endl;
		bool isValid = true;
		cout << tabs << "Zadajte nazov noveho produktu: "; // info pre uzivatela
		getline(cin, userProductName);
		cout << tabs << "Zadajte cenu noveho produktu: "; // info pre uzivatela
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
		fil.open("Produkty.txt", ios::in); // otvorenie suboru na citanie - ios::in
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
			fil.open("Produkty.txt", ios::app); // otvorenie suboru na pridavanie - ios::app - append
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
			cout << tabs << "Produkt uz je v zozname alebo ste zadali neplatnu cenu" << endl; // info pre uzivatela
			string yesOrNo; // uskladnenie inputu
			cout << tabs << "Chcete zadat produkt este raz? ano/nie" << endl; // info pre uzivatela
			cout << tabs << "-> "; 
			cin >> yesOrNo; // input
			transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper); // pretransformovanie inputu na velke pismena
			if (yesOrNo == "ANO") // kontrola inputu
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
	string userProductName; // uskladnenie inputu
	string userProductPrice; // uskladnenie ceny
	cout << tabs << "**********************" << endl; 
	cout << tabs << "* Upravenie produktu *" << endl; // GUI prvok
	cout << tabs << "**********************" << endl << endl;
	while (true) 
	{
		string line;
		cout << tabs << "Zoznam produktov: " << endl << endl; // info pre uzivatela
		printFileContent("Produkty.txt");
		cout << endl << tabs << "Zadajte nazov produktu ktory chcete upravit: "; // info pre uzivatela
		getline(cin, userProductName); // input
		fin.open("Produkty.txt"); // otvorenie suboru
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
			cout << tabs << "Zadajte novy nazov produktu: "; // info pre uzivatela
			getline(cin, editeProductName); // input
			cout << tabs << "Zadajte novu cenu produktu: "; // info pre uzivatela
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
				fin.open("Produkty.txt"); // otvorenie suboru
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
				remove("Produkty.txt"); // odstranenie suboru
				rename("temp.txt", "Produkty.txt"); // premenovanie suboru temp.txt na Produkty.txt
				if (fin.is_open() || temp.is_open()) // kontrola ci je subor zatvoreny
					fileCouldntBeClosed(); // chybova funkcia
				break; // opustenie cyklu
			}
			else // ak nie je validna cena
			{
				string yesOrNo; // uskladnenie inputu
				cout << tabs << "Cena produktu je neplatna!" << endl; // info pre pouzivatela
				cout << tabs << "Chcete znova zadat nazov produktu? ano/nie " << endl; // info pre pouzivatela
				cout << tabs << "-> "; 
				cin >> yesOrNo; // input
				transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper); // transform inputu na velke pismena
				if (yesOrNo == "ANO") // overnie inputu
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
			cout << tabs << "Produkt ktory ste zadali sa v zozname nenachadza" << endl; // info pre uzivatela
			cout << tabs << "Chcete znova zadat nazov produktu? ano/nie " << endl; // info pre uzivatella
			cout << tabs << "-> "; 
			cin >> yesOrNo; // innput
			transform(yesOrNo.begin(), yesOrNo.end(), yesOrNo.begin(), ::toupper); // transform inputu na velke pismena
			if (yesOrNo == "ANO") // overenie inputu
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
	ofstream temp; // ofstream na zapisovanie do suboru
	fin.open("Produkty.txt"); // otvorenie suboru ifstreamom
	cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	cout << tabs << "!!!!!! Vymazanie produktu !!!!!!" << endl; // info pre uzivatela
	cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	cout << tabs << "Zoznam produktov: " << endl << endl; // vypisanie produktov
	if(fin.is_open()) // kontrola, ci je subor otvoreny
	{
		string line; // premenna pre funkciu getline
		printFileContent("Produkty.txt");
		fin.close(); // zatvorenie suboru
		if (fin.is_open()) // kontorola ci je subor zatvoreny
			fileCouldntBeClosed();
		string userInput; // vytvorenie premennej typu string na uskladnenie inputu od pouzivatela
		cout << endl << tabs << "Zadajte nazov produktu: "; // info pre pouzivatela
		getline(cin, userInput); // ulozenie inputu do premennej
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
	string tabs = "\t\t";
	system("CLS"); // vycistenie obrazovky
	ofstream temp; // ofstream na zapisovanie do suboru
	ifstream fin; // ifstream na citanie suboru
	if (!isEmpty) // ak subor nie je prazdny
	{
		cout << tabs << "****************************" << endl;
		cout << tabs << "* Zmena udajov pre blociky *" << endl; // GUI prvok
		cout << tabs << "****************************" << endl << endl;
	}
	else // ak je subor prazdny
	{
		cout << tabs << "****************************************" << endl; 
		cout << tabs << "* Nastavenie novych udajov pre blociky *" << endl; // GUI prvok
		cout << tabs << "****************************************" << endl << endl;
	}
	fin.open("Udaje.txt"); // otvorenie suboru
	if (!fin.is_open()) // kontrola ci je subor otvoreny
		fileCouldntBeOpened(); // chybova funkcia
	cout << tabs << "Aktulane udaje: " << endl << endl; // info pre uzivatela
	printFileContent("Udaje.txt");
	cout << tabs << endl;
	fin.close(); // zatvorenie suboru
	if (fin.is_open()) // kontrola ci je subor zatvoreny
		fileCouldntBeClosed(); // chybova funkcia
	string userName; // uskladnenie inputu
	string userAdress; // uskladnenie inputu
	string userICO; // uskladnenie inputu
	string userDPH; // uskladnenie inputu
	cout << tabs << "Zadajte novy nazov obchodu: "; // info pre uzivatela
	cin.ignore(); // na ignorovanie noveho riadku. Program by bez toho nefungoval
	getline(cin, userName); // input celeho riadku nie len jedneho slova
	cout << tabs << "Zadajte novu adresu: "; // info pre uzivatela
	getline(cin, userAdress); // input celeho riadku nie len jedneho slova
	cout << tabs << "Zadajte nove ICO: "; // info pre uzivatela
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
			cout << tabs << "Zadali ste nespravne ICO, skuste to znova: "; // info pre uzivatela
			continue; // navrat na zaciatok cyklu
		}
	}
	while (true)
	{
		cout << tabs << "Zadajte novu hodnotu DPH bez znaku %: "; // info pre uzivatela
		try // exception
		{
			cin >> userDPH; // input
			float userDPHfloat = stof(userDPH); // konverzia stringu na float. Tymto viem zabranit tomu aby nahodou uzivatel nezadal nieco co nechcem
			break; // opustenie cyklu
		}
		catch (...) // ak je neuspesna konverzia
		{
			cout << tabs << "Zadali ste nespravnu DPH, skuste to znova" << endl; // info pre uzivatela
			continue; // navrat na zaciatok cyklu
		}
	}
	temp.open("temp.txt"); // otvorenie suboru
	if (!temp.is_open()) // kontrola ci je subor otvoreny
		fileCouldntBeOpened(); // chybova funkcia
	temp << "Nazov:\t" << userName << endl; // zapisanie do suboru
	temp << "Adresa:\t" << userAdress << endl;  // zapisanie do suboru
	temp << "ICO:\t" << userICO << endl; // zapisanie do suboru
	temp << "DPH:\t" << userDPH + "%";
	temp.close(); // zatvorenie suboru
	if (temp.is_open()) // kontrola ci je subor zatvoreny
		fileCouldntBeClosed(); // chybova funkcia
	remove("Udaje.txt"); // odstranenie suboru
	rename("temp.txt", "Udaje.txt"); // premenovanie suboru
	system("CLS"); // vycistenie obrazovky
} 

// zmena prihlasovacich udajov
void changeLoginData(bool isEmpty) {
	string tabs = "\t\t";
	system("CLS"); // vycistenie obrazovky
	if (!isEmpty)  // ak subor nie je prazdny 
	{
		cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl; // maly GUI prvok
		cout << tabs << "!!!!!!! Zmena prihlasovacich udajov !!!!!!!" << endl; // upozornenie pre uzivatela
		cout << tabs << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl << endl;
	}
	else // ak je subor prazdny
	{
		cout << tabs << " =============================================" << endl;
		cout << tabs << "||  Nastavenie novych prihlasovacich udajov  ||" << endl; // GUI prvok
		cout << tabs << " =============================================" << endl << endl;
	}
	ofstream temp; // ofstream na zapisovanie do suboru
	string loginData; // uskladnenie inputu
	string passwordData; // uskladnenie inputu
	cout << tabs << "Zadajte novy login: "; // vypytanie si loginu od uzivatela
	cin >> loginData;  // ulozenie inputu do jednej z premennych
	cout << tabs << "Zadajte nove heslo: "; // vypytanie si hesla od uzivatela
	cin >> passwordData; // uskladnenie inputu do jednej z premennych
	temp.open("temp.txt"); // otvorenie suboru ofstreamom
	if (temp.is_open()) // kontrola ci je subor otvoreny
	{
		temp << loginData << endl; // zapisanie loginu do suboru temp.txt
		temp << passwordData; // zapisanie hesla do suboru temp.txt
		temp.close(); // zatvorenie suboru
		if (temp.is_open()) // kontrola ci je subor zatvoreny
			fileCouldntBeClosed(); // chybova funkcia
		else
		{
			remove("AdminLogin.txt"); // odstranenie povodneho suboru AdminLogin.txt
			rename("temp.txt", "AdminLogin.txt"); // premenovanie docasneho suboru temp.txt na AdminLogin.txt
		}
	}
	else
		fileCouldntBeOpened(); // chybova funkcia
	system("CLS"); // vycistenie obrazovky
}

void loginSystem() {
	string tabs = "\t\t";
	system("CLS");
	cout << tabs << "**********************************************" << endl;
	cout << tabs << "* Najskor sa prihlaste, ak chcete pokracovat *" << endl; // GUI prvok
	cout << tabs << "**********************************************" << endl << endl;
	int counter = 0; // pomocou counteru program vie ci ma overit login alebo heslo
	ifstream fin; // ifstream - subor je len na citanie
	string loginstr; // uskladnenie inputu
	string passwordstr; // uskladnenie inputu
	// Login form
	cout << tabs << "Login: "; // info pre uzivatela
	cin >> loginstr; // input
	cout << tabs << "Heslo: "; // info pre uzivatela
	cin >> passwordstr; // input
	fin.open("AdminLogin.txt"); // otvorenie suboru
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
					system("CLS"); // vycistenie obrazovky
					cout << tabs << "Zistili sme neopravneny pristup" << endl; // info pre uzivatela
					fin.close(); // zatvorenie suboru
					exit(0); // vypnutie programu
				}
			}
			else if(counter == 1) 
			{
				if (arra == passwordstr) // program skontroluje heslo
					break; // counter sa uz neinkrementuje ale pomocou break sa dostaneme von z cyklu
				else
				{
					// tato cast sa spusti ak je nespravne heslo
					system("CLS"); // vycistenie obrazovky
					cout << tabs << "Zistili sme neopravneny pristup" << endl; // info pre uzivatela
					fin.close(); // zatvorenie suboru
					exit(0); // vypnutie programu
				}
			}
		}
		// zatvorenie suboru
		fin.close(); // zatvorenie suboru
		if (fin.is_open()) // overenie ci je subor skutocne zatvoreny
			fileCouldntBeClosed(); // chybova funkcia
	}
	else
		fileCouldntBeOpened(); // chybova funkcia
	// vycistenie konzoly v pripade spravneho loginu a hesla
	system("CLS"); // vycistenie obrazovky
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
	else if (counter == 2) // pre changeReceiptData()
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
	int isFileEmpty = checkIfFileIsEmpty("AdminLogin.txt"); // kontrola ci je subor prazdny
	int filLineCount = checkFileLines("AdminLogin.txt"); // kontrola poctu riadkov suboru
	string tabs = "\t\t";
	if (isFileEmpty != 0 && filLineCount != 0) // kontrola ci je subor prazdny alebo ma malo riadkov
		loginSystem(); // login
	else
		changeLoginData(true); // zmena prihlasovacich udajov
	// uvitanie pouzivatela
	cout << tabs << tabs << "~~~~~~~Vitajte v nasom market systeme~~~~~~~" << endl;
	cout << tabs << tabs << ":::::::Vyberte prosim, co chete robit:::::::" << endl << endl << tabs << tabs << "--------------------------------------------" << endl << endl;
	int intMenuInput; // premenna na uskladnenie inputu vo forme intu
	string strmenuInput; // premenna na uskladnenie inputu vo forme stringu
	while (true)
	{
		mainMenu(); // zavolanie hlavneho menu
		cout << tabs << "\tVas vyber -> ";
		while (true) // cyklus na kontrolu exception
		{
			cin >> strmenuInput; // input od uzivatela
			cin.ignore(); // Bez tohto by to nefungovalo
			try // exception
			{
				intMenuInput = stoi(strmenuInput); // konverzia stringu na int  
				if (intMenuInput >= 0 && intMenuInput <= 8) // kontrola, ci je cislo v intervale od 1 po 7
					break; // opustenie cyklu
				else
				{
					// v pripade nesplnenej podmienky
					// chybova hlaska
					cout << tabs << "Nespravny input" << endl; // info pre uzivatela
					cout << tabs << "Zadaj svoj vyber znova -> ";
					continue; // navrat na zaciatok cyklu
				}
			}
			catch (...) // zachytenie errorov napriklad pri konverzii
			{
				// chybova hlaska
				system("CLS");
				cout << tabs << "Nespravny input" << endl; // info pre uzivatela
				cout << tabs << "Zadaj svoj vyber znova -> ";
				continue; // navrat na zaciatok cyklu
			}
		}
		if (intMenuInput == 1) // kontrola co uzivatel zadal
		{
			int isEmpty = checkIfFileIsEmpty("Udaje.txt"); // kontrola suboru ci je prazdny
			int lineCount = checkFileLines("Udaje.txt"); // kontrola poctu riadkov v subore
			if (isEmpty == 0 && (lineCount == 0 || lineCount == 1)) // kontrola ci nie je subor prazdny alebo ma malo riadkov
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
			if (checkIfFileIsEmpty("Produkty.txt") == 0)
			{
				addProduct();
				continue;
			}
			removeProduct(); // zavloanie funkcie na vymazanie produktu
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 4)
		{
			if (checkIfFileIsEmpty("Produkty.txt") == 0)
			{
				addProduct();
				continue;
			}
			editProduct(); // upravenie produktu
			continue; // navrat na zaciatok cyklu
		}
		else if (intMenuInput == 5)
		{
			loginSystem(); // autentifikacia
			int isEmpty = checkIfFileIsEmpty("Udaje.txt"); // kontrola ci je subor prazdny
			int lineCount = checkFileLines("Udaje.txt"); // kontrola poctu riadkov subora
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
			exitFunc(); // zavolanie funkcie na vypnutie programu
			break; // opustenie cyklu
		}
	}
	return 0;
}

