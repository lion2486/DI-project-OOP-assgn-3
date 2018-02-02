#include <iostream>
#include <cstdlib>
#include <list>
#include <vector>
#include "items.h"
#include "spells.h"
#include "living.h"
#include "world.h"

using namespace std;

void Party::createHero(string name, int hClass)
{
	if (hClass == 1)
		party.push_back(new Warrior(name, 1, 200, 200, 100, 100, 10, 5, 8, 2500, 1000, 0));
	else if (hClass == 2)
		party.push_back(new Sorcerer(name, 1, 100, 100, 200, 200, 5, 10, 8, 1500, 1000, 0));
	else
		party.push_back(new Paladin(name, 1, 150, 150, 150, 150, 8, 8, 5, 2000, 1000, 0));
}

bool Party::isAlive(){
	for(int i = 0; i < size() ; i++)
		if(get(i+1)->getCurHealthP() > 0)
			return true;
	return false;
}

/* It returns a pointer to the <pos> hero if exists, otherwise the first one. */
Hero * Party::get(int pos)
{
	list<Hero*>::iterator ptr;
	ptr = party.begin();
	if(pos > size())
		return *ptr;
	while(-- pos){
		ptr ++;
	}
	return *ptr;
}

unsigned short Party::min_lvl(){
	unsigned short lvl = get(1)-> getLevel();
	if(party.size() > 1)
		if((get(2) -> getLevel()) < lvl)
			lvl = get(2) -> getLevel();
	if(party.size() == 3)
		if((get(3) -> getLevel()) < lvl)
			lvl = get(3) -> getLevel();
	return lvl;
}

void Square::print_square()
{
	if(this -> hasMarket())
		cout << 'M';
	else if(this -> isCommon())
		cout << 'C';
	else if(this -> isNonAccessible())
		cout << 'X';
	cout << ' ';
}

Market::Market(vector<Item *> * it, vector<Spell *> * sp)
:item_list(it), spell_list(sp)
{ }

void Market::Shop()
{
	vector<Item *>::iterator it;
	unsigned int k, count = 1;
	for (k = 0; k < (*item_list).size(); k++) {
		cout << count++ << ". ";
		(*item_list)[k]->showInfo();
	}
	for (k = 0; k < (*spell_list).size(); k++) {
		cout << count++ << ". ";
		(*spell_list)[k]->showInfo();
	}
	cout << endl;
	return;
}

bool Common::access(Party party)
{
	int lvl = party.min_lvl();
	for (int i = 1; i <= party.size(); i++){
		if (rand() % 100 >= RATE) {
			unsigned int health = 25 + rand() % (75 * lvl);
			unsigned int minDamage;
			unsigned int type = rand() % 3;
			if (type == 0) {
				minDamage = 10 * lvl + rand() % (15 * lvl);
				monster.push_back(new Dragon(rand_name(), lvl, health - rand() % (health/4), health,
					minDamage, minDamage + rand() % (20*lvl), rand() % (5*lvl), rand() % (7*lvl)));
			}
			else if (type == 1) {
				minDamage = 5 * lvl + rand() % (10 * lvl);
				monster.push_back(new Exoskeleton(rand_name(), lvl, health - rand() % (health/4), health,
					minDamage, minDamage + rand() % (10*lvl), rand() % (10*lvl), rand() % (7*lvl)));
			}
			else {
				minDamage = 2 * lvl + rand() % (5 * lvl);
				monster.push_back(new Spirit(rand_name(), lvl, health - rand() % (health/4), health,
					minDamage, minDamage + rand() % (5*lvl), rand() % (5*lvl), rand() % (15*lvl)));
			}
		}
	}
	if(!monster.empty())
		return true;
	return false;
}

bool Common::monsterIsAlive()
{
	for(unsigned int i = 0; i < monster.size(); i++){
		if(monster[i]->getCurHealthP() > 0)
			return true;
	}
	return false;
}

Monster * Common::next()
{
	for(unsigned int i = 0; i < monster.size(); i++)
		if(monster[i]->getCurHealthP() > 0)
			return monster[i];
	return NULL;
}

Grid::Grid()
{
	vector<string> * mon;
	vector<Item *> * it;
	vector<Spell *> * sp;
	dimensionX = 10;
	dimensionY = 20;
	mon = load::readMonsters();
	it = load::readItems();
	sp = load::readSpells();
	position.x = 0;
	position.y = 0;
	map[0][0] = new Market(it, sp);
	for(unsigned int tmp2 = 0; tmp2 < dimensionX; tmp2 ++)
		for(unsigned int tmp = 0; tmp < dimensionY; tmp ++)
			if(tmp2 == 0 && tmp == 0)
				continue;
			else
				map[tmp2][tmp] = randSquare(mon, it, sp);
}
Grid::~Grid()
{
	for(unsigned int tmp2=0; tmp2 < dimensionX; tmp2 ++)
		for(unsigned int tmp =0; tmp < dimensionY; tmp ++)
			delete map[tmp2][tmp];
}

// Create a hero
void Grid::createHero(string name, int hClass)
{
	party.createHero(name, hClass);
}

/* Returns a pointer to a random-created block. */
Square * Grid::randSquare(vector<string> * mon, vector<Item *> * it, vector<Spell *> * sp)
{
		switch(rand()%10){
			case 0:
			case 1:		return new NonAccesible;
			case 2:
			case 3:		return new Market(it, sp);
			default:	return new Common(mon);
		}
	}

/*
 * Directions: 	8, w -> up
 * 				6, d -> right
 * 				2, s -> down
 * 				4, a -> left
 */
void Grid::move(char direction)
{
	bool flag = false;
	switch(direction){
		case '6':
		case 'd':
			if(position.y + 1 < dimensionY )
						if(!map[position.x][position.y + 1]->isNonAccessible())
						{
							++ position.y;
							flag = true;
						}
					break;
		case '2':
		case 's':	if(position.x + 1 < dimensionX )
						if(!map[position.x + 1][position.y]->isNonAccessible())
						{
							++ position.x;
							flag = true;
						}
					break;
		case '4':
		case 'a':	if(position.y >= 1 )
						if(!map[position.x][position.y - 1]->isNonAccessible())
						{
							-- position.y;
							flag = true;
						}
					break;
		case '8':
		case 'w':	if(position.x >= 1 )
						if(!map[position.x - 1][position.y]->isNonAccessible())
						{
							-- position.x;
							flag = true;
						}
					break;
		default: 	flag = false;
	}
	if(flag)
		cout << "You just moved!" << endl;
	else
		cout << "You cannot move there!" << endl;
	if(map[position.x][position.y]->isCommon())
		if(map[position.x][position.y] -> access(party))
			battle();
	return;
}

/*
 * Starts a trade in the game, it checkes if the heroes are on a market area,
 * and then it handles the buy and sell functions for a hero chosen.
 */
void Grid::trade()
{
	char ch;
	int tmp;
	Hero * hero;
	string line;
	if(!map[position.x][position.y] -> hasMarket()){
		cout << "You are not in a Market Place!" << endl;
		return;
	}
	cout << "What do you want to do? (b: buy / s: sell)" << endl;
	cin >> ch;
	hero = selectHero();
	if (ch == 'b' || ch == 'B'){
		Market * m = dynamic_cast <Market *>(map[position.x][position.y]);
		m -> Shop();
		cout << "Enter the number of the item or spell you want to buy" << endl;
		cin >> tmp;
		while (cin.fail()) {
			cout << "Wrong input!" << endl;
			cin.clear();
			cin >> line >> tmp;
		}
		// Buy the item selected
		if(tmp > 0 && tmp <= (int)m -> getItemList() -> size())
			hero -> buy((*m -> getItemList())[tmp - 1]);
		// Buy the spell selected
		else if(tmp > (int)m -> getItemList() -> size() && tmp <= (int)(m -> getItemList() -> size() + m -> getSpellList() -> size()))
			hero -> buy((*m -> getSpellList())[tmp - (int) m->getItemList()->size() - 1]);
	}
	else if (ch == 's' || ch == 'S'){
		int id;
		id = hero -> checkInventory();
		hero -> showSpellbook(id);
		cout << "Enter the number of the item you want to sell." << endl;
		cin >> tmp;
		while (cin.fail()) {
			cout << "Wrong input!" << endl;
			cin.clear();
			cin >> line >> tmp;
		}
		hero -> sell(tmp);
	}
	else
		cout << "Wrong option!" << endl;
	return;
}

// Conduct the battle!
void Grid::battle()
{
	char choice;
	string line;
	bool end = false;
	Common * com = dynamic_cast<Common *> (map[position.x][position.y]);

	cout << "\nYou have been engaged in a battle! Fight!\n" << endl;
	do {
		for (int h = 1; h <= party.size(); h++){
			int tmp;
			Hero * cur = party.get(h);
			bool repeat;
			if(!com -> monsterIsAlive()){
				end = true;
				break;
			}
			if(party.get(h)->getCurHealthP() <= 0)
				continue;
			do {
				repeat = false;
				cout << "What do you want to do with hero number " << h << "?" << endl;
				cout << "Press h for help" << endl;
				cin >> choice;
				// player action
				switch(choice){
						case '1': //attack
								cur->attack(* com -> next());
								break;
						case '2': //castSpell
								if (cur -> showSpellbook() == true) {
									cout << "Give the number of the spell you want to cast" << endl;
									cin >> tmp;
									if(cur->castSpell(tmp, *com -> next()) == -1)
										repeat = true;
								}
								else
									repeat = true;
								break;
						case '3': //use
								if (cur->checkInventory(2) != -1) {
									cout << "Give the number of the potion you want to use" << endl;
									cin >> tmp;
									if(!cur->use(tmp))
										repeat = true;
								}
								else
									repeat = true;
								break;
						case '4': //(UN)/equip
								if (cur->checkInventory(1) != -1) {
									cout << "Give the number of the item you want to equip" << endl;
									cin >> tmp;
									if(!cur->equip(tmp))
										repeat = true;
								}
								else
									repeat = true;
								break;
						case 'v': // display stats
								displayStats();
								repeat = true;
								break;
						case 'h': // help
								battleHelp();
								repeat = true;
								break;
						default: // error
								cout << "Enter a valid option." << endl;
								cin >> line;
								repeat = true;
								break;
				}
			} while (repeat);
		}
		//monsters' action
		for (unsigned int i = 0; i < com->getMonsters()->size(); i++){
			int j = 1;
			Hero * tmp = party.get(j);
			com -> getMonsters() -> at(i) -> checkDebuff();
			while(tmp->getCurHealthP() == 0 && j <= party.size())
				 tmp = party.get(++j);
			if(tmp->getCurHealthP() > 0 && com -> getMonsters() -> at(i) -> getCurHealthP() > 0)
				com -> getMonsters()->at(i)->attack(*tmp);
		}
		//recover hp/mp (hero - monster)
		for (int h = 1; h <= party.size(); h++)
		{
			Hero * hero = party.get(h+1);
			if(hero->getCurHealthP() > 0){
				hero->setCurHealth(hero->getHealthP() * 5 / 100);
				hero->setCurMagicPower(hero->getCurMagicPower() * 4 / 100);
			}

		}

		for(unsigned int i = 0; i < com->getMonsters()->size(); i++){
			if(com -> getMonsters()->at(i)->getCurHealthP() > 0)
				com -> getMonsters() -> at(i)->setCurHealth(com -> getMonsters() -> at(i)->getHealthP() * 3 / 100);
		}
		//end of round
	} while(party.isAlive() && com -> monsterIsAlive() && !end);

	cout << "End of Battle!" << endl;

	if(party.isAlive() && !(com -> monsterIsAlive()))
		cout << "Players won the battle!" << endl;

	for(int h = 0; h < party.size(); h++)
	{
		if(party.isAlive() && !(com -> monsterIsAlive())){
		party.get(h+1) -> setExperience((party.get(h+1) -> getLevel()) * (com -> getMonsters() -> size()) * 100);
		party.get(h+1) -> setMoney((party.get(h+1) -> getLevel()) * (com -> getMonsters() -> size()) * 25);
		}
		if(party.get(h+1)->getCurHealthP() <= 0){
			cout << "Player " << party.get(h+1)->getName() << " resurrected for the half of his money" << endl;
			party.get(h+1)->setMoney(party.get(h+1)->getMoney() * (-1) / 2);
			party.get(h+1)->setCurHealth(party.get(h+1)->getHealthP() / 2);
		}
	}

	for(unsigned int i = 0; i < com->getMonsters() -> size() ; i ++)
		delete com->getMonsters() -> at(i);
	com->getMonsters()->clear();

	return;
}

void Grid::displayStats()
{
	for(int h = 0; h < party.size(); h++)
		party.get(h+1)->displayStats();
	if(map[position.x][position.y] -> isCommon())
		for(unsigned int i = 0; i < dynamic_cast<Common *> (map[position.x][position.y])->getMonsters() -> size(); i++)
			dynamic_cast<Common *> (map[position.x][position.y]) -> getMonsters() -> at(i)->displayStats();
	return;
}

/* It dispays the map of the game in the terminal */
void Grid::displayMap(void)
{
	for(unsigned int tmp2 = 0; tmp2 < dimensionX; tmp2 ++){
		for(unsigned int tmp = 0; tmp < dimensionY; tmp ++){
			if(tmp2 == position.x && tmp == position.y)
				cout << 'P';
			else
				cout << ' ';
			map[tmp2][tmp] -> print_square();
		}
		cout << endl;
	}
}

/*
 * This function makes sure that you act to the hero that you want to.
 * If you have more than one hero, you will be choose between them.
 */
Hero * Grid::selectHero()
{
	int ch;
	string line;
	if(party.size() == 1)
		return party.get(1);
	cout << "Select your character:" << endl;
	for(int i = 1; i <= party.size(); i++)
		cout << i << ". " << party.get(i) -> getName() <<endl;
	cin >> ch;
	while(cin.fail() || ch < 1 || ch > party.size() ){
		cout << "Select a valid character!" << endl;
		cin.clear();
		cin >> line >> ch;
	}
	return party.get(ch);
}

void Grid::battleHelp(void)
{
	cout << "1 -> Attack" << '\n'
		 << "2 -> Cast a spell" << '\n'
		 << "3 -> Use a potion" << '\n'
		 << "4 -> Equip an item" << '\n'
		 << "v -> Display stats" << '\n' << endl;
}

void Grid::checkInventory()
{
	int id;
	Hero * cur = selectHero();
	id = cur -> checkInventory();
	cur->showSpellbook(id);
}

void Grid::equip(){
	int tmp;
	Hero * cur = selectHero();
	cur->checkInventory(1);
	cout << "Give the number of the item you want to use" << endl;
	cin >> tmp;
	cur->equip(tmp);
}

void Grid::use(){
	int tmp;
	Hero * cur = selectHero();
	cur->checkInventory(2);
	cout << "Give the number of the item you want to use" << endl;
	cin >> tmp;
	cur->use(tmp);
}

bool Grid::quitGame(){
	for(int i = 1; i <= party.size(); i ++)
		delete party.get(i);
	return true;
}
