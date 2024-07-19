#include <bits/stdc++.h>
#include "GameController.h"
#include "UserAction.h"

using namespace wfrest;
using namespace ctl;
using namespace std;

GameController& controller = GameController::getInstance("default");

const string RecipeMode = "all";

// A template GLOBAL VARIABLE vector to store operations
// Feel free to modify this data structure! (or create your own to use)
queue<char> operations;
pair<int, int> CuttingCounter2, NormalCounter2;

const map<Counter, pair<int, int>> counterPosition = {
    { BreadCounter, {0, 1} },
    { CabbageCounter, {8, 4} }, // Used to be {8, 2}
    { CheeseBlockCounter, {0, 10} },
    { TomatoCounter, {8, 7} }, // Used to be {8, 5}
    { RawPattyCounter, {0, 20} },
    { StoveCounter, {0, 19} }, // Used to be {0, 19}
    { PlatesCounter, {3, 20} },
    { TrashCounter, {6, 20} },
    { DeliveryCounter, {1, 20} },

    // There are 2 cutting counters, this is only one of it
    { CuttingCounter, {8, 15} },
	//{ CuttingCounter2, {0, 14} },
    // There are so many normal counters, this is only one of it
    // { NormalCounter, {8, 20} }
    { NormalCounter, {8, 13} }
	//{ NormalCounter2, {0, 13} }
};

const map<Counter, vector<pair<int, int>>> counterPositions = {
    { BreadCounter, { {0, 1}, {0, 2} } },
    { CabbageCounter, { {8, 2}, {8, 3}, {8, 4} } },
    { CheeseBlockCounter, { {0, 8}, {0, 9}, {0, 10} } },
    { TomatoCounter, { {8, 5}, {8, 6}, {8, 7} } },
    { RawPattyCounter, { {0, 20} } },
    { StoveCounter, { {0, 17}, {0, 18}, {0, 19} } },
    { PlatesCounter, { {2, 20}, {3, 20}, {4, 20} } },
    { TrashCounter, { {5, 20}, {6, 20}, {7, 20} } },
    { DeliveryCounter, { {0, 20}, {1, 20} } },

    // There are 2 cutting counters, this is only one of it
    { CuttingCounter, { {8, 14}, {8, 15}, {8, 16} } },
    // There are so many normal counters, this is only one of it
    { NormalCounter, { {8, 11}, {8, 12}, {8, 13} } }
};

const map<Counter, char> counterDirection = {
    { BreadCounter, 'w' },
    { CabbageCounter, 's' },
    { CheeseBlockCounter, 'w' },
    { TomatoCounter, 's' },
    { RawPattyCounter, 'w' },
    { StoveCounter, 'w' },
    { PlatesCounter, 'd' },
    { TrashCounter, 'd' },
    { DeliveryCounter, 'd' },

    // There are 2 cutting counters, this is only one of it
    { CuttingCounter, 's' },
	//{ CuttingCounter, 'w' },
    // There are so many normal counters, this is only one of it
    { NormalCounter, 's' }
    //{ NormalCounter, 'w' }
};

void DefaultInitialize();
void DefaultSendOperation();

// Init the game (DO NOT MODIFY THIS FUNCTION)
void UserAction::InitGame() {
    Initialize();
    // Set the response to "ok" when finished initialization
    controller.SetResponse("ok");
}

// const string RecipeMode = "salad_cheeseburger";
// Just a cute Initializing function
void UserAction::Initialize() {
    cout << "Initializing the game..." << endl;
    DefaultInitialize();
}

// Main Function of you game logic
void UserAction::SendOperation() {
    DefaultSendOperation();
}

void MovePointToPoint(pair<int, int> from, pair<int, int> to, GameController& controller) {
    char xdir = (from.first < to.first) ? 's' : 'w';
    char ydir = (from.second < to.second) ? 'd' : 'a';
    while (from.first != to.first) {
        operations.push(xdir);
        from.first += (from.first < to.first) ? 1 : -1;
    }
    while (from.second != to.second) {
        operations.push(ydir);
        from.second += (from.second < to.second) ? 1 : -1;
    }
}

void MoveCounterToPoint(Counter from, pair<int, int> to, GameController& controller) {
	pair<int, int> fromPosition=counterPosition.at(from);
	MovePointToPoint(fromPosition, to, controller);
	operations.push('w');
	operations.push('e');
}

void MovePointToCounter(pair<int, int> fromPoint, Counter toCounter, GameController& controller) {
    pair<int, int> targetPosition = counterPosition.at(toCounter);
    MovePointToPoint(fromPoint, targetPosition, controller);
    operations.push(counterDirection.at(toCounter));
    operations.push('e');
}

void MoveCounterToCounter(Counter from, Counter to, GameController& controller) {
    pair<int, int> fromPosition = counterPosition.at(from);
    MovePointToCounter(fromPosition, to, controller);
}

Recipe SaladModeStrategy() {
    return Salad;
}

Recipe SaladCheeseburgerModeStrategy() {
    vector<Order> orderList = controller.GetOrderList();
    if (orderList.empty()) {
        return Salad;
    }
	return orderList[0].recipe;
}

Recipe AllRecipeModeStrategy() {
    vector<Order> orderList = controller.GetOrderList();
    while(orderList.empty()) {
		return Burger;
	}
	int id=(int)orderList.size()-1;
	while(id>0) {
		if(orderList[id].recipe==Salad || orderList[id].recipe==Burger) break;
		id--;
	}
	if(orderList[id].recipe!=CheeseBurger && orderList[id].recipe!=MegaBurger) return orderList[id].recipe;
	id=(int)orderList.size()-1;
	while(id>0) {
		if(orderList[id].recipe==CheeseBurger) break;
		id--;
	}
	if(orderList[id].recipe!=MegaBurger) return orderList[id].recipe;
	id=(int)orderList.size()-1;
	return orderList[id].recipe;
}

Recipe GetNextOrder() {
    if (RecipeMode == "salad") {
        return SaladModeStrategy();
    } else if (RecipeMode == "salad_cheeseburger") {
        return SaladCheeseburgerModeStrategy();
    } else {
        return AllRecipeModeStrategy();
    }
}

void CutIngredient(int times, GameController& controller) {
    for (int i = 0; i < times; i ++) {
        operations.push('f');
    }
	operations.push('e');
}

void MakeSalad(GameController& controller) {
	pair<int, int> playerPosition = controller.GetPlayerPosition();
	MovePointToCounter(playerPosition, PlatesCounter, controller);
	MoveCounterToCounter(PlatesCounter, NormalCounter, controller);
    MoveCounterToCounter(NormalCounter, CabbageCounter, controller);
    MoveCounterToCounter(CabbageCounter, CuttingCounter, controller);
    CutIngredient(5, controller);
    MoveCounterToCounter(CuttingCounter, NormalCounter, controller);
    MoveCounterToCounter(NormalCounter, TomatoCounter, controller);
    MoveCounterToCounter(TomatoCounter, CuttingCounter, controller);
    CutIngredient(3, controller);
    MoveCounterToCounter(CuttingCounter, NormalCounter, controller);
    operations.push('e'); //grab the salad plate
}

void MakeBurger(GameController& controller) {
    pair<int, int> playerPosition = controller.GetPlayerPosition();
    MovePointToCounter(playerPosition, RawPattyCounter, controller);
	MoveCounterToCounter(RawPattyCounter, StoveCounter, controller);
	MoveCounterToCounter(StoveCounter, PlatesCounter, controller);
	MoveCounterToPoint(PlatesCounter, NormalCounter2, controller);
	MovePointToCounter(NormalCounter2, BreadCounter, controller);
	MoveCounterToPoint(BreadCounter, NormalCounter2, controller);
	string blank="swswswe";
	for(char s:blank) {
		operations.push(s);
	}
	MovePointToCounter(NormalCounter2, StoveCounter, controller);
}

void MakeCheeseBurger(GameController& controller) {
    pair<int, int> playerPosition = controller.GetPlayerPosition();
	MovePointToCounter(playerPosition, PlatesCounter, controller);
    MoveCounterToPoint(PlatesCounter, NormalCounter2, controller);
	MovePointToCounter(NormalCounter2, RawPattyCounter, controller);
    MoveCounterToCounter(RawPattyCounter, StoveCounter, controller);
    MoveCounterToCounter(StoveCounter, BreadCounter, controller);
    MoveCounterToPoint(BreadCounter, NormalCounter2, controller);
	MovePointToCounter(NormalCounter2, CheeseBlockCounter, controller);
    MoveCounterToPoint(CheeseBlockCounter, CuttingCounter2, controller);
    CutIngredient(3, controller);
	MovePointToPoint(CuttingCounter2, NormalCounter2, controller);
	operations.push('w');
	operations.push('e');
	operations.push('e');
	MovePointToCounter(NormalCounter2, StoveCounter, controller);
}

void MakeMegaBurger(GameController& controller) {
    MakeCheeseBurger(controller);
	pair<int, int> playerPosition = controller.GetPlayerPosition();
	MovePointToCounter(playerPosition, NormalCounter, controller);
    MoveCounterToCounter(NormalCounter, CabbageCounter, controller);
    MoveCounterToCounter(CabbageCounter, CuttingCounter, controller);
    CutIngredient(5, controller);
    MoveCounterToCounter(CuttingCounter, NormalCounter, controller);
    MoveCounterToCounter(NormalCounter, TomatoCounter, controller);
    MoveCounterToCounter(TomatoCounter, CuttingCounter, controller);
    CutIngredient(3, controller);
    MoveCounterToCounter(CuttingCounter, NormalCounter, controller);
	operations.push('e');
}

void DefaultSendOperation() {
    char s;
    if (operations.empty()) {
		if(controller.GetOrderList().empty()) {
			controller.InteractSpecial();
			return;
		}
	}
	if(operations.empty()) {
        if (!controller.GetPlayerHoldItems().empty()) {
            MovePointToCounter(controller.GetPlayerPosition(), DeliveryCounter, controller);
			return;
        }
		Recipe orderRecipe = GetNextOrder();
        if (orderRecipe == Salad) MakeSalad(controller);
        if (orderRecipe == Burger) MakeBurger(controller);
        if (orderRecipe == CheeseBurger) MakeCheeseBurger(controller);
        if (orderRecipe == MegaBurger) MakeMegaBurger(controller);
    }
    s = operations.front();
    operations.pop();
    if (s == 'w') controller.MoveUp();
    if (s == 's') controller.MoveDown();
    if (s == 'a') controller.MoveLeft();
    if (s == 'd') controller.MoveRight();
    if (s == 'e') controller.Interact();
    if (s == 'f') controller.InteractSpecial();
}

void DefaultInitialize() {
    NormalCounter2.first=0;
    NormalCounter2.second=13;
    CuttingCounter2.first=0;
    CuttingCounter2.second=14;
	while (!operations.empty()) {
        operations.pop();
    }
	string tmp="wfffddddddddddffffe"; //move to (3, 20) as default
	for(char c:tmp) operations.push(c);
}
