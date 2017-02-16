#ifndef MGEDEMO_H
#define MGEDEMO_H


//Include files
#include <glm.hpp>
#include <iostream>
#include <string>
#include <mge/core/AbstractGame.hpp>

class Level;

class MGEDemo: public AbstractGame
{
	//PUBLIC FUNCTIONS
	public:
		MGEDemo();
		virtual ~MGEDemo();
		virtual void initialize();
	protected:
		virtual void _initializeScene();
	private:
		Level* _game = nullptr;
		MGEDemo(const MGEDemo&);
		MGEDemo& operator=(const MGEDemo&);
};

#endif // MGEDEMO_H
