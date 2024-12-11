#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// Classes
class Weapon {
    public:
        string name;
        int power;
        int price;
};

class Zombie {
    public:
        string name;
        int power;
        int exp;
        int zhp;
        int lvlRestriction;
};

// Data saving struct
struct gameData {
    int playerHp;
    int playerLvl;
    int playerExp;
    int gold;
    vector<string> inventory;
};

// Functions
gameData loadGameData();
gameData currentSessionData();
Weapon loadWeapon();
void mainMenu();
void choiceHandler(char choice);
void play();
void displaySession();
void deleteSavedData();
void saveData();
void updateCurrentSession();
void cheat();
void goStore();
void displayWeaponList();
void buyWeapon();
void sellWeapon();
void goLair();
void checkInventory();
void encounter();
void battle(Zombie enemy);
void heal();
void checkExp();
void tutorial();
void lore();
void playerStats();


// Global variables
bool run = true;
int times_played;
string store[5] = {"Stick", "Bow", "Maze", "Sword", "Master Sword"};
gameData stats = loadGameData();
vector<string> rules = {
    "Choose from the menus by pressing the keys between [] or the number displayed.",
    "Buy and sell weapons while u keep at least 1 weapon to be equipped.",
    "Kill zombies, check your level and the level restriction to encounter them."
};

// Weapons
Weapon stick        = {"Stick", 10, 1};
Weapon bow          = {"Bow", 20, 100};
Weapon maze         = {"Maze", 50, 400};
Weapon sword        = {"Sword", 100, 1000};
Weapon masterSword  = {"Master Sword", 250, 3000};

Weapon equipped = loadWeapon();
vector<Weapon> weaponList = {stick, bow, maze, sword, masterSword};
int power;

// Zombies
Zombie low          = {"Weak Zombie",   5,   100,  50,    1};
Zombie middle       = {"Middle Zombie", 20,  300,  1500,  4};
Zombie strong       = {"Strong Zombie", 100, 700,  10000, 7};
Zombie finalBoss    = {"Final Boss",    200, 2000, 30000, 10};

// Program starts
int main() {
    system ("CLS");
    cout << "Welcome to Zombie Frenzy!!\n";
    mainMenu();
    return 0;
}

// Function definitions
gameData currentSessionData(){
    return stats;
}

void checkExp(){
    //Set level exp limit and the hp increase
    int lvlLimit;
    int hpInc;

    //Based on player level, increase the amount of exp needed to level up
    switch(stats.playerLvl){
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            lvlLimit = 500;
            hpInc = 250;
            break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            lvlLimit = 1000;
            hpInc = 300;
            break;
        default:
            lvlLimit = 1500;
            hpInc = 300;
            break;
    }

    // Check exp is higher than the limit, and level up
    while(stats.playerExp > lvlLimit){
        stats.playerLvl += 1;
        stats.playerHp  += hpInc;
        stats.playerExp -= lvlLimit;
    }
}

void saveData(){
    // Saving data
    cout << "\n\nDo you want to save the session's data?(Y/N)\n";
    cout << "This action cannot be undone.\n";

    // Choice handler
    char choice;
    cin >> choice;
    choice = toupper(choice);
    if(choice == 'N' || choice == 'n'){
        // Choosing no warning
        cout << "\nBack to main menu?(Y/N)\n";
        cout << "Choosing (N) will close the application and erase your progress.\n";
        cin >> choice;
        choice = toupper(choice);
        if (choice == 'Y' || choice == 'N'){
            mainMenu();
        } else {
            run = false;
        }
    } else {

        // Saving data
        gameData saveData = currentSessionData();

        // Open save text file
        ofstream overwriteFile;
        overwriteFile.open("gameData.txt");

        // Check if the file was successfully opened
        if (!overwriteFile.is_open()) {
            cout << "Error: Could not open gameData.txt\n";
            mainMenu();
        }

        // Write to the file new values
        overwriteFile << saveData.playerHp << "\n";
        overwriteFile << saveData.playerLvl << "\n";
        overwriteFile << saveData.playerExp << "\n";
        overwriteFile << saveData.gold << "\n";
        for(int i = 0; i < saveData.inventory.size(); i++) {
            overwriteFile << saveData.inventory[i] << "\n";

        }

        // Close the file
        overwriteFile.close();

        // Weapon save file
        Weapon saveWeapon = equipped;

        ofstream overwriteWeapon;
        overwriteWeapon.open("weaponData.txt");

        // Check if the file was successfully opened
        if (!overwriteFile.is_open()) {
            cout << "Error: Could not open weaponData.txt\n";
            mainMenu();
        }

        // Write to the file new values
        overwriteWeapon << saveWeapon.name << "\n";
        overwriteWeapon << saveWeapon.power << "\n";
        overwriteWeapon << saveWeapon.price << "\n";

        // Close the file
        overwriteWeapon.close();

        // Success message
        cout << "\nData has been saved.\n";
        cout << "\nBack to main menu? Or back to the game? (M/P/close game)\n";
        cin >> choice;
        choice = toupper(choice);
        switch(choice){
            case 'M':
                mainMenu();
                break;
            case 'P':
                play();
                break;
            default:
                run = false;
                break;
        }
    }
}

Weapon loadWeapon(){
    // String to write & weapon
    string loadWeapon;
    Weapon loadedWeapon;

    // Read from the text file
    ifstream weaponFile("weaponData.txt");

    // Check if the file was successfully opened
    if (!weaponFile.is_open()) {
        cout << "Error: Could not open gameData.txt\n";
        // Initialize with default values
        loadedWeapon = stick;
    }

    int line = 0;

    // Read lines
    while (getline (weaponFile, loadWeapon)){
        if(line == 0){
            loadedWeapon.name = loadWeapon;
        } else if (line == 1){
            loadedWeapon.power = stoi(loadWeapon);
        } else {
            loadedWeapon.price = stoi(loadWeapon);
        }
        line++;
    }

    return loadedWeapon;
}

gameData loadGameData(){
    // Create a text string, which is used to output the text file
    string gameText;
    int line = 0;

    gameData currentSession;

    // Read from the text file
    ifstream readFile("gameData.txt");

    // Check if the file was successfully opened
    if (!readFile.is_open()) {
        cout << "Error: Could not open gameData.txt\n";
        // Initialize with default values
        currentSession.playerHp = 100;
        currentSession.playerLvl = 1;
        currentSession.playerExp = 0;
        currentSession.gold = 0;
        return currentSession;
    }


    // While loop to read the file line by line
    vector<string> inventory;
    while (getline (readFile, gameText)) {
      // Save the text from the file
      if(line == 0){
        currentSession.playerHp =  stoi(gameText);
      } else if(line == 1){
        currentSession.playerLvl = stoi(gameText);
      } else if(line == 2){
        currentSession.playerExp = stoi(gameText);
      } else if(line == 3){
        currentSession.gold = stoi(gameText);
      } else {
        inventory.push_back(gameText);
      }
      line++;
    }
    currentSession.inventory = inventory;

    // Close the file
    readFile.close();

    return currentSession;
}

void mainMenu(){
    // Clear console
    system ("CLS");

    //Update level
    checkExp();

    // Main menu
    char choice;
    cout << "\nMain Menu:\n";
    cout << "[P]lay\n";
    cout << "[Z]stats\n";
    cout << "[L]ore\n";
    cout << "[T]utorial\n";
    cout << "[D]elete saved data\n";
    cout << "[C]lose game\n";
    cout << "\n";

    // Function to handle main menu & extra choices
    cin >> choice;
    choiceHandler(choice);
}

void choiceHandler(char choice){
    // Make sure the character choice is Uppercase
    choice = toupper(choice);

    // Choice controller
    switch(choice){
        case 'P':
            play();
            break;
        case 'Z':
            displaySession();
            break;
        case 'C':
            run = false;
            break;
        case 'D':
            deleteSavedData();
            break;
        case 'S':
            saveData();
            break;
        case 'X':
            cheat();
            break;
        case 'L':
            lore();
            break;
        case 'T':
            tutorial();
            break;
        default:
            run = false;
            break;
    }
}

void cheat(){
    // This function mainly used for testing, is a cheat to obtain better resources and stats
    gameData stats = currentSessionData();
    stats.playerLvl = 100;
    stats.playerHp = 10000;
    stats.gold = 100000;

    // Open a text file
    ofstream overwriteFile;
    overwriteFile.open("gameData.txt");

    // Check if the file was successfully opened
    if (!overwriteFile.is_open()) {
        cout << "Error: Could not open gameData.txt\n";
        mainMenu();
    }

    // Write to the file cheat values
    overwriteFile << stats.playerHp << "\n";
    overwriteFile << stats.playerLvl << "\n";
    overwriteFile << stats.playerExp << "\n";
    overwriteFile << stats.gold << "\n";
    for(int i = 0; i < stats.inventory.size(); i++) {
        if(i == stats.inventory.size() - 1){
            overwriteFile << stats.inventory[i];
        } else {
            overwriteFile << stats.inventory[i] << ", ";
        }
    }

    // Close the file
    overwriteFile.close();

    cout << "\nCheat successful. Player LVL is now " << stats.playerLvl << " and player HP is " << stats.playerHp << "\n\n";
    mainMenu();
}

void play(){
    // Update lvl
    checkExp();

    // Play menu
    cout << "\nYou are in Zown. Where do you want to go?\n\n";
    cout << "[1] Store\n";
    cout << "[2] Enemy Lair\n";
    cout << "[3] Inventory\n";
    cout << "[4] Save\n";
    cout << "[5] Player profile\n";
    cout << "[6] Go back to Main menu\n";

    //Choice handler for play menu
    int choice;
    cin >> choice;
    switch(choice){
        case 1:
            goStore();
            break;
        case 2:
            goLair();
            break;
        case 3:
            checkInventory();
            break;
        case 4:
            saveData();
            break;
        case 5:
            playerStats();
            break;
        case 6:
            mainMenu();
            break;
        default:
            play();
            break;
    }
}

void goStore(){
    // Store menu
    cout << "\nYou are at the store. What do you want to do?\n\n";
    cout << "[1] Weapon List\n";
    cout << "[2] Buy Weapon\n";
    cout << "[3] Sell Weapon\n";
    cout << "[4] Town Menu\n";

    // Store choice handler
    int choice;
    cin >> choice;
    switch(choice){
         case 1:
            displayWeaponList();
            break;
        case 2:
            buyWeapon();
            break;
        case 3:
            sellWeapon();
            break;
        case 4:
            play();
            break;
        default:
            mainMenu();
            break;
    }
}

void displayWeaponList(){
    // Showcase the available weapons in the shop
    cout << "\nHere are the available weapons:\n";
    for(auto it : weaponList){
        cout << "Name: " << it.name << ", Power: " << it.power << ", Price: " << it.price <<"\n";
    }

    cout << "\nPress any character key to continue to the shop.";
    char choice;
    cin >> choice;
    goStore();
}

void buyWeapon(){
    // Check if player has gold
    if(stats.gold > 0){
        cout << "\nYou have " << stats.gold << " gold. What weapon do you want to buy?\n";

        // List display
        int line = 1;
        for(auto it : weaponList){
            cout << line << ". " << it.name << ", Price: " << it.price <<"\n";
            line++;
        }
        cout << weaponList.size() + 1 << ". Go back.\n";

        // Choice handler for the weapon to purchase
        int choice;
        cin >> choice;

        switch(choice){
            case 1:
                if(weaponList[0].price <= stats.gold) {
                    cout << "Would you like to buy a " << weaponList[0].name << "? (Y/N)\n";

                    char choice;
                    cin >> choice;
                    choice = toupper(choice);
                    if(choice == 'Y'){
                        stats.gold -= weaponList[0].price;
                        stats.inventory.push_back(weaponList[0].name);
                        cout << "Thanks for buying a " << weaponList[0].name << " at our Store. You have " << stats.gold << " gold now.\n";
                        cout << "Press any character key to continue to the shop.\n";
                        cin >> choice;

                        goStore();

                    } else { goStore(); }

                } else {
                    cout << "You don't have enough gold\n";
                    cout << "Press any character key to continue to the shop.\n";
                    char choice;
                    cin >> choice;

                    goStore();
                }
                break;
            case 2:
                if(weaponList[1].price <= stats.gold) {
                    cout << "Would you like to buy a " << weaponList[1].name << "? (Y/N)\n";

                    char choice;
                    cin >> choice;
                    choice = toupper(choice);

                    if(choice == 'Y'){
                        stats.gold -= weaponList[1].price;
                        stats.inventory.push_back(weaponList[1].name);
                        cout << "Thanks for buying a " << weaponList[1].name << " at our Store. You have " << stats.gold << " gold now.\n";
                        equipped = weaponList[1];
                        cout << "You equipped your " << equipped.name << "\n";
                        cout << "Press any character key to continue to the shop.\n";
                        cin >> choice;

                        goStore();

                    } else { goStore(); }

                } else {
                    cout << "You don't have enough gold\n";
                    cout << "Press any character key to continue to the shop.\n";
                    char choice;
                    cin >> choice;

                    goStore();
                }
                break;
            case 3:
                if(weaponList[2].price <= stats.gold) {
                    cout << "Would you like to buy a " << weaponList[2].name << "? (Y/N)\n";

                    char choice;
                    cin >> choice;
                    choice = toupper(choice);

                    if(choice == 'Y'){
                        stats.gold -= weaponList[2].price;
                        stats.inventory.push_back(weaponList[2].name);
                        cout << "Thanks for buying a " << weaponList[2].name << " at our Store. You have " << stats.gold << " gold now.\n";
                        equipped = weaponList[2];
                        cout << "You equipped your " << equipped.name << "\n";
                        cout << "Press any character key to continue to the shop.\n";
                        cin >> choice;

                        goStore();

                    } else { goStore(); }

                } else {
                    cout << "You don't have enough gold\n";
                    cout << "Press any character key to continue to the shop.\n";
                    char choice;
                    cin >> choice;

                    goStore();
                }
                break;
            case 4:
                if(weaponList[3].price <= stats.gold) {
                    cout << "Would you like to buy a " << weaponList[3].name << "? (Y/N)\n";

                    char choice;
                    cin >> choice;
                    choice = toupper(choice);

                    if(choice == 'Y'){
                        // set gold?
                        stats.gold -= weaponList[3].price;
                        stats.inventory.push_back(weaponList[3].name);
                        cout << "Thanks for buying a " << weaponList[3].name << " at our Store. You have " << stats.gold << " gold now.\n";
                        equipped = weaponList[3];
                        cout << "You equipped your " << equipped.name << "\n";
                        cout << "Press any character key to continue to the shop.\n";
                        cin >> choice;

                        goStore();

                    } else { goStore(); }

                } else {
                    cout << "You don't have enough gold\n";
                    cout << "Press any character key to continue to the shop.\n";
                    char choice;
                    cin >> choice;

                    goStore();
                }
                break;
            case 5:
                if(weaponList[4].price <= stats.gold) {
                    cout << "Would you like to buy a " << weaponList[4].name << "? (Y/N)\n";

                    char choice;
                    cin >> choice;
                    choice = toupper(choice);

                    if(choice == 'Y'){
                        stats.gold -= weaponList[4].price;
                        stats.inventory.push_back(weaponList[4].name);
                        cout << "Thanks for buying a " << weaponList[4].name << " at our Store. You have " << stats.gold << " gold now.\n";
                        equipped = weaponList[4];
                        cout << "You equipped your " << equipped.name << "\n";
                        cout << "Press any character key to continue to the shop.\n";
                        cin >> choice;

                        goStore();

                    } else { goStore(); }

                } else {
                    cout << "You don't have enough gold\n";
                    cout << "Press any character key to continue to the shop.\n";
                    char choice;
                    cin >> choice;

                    goStore();
                }
                break;
            case 6:
                cout << "Press any character key to continue to the shop.\n";

                char choice;
                cin >> choice;

                goStore();

                break;
            default:
                goStore();
                break;
        }
    } else {
        cout << "\nYou don't have gold. Come back when you have some.\n";
        cout << "Press any character key to continue to the shop.\n";

        char choice;
        cin >> choice;

        goStore();
    }
}

void sellWeapon(){
    // Check if more than one weapon is in the inventory
    if(stats.inventory.size() > 1){
        cout << "\nHere are your weapons: \n";

        for(string inv : stats.inventory){
            cout << inv << "\n";
        }

        // Choosing what weapon to sell
        cout << "\nWhich weapon do you want to sell?\n";
        string weaponChoice;
        getline(cin, weaponChoice);

        for(string inv : stats.inventory){
            // Find if input weapon exist in the inventory
            if(inv.find(weaponChoice) != string::npos){
                int chosenPrice;
                for(Weapon weap : weaponList){
                    if(inv == weap.name){
                        chosenPrice = weap.price /2;
                    }
                }

                //Secure the choice is right
                cout << "\nDo you want to sell: " << inv << " for " << chosenPrice << " gold?(Y/N) \n";
                char choice;
                cin >> choice;
                if(choice == 'Y') {
                    cout << "Thanks for selling a " << inv << ". We hope that you come again soon.\n";

                    // Overwrite the inventory with the new weapons that were not sold
                    stats.gold += chosenPrice;
                    vector<string> newStats;

                    for(string pop : stats.inventory){
                        if(pop != inv){
                            newStats.push_back(pop);
                        } else { continue; }
                    }
                    stats.inventory = newStats;

                    // End msge
                    cout << "You have " << stats.gold << " gold now.\n";
                    cout << "\nPress any character key to continue to the shop.";
                    char choice;
                    cin >> choice;
                    goStore();
                }

            } else {
                cout << "Couldn't find a match. \n";
                cout << "\nPress any character key to continue to the shop.";
                char choice;
                cin >> choice;
                goStore();
            }
        }



    } else {
        cout << "\nYou don't have enough weapons to sell. You need to have at least two.";
    }

    cout << "\nPress any character key to continue to the shop.";
    char choice;
    cin >> choice;
    goStore();
}

void goLair(){
    cout << "\nIt's time to wreck some zombies!!\n";

    // Check a weapon is equipped
    if(equipped.name == ""){
        // Choose weapon to equip
        int index = 0;
        cout << "First equip a weapon:\n";

        // Display weapons in inventory
        for(string inv : stats.inventory){
            cout << index + 1 << ". " << inv << "\n";
            index++;
        }

        //Choice handler
        int choice;
        cin >> choice;

        // Check number is not bigger than the amount of weapons in the inventory
        if (choice > stats.inventory.size()){
            cout << "\nThat weapon is not available.\n";

            goLair();

        } else {
            // Equipping the chosen weapon
            string chosenWeapon = stats.inventory[choice - 1];
            cout << "\nYou have chosen a " << chosenWeapon;

            // Showing off the stats of said weapon
            for(Weapon wea : weaponList){
                if(wea.name == chosenWeapon){
                    power = wea.power;
                    cout << " which has " << power << " power.\n";
                    equipped = wea;
                } else { continue; }
            }
        }
    } else {
        cout << "\nYou have a " << equipped.name << " equipped, which gives you " << equipped.power << " power.\n";
    }

    cout << "\nWith your " << equipped.name << " you are ready to defeat some monsters.\n";
    encounter();
}

void encounter(){
    // Update level
    checkExp();

    // Fight menu
    cout << "\nWhat would you like to do?\n";
    cout << "1. Kill Weak Zombie        (1-3 lvl)\n";
    cout << "2. Kill Middle Zombie      (4-6 lvl)\n";
    cout << "3. Kill Strong Zombie      (7-10 lvl)\n";
    cout << "4. Kill Final boss         (min 10)\n";
    cout << "5. Heal yourself           500 gold\n";
    cout << "6. Leave\n";

    // Choice handler for battle, restricting by player level
    int enco;
    cin >> enco;
    switch(enco){
        case 1:
            battle(low);
            break;
        case 2:
            if(stats.playerLvl >= middle.lvlRestriction){
                battle(middle);
            } else {
                cout << "\nYour level is too low: " << stats.playerLvl << " try again when your level is higher.\n";
                encounter();
            }
            break;
        case 3:
            if(stats.playerLvl >= strong.lvlRestriction){
                battle(strong);
            } else {
                cout << "\nYour level is too low: " << stats.playerLvl << " try again when your level is higher.\n";
                encounter();
            }
            break;
        case 4:
            if(stats.playerLvl >= finalBoss.lvlRestriction){
                battle(finalBoss);
            } else {
                cout << "\nYour level is too low: " << stats.playerLvl << " try again when your level is higher.\n";
                encounter();
            }
            break;
        case 5:
            heal();
            break;
        case 6:
        default:
            play();
            break;
    }
}

void battle(Zombie enemy){
    // Zombie's hp during the fight
    int currentHp = enemy.zhp;
    cout << "\nYou have found an enemy! The " << enemy.name << " wants to attack!\n";

    // Encounter
    do {
        cout << "Fast, what will you do?\n";
        cout << "1. Attack\n";
        cout << "2. Leave\n";

        // Player attack or leave
        int choice;
        cin >> choice;
        if(choice == 1){
            cout << "You hit " << equipped.power * stats.playerLvl << " damage points.\n";
            currentHp -= equipped.power * stats.playerLvl;
            currentHp = currentHp < 0 ? 0 : currentHp;
            cout << "Hp left: " << currentHp << "\n";
        } else {
            encounter();
        }

        // Monster attacks back if not defeated
        if(stats.playerHp > 0 && currentHp > 0){
            cout << "Enemy attacks: you lost " << enemy.power << "Hp points.\n";
            stats.playerHp -= enemy.power;
            cout << "Your Hp is now " << stats.playerHp  << "\n";
        }

    } while (currentHp > 0 && stats.playerHp > 0);

    // Hp checker handler to check if player win or die
    if(stats.playerHp <= 0){
        cout << "You have no Hp left. You are dead and will lose your progress.\n";
        cout << "Press any key to proceed.\n";

        char dead;
        cin >> dead;

        mainMenu();

    } else if(currentHp <= 0){
        // Win, player get loot and exp
        cout << "You win! You earn " << enemy.exp << " exp points and go back to town.\n";
        stats.playerExp += enemy.exp;
        cout << "On the way back you find " << enemy.exp * 2 << " gold!!\n";
        stats.gold += enemy.exp * 2;

        cout << "Press any key to proceed.\n";
        char live;
        cin >> live;

        play();
    } else {
        // Error handler
        cout << "Something went wrong, you are taken to town.\n";
        play();
    }
}

void heal(){
    // Purchased a heal
    stats.playerHp += 100;
    stats.gold -= 500;

    cout << "You healed 100 hp points, your health is now " << stats.playerHp << "\n";
    cout << "You spent 500 gold, you have now " << stats.gold << " gold.\n\n";

    encounter();
}

void checkInventory(){
    // Display inventory
    cout << "\nHere is your inventory:\n";
    cout << "Gold: " << stats.gold << "\n";

    for(string inv : stats.inventory){
        cout << inv << "\n";
    }

    cout << "\nPress any character key to continue to town menu.\n";
    char choice;
    cin >> choice;

    play();
}

void displaySession(){
    // Check stats without going in the game
    cout << "\nStats:\n";

    // Load previous sessions saved data
    gameData stats = loadGameData();

    // Display data
    cout << "Player HP:  " << stats.playerHp  << "\n";
    cout << "Player LVL: " << stats.playerLvl << "\n";
    cout << "Player EXP: " << stats.playerExp << "\n";
    cout << "Gold:       " << stats.gold << "\n\n";
    cout << "Inventory: \n";
    for (string inv : stats.inventory){
        cout << "   " << inv << "\n";
    }

    // Display last equipped weapon
    cout << "\nEquipped weapon: " << equipped.name << "\n\n";

    cout << "\n\nBack to main menu?(Y/N)\n";
    char choice;
    cin >> choice;
    choice = toupper(choice);
    if(choice == 'N'){
        cout << "\nDo you want to close the game?(Y/N)\n";
        cin >> choice;
        choice = toupper(choice);
        if (choice == 'Y'){
            run = false;
        } else {
            mainMenu();
        }
    } else {
        mainMenu();
    }
}

void deleteSavedData(){
    // Reset game data to default
    cout << "\n\nAre you sure that you want to delete the saved data?(Y/N)\n";
    cout << "This action cannot be undone.\n";

    // Choice handler
    char choice;
    cin >> choice;
    choice = toupper(choice);
    if(choice == 'N'){
        cout << "\nBack to main menu?(Y/N)\n";
        cin >> choice;
        choice = toupper(choice);
        if (choice == 'N'){
            run = false;

        } else { mainMenu(); }

    } else {
        //Loading current data
        gameData delData = loadGameData();

        // Open game file
        ofstream overwriteFile;
        overwriteFile.open("gameData.txt");

        // Check if the file was successfully opened
        if (!overwriteFile.is_open()) {
            cout << "Error: Could not open gameData.txt\n";
            mainMenu();
        }

        // Set default
        delData.playerHp = 100;
        delData.playerLvl = 1;
        delData.playerExp = 0;
        delData.gold = 0;
        // Delete extra weapons and add Stick
        if(delData.inventory.size() > 1){
            for(int i = delData.inventory.size(); i < 1; i--){
                if(i < 0){
                    delData.inventory.pop_back();
                } else {
                    break;
                }
            }

        } else {
            delData.inventory[0] = "Stick";
        }

        // Write to the file default values
        overwriteFile << delData.playerHp << "\n";
        overwriteFile << delData.playerLvl << "\n";
        overwriteFile << delData.playerExp << "\n";
        overwriteFile << delData.gold << "\n";
        overwriteFile << delData.inventory[0] << "\n";


        // Close the file
        overwriteFile.close();

        // Success delete message
        cout << "\nSaved data has been deleted, default values restored.\n";
        cout << "\nBack to main menu?(Y/N)\n";
        cin >> choice;
        choice = toupper(choice);
        if (choice == 'N'){
            run = false;

        } else { mainMenu(); }
    }
}

void playerStats(){
    // Update lvl
    checkExp();

    // Display full profile
    cout << "\nProfile:\n";
    cout << "   Player lvl: " << stats.playerLvl << "\n";
    cout << "   Player hp : " << stats.playerHp  << "\n";
    cout << "   Player exp: " << stats.playerExp << "\n";
    cout << "   Equipped  : " << equipped.name << "\n\n";

    cout << "Power " <<  stats.playerLvl * equipped.power << "\n\n";

    cout << "Inventory:\n";
    cout << "   Gold      : " << stats.gold << "\n";
    for(string inv : stats.inventory){
        cout << "    " << inv << "\n";
    }

    // Leave
    cout << "\nPress any character key to continue to town menu.\n";
    char choice;
    cin >> choice;

    play();
}

void lore(){
    cout << "You are the saviour of Zown, a small village where you have spent most of you life. \n";
    cout << "You defeated monsters since you became 12, never failed your fellow citizens. Will you keep up to your name and achieve victory once again?\n";

    char choice;
    cout << "Press a key to start playing\n";
    cin >> choice;

    play();
}

void tutorial(){
    // Small tutorial
    cout << "\nWelcome to Zombie Frenzy's tutorial.\n";
    cout << "Do you want to see the rules?(Y/any)\n";

    char choice;
    cin >> choice;
    cout << "\n";
    choice = toupper(choice);

    // Display rules
    int line = 1;
    if (choice == 'Y'){
        for(string rule : rules){
            cout << "   " << line << ". " << rule << "\n";
        }

    } else { main(); }

    cout << "\nPress any character key to restart the game.\n";
    cin >> choice;

    main();
}
