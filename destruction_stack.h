#ifndef DESTRUCTIONSTACK_H
#define	DESTRUCTIONSTACK_H

#include <stack>
#include <functional>

class DestructionStack {
public:
	void push(std::function<void()> destructor);
	
	~DestructionStack();
	
private:
	std::stack<std::function<void()> > destructors;
};

#endif	/* DESTRUCTIONSTACK_H */
