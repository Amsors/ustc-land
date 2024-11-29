#include <map>
#include <tuple>

#include "spdlog/spdlog.h"
#include "game/logic/register.h"



bool MainApplication::addCard() {
    std::vector<std::shared_ptr<Card>> newStack;


    int randkind = std::rand() % 3;
    if (randkind == 0) {
        newStack.emplace_back(std::make_shared<Card>(Card::ROLE, "_man"));
    }
    else if (randkind == 1) {
        int num = reg.regArrayElements["item"].size();
        int randcard = std::rand() % num;
        std::string name = reg.regArrayElements["item"].at(randcard);
        std::cout << "trying to add card " << name << std::endl;
        newStack.emplace_back(std::make_shared<Card>(Card::ITEM, name));
    }
    else if (randkind == 2) {
        int num = reg.regArrayElements["spot"].size();
        int randcard = std::rand() % num;
        std::string name = reg.regArrayElements["spot"].at(randcard);
        std::cout << "trying to add card " << name << std::endl;
        newStack.emplace_back(std::make_shared<Card>(Card::SPOT, name));
    }
    Stack tmp = Stack(newStack, UNCHECKED, stamp);
    stamp++;
    stacks.emplace_back(tmp);

    showCard();
    return true;
}

void MainApplication::showCard() {
    return;

    for (int i = 0; i < this->stacks.size(); i++) {
        std::cout << "stack: " << i << ":\n";
        for (int j = 0; j < this->stacks.at(i).cards.size(); j++) {
            std::cout << "    card " << j << " " << this->stacks.at(i).cards.at(j).get()->getName() << std::endl;
        }
    }
}

int framecnt = 0;
void MainApplication::checkCard() {
    framecnt++;
    int dooutput = 0;
    if (framecnt == 100) {
        std::cout << "=====\n";
        dooutput = 1;
        framecnt = 0;
    }
    
    for (int i = 0; i < stacks.size(); i++) {
        
        if (dooutput) {
            std::cout << "checking stack " << i << " stamp is " << this->stacks[i].stamp
                << " status is " << this->stacks[i].status << "\n";
        }

        if (stacks[i].status != UNCHECKED) {
            continue;
        }
        std::vector<std::shared_ptr<Card>>& stack = this->stacks.at(i).cards;
        CardSet tmp(stack);
        tmp.showCardDetail();
        std::cout << "  total " << stack.size() << " stacks\n";
        auto it = reg.cardSetMap.find(tmp);
        if (it == reg.cardSetMap.end()) {
            stacks[i].status = CHECKED_N;
        }
        else {
            std::cout << "match " << it->second << std::endl;

            stacks[i].status = CHECKED_P;
            stacks[i].cardSet = it->second;
            stacks[i].timeUntil = reg.cardSetTimeNeeded[it->second] + lastFrame;

            reg.outputAttribute();
        }
    }
    
}

void MainApplication::check_all_cards() {
    int stackSum = this->stacks.size();
    std::cout << "------------\n";
    for (int i = 0; i < stackSum; i++) {
        CardSet tmp(this->stacks.at(i).cards);
        std::cout << "\nstack " << i << " ";
        tmp.showCardDetail();  
    }
    //reg.outputAttribute();
}

void MainApplication::giveReward() {
    while (this->rewards.size() > 0) {
        std::cout << "reward!\n";
        rewards.pop();
    }
    /*
    

    while (this->rewards.empty() == false) {
        Reward* r = reg.rewardPtr[this->rewards.front()];
        SPDLOG_LOGGER_TRACE(spdlog::get("main"), "try giving reward {}", r->getName());
        if (r->getType() == "card") {
            /*bool given = false;
            for (std::string i : reg.allCardType) {
                if (reg.allCard[i].contains(r->getCardName())) {

                    given = true;
                    break;
                }
            }  //
            if (!given) {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "reward {} : {} does not exist", r->getType(), r->getCardName());
            }
            if (reg.allCard["item"].contains(r->getCardName())) {
                std::vector<std::shared_ptr<Card>> newStack;
                newStack.emplace_back(std::make_shared<Card>(Card::ITEM, r->getCardName()));
                cards.emplace_back(std::move(newStack));

                cardStatus.emplace_back(std::tuple(UNCHECKED, "N/A", 0.0, this->stamp));
                stamp++;
            }
            else if (reg.allCard["role"].contains(r->getCardName())) {
                std::vector<std::shared_ptr<Card>> newStack;
                newStack.emplace_back(std::make_shared<Card>(Card::ROLE, r->getCardName()));
                cards.emplace_back(std::move(newStack));

                cardStatus.emplace_back(std::tuple(UNCHECKED, "N/A", 0.0, this->stamp));
                stamp++;
            }
            else if (reg.allCard["spot"].contains(r->getCardName())) {
                std::vector<std::shared_ptr<Card>> newStack;
                newStack.emplace_back(std::make_shared<Card>(Card::SPOT, r->getCardName()));
                cards.emplace_back(std::move(newStack));

                cardStatus.emplace_back(std::tuple(UNCHECKED, "N/A", 0.0, this->stamp));
                stamp++;
            }
            else {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "no card {}", r->getCardName());
            }
        }
        else if (r->getType() == "attributeValue") {
            if (reg.regAttribute.contains(r->getAttributeName()) == false) {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "reward {} : {} does not exist", r->getType(), r->getAttributeName());
            }
            if (r->getChange() == "ratio_of_rest") {
                reg.regAttribute[r->getAttributeName()]->getAttributeValue() +=
                    (reg.regAttribute[r->getAttributeName()]->getMax() -
                        reg.regAttribute[r->getAttributeName()]->getAttributeValue()) * (r->getChangeValue());
            }
            else if (r->getChange() == "add") {
                reg.regAttribute[r->getAttributeName()]->getAttributeValue() += r->getChangeValue();
            }
            else if (r->getChange() == "mult") {
                reg.regAttribute[r->getAttributeName()]->getAttributeValue() *= r->getChangeValue();
            }
            else {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "change: {} does not exist", r->getChange());
            }
        }
        else if (r->getType() == "attributeArray") {
            if (reg.regAttribute.contains(r->getAttributeName()) == false) {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "reward {} : {} does not exist", r->getType(), r->getAttributeName());
            }
            if (reg.regAttribute[r->getAttributeName()]->getAttributeArray().contains(r->getKey()) == false) {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "{} not in registered array {}", r->getKey(),
                    reg.regAttribute[r->getAttributeName()]->getAttributeMatchKey());
            }
            if (r->getChange() == "add") {
                reg.regAttribute[r->getAttributeName()]->getAttributeArray()[r->getKey()] += r->getChangeValue();
            }
            else if (r->getChange() == "mult") {
                reg.regAttribute[r->getAttributeName()]->getAttributeArray()[r->getKey()] *= r->getChangeValue();
            }
            else {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "change: {} does not exist", r->getChange());
            }
        }
        else {
            SPDLOG_LOGGER_WARN(spdlog::get("main"), "reward type: {} does not exist", r->getType());
        }
        this->rewards.pop();
    }
    */
}

void MainApplication::processWaitingCard() {

    for (auto& stack : this->stacks) {
        if (stack.status < CHECKED_P) {
            continue;
        }
        if (stack.timeUntil <= lastFrame) {
            std::cout << "need " << stack.timeUntil << " now " << lastFrame << "\n";
            for (int i = 0; i < reg.cardSetToFormula[stack.cardSet].size(); i++) {
                rewards.emplace(reg.cardSetToFormula[stack.cardSet].at(i));
            }
            stack.status = UNCHECKED;
        }
    }
    return;
}
