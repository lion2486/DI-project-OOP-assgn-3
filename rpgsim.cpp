#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "items.h"
#include "spells.h"
#include "living.h"
#include "world.h"
using namespace std;

/*
 * Read all the weapons, armor and potions from the corresponding files
 * and return a pointer of the vector they are stored in
 */
vector<Item*>* load::readItems() {
	ifstream inWeapons("weapons.txt"), inArmor("armor.txt"), inPotions("potions.txt");
	vector<Item*>* items = new vector<Item*>;
	string line, name, one;
	unsigned int price, weight;
	unsigned short minLevel;

	// Read the weapons
	while (!inWeapons.eof()) {
		string both;
		int damage;
		bool bothH;

		getline(inWeapons, line);
		if (line.empty()) continue;
		line += " 1";
		istringstream is(line);
		is >> name >> price >> minLevel >> weight >> damage >> both >> one;
		if (both == "true")
			bothH = true;
		else if (both == "false")
			bothH = false;
		else {
			cerr << "Wrong input!" << endl;
			return NULL;
		}
		if (one != "1" || !is.eof()) {
			cerr << "Wrong input!" << endl;
			return NULL;
		}
		items->push_back(new Weapon(name, price, minLevel, weight, damage, bothH));
	}
	// Read the armor
	while (!inArmor.eof()) {
		int armorClass;
		getline(inArmor, line);
		if (line.empty()) continue;
		line += " 1";
		istringstream is(line);
		is >> name >> price >> minLevel >> weight >> armorClass >> one;
		if (one != "1" || !is.eof()) {
			cerr << "Wrong input!" << endl;
			return NULL;
		}
		items->push_back(new Armor(name, price, minLevel, weight, armorClass));
	}
	// Read the potions
	while (!inPotions.eof()) {
		int stat, amount;
		getline(inPotions, line);
		if (line.empty()) continue;
		line += " 1";
		istringstream is(line);
		is >> name >> price >> minLevel >> weight >> stat >> amount >> one;
		if (one != "1" || !is.eof()) {
			cerr << "Wrong input!" << endl;
			return NULL;
		}
		if (stat == 0)
			items->push_back(new HealthPotion(name, price, minLevel, weight, amount));
		else if (stat == 1)
			items->push_back(new ManaPotion(name, price, minLevel, weight, amount));
		else if (stat == 2)
			items->push_back(new StrengthPotion(name, price, minLevel, weight, amount));
		else if (stat == 3)
			items->push_back(new DexterityPotion(name, price, minLevel, weight, amount));
		else if (stat == 4)
			items->push_back(new AgilityPotion(name, price, minLevel, weight, amount));
	}
	return items;
}

/*
 * Read all the spells from the corresponding files and
 * return a pointer of the vector they are stored in
 */
vector<Spell*>* load::readSpells() {
	ifstream inIceSp("ice_sp.txt"), inFireSp("fire_sp.txt"), inLightingSp("lighting_sp.txt");
	vector<Spell*>* spells = new vector<Spell*>;
	string line, name, one;
	unsigned int price, minDamage, maxDamage, mana, rounds;
	unsigned short minLevel;
	int modifier;

	// Read the Ice Spells
	while (!inIceSp.eof()) {
		getline(inIceSp, line);
		if (line.empty()) continue;
		line += " 1";
		istringstream is(line);
		is >> name >> price >> minLevel >> minDamage >> maxDamage >> mana
			>> rounds >> modifier >> one;
		if (one != "1" || !is.eof()) {
			cerr << "Wrong input!" << endl;
			return NULL;
		}
		spells->push_back(new IceSpell(name, price, minLevel, minDamage, maxDamage, mana, rounds, modifier));
	}
	// Read the Fire Spells
	while (!inFireSp.eof()) {
		getline(inFireSp, line);
		if (line.empty()) continue;
		line += " 1";
		istringstream is(line);
		is >> name >> price >> minLevel >> minDamage >> maxDamage >> mana
			>> rounds >> modifier >> one;
		if (one != "1" || !is.eof()) {
			cerr << "Wrong input!" << endl;
			return NULL;
		}
		spells->push_back(new FireSpell(name, price, minLevel, minDamage, maxDamage, mana, rounds, modifier));
	}
	// Read the lighting Spells
	while (!inLightingSp.eof()) {
		getline(inLightingSp, line);
		if (line.empty()) continue;
		line += " 1";
		istringstream is(line);
		is >> name >> price >> minLevel >> minDamage >> maxDamage >> mana
			>> rounds >> modifier >> one;
		if (one != "1" || !is.eof()) {
			cerr << "Wrong input!" << endl;
			return NULL;
		}
		spells->push_back(new LightingSpell(name, price, minLevel, minDamage, maxDamage, mana, rounds, modifier));
	}
	return spells;
}


// Read the names of the monster and return a pointer of the vector they are stored in
vector<string>* load::readMonsters() {
	ifstream inMonsters("monsters.txt");
	vector<string>* monsters = new vector<string>;
	string line, name, one;

	while (!inMonsters.eof()) {
		getline(inMonsters, line);
		if (line.empty()) continue;
		line += " 1";
		istringstream is(line);
		is >> name >> one;
		if (one != "1" || !is.eof()) {
			cerr << "Wrong input!" << endl;
			return NULL;
		}
		monsters->push_back(name);
	}
	return monsters;
}

// Print the help menu
void showHelp(void) {
	cout << "Help: " << '\n'
		 << "w/d/s/a -> Move up/right/down/left" << '\n'
		 << "8/6/2/4 -> Move up/right/down/left" << '\n'
		 << "m -> Show map" << '\n'
		 << "i -> Check inventory" << '\n'
		 << "e -> Equip a weapon or armor" << '\n'
		 << "f -> Use a potion" << '\n'
		 << "t -> Show Market" << '\n'
		 << "g -> Show Stats" << '\n'
		 << "q -> Quit Game" << endl;
}

int main(void)
{
	srand(time(NULL));
	Grid map;
	int num;
	char ch;
	string line;

	cout << "Please enter the number of heroes you want to join you party. (1-3)" << endl;
	cin >> num;
	while (num < 1 || num > 3 || cin.fail()) {
		cout << "Wrong input! You have to enter a number from 1 to 3." << endl;
		cin.clear();
		cin >> line >> num;
	}
	for (int i = 1; i <= num; i++) {
		// Create each hero
		string name;
		int hClass;
		cout << "Enter the name of hero number " << i << ':' << endl;
		cin >> name;
		cout << "Enter the class of hero number " << i << ':' << '\n'
			 << "1 -> Warrior\n2 -> Sorcerer\n3 -> Paladin" << endl;
		cin >> hClass;
		while (hClass < 1 || hClass > 3 || cin.fail()) {
			cout << "Wrong input!" << '\n'
				 << "1 -> Warrior\n2 -> Sorcerer\n3 -> Paladin" << endl;
			cin.clear();
			cin >> line >> hClass;
		}
		map.createHero(name, hClass);
	}
	showHelp();
	cout << "To show the help menu press h." << endl;

	cin >> ch;
	while ((int) ch != EOF) {
		switch(ch){
		case 'h':	showHelp();
					break;
		case 'w':
		case 'd':
		case 's':
		case 'a':
		case '2':
		case '4':
		case '8':
		case '6':	map.move(ch);
					break;
		case 'g':
		case 'G':	map.displayStats();
					break;
		case 'm':
		case 'M':	map.displayMap();
					break;
		case 'i':
		case 'I':	map.checkInventory();
					break;
		case 'e':
		case 'E':	map.equip();
					break;
		case 'f':
		case 'F':	map.use();
					break;
		case 't':
		case 'T':	map.trade();
					break;
		case 'q':
		case 'Q':	cout << "Are you sure you want to exit the game? (Y/n)" << endl;
					cin >> ch;
					if (ch == 'Y' && map.quitGame()) {
						cout << "Exited game." << endl;
						return EXIT_SUCCESS;
					}
					cout << "Quit Game aborted!" << endl;
					break;
		default: cout << "Wrong option!" << endl;
		}
		cin >> ch;
	}
	return EXIT_SUCCESS;
}
