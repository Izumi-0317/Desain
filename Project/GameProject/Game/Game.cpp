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

bool Game::m_cameraMode = true;
const float ROOM_SIZE = 16.5f;

Game::Game()
	:Base(eScene)
	, m_delayTime(30)
	, m_isComplete(false)
	, m_isGameOver(false)
	, m_isCaptured(false){
	Base::Add(new Player(CVector3D(0, 0, 0)));
	Base::Add(new Paladin(CVector3D(0, 0, -ROOM_SIZE)));
	Base::Add(new Boss(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE * 4)));
	Base::Add(new UIAmmo());
	Base::Add(new UIHP(ePlayer));
	Base::Add(new UIPotion());

	Base::Add(new Room(CVector3D(0, 0, 0), 0, Room::RoomType::R2I));
	Base::Add(new Room(CVector3D(0, 0, -ROOM_SIZE), 180, Room::RoomType::R3));
	Base::Add(new Room(CVector3D(-ROOM_SIZE, 0, -ROOM_SIZE), -90, Room::RoomType::R1));
	Base::Add(new Room(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE), 90, Room::RoomType::R2L));
	Base::Add(new Room(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE * 2), 0, Room::RoomType::R2I));
	Base::Add(new Room(CVector3D(ROOM_SIZE, 0, -ROOM_SIZE * 4 + 7.5f), 180, Room::RoomType::RBoss));

	Base::Add(new Door(CVector3D(1, 0, 8.25f), 90, true));
	Base::Add(new Door(CVector3D(1, 0, -8.25f), 90));
	Base::Add(new Door(CVector3D(-8.2f, 0, -15.4f)));
	Base::Add(new Door(CVector3D(8.2f, 0, -15.4f)));
	Base::Add(new Door(CVector3D(17.6f, 0, -24.75f), 90));
	Base::Add(new Door(CVector3D(17.5f, 0, -41.25f), 90));
	Base::Add(new Door(CVector3D(17.6f, 0, -75.5f), 90, true));

	Base::Add(new Chest(CVector3D(-ROOM_SIZE, 0, -ROOM_SIZE), 0, 2));

	Base::Add(new Camera());

	m_completeTF = new CTextureFrame(1920.0f, 1080.0f, CVector4D(1.0f, 1.0f, 1.0f, 1.0f));
}

void Game::Update(){
	if (m_isComplete || m_isGameOver) {
		if (m_delayTime-- <= 0) {
			if (m_isComplete && !m_isCaptured) {
				m_completeTF->BeginDraw();
				glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				Base::RenderALL();
				m_completeTF->EndDraw();
				m_isCaptured = true;
			}
			KillALL();
			(m_isComplete) ?
				Base::Add(new Complete(*m_completeTF->GetTexture())) :
				Base::Add(new GameOver());
			delete m_completeTF;
		}
	}
}
