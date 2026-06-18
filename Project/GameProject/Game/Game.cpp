#include "Game.h"
#include "Camera.h"
#include "Field.h"
#include "Player.h"
#include "Paladin.h"

bool Game::m_cameraMode = true;

Game::Game()
	:Base(eScene){
	Base::Add(new Player(CVector3D(0, 0, 0)));
	Base::Add(new Paladin(CVector3D(0, 0, 0)));

	Base::Add(new Field());
	Base::Add(new Camera());
}

void Game::Update()
{
}
