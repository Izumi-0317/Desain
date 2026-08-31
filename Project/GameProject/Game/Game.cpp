#include "Game.h"
#include "Camera.h"
#include "Game/Obj/Chara/Boss.h"
#include "Game/Obj/Chara/Player.h"
#include "Game/Obj/Chara/Paladin.h"
#include "Game/Obj/Gimmick/Chest.h"
#include "Game/Obj/Gimmick/Door.h"
#include "Game/Obj/Room.h"
#include "Title/Complete.h"
#include "Title/GameOver.h"
#include "UI/UIAmmo.h"
#include "UI/UIHP.h"
#include "UI/UIPotion.h"
#include "UI/UIPrompts.h"
#include "UI/UIScope.h"
#include "UI/UIVignette.h"

bool Game::m_cameraMode = true;
const float ROOM_SIZE = 16.5f;

Game::Game()
	: ObjectBase(ObjectType::eScene)
	, m_delayTime(25)
	, m_isComplete(false)
	, m_isGameOver(false){
	new Player(CVector3D(0, 0, 0));
	new Paladin(CVector3D(0, 0, -ROOM_SIZE));
	new Boss(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE * 4));
	new UIAmmo();
	new UIHP(ObjectType::ePlayer);
	new UIPotion();
	new UIPrompts();
	new UIScope();
	new UIVignette();

	new Room(CVector3D(0, 0, 0), 0, Room::RoomType::R2I);
	new Room(CVector3D(0, 0, -ROOM_SIZE), 180, Room::RoomType::R3);
	new Room(CVector3D(-ROOM_SIZE, 0, -ROOM_SIZE), -90, Room::RoomType::R1);
	new Room(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE), 90, Room::RoomType::R2L);
	new Room(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE * 2), 0, Room::RoomType::R2I);
	new Room(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE * 4 + 7.5f), 180, Room::RoomType::RBoss);

	new Door(CVector3D(1, 0, 8.25f), 90, true);
	new Door(CVector3D(1, 0, -8.25f), 90);
	new Door(CVector3D(-8.2f, 0, -15.4f));
	new Door(CVector3D(8.2f, 0, -15.4f));
	new Door(CVector3D(17.6f, 0, -24.75f), 90);
	new Door(CVector3D(17.5f, 0, -41.25f), 90);
	new Door(CVector3D(17.6f, 0, -75.5f), 90, true);

	new Chest(CVector3D(-ROOM_SIZE, 0, -ROOM_SIZE), 0, 2);
	new Chest(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE), 180, 1);

	new Camera();
}

void Game::Update(){
	if (m_isComplete || m_isGameOver) {
		if (m_delayTime-- <= 0) {
			if (m_isComplete) {
				new Complete();
			}
			else {
				new GameOver();
			}
			SetKill();
		}
	}
}
