#include "P3P/SelectWheel.hpp"
#include "P3P/Menu.hpp"
#include <JCPPEngine/TextureManager.hpp>

void stopFunctionNext(int pAnimIndex, GameObject* pOwner)
{
	SelectWheel* selectWheel = (SelectWheel*)pOwner;
	selectWheel->animating = false;
	SelectWheel::singletonInstance->DisplayCharacter(SelectWheel::singletonInstance->onSkin);
}

void stopFunctionLast(int pAnimIndex, GameObject* pOwner)
{
	SelectWheel* selectWheel = (SelectWheel*)pOwner;
	selectWheel->animating = false;
	SelectWheel::singletonInstance->DisplayCharacter(SelectWheel::singletonInstance->onSkin);
}

SelectWheel* SelectWheel::singletonInstance = nullptr;

SelectWheel::SelectWheel(std::string pSpritePath, std::string pBackModelPath, std::string pFrontModelPath, std::string pCharacterModelPath, sf::Vector2f pSpritePosition,
	glm::mat4 pModelPosition, void(*pPressPunction)(), std::string NextCharAnim, std::string LastCharAnim, glm::vec3 pModelOffset, std::string pSelectButton, Menu *pMenu) : GameObject()
{
	if (singletonInstance != nullptr)
		return;
	singletonInstance = this;

	_menu = pMenu;
	_pressFunction = pPressPunction;

	//Create skins 
	sf::String SkinOne = "Caracter wheel sel 1.png";
	sf::String SkinTwo = "Caracter wheel sel 2.png";
	sf::String SkinThree = "Caracter wheel sel 3.png";
	sf::String SkinFour = "Caracter wheel sel 4.png";

	//Add skins to vector
	_skins.push_back(SkinOne);
	_skins.push_back(SkinTwo);
	_skins.push_back(SkinThree);
	_skins.push_back(SkinFour);

	//Set max skin size
	_maxSkins = _skins.size();

	//Setup Select Wheel
	offsetVec = glm::vec3(pModelOffset.x, pModelOffset.y, pModelOffset.z);
	animator = new AnimationBehaviour({ NextCharAnim, LastCharAnim });

	backModel = new GameObject(pBackModelPath);
	backModel->setParent(this);
	backModel->setMaterial(new LitMaterial("SelectWheelInner.png"));
	backModel->setBehaviour(animator);
	backModel->scale(2.3f);

	frontModel = new GameObject(pFrontModelPath);
	frontModel->setParent(this);
	frontModel->setMaterial(new LitMaterial("SelectWheelCase.png"));
	frontModel->scale(2.3f);

	_selectButton = new GameObject(pSelectButton);
	_selectButton->setParent(this);
	_selectButton->setMaterial(new LitMaterial("ButtonGreen.png"));
	_selectButton->scale(0.5f);
	_selectButton->translate(glm::vec3(-0.92f, 0.48f, 0.1f));

	//Set transform and translate
	this->setTransform(pModelPosition);
	this->translate(offsetVec);

	//Set sprite texture and position
	sprite = new sf::Sprite(*JCPPEngine::TextureManager::GetTexture(pSpritePath));
	sprite->setPosition(pSpritePosition);

	//Set  skin index
	_menu->characterSkin = _skins[onSkin];
	_menu->characterSkinIndex = onSkin;

	//Sets Position Of Select Light And Skin
	DisplayCharacter(onSkin);
}

SelectWheel::~SelectWheel()
{
	setParent(nullptr);
	singletonInstance = nullptr;
	delete sprite;
	GameObject::~GameObject();
}

void SelectWheel::NextTexture()
{
	if (!_selectLightHidden)
		HideSelectLight();
	//Check Skin "Bounds"
	if (onSkin >= (_maxSkins - 1) && !animating) {
		//Play next texture animation and set onSkin int to 0
		animator->playAnimation(0, false, true, &stopFunctionNext, this);
		animating = true;
		onSkin = 0;
	} else if (onSkin < (_maxSkins-1) && !animating) {
		//Play next texture animation and set onSkin int to next
		animator->playAnimation(0, false, true, &stopFunctionNext, this);
		animating = true;
		onSkin++;
	} 
}

void SelectWheel::LastTexture()
{
	if (!_selectLightHidden)
		HideSelectLight();
	//Check Skin "Bounds"
	if (onSkin <= 0 && !animating) {
		//Play next texture animation and set onSkin int to the max skin
		animator->playAnimation(1, false, true, &stopFunctionLast, this);
		animating = true;
		onSkin= _maxSkins - 1;
	}
	else if (onSkin > 0 && !animating) 
	{
		//Play next texture animation and set onSkin int to last
		animator->playAnimation(1, false, true, &stopFunctionLast, this);
		animating = true;
		onSkin--;
	}
}

void SelectWheel::ShowSelectLight()
{
	//Move the select light into view
	_selectButton->translate(glm::vec3(0, 0, +2));
	_selectLightHidden = false;
}

void SelectWheel::HideSelectLight()
{
	//Move the select light out of view
	_selectButton->translate(glm::vec3(0, 0, -2));
	_selectLightHidden = true;
}

void SelectWheel::SelectTexture()
{
	//Set skin valiables
	_menu->characterSkin = _skins[onSkin];
	_menu->characterSkinIndex = onSkin;
	_selectedSkin = onSkin;
	if (_selectLightHidden)
	{
		//If light is hidden show it and set flag
		ShowSelectLight();
	}
}

void SelectWheel::DisplayCharacter(int pskinNumber)
{
	//Set characters material euqal the skin in vector at index pskinNumber
	backModel->setMaterial(new LitMaterial(_skins[pskinNumber]));

	//If the skin number is equal to selected skin and the select "light" is hidden
	if (!_selectLightHidden)
	{
		//Hide "light" and set flag
		HideSelectLight();
	}
	if (pskinNumber == _selectedSkin && _selectLightHidden) 
	{
		//Show "light" and set flag
		ShowSelectLight();
	}
}