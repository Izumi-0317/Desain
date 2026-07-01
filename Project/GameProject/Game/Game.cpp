#include "Game.h"
#include "Camera.h"
#include "Game/Obj/Field.h"
#include "Game/Obj/Chara/Player.h"
#include "Game/Obj/Chara/Paladin.h"
#include "UI/AmmoUI.h"

bool Game::m_cameraMode = true;

Game::Game()
	:Base(eScene){
	Base::Add(new Player(CVector3D(0, 0, 0)));
	Base::Add(new Paladin(CVector3D(0, 0, 0)));
	Base::Add(new AmmoUI());

	Base::Add(new Field());
	Base::Add(new Camera());
}

void Game::Update()
{
}
