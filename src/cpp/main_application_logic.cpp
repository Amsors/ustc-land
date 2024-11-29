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
    Stack tmp = Stack(newStack, UNCHECKED, lastFrame, stamp);
    stacks.emplace_back(tmp);
    //cards.emplace_back(std::move(newStack));
    //cardStatus.emplace_back(std::tuple(UNCHECKED,"N/A", 0.0,this->stamp));
    stamp++;

    showCard();
    return true;
}

void MainApplication::showCard() {
    return;

    for (int i = 0; i < this->cards.size(); i++) {
        std::cout << "stack: " << i << ":\n";
        for (int j = 0; j < this->cards.at(i).size(); j++) {
            std::cout << "    card " << j << " " << this->cards.at(i).at(j).get()->getName() << std::endl;
        }
    }
}

void MainApplication::checkCard() {
    for (int i = 0; i < stacks.size(); i++) {
        if (/*std::get<CHECKSTATUS>(this->cardStatus[i])*/ != UNCHECKED) {
            continue;
        }
        std::vector<std::shared_ptr<Card>>& stack = this->cards.at(i);
        CardSet tmp(stack);
        tmp.showCardDetail();
        auto it = reg.cardSetMap.find(tmp);
        if (it == reg.cardSetMap.end()) {
            std::get<CHECKSTATUS>(cardStatus[i]) = CHECKED_N;
            std::get<CARDSET>(cardStatus[i]) = "N/A";
            std::get<TIME>(cardStatus[i]) = lastFrame;
        }
        else {
            std::cout << "match " << it->second << std::endl;
            std::get<CHECKSTATUS>(cardStatus[i]) = CHECKED_P;
            std::get<CARDSET>(cardStatus[i]) = it->second;
            std::get<TIME>(cardStatus[i]) = lastFrame;

            /*for (int j = 0; j < reg.cardSetToFormula[it->second].size(); j++) {
                std::string formulaName = reg.cardSetToFormula[it->second].at(j);
                for (int k = 0; k < reg.formulaPtr[formulaName]->getRewardName().size(); k++) {
                    this->rewards.push(reg.formulaPtr[formulaName]->getRewardName().at(k));
                }
            }*/

            reg.outputAttribute();
        }
    }
    
}

void MainApplication::check_all_cards() {
    int stackSum = this->cards.size();
    std::cout << "------------\n";
    for (int i = 0; i < stackSum; i++) {
        CardSet tmp(this->cards.at(i));
        std::cout << "\nstack " << i << " ";
        tmp.showCardDetail();
        auto it = reg.cardSetMap.find(tmp);
        if (it == reg.cardSetMap.end()) {

        }
        else {
            std::cout << "match " << it->second << std::endl;
            for (int j = 0; j < reg.cardSetToFormula[it->second].size(); j++) {
                std::string formulaName = reg.cardSetToFormula[it->second].at(j);
                for (int k = 0; k < reg.formulaPtr[formulaName]->getRewardName().size(); k++) {
                    this->rewards.push(reg.formulaPtr[formulaName]->getRewardName().at(k));
                }
            }
            reg.outputAttribute();
        }
    }

    //reg.outputAttribute();
}

void MainApplication::giveReward() {

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
            }
            if (!given) {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "reward {} : {} does not exist", r->getType(), r->getCardName());
            }*/
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

}

void MainApplication::processWaitingCard() {
    return;
}
