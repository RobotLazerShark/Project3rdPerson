#include <P3P/objects/Socket.hpp>
#include <P3P/Level.hpp>

#define _USE_MATH_DEFINES
#include <math.h>

Socket::Socket(int pX, int pZ, int pOrientation) : GameObject()
{
	//Set up model
	_model = new GameObject("cube_flat.obj");
	_model->translate(glm::vec3(0, 0.5f, 0));
	_model->setMaterial(offMaterial);
	_model->setParent(this);
	translate(glm::vec3(pX * Level::TILESIZE, 0, pZ * Level::TILESIZE));

	//save orientation
	_orientation = pOrientation;
	switch (_orientation)
	{
	case 2:
		_model->rotate(M_PI, glm::vec3(0, 1, 0));
		break;
	case 3:
		_model->rotate(-M_PI / 2, glm::vec3(0, 1, 0));
		break;
	case 4:
		_model->rotate(M_PI / 2, glm::vec3(0, 1, 0));
		break;
	}

	//save array position
	_currentTile[0] = pX;
	_currentTile[1] = pZ;
}

void Socket::update(float pStep, bool pUpdateWorldTransform)
{
	GameObject::update(pStep, pUpdateWorldTransform);
	
	//check if plug connected
	Plug * plug;
	switch (_orientation)
	{
	case 1:
		if (_currentTile[1] + 1 < Level::map->height)
		{
			plug = dynamic_cast <Plug*> ((GameObject*)Level::map->objectTiles[_currentTile[0]][_currentTile[1] + 1]);
		}
		break;
	case 2:
		if (_currentTile[1] - 1 >= 0)
		{
			plug = dynamic_cast <Plug*> ((GameObject*)Level::map->objectTiles[_currentTile[0]][_currentTile[1] - 1]);
		}
		break;
	case 3:
		if (_currentTile[0] - 1 >= 0)
		{
			plug = dynamic_cast <Plug*> ((GameObject*)Level::map->objectTiles[_currentTile[0] - 1][_currentTile[1]]);
		}
		break;
	case 4:
		if (_currentTile[0] + 1 < Level::map->width)
		{
			plug = dynamic_cast <Plug*> ((GameObject*)Level::map->objectTiles[_currentTile[0] + 1][_currentTile[1]]);
		}
		break;
	}

	//update state
	if (plug != nullptr && plug->_orientation == _orientation)
	{
		if (!activated)
		{
			plug->plugged = true;
			activated = true;
			if (target != nullptr)
			{
				target->setActive(true);
			}
			_model->setMaterial(onMaterial);
		}
	}
}