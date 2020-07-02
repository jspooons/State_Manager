#include <iostream>
#include <memory>
#include <stdexcept>
#include "State_Manager.h"
#include "State.h"



using namespace std;

class MainMenuState : public State {
	std::shared_ptr<State_Manager> stateManager;

public:
	void enter() {
		cout << "MainMenuState has been entered" << endl;
	}

	void exit() {
		cout << "MainMenuState is exiting" << endl;
	}

	void update(float elapsedTime) {
		cout << "MainMenuState has been updated" << endl;
	}

	void draw(float elapsedFrameTime) {
		cout << "MainMenuState has been drawn" << endl;
	}
};

class IntroState : public State {
	std::shared_ptr<State_Manager> stateManager;



public:
	IntroState(const std::shared_ptr<State_Manager>& stateManager) :
		stateManager(stateManager) {}

	void enter() {
		cout << "IntroState has been entered" << endl;
	}

	void exit() {
		cout << "IntroState is exiting" << endl;
	}

	void update(float elapsedTime) {
		cout << "IntroState has been updated, switching to MainMenuState" << endl;
		this->stateManager->Switch(std::shared_ptr<State>(new MainMenuState()));
	}
};

void state_console_app() {
	std::shared_ptr<State_Manager> stateManager = std::make_shared<State_Manager>();

	cout << "*** Pushing intro game state onto the stack ***" << endl;
	stateManager->push(std::make_shared<IntroState>(stateManager));

	for (std::size_t index = 0; index < 3; ++index) {
		cout << endl;
		cout << "*** Main loop cycle" << (index + 1) << " ***" << endl;
		stateManager->draw(1.0f);
		stateManager->update(1.0f);
	}

	cout << endl;
	cout << "*** shutting down the state manager ***" << endl;
	stateManager.reset();
}


int main() {
	state_console_app();
}