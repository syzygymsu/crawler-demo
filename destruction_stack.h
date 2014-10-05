#ifndef DESTRUCTIONSTACK_H_
#define	DESTRUCTIONSTACK_H_

#include <stack>
#include <functional>


// Класс для автоматического освобождения ресурсов.
// Полезен для освобождения иерархических ресурсов, выделенных в C-стиле.
// Пример:
//   DestructionStack destructors;
//   void *p1 = malloc(...);
//   if(!p1) return;
//   destructors.push([p1](){ free(p1); });
//   void *p2 = malloc(...);
//   if(!p2) return;
//   destructors.push([p2](){ free(p2); });
class DestructionStack {
public:
	// Добавляет деструктор в стек
	void push(std::function<void()> destructor);
	
	// Вызывает все деструкторы в обратном порядке
	~DestructionStack();
private:
	// Стек деструкторов
	std::stack<std::function<void()> > destructors_;
};


#endif	/* DESTRUCTIONSTACK_H_ */
