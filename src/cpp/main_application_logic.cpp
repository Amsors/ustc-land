#include <map>
#include <tuple>

#include "spdlog/spdlog.h"
#include "game/logic/register.h"



bool MainApplication::addCard() {

    while (newCards.empty() == false) {
        std::string card = newCards.front();
        newCards.pop();
        bool given = false;
        for (const auto& cardType : reg.allCardType) {
            if (reg.allCard[cardType].contains(card)) {

                std::vector<std::shared_ptr<Card>> newStack;
                newStack.emplace_back(std::make_shared<Card>(cardType, card));
                Stack tmp = Stack(newStack, UNCHECKED, stamp);
                stamp++;
                stacks.emplace_back(tmp);
                reg.cardAttained[card] = true;

                given = true;
                break;
            }
        }
        if (given == false) {
            SPDLOG_LOGGER_WARN(spdlog::get("main"), "no registered card: {}", card);
        }
    }

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
    if (framecnt == 100 && 0) {
        std::cout << "=====output at "<<lastFrame<<"\n";
        dooutput = 1;
        framecnt = 0;
    }
    
    for (int i = 0; i < stacks.size(); i++) {
        
        if (dooutput && 0) {
            std::cout << "checking stack " << i << " stamp is " << this->stacks[i].stamp
                << " status is " << this->stacks[i].status << "\n";
        }

        if (stacks[i].status != UNCHECKED) {
            continue;
        }
        std::vector<std::shared_ptr<Card>>& stack = this->stacks.at(i).cards;
        CardSet tmp(stack);
        //tmp.showCardDetail();
        //std::cout << "  total " << stack.size() << " stacks\n";
        auto it = reg.cardSetMap.find(tmp);
        if (it == reg.cardSetMap.end()) {
            stacks[i].status = CHECKED_N;
        }
        else {
            SPDLOG_LOGGER_TRACE(spdlog::get("main"),
                "match cardset: {} at time: {}. stack stamp: {}", it->second, lastFrame, stacks[i].stamp);

            stacks[i].status = CHECKED_P;
            stacks[i].cardSet = it->second;
            stacks[i].timeUntil = reg.cardSetTimeNeeded[it->second] + lastFrame;

            //reg.outputAttribute();
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
    while (this->rewards.empty() == false) {
        Reward* r = reg.rewardPtr[this->rewards.front()];
        this->rewards.pop();
        SPDLOG_LOGGER_TRACE(spdlog::get("main"), "try giving reward {}", r->getName());
        if (r->getType() == "card") {
            newCards.push(r->getCardName());
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
        reg.outputAttribute();
        updateAdvancement();
    } 
}

void MainApplication::processWaitingCard() {

    for (auto& stack : this->stacks) {
        if (stack.status < CHECKED_P) {
            continue;
        }
        if (stack.timeUntil <= lastFrame) {
            //std::cout << "need " << stack.timeUntil << " now " << lastFrame << "\n";
            for (int i = 0; i < reg.cardSetToFormula[stack.cardSet].size(); i++) {
                std::string formulaName = reg.cardSetToFormula[stack.cardSet].at(i);
                reg.formulaAttained[formulaName] = true;
                for (int j = 0; j < reg.formulaPtr[formulaName]->getRewardName().size(); j++) {
                    rewards.emplace(reg.formulaPtr[formulaName]->getRewardName().at(j));
                }
            }
            stack.status = UNCHECKED;
        }
    }
    return;
}


void MainApplication::updateAdvancement() {
    //assert(false);
    for (const auto& ad : reg.regAdvancement) {
        if (reg.advancementStatus[ad.first] == Advancement::LOCKED_P) {
            continue;
        }
        if (reg.advancementStatus[ad.first] == Advancement::SHOWN_P) {
            continue;
        }
        if (ad.second->checkAdvancement() == true) {
            SPDLOG_LOGGER_TRACE(spdlog::get("main"), "match advancement: {}", ad.first);
            reg.advancementStatus[ad.first] = Advancement::SHOWN_P;
        }
        else {
            std::cout << "do not match " << ad.first << "\n";
        }
    }
}
