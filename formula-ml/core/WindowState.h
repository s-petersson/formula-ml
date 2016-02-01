#ifndef CORE_WINDOWSTATE
#define CORE_WINDOWSTATE

class WindowState {
public:
	virtual void update(float dt) = 0;
	virtual void render() = 0;
};

#endif