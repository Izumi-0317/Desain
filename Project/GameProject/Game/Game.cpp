#include "Game.h"
#include "Camera.h"
#include "Game/Obj/Gimmick/Chest.h"
#include "Game/Obj/Gimmick/Door.h"
#include "Game/Obj/Room.h"
#include "Game/Obj/Chara/Player.h"
#include "Game/Obj/Chara/Paladin.h"
#include "UI/AmmoUI.h"
#include "UI/HPUI.h"
#include "UI/PotionUI.h"

bool Game::m_cameraMode = true;
const float ROOM_SIZE = 16.5f;

Game::Game()
	:Base(eScene){
	Base::Add(new Player(CVector3D(0, 0, 0)));
	//Base::Add(new Paladin(CVector3D(0, 0, -16.5f)));
	Base::Add(new AmmoUI());
	Base::Add(new HPUI());
	Base::Add(new PotionUI());

	Base::Add(new Room(CVector3D(0, 0, 0), 0, 1));
	Base::Add(new Room(CVector3D(0, 0, -ROOM_SIZE), 180, 3));
	Base::Add(new Room(CVector3D(-ROOM_SIZE, 0, -ROOM_SIZE), -90, 1));
	Base::Add(new Room(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE), 90, 2, false));
	Base::Add(new Room(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE * 2), 0, 2));

	Base::Add(new Door(CVector3D(1, 0, -8.25f), 90));
	Base::Add(new Door(CVector3D(-8.2f, 0, -15.4f)));
	Base::Add(new Door(CVector3D(8.2f, 0, -15.4f)));
	Base::Add(new Door(CVector3D(17.6f, 0, -24.75f), 90));
	Base::Add(new Door(CVector3D(17.5f, 0, -41.25f), 90));

	Base::Add(new Chest(CVector3D(-ROOM_SIZE, 0, -ROOM_SIZE), 0, 2));

	Base::Add(new Camera());
}

void Game::Update()
{
}
