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
        this->hp-=damage;
    }

    void heal(int amount) {
        this->hp+=amount;
        if(this->hp>initialHp)this->hp=initialHp;
    }

    void activateShield() {
        this->shieldActive=true;
    }

    void applyStun() {
        this->stunned=true;
    }

    void resetTurnStatus() {
        this->stunned=false;
    }

    bool isDefeated() const {
        if(this->hp<=0)return true;
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
        if (name == "HXB") {
            cardLibrary[cardId] = make_shared<AttackCard>(name, value);
        } else if (name == "SH") {
            cardLibrary[cardId] = make_shared<HealCard>(name, value);
        } else if (name == "TFE") {
            cardLibrary[cardId] = make_shared<StunCard>(name, value);
        } else if (name == "AS") {
            cardLibrary[cardId] = make_shared<ShieldCard>(name, value);
        }
    }

    void playTurn(Player& currentPlayer, Player& opponent, int cardId, Player& target) {
        if (currentPlayer.stunned) {
            currentPlayer.resetTurnStatus();
            return;
        }

        auto it = cardLibrary.find(cardId);
        if (it == cardLibrary.end()) {
            //cout << "Invalid card ID: " << cardId << endl;
            return;
        }
        it->second->use(currentPlayer, target);

        if (opponent.isDefeated()) {
            // if(playerA.name==opponent.name){
            //     cout<<playerB.name<<" wins!"<<endl;
            // }else{
            //     cout<<playerA.name<<" wins!"<<endl;
            // }

            //cout<<currentPlayer.name<<" wins!"<<endl;

            this->displayPlayerStatus();
            //为什么这两行会存在区别呢
            exit(0);
            return;
        }
    }
    Player& getPlayerA(){
        return playerA;
    }
    Player& getPlayerB(){
        return playerB;
    }

    void displayPlayerStatus() {
        if(playerA.isDefeated()){
            cout<<playerB.name<<" wins!"<<endl;
            return;
        }
        if(playerB.isDefeated()){
            cout<<playerA.name<<" wins!"<<endl;
            return;
        }

        cout<<playerA.name<<" "<<playerA.hp<<" ";
        if(playerA.shieldActive)cout<<"(Shield Active) ";
        if(playerA.stunned)cout<<"(Stunned) ";
        cout<<endl;

        cout<<playerB.name<<" "<<playerB.hp<<" ";
        if(playerB.shieldActive)cout<<"(Shield Active) ";
        if(playerB.stunned)cout<<"(Stunned) ";
        cout<<endl;
    }

    void processAction(const string& currentPlayerName, int cardId, const string& targetPlayerName) {
        if (currentPlayerName != playerA.name && currentPlayerName != playerB.name) {
            //cout << "Invalid player name: " << currentPlayerName << endl;
            return;
        }
        if (targetPlayerName != playerA.name && targetPlayerName != playerB.name) {
            //cout << "Invalid target name: " << targetPlayerName << endl;
            return;
        }

        Player& currentPlayer = (currentPlayerName == playerA.name) ? playerA : playerB;
        Player& opponent = (currentPlayerName == playerA.name) ? playerB : playerA;
        Player& target = (targetPlayerName == playerA.name) ? playerA : playerB;

        playTurn(currentPlayer, opponent, cardId, target);
    }
};

void AttackCard::use(Player& user, Player& target) {
    if (target.shieldActive) {
        target.shieldActive = false; // 护盾吸收伤害
    } else {
        target.takeDamage(value); // 直接扣除生命值
    }
}

void HealCard::use(Player& user, Player& target) {
    int amount=this->value;
    target.heal(amount);
}

void StunCard::use(Player& user, Player& target) {
    target.applyStun();
}

void ShieldCard::use(Player& user, Player& target) {
    target.activateShield();
}

int main() {
    // TODO
    string name1,name2;
    int initialhp1,initialhp2;
    cin>>name1>>initialhp1;
    cin>>name2>>initialhp2;

    Game game=Game(name1,initialhp1,name2,initialhp2);

    int id;
    string cardname;
    int value;
    for (size_t i = 0; i < 4; i++)
    {
        cin>>id>>cardname>>value;
        game.addCardToLibrary(id,cardname,value);
    }
    
    string input;
    cin>>input;
    int iid;string tt;
    if(!isdigit(input[0])){
        cin>>iid>>tt;
        game.processAction(input,iid,tt);
    }

    // string user;
    // while (cin >> user) {
    //     int cid;
    //     string target;

    //     cin >>cid >> target; // 读取卡牌ID和目标玩家
    //     game.processAction(user, cid, target);
    // }

    string user, target;
    int cid;
    while (cin >> user >> cid >> target) {
        game.processAction(user, cid, target);
    }

    game.displayPlayerStatus();
    return 0;
}