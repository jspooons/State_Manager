#pragma once

#include <vector>
#include <memory>
#include "Modality.h"

class State;

class State_Manager
{
	typedef std::pair<std::shared_ptr<State>, Modality::Enum> StateModePair;
	std::vector<StateModePair> stack;
	std::vector<State*> exposables;
public:
	State_Manager();
	~State_Manager();
	std::shared_ptr<State> peek() const;
	void push(const std::shared_ptr<State>& state, Modality::Enum mode = Modality::Exclusive);
	std::shared_ptr<State> pop();
	std::shared_ptr<State> Switch(const std::shared_ptr<State>& state, Modality::Enum mode = Modality::Exclusive) {
		std::shared_ptr<State> currentState = peek();
		if (currentState) {
			pop();
		}

		push(state, mode);

		return currentState;
	}
	void update(float elapsedTime);
	void draw(float elapsedFrameTime);
	void rebuild_exposables();
	void add_to_exposables(State* state);
	void remove_from_exposables(State* state);
	void notifyCoveredStates();
	void notifyRevealedStates();
};

