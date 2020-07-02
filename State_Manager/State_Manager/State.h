#pragma once

class State
{
	const bool popup = false;
public:
	virtual ~State() {};
	virtual void exit() {};
	virtual void enter() {};
	virtual void cover() {};
	virtual void reveal() {};
	virtual void update(float elapsedTime) {};
	virtual void draw(float elapsedFrameTime) {};
};

