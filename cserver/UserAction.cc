#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <bits/stdc++.h>
#include "GameController.h"
#include "UserAction.h"

using namespace wfrest;
using namespace ctl;
using namespace std;

GameController& controller = GameController::getInstance("default");

// -- DON'T MODIFY ANY CODE ABOVE THIS LINE -- //
// -- YOU ARE ALLOW TO ADD HEADER FILES UNDER THIS LINE -- //

// TODO: Choose your recipe mode
// you may want to have different strategy for different recipe mode

const string RecipeMode = "Salad";
// const string RecipeMode = "SaladAndCheeseBurger";
// const string RecipeMode = "AllRecipe";


// A template GLOBAL VARIABLE vector to store operations
// Feel free to modify this data structure! (or create your own to use)
deque<string> operations;

// A template map to store the position of each counter
// Question: How do you extend this map to store more than one position for a counter?
// Question2: cutting / normal counter have more than one position, how will you handle it?
const map<Counter, pair<int, int>> counterPosition = {
    { BreadCounter, {0, 1} },
    { CabbageCounter, {8, 2} },
    { CheeseBlockCounter, {0, 9} },
    { TomatoCounter, {8, 5} },
    { RawPattyCounter, {0, 20} },
    { StoveCounter, {0, 19} },
    { PlatesCounter, {3, 20} },
    { TrashCounter, {6, 20} },
    { DeliveryCounter, {1, 20} },

    // There are 2 cutting counters, this is only one of it
    { CuttingCounter, {8, 15} },
    // There are so many normal counters, this is only one of it
    { NormalCounter, {8, 20} }
};

const map<Counter, string> counterDirection = {
    { BreadCounter, "w" },
    { CabbageCounter, "s" },
    { CheeseBlockCounter, "w" },
    { TomatoCounter, "s" },
    { RawPattyCounter, "w" },
    { StoveCounter, "w" },
    { PlatesCounter, "d" },
    { TrashCounter, "d" },
    { DeliveryCounter, "d" },

    // There are 2 cutting counters, this is only one of it
    { CuttingCounter, "s" },
    // There are so many normal counters, this is only one of it
    { NormalCounter, "d" }
};

void DefaultInitialize();
void DefaultSendOperation();
void SaladInitialize();
void MakeSalad();
void MakeBurger();
void MakeCheeseBurger();
void MakeMegaBurger();
Recipe GetNextOrder();

// Init the game (DO NOT MODIFY THIS FUNCTION)
void UserAction::InitGame() {
    Initialize();
    // Set the response to "ok" when finished initialization
    controller.SetResponse("ok");
}

// Just a cute Initializing function
void UserAction::Initialize() {
    // TODO: You can do some initialization in this function.
    // Feel free to modify this function.
    // DefaultInitialize() will make you a MEGABurger!
    cout << "Initializing the game..." << endl;
   	if(RecipeMode=="Salad") SaladInitialize();
	else DefaultInitialize();
}

// Main Function of you game logic
void UserAction::SendOperation() {
    // TODO: Implement your gaming logic here
    // DefaultSendOperation() will make you a MEGABurger!
	if(operations.empty()) {
		Recipe nxt=GetNextOrder();
		if(nxt==Salad) MakeSalad();
		else if(nxt==Burger) MakeBurger();
		else if(nxt==CheeseBurger) MakeCheeseBurger();
		else if(nxt==MegaBurger) MakeMegaBurger();
	}
    DefaultSendOperation();
}


// -- Moving series functions Below -- //

void MovePointToPoint(pair<int, int> from, pair<int, int> to) {
    // TODO: Move from one point to another point
	int leftmoves=from.second-to.second;
	//int rightmoves=-leftmoves;
	int upmoves=from.first-to.second;
	//int upmoves=-downmoves;
	while(leftmoves!=0) {
		if(leftmoves<0)  {
			leftmoves++;
			operations.push_back("a");
		}
		else {
			leftmoves--;
			operations.push_back("d");
		}
	}
	while(upmoves!=0) {
		if(upmoves<0) {
			upmoves++;
			operations.push_back("s");
		}
		else {
			upmoves--;
			operations.push_back("w");
		}
	}
}

/*void MovePointToCounter(pair<int, int> fromPoint, Counter toCounter) {
    // TODO: Move from one point to a counter
    pair<int, int> targetPosition = counterPosition.at(toCounter);

}

void MoveCounterToCounter(Counter from, Counter to) {
    // TODO: Move from one counter to another counter
}*/

// -- Moving series functions Above -- //
// -- Strategy for different Recipe Mode Below -- //

Recipe SaladModeStrategy() {
    // Only salad will be in order in Salad Mode
	//MakeSalad();
    return Salad;
}

Recipe SaladCheeseburgerModeStrategy() {
    // TODO: Implement your salad cheeseburger mode strategy here
    // Below is a simple example, feel free to change it

    vector<Order> orderList = controller.GetOrderList();
    if (orderList.empty()) {
        return Salad;
    }
    return orderList[0].recipe;
}

Recipe AllRecipeModeStrategy() {
    // TODO: Implement your all recipe mode strategy here
    // Below is a simple example, feel free to change it

    return MegaBurger;
}

Recipe GetNextOrder() {
    // Return the next order based on the Recipe Mode
    if (RecipeMode == "Salad") {
        return SaladModeStrategy();
    } else if (RecipeMode == "SaladAndCheeseBurger") {
        return SaladCheeseburgerModeStrategy();
    } else {
        return AllRecipeModeStrategy();
    }
}

// -- Strategy for different Recipe Mode Above -- //

// -- Miscallaneous functions Below -- //

void CutIngredient(int times) {
    // TODO: Cut the Ingredient for times
	for(int i=1;i<=times;i++) {
		operations.push_back("f");
	}
}

// -- Miscallaneous functions Above -- //

// -- Pipeline Funtions Below -- //

// You are welcome to change the function prototype
// Like changing the return type or adding more parameters
void MakeSalad() {
    // TODO: Salad Making Pipeline
	// initial position: (4, 20), last operation is "d"
	operations.push_back("e"); //grab plate
	pair<int, int> pos, tar;
	pos.first=4; pos.second=20;
	tar.first=8; tar.second=13;
	MovePointToPoint(pos, tar);
	pos=tar;
	operations.push_back("e"); //put plate on (8, 13)
	tar.first=8; tar.second=7;
	MovePointToPoint(pos, tar);
	pos=tar;
	operations.push_back("s");
	operations.push_back("e"); //get tomato
	tar.first=8; tar.second=14;
	MovePointToPoint(pos, tar);
	pos=tar;
	operations.push_back("s");
	operations.push_back("e"); //put tomato to cutting counter
	CutIngredient(3); // cut tomato
	operations.push_back("e");
	tar.first=8; tar.second=13;
	MovePointToPoint(pos, tar);
	pos=tar;
	operations.push_back("s");
	operations.push_back("e"); //put sliced tomato on plate
	tar.first=8; tar.second=4;
	MovePointToPoint(pos, tar);
	pos=tar;
	operations.push_back("s");
	operations.push_back("e"); //get cabbage
	tar.first=8; tar.second=14;
	MovePointToPoint(pos, tar);
	pos=tar;
	operations.push_back("s");
	operations.push_back("e"); //put cabbage to cutting counter
	CutIngredient(5); // cut cabbage
	operations.push_back("e");
	tar.first=8; tar.second=13;
	MovePointToPoint(pos, tar);
	pos=tar;
	operations.push_back("s");
	operations.push_back("e"); //put sliced cabbage on plate
	operations.push_back("e"); //grab plate
	tar.first=1; tar.second=19;
	MovePointToPoint(pos, tar); //move to (1, 19)
	pos=tar;
	tar.first=1; tar.second=20;
	MovePointToPoint(pos, tar);
	pos=tar; //move to delivery counter
	operations.push_back("e"); //deliver the salad
	tar.first=4; tar.second=20;
	MovePointToPoint(pos, tar);
	pos=tar;
	operations.push_back("d"); //move to default: (4, 20)
}

void SimpleExample() {
    // The beginning steps of making a salad

    operations.clear();
    // Move to Cabbage Counter
    pair<int, int> playerPosition = controller.GetPlayerPosition();
    MovePointToCounter(playerPosition, CabbageCounter);
    // Grab Cabbage
    operations.push_back("e");
    // Move to Cutting Counter
    MoveCounterToCounter(CabbageCounter, CuttingCounter);
    // Cut Cabbage
    operations.push_back("e");
    // ... Do The Rest By Yourself !
}

void MakeBurger() {}
void MakeCheeseBurger() {}
void MakeMegaBurger() {}

// -- Pipeline Funtions Below -- //

// -- Default Series Function Below -- //

// SendOperation function template, free MEGABurger for you!
void DefaultSendOperation() {
    string s = "";
    if (!operations.empty()) {
        s = operations.front();
        operations.pop_front();
        cout << "Operation: " << s << endl;
    }
    if (s == "w") controller.MoveUp();
    if (s == "s") controller.MoveDown();
    if (s == "a") controller.MoveLeft();
    if (s == "d") controller.MoveRight();
    if (s == "e") controller.Interact();
    if (s == "f") controller.InteractSpecial();
}

void DefaultInitialize() {
	vector<string> tmp;
    tmp = {
        /*"w", "w", "w", "w", "e", "d", "d", "d", "d", "w", "e", "f", "f", "f", // grab cheese and cut it
        "d", "d", "d", "d", "d", "d", "w", "e", "a", "w", "e", "s", "s", "d", "e", // fry meat and grab plate
        "a", "a", "a", "a", "w", "w", "e", "d", "w", "e", // grab cheese slices and cooked meat
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "e", "e", "e", "e", "e", "e", "e", "e", "e", "e", // wait for the meat to be cooked
        "a", "a", "a", "a", "w", "e", // put the plate down
        "s", "s", "s", "s", "s", "s", "s", "s", // move down
        "a", "a", "a", "a", "a", "a", "s", "e",  // grab tomato
        "d", "d", "d", "d", "d", "d", "d", "s", "e", "f", "f", "f", // cut tomato
        "e", "a", "s", "e", "a", "a", "a", "a", "a", "a", "a", "a", "a", "s", "e", // grab lettuce
        "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", // move to cutting counter
        "s", "e", "f", "f", "f", "f", "f", // cut lettuce
        "e", "a", "w", "w", "w", "w", "w", "w", "w", "w", "e", "e", // grab plate
        "s", "s", "s", "s", "s", "s", "s", "s", // move down
        "e", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", "a", // move to bottom left
        "w", "w", "w", "w", "w", "w", "w", "w", "w", "w", // move to bread counter
        "d", "w", "e", "a", "w", "e", "d", "w", "e", // grab bread
        "e", "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", "d",
        "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", "e" // finish*/
		"w", "w", "w", "d", "d", "d", "d", "d", "d", "d", "d", "d", "d", "w", "e", //grab meet
		"a", "w", "e"															   //cook meet
   		"s", "s", "s", "s", "d" //move to (4, 20) default
	};
	for(string ss:tmp) {
		operations.push_back(ss);
	}
    //reverse(operations.begin(), operations.end());
}

void SaladInitialize() {
	vector<string> tmp;
	tmp = {
		"d", "d", "d", "d", "d", "d", "d", "d", "d", "d"	//move to (4, 20) default
	};
	for(string ss:tmp) {
		operations.push_back(ss);
	}
}
