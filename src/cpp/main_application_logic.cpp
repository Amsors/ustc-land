#include <map>
#include <tuple>

#include "spdlog/spdlog.h"
#include "game/logic/registry.h"

bool MainApplication::addCard() {
    while(newCards.empty() == false) {
        std::string card = newCards.front();
        newCards.pop();
        bool given = false;
        for(const auto &cardType: reg.allCardType) {
            if(reg.allCard[cardType].contains(card)) {
                std::vector<std::shared_ptr<Card>> newStack;
                newStack.emplace_back(std::make_shared<Card>(cardType, card));
                Stack tmp = Stack(newStack, UNCHECKED, stamp);
                stamp++;

                //防止在拖动卡牌过程中因reward产生的新的卡牌成为被移动对象
                if(stacks.empty()) {
                    stacks.emplace_back(tmp);
                } else {
                    stacks.insert(stacks.end() - 1, tmp);
                }
                reg.cardAttained[card] = true;

                given = true;
                break;
            }
        }
        if(given == false) {
            SPDLOG_LOGGER_WARN(spdlog::get("main"), "no registered card: {} when trying to add card", card);
        }
    }

    return true;
}

void MainApplication::showCard() {
    return;

    for(int i = 0; i < this->stacks.size(); i++) {
        std::cout << "stack: " << i << ":\n";
        for(int j = 0; j < this->stacks.at(i).cards.size(); j++) {
            std::cout << "    card " << j << " " << this->stacks.at(i).cards.at(j).get()->getName() << std::endl;
        }
    }
}

//int framecnt = 0;
void MainApplication::checkCard() {
    //    framecnt++;
    //    int dooutput = 0;
    //    if (framecnt == 100) {
    //        std::cout << "=====output at "<<lastFrame<<"\n";
    //        dooutput = 1;
    //        framecnt = 0;
    //    }

    for(int i = 0; i < stacks.size(); i++) {
        //if (dooutput) {
        //    std::cout << "checking stack " << i << " stamp is " << this->stacks[i].stamp
        //        << " status is " << this->stacks[i].status << "\n";
        //}

        if(stacks[i].status != UNCHECKED) {
            continue;
        }
        std::vector<std::shared_ptr<Card>> &stack = this->stacks.at(i).cards;
        std::string vagueMatch;
        CardSet tmp(stack, vagueMatch);
        auto it = reg.cardSetMap.find(tmp);
        if(it == reg.cardSetMap.end()) {
            stacks[i].status = CHECKED_N;
        } else {
            if(reg.cardSetAttained[it->second] == true && reg.cardSetIsOnce[it->second] == true) {
                stacks[i].status = CHECKED_N;
                SPDLOG_LOGGER_TRACE(spdlog::get("main"),
                    "match cardset(attained): {} at time: {}. stack stamp: {}", it->second, lastFrame, stacks[i].stamp);
            } else {
                SPDLOG_LOGGER_TRACE(spdlog::get("main"),
                    "match cardset: {} at time: {}. stack stamp: {}", it->second, lastFrame, stacks[i].stamp);
                stacks[i].status = CHECKED_P;
                stacks[i].cardSet = it->second;
                stacks[i].timeUntil = reg.cardSetTimeNeeded[it->second] + lastFrame;
                stacks[i].timeStart = lastFrame;
                stacks[i].progress = 0;
                stacks[i].vagueMatch = vagueMatch;
                if(reg.cardSetLostCard.contains(it->second)) {
                    stacks[i].lostCard = reg.cardSetLostCard[it->second];
                } else {
                    SPDLOG_LOGGER_WARN(spdlog::get("main"),
                        "lostcard {} not registered at time: {}. stack stamp: {}", it->second, lastFrame, stacks[i].stamp);
                }
                //reg.outputAttribute();
            }
        }
    }
}

void MainApplication::check_all_cards() {
    int stackSum = this->stacks.size();
    std::cout << "totally " << this->stacks.size() << "cards\n";
    //std::cout << "------------\n";
    for(int i = 0; i < stackSum; i++) {
        std::string NONEED;
        CardSet tmp(this->stacks.at(i).cards, NONEED);
        //std::cout << "\nstack " << i << " ";
        //tmp.showCardDetail();
    }
    //reg.outputAttribute();
}

void MainApplication::giveReward() {
    while(this->rewards.empty() == false) {
        Reward *r = reg.rewardPtr[this->rewards.front()];
        this->rewards.pop();
        SPDLOG_LOGGER_TRACE(spdlog::get("main"), "give reward {}", r->getName());
        if(r->getType() == "card") {
            newCards.push(r->getCardName());
        } else if(r->getType() == "attributeValue") {
            if(reg.regAttribute.contains(r->getAttributeName()) == false) {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "reward {} : {} does not exist", r->getType(), r->getAttributeName());
            }
            if(r->getChange() == "ratio_of_rest") {
                reg.regAttribute[r->getAttributeName()]->getAttributeValue() +=
                (reg.regAttribute[r->getAttributeName()]->getMax() -
                    reg.regAttribute[r->getAttributeName()]->getAttributeValue()) * (r->getChangeValue());
            } else if(r->getChange() == "add") {
                reg.regAttribute[r->getAttributeName()]->getAttributeValue() += r->getChangeValue();
            } else if(r->getChange() == "mult") {
                reg.regAttribute[r->getAttributeName()]->getAttributeValue() *= r->getChangeValue();
            } else {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "change: {} does not exist", r->getChange());
            }
        } else if(r->getType() == "attributeArray") {
            if(reg.regAttribute.contains(r->getAttributeName()) == false) {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "reward {} : {} does not exist", r->getType(), r->getAttributeName());
            }
            if(reg.regAttribute[r->getAttributeName()]->getAttributeArray().contains(r->getKey()) == false) {
                reg.regAttribute[r->getAttributeName()]->getAttributeArray().emplace(std::pair<std::string, double>(r->getKey(), 0.0));
            }
            if(reg.regAttribute[r->getAttributeName()]->getAttributeArray().contains(r->getKey()) == false) {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "{} not in registered array {}", r->getKey(),
                    reg.regAttribute[r->getAttributeName()]->getAttributeMatchKey());
            }
            if(r->getChange() == "add") {
                reg.regAttribute[r->getAttributeName()]->getAttributeArray()[r->getKey()] += r->getChangeValue();
            } else if(r->getChange() == "mult") {
                reg.regAttribute[r->getAttributeName()]->getAttributeArray()[r->getKey()] *= r->getChangeValue();
            } else {
                SPDLOG_LOGGER_WARN(spdlog::get("main"), "change: {} does not exist", r->getChange());
            }
        } else {
            SPDLOG_LOGGER_WARN(spdlog::get("main"), "reward type: {} does not exist", r->getType());
        }
        //reg.outputAttribute();
        updateAdvancement();
    }

    ((ListBar*)listBar)->updateInfo();
}

void MainApplication::processWaitingCard() {
    for(auto &stack: this->stacks) {
        if(stack.status < CHECKED_P) {
            continue;
        }
        stack.progress = (lastFrame - stack.timeStart) / (stack.timeUntil - stack.timeStart);
        if(stack.progress > 1.0) {
            stack.progress = 1.0;
        }
        if(stack.timeUntil <= lastFrame) {
            //std::cout << "need " << stack.timeUntil << " now " << lastFrame << "\n";
            SPDLOG_LOGGER_TRACE(spdlog::get("main"),
                "cardset {} attained in on stack with stamp {}, time is {}", stack.cardSet, stack.stamp, lastFrame);
            if(reg.cardSetAttained[stack.cardSet] == true && reg.cardSetIsOnce[stack.cardSet] == true) {
                stack.status = UNCHECKED;
                continue;
            }
            reg.cardSetAttained[stack.cardSet] = true;
            for(int i = 0; i < reg.cardSetToFormula[stack.cardSet].size(); i++) {
                std::string formulaName = reg.cardSetToFormula[stack.cardSet].at(i);

                //std::cout << "card set " << stack.cardSet << " vaguematch "<<stack.vagueMatch<<"\n";
                if(reg.formulaPtr[formulaName]->getVagueMatch() != stack.vagueMatch) {
                    continue;
                }
                reg.formulaAttained[formulaName] = true;

                if(reg.cardSetLostCard.contains(stack.cardSet)) {
                    std::vector<std::string> &tmpset = reg.cardSetLostCard[stack.cardSet];
                    for(auto const &name: tmpset) {
                        stack.del(name);
                    }
                } else {
                    SPDLOG_LOGGER_WARN(spdlog::get("main"),
                        "lostcard set for cardset {} not registered", stack.cardSet);
                }

                for(int j = 0; j < reg.formulaPtr[formulaName]->getRewardName().size(); j++) {
                    rewards.emplace(reg.formulaPtr[formulaName]->getRewardName().at(j));
                }
            }
            stack.status = UNCHECKED;
        }
    }
    return;
}

void MainApplication::updateAdvancement() {
    reg.AttributeOutput();
    reg.AdvancementOutput();
    for(const auto &ad: reg.regAdvancement) {
        if(reg.advancementStatus[ad.first] == Advancement::LOCKED_P) {
            continue;
        }
        if(reg.advancementStatus[ad.first] == Advancement::SHOWN_P) {
            continue;
        }
        if(ad.second->checkAdvancement() == true) {
            SPDLOG_LOGGER_TRACE(spdlog::get("main"), "match advancement: {}", ad.first);
            reg.advancementStatus[ad.first] = Advancement::SHOWN_P;
        } else {
            //std::cout << "do not match " << ad.first << "\n";
        }
    }
}

//void MainApplication::deleteCard() {
//
//}

void Stack::del(std::string a) {
    //cards.at(0) 是最下面的卡
    //this->show();
    SPDLOG_LOGGER_TRACE(spdlog::get("main"), "delete card {}", a);
    bool finished = false;
    for(int i = 0; i < this->cards.size(); i++) {
        //std::cout << cards.at(i)->getName() << "\n";
        if(cards.at(i)->getName() == a) {
            //std::cout << "at index " << i << "trying to delete " << a << "\n";
            Card::Position lastPos = cards.at(i)->getPosition();
            cards.erase(cards.begin() + i);
            for(int j = i; j < cards.size(); j++) {
                Card::Position tmp = cards.at(j)->getPosition();
                cards.at(j)->move(lastPos - tmp);
                lastPos = tmp;
            }
            finished = true;
            break;
        }
    }
    if(finished == false) {
        SPDLOG_LOGGER_WARN(spdlog::get("main"), "card {} not found when trying to delete it", a);
    }
}

void Stack::show() {
    std::cout << "-----\n";
    std::cout << "stack with stamp " << this->stamp << ":\n";
    std::cout << "totally " << this->cards.size() << " cards\n";
    for(int i = 0; i < this->cards.size(); i++) {
        std::cout << "at index " << i << " is card " << cards.at(i)->getName() << "\n";
    }
    std::cout << "status is " << this->status << "\n";
    std::cout << "cardset is " << this->cardSet << "\n";
    std::cout << "vaguematch is " << this->vagueMatch << "\n";
    std::cout << "-----\n";
}
