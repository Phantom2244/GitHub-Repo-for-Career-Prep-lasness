//
// Created by Leo Asness on 3/18/26.
//

#include "Knight.h"
#include "Rogue.h"
#include "Mage.h"
#include "Dragon.h"

#include <iostream>
#include <fstream>
#include <cstdlib> // Required for rand() and srand()
#include <ctime>   // Required for time()
#include <memory>
#include <vector>
using namespace std;

void dragon_attacks(int, int, vector<unique_ptr<Adventurer>>, vector<unique_ptr<Adventurer>>);

// INFORM THE PYTHON UI WHAT CHARACTER TO DISPLAY, BASED ON WHOSE TURN IT IS !
void writeTurnToFile(const string& turnName) {
    // cout << "Writing to file " << turnName << endl;
    ofstream file("turn.txt", ios::trunc);
    if (file.is_open()) {
        file << turnName;
        file.close();
    }
}

void writeHPToFile(string turn, int knight_hp, int rogue_hp, int mage_hp, int dragon_hp) {
    ofstream file("hp.txt", ios::trunc);

    if (file.is_open()) {
        file << "turn=" << turn << endl;
        file << "knight_hp=" << knight_hp << endl;
        file << "rogue_hp=" << rogue_hp << endl;
        file << "mage_hp=" << mage_hp << endl;
        file << "dragon_hp=" << dragon_hp << endl;
        file.close();
    }
}




int main() {

    srand(time(nullptr));
    system("python3 turn_display.py &");

    int turns = 0;

    int which_dragon_attack = 0;
    bool dragon_selecting_target = false;
    int dragon_target = 0;
    string dragon_target_str;

    int dragon_hp = 0;
    string dragon_hp_str;

    int knight_hp = 0;
    string knight_hp_str;

    int rogue_hp = 0;
    string rogue_hp_str;

    int mage_hp = 0;
    string mage_hp_str;

    string turn_choice;
    bool still_rogue_turn = false;
    bool still_knight_turn = false;
    bool still_mage_turn = false;

    int sneak_attack_on_cooldown_until = 0;
    int shield_on_cooldown_until = 0;
    int revive_on_cooldown_until = 0;
    bool party_being_shielded = false;

    vector<unique_ptr<Adventurer>> party;
    party.push_back(make_unique<Knight>());
    party.push_back(make_unique<Rogue>());
    party.push_back(make_unique<Mage>());

    vector<unique_ptr<Adventurer>> dragon;
    dragon.push_back(make_unique<Dragon>());

    vector<unique_ptr<Adventurer>> test_dragon;
    dragon.push_back(make_unique<Dragon>());


    while (dragon[0]->get_hp() > 0 && (party[0]->get_hp() > 0 || party[1]->get_hp() > 0 || party[2]->get_hp() > 0)) {
        turns++;
        cout << "Turn " + to_string(turns) << endl;


        // DRAGON'S ATTACK TYPE ---------------------------------
        // RANDOM NUMBER GENERATOR TO SELECT ONE OF THE THREE ATTACK TYPES
        // EACH ATTACK TYPE HAS A DIFFERENT CHANCE OF HAPPENING
        // CLAW     1-3     50% probability
        // BITE     4-5     33% probability
        // FLAME    6       17% probability
        which_dragon_attack = (rand() % 6) + 1;


        // DRAGON'S TARGET ---------------------------------
        // RANDOM NUMBER GENERATOR TO SELECT ONE OF THE THREE ATTACK TYPES
        dragon_selecting_target = true;
        while (dragon_selecting_target == true) {
            dragon_target = (rand() % 3);
            if ((dragon_target == 0 && party[0]->get_hp() > 0) || (dragon_target == 1 && party[1]->get_hp() > 0) || (dragon_target == 2 && party[2]->get_hp() > 0) || (which_dragon_attack == 7)) {
                dragon_selecting_target = false;
            }
        }
        dragon_target_str = party[dragon_target]->get_class();




        // **********************************************************************
        // HEALTH DISPLAY
        // **********************************************************************

        dragon_hp = dragon[0]->get_hp();
        dragon_hp_str = to_string(dragon_hp);
        cout << "The Dragon has " + dragon_hp_str + " HP remaining." << endl;

        knight_hp = party[0]->get_hp();
        knight_hp_str = to_string(knight_hp);
        cout << "The Knight has " + knight_hp_str + " HP remaining." << endl;

        rogue_hp = party[1]->get_hp();
        rogue_hp_str = to_string(rogue_hp);
        cout << "The Rogue has " + rogue_hp_str + " HP remaining." << endl;

        mage_hp = party[2]->get_hp();
        mage_hp_str = to_string(mage_hp);
        cout << "The Mage has " + mage_hp_str + " HP remaining." << endl;




        // DRAGON PLAN OF ATTACK ---------------------------------
        if (which_dragon_attack <= 3) {
            cout << "The Dragon raises a claw at " + dragon_target_str + "..." << endl;
        }
        else if (which_dragon_attack <= 5) {
            cout << "The Dragon snarls at " + dragon_target_str + "..." << endl;
        }
        else {
            cout << "The Dragon exhales a puff of smoke..." << endl;
        }




        // **********************************************************************
        // ROGUE
        // **********************************************************************

        if (party[1]->get_hp() > 0) {
            still_rogue_turn = true;
            cout << "Rogue's Turn" << endl;
            // DISPLAY CHARACTER -- 1st WRITE TURN TO FILE ...
            writeTurnToFile("Rogue");
            writeHPToFile("Rogue",party[0]->get_hp(),party[1]->get_hp(),party[2]->get_hp(),dragon[0]->get_hp());
            if (sneak_attack_on_cooldown_until > turns) {
                cout << "Sneak Attack on cooldown for " + to_string(sneak_attack_on_cooldown_until - turns) + " turn(s)." << endl;
            }
            while (still_rogue_turn == true) {
                cout << "Should the Rogue attack for 20 damage (type \"attack\"), heal a companion 5 HP (type \"heal\"), or use Sneak Attack to deal double damage, but can't use it again for 2 turns (type \"sneak attack\"): ";
                getline(cin, turn_choice);
                if (turn_choice == "attack") {
                    dragon[0]->harmed(party[1]->get_attack());
                    still_rogue_turn = false;
                }
                else if (turn_choice == "heal") {
                    cout << "Which class should be healed: ";
                    getline(cin, turn_choice);
                    if (turn_choice == "knight") {
                        party[0]->being_healed(party[1]->get_healing());
                        still_rogue_turn = false;
                    }
                    else if (turn_choice == "rogue") {
                        party[1]->being_healed(party[1]->get_healing());
                        still_rogue_turn = false;
                    }
                    else if (turn_choice == "mage") {
                        party[2]->being_healed(party[1]->get_healing());
                        still_rogue_turn = false;
                    }
                    else {
                        cout << "That is not a class in the party." << endl;
                    }
                }
                else if (turn_choice == "sneak attack") {
                    if (turns >= sneak_attack_on_cooldown_until) {
                        dragon[0]->harmed((party[1]->get_attack())*2);
                        sneak_attack_on_cooldown_until = (turns + 3);
                        still_rogue_turn = false;
                    }
                    else {
                        cout << "Sneak Attack still on cooldown" << endl;
                    }
                }
                else {
                    cout << "Command not one of the options." << endl;
                }
            }
        }




        // **********************************************************************
        // KNIGHT
        // **********************************************************************

        if (party[0]->get_hp() > 0) {
            still_knight_turn = true;
            cout << "Knight's Turn" << endl;
            // DISPLAY CHARACTER -- 1st WRITE TURN TO FILE ...
            writeTurnToFile("Knight");
            writeHPToFile("Knight",party[0]->get_hp(),party[1]->get_hp(),party[2]->get_hp(),dragon[0]->get_hp());
            if (shield_on_cooldown_until > turns) {
                cout << "Shield on cooldown for " + to_string(shield_on_cooldown_until - turns) + " turn." << endl;
            }
            while (still_knight_turn == true) {
                cout << "Should the Knight attack for 10 damage (type \"attack\"), heal a companion 5 HP (type \"heal\"), or use Shield to protect an ally, but can't use it again for 1 turns (type \"shield\"): ";
                getline(cin, turn_choice);
                if (turn_choice == "attack") {
                    dragon[0]->harmed(party[0]->get_attack());
                    still_knight_turn = false;
                }
                else if (turn_choice == "heal") {
                    cout << "Which class should be healed: ";
                    getline(cin, turn_choice);
                    if (turn_choice == "knight") {
                        party[0]->being_healed(party[0]->get_healing());
                        still_knight_turn = false;
                    }
                    else if (turn_choice == "rogue") {
                        party[1]->being_healed(party[0]->get_healing());
                        still_knight_turn = false;
                    }
                    else if (turn_choice == "mage") {
                        party[2]->being_healed(party[0]->get_healing());
                        still_knight_turn = false;
                    }
                    else {
                        cout << "That is not a class in the party." << endl;
                    }
                }
                else if (turn_choice == "shield") { // make it more clear what shield does
                    if (turns >= shield_on_cooldown_until) {
                        if (dragon_target_str != "knight" || which_dragon_attack != 7) {
                            party_being_shielded = true;
                            shield_on_cooldown_until = (turns + 2);
                            still_knight_turn = false;
                        }
                        else {
                            cout << "The Dragon already planned to attack the Knight." << endl;
                        }
                    }
                    else {
                        cout << "Shield still on cooldown" << endl;
                    }
                }
                else {
                    cout << "Command not one of the options." << endl;
                }
            }
        }




        // **********************************************************************
        // MAGE
        // **********************************************************************

        if (party[2]->get_hp() > 0) {
            still_mage_turn = true;
            cout << "Mage's Turn" << endl;
            // DISPLAY CHARACTER -- 1st WRITE TURN TO FILE ...
            writeTurnToFile("Mage");
            writeHPToFile("Mage",party[0]->get_hp(),party[1]->get_hp(),party[2]->get_hp(),dragon[0]->get_hp());
            if (revive_on_cooldown_until > turns) {
                cout << "Revive on cooldown for " + to_string(revive_on_cooldown_until - turns) + " turn(s)." << endl;
            }
            while (still_mage_turn == true) {
                cout << "Should the Mage attack for 5 damage (type \"attack\"), heal a companion 30 HP (type \"heal\"), or use Revive to bring a dead ally back to life with half HP, but can't use it again for 3 turns (type \"revive\"): ";
                getline(cin, turn_choice);
                if (turn_choice == "attack") {
                    dragon[0]->harmed(party[2]->get_attack());
                    still_mage_turn = false;
                }
                else if (turn_choice == "heal") {
                    cout << "Which class should be healed: ";
                    getline(cin, turn_choice);
                    if (turn_choice == "knight") {
                        party[0]->being_healed(party[2]->get_healing());
                        still_mage_turn = false;
                    }
                    else if (turn_choice == "rogue") {
                        party[1]->being_healed(party[2]->get_healing());
                        still_mage_turn = false;
                    }
                    else if (turn_choice == "mage") {
                        party[2]->being_healed(party[2]->get_healing());
                        still_mage_turn = false;
                    }
                    else {
                        cout << "That is not a class in the party." << endl;
                    }
                }
                else if (turn_choice == "revive") {
                    if (turns >= revive_on_cooldown_until) {
                        cout << "Select which ally to revive: ";
                        getline(cin, turn_choice);
                        if (turn_choice == "knight") {
                            if (party[0]->get_hp() <= 0) {
                                party[0]->set_hp(50);
                                revive_on_cooldown_until = (turns + 2);
                                still_mage_turn = false;
                            }
                            else {
                                cout << "The knight is still alive." << endl;
                            }
                        }
                        else if (turn_choice == "rogue") {
                            if (party[1]->get_hp() <= 0) {
                                party[1]->set_hp(45);
                                revive_on_cooldown_until = (turns + 2);
                                still_mage_turn = false;
                            }
                            else {
                                cout << "The rogue is still alive." << endl;
                            }
                        }
                        else {
                            cout << "That is not a class in the party." << endl;
                        }
                    }
                    else {
                        cout << "Revive still on cooldown" << endl;
                    }
                }
                else {
                    cout << "Command not one of the options." << endl;
                }
            }
        }




        // **********************************************************************
        // DRAGON
        // **********************************************************************

        if (dragon[0] -> get_hp() > 0) {

            writeTurnToFile("Dragon");
            writeHPToFile("Dragon",party[0]->get_hp(),party[1]->get_hp(),party[2]->get_hp(),dragon[0]->get_hp());

            if (party_being_shielded == false) {
                if (which_dragon_attack <= 3) {
                    cout << "The Dragon slashes at " + dragon_target_str + "!" << endl;
                    party[dragon_target]->harmed(dragon[0]->get_attack());
                }
                else if (which_dragon_attack <= 5) {
                    cout << "The Dragon bites " + dragon_target_str + "!" << endl;
                    party[dragon_target]->harmed((dragon[0]->get_attack())*2);
                }
                else {
                    cout << "The Dragon breaths fire on the party!" << endl;
                    party[0]->harmed(dragon[0]->get_attack());
                    party[1]->harmed(dragon[0]->get_attack());
                    party[2]->harmed(dragon[0]->get_attack());
                }
            }
            else {
                if (which_dragon_attack <= 3) {
                    cout << "The Dragon attempts to slash at " + dragon_target_str + "!" << endl;
                    cout << "The Knight protects " + dragon_target_str + "!" << endl;
                    party[0]->harmed(dragon[0]->get_attack());
                    party_being_shielded = false;
                }
                else if (which_dragon_attack <= 5) {
                    cout << "The Dragon attempts to bite " + dragon_target_str + "!" << endl;
                    cout << "The Knight protects " + dragon_target_str + "!" << endl;
                    party[0]->harmed((dragon[0]->get_attack())*2);
                    party_being_shielded = false;
                }
                else {
                    cout << "The Dragon attempts to breath fire on the party!" << endl;
                    cout << "The Knight protects the party!" << endl;
                    party[0]->harmed((dragon[0]->get_attack())*3);
                    party_being_shielded = false;
                }
            }
        }
    }
    if (dragon[0]->get_hp() <= 0) {
        cout << "The Dragon has been defeated! You win!" << endl;
    }
    else {
        cout << "The party has been defeated..." << endl;
    }
}
