#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

using std::unordered_map;
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::min;
using std::max;
using std::shared_ptr;
using std::make_shared;

class Card {
public:
    string name;
    int value;

    Card(const string& name, int value) : name(name), value(value) {}

    virtual void use(class Player& user, class Player& target) = 0;

    virtual ~Card() = default;
};

class AttackCard : public Card {
public:
    AttackCard(const string& name, int value) : Card(name, value) {}

    void use(Player& user, Player& target) override;
};

class HealCard : public Card {
public:
    HealCard(const string& name, int value) : Card(name, value) {}

    void use(Player& user, Player& target) override;
};

class StunCard : public Card {
public:
    StunCard(const string& name, int value) : Card(name, value) {}

    void use(Player& user, Player& target) override;
};

class ShieldCard : public Card {
public:
    ShieldCard(const string& name, int value) : Card(name, value) {}

    void use(Player& user, Player& target) override;
};

class Player {
public:
    string name;
    int hp;
    int initialHp;
    bool shieldActive = false;
    bool stunned = false;

    Player(const string& name, int initialHp) : name(name), hp(initialHp), initialHp(initialHp) {}

    void takeDamage(int damage) {
        // TODO
    }

    void heal(int amount) {
        // TODO
    }

    void activateShield() {
        // TODO
    }

    void applyStun() {
        // TODO
    }

    void resetTurnStatus() {
        // TODO
    }

    bool isDefeated() const {
        // TODO
        return false;
    }
};

class Game {
private:
    Player playerA, playerB;
    unordered_map<int, shared_ptr<Card>> cardLibrary;

public:
    Game(const string& playerAName, int hpA, const string& playerBName, int hpB)
        : playerA(playerAName, hpA), playerB(playerBName, hpB) {}

    void addCardToLibrary(int cardId, const string& name, int value) {
        // TODO
    }

    void playTurn(Player& currentPlayer, Player& opponent, int cardId, Player& target) {
        if (currentPlayer.stunned) {
            currentPlayer.resetTurnStatus();
            return;
        }

        auto it = cardLibrary.find(cardId);
        it->second->use(currentPlayer, target);

        if (opponent.isDefeated()) {
            this->displayPlayerStatus();
            exit(0);
        }
    }

    void displayPlayerStatus() {
        // TODO
    }

    void processAction(const string& currentPlayerName, int cardId, const string& targetPlayerName) {
        Player& currentPlayer = (currentPlayerName == playerA.name) ? playerA : playerB;
        Player& opponent = (currentPlayerName == playerA.name) ? playerB : playerA;
        Player& target = (targetPlayerName == playerA.name) ? playerA : playerB;

        playTurn(currentPlayer, opponent, cardId, target);
    }
};

void AttackCard::use(Player& user, Player& target) {
    // TODO
}

void HealCard::use(Player& user, Player& target) {
    // TODO
}

void StunCard::use(Player& user, Player& target) {
    // TODO
}

void ShieldCard::use(Player& user, Player& target) {
    // TODO
}

int main() {
    // TODO
    return 0;
}