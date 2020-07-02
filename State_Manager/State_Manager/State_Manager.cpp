#pragma
#include <stdexcept>
#include <memory>
#include "State_Manager.h"
#include "State.h"


//You shouldn't always use shared_ptr because there are more types of smart pointer than just shared_ptr. The Standard Library also provides unique_ptr for example. You must evaluate which better suits the task at hand.
//Smart pointers in general are however the preferred way of safely handling dynamically allocated objects in modern C++

State_Manager::State_Manager() {}

State_Manager::~State_Manager() {
	while (!this->stack.empty()) {
		pop();
	}
}

std::shared_ptr<State> State_Manager::peek() const {
	if (this->stack.empty()) {
		return std::shared_ptr<State>();
	}
	else {
		return this->stack.at(this->stack.size() - 1).first;
	}
}

void State_Manager::push(
	const std::shared_ptr<State>& state, Modality::Enum mode)
{
	this->stack.push_back(std::make_pair(state, mode));

	if (mode == Modality::Exclusive) {
		this->exposables.clear();
	}

	add_to_exposables(state.get());
	notifyCoveredStates();
	state->enter();
}

std::shared_ptr<State> State_Manager::pop() {
	if (this->stack.empty()) {
		throw std::runtime_error("Attempted to pop from an empty game state stack");
	}

	StateModePair popped = this->stack.at(this->stack.size() - 1);
	popped.first->exit();
	this->stack.pop_back();

	if (popped.second == Modality::Exclusive) {
		rebuild_exposables();
	}
	else {
		remove_from_exposables(popped.first.get());
	}

	notifyRevealedStates();

	return popped.first;
}

void State_Manager::update(float elapsedTime) {
	for (std::size_t index = 0; index < this->exposables.size(); ++index) {
		this->exposables.at(index)->update(elapsedTime);
	}
}

void State_Manager::draw(float elapsedFrameTime) {
	for (std::size_t index = 0; index < this->exposables.size(); ++index) {
		this->exposables.at(index)->draw(elapsedFrameTime);
	}
}

void State_Manager::rebuild_exposables() {
	this->exposables.clear();

	if (this->stack.empty()) {
		return;
	}

	std::size_t index = this->stack.size() - 1;
	while (index > 0) {
		if (this->stack.at(index).second == Modality::Exclusive) {
			break;
		}
		index--;
	}

	while (index < this->stack.size()) {
		add_to_exposables(this->stack.at(index).first.get());
		++index;
	}
}

void State_Manager::add_to_exposables(State* state) {
	this->exposables.push_back(state);
}

void State_Manager::remove_from_exposables(State* state) {
	this->exposables.pop_back();
}

void State_Manager::notifyCoveredStates() {
	if (this->stack.size() < 2) {
		return;
	}

	std::size_t index = this->stack.size() - 2;
	while (index > 0) {
		if (this->stack.at(index).second == Modality::Exclusive) {
			break;
		}
		--index;
	}

	while (index < this->stack.size() - 1) {
		this->stack.at(index).first->cover();
	}
}

void State_Manager::notifyRevealedStates() {
	if (this->stack.empty()) {
		return;
	}

	std::size_t index = this->stack.size() - 1;
	while (index > 0) {
		if (this->stack.at(index).second == Modality::Exclusive) {
			break;
		}
		--index;
	}

	while (index < this->stack.size()) {
		this->stack.at(index).first->reveal();
	}
}