#include "Gun.h"
#include "Base/ObjectManager.h"
#include "Game/Camera.h"
#include "Game/Obj/Chara/Player.h"

namespace {
	constexpr int MAX_AMMO = 30;	//最大装填数
}

Gun::Gun(std::string name)
	: WeaponBase(ObjectType::eGun)
	, m_loadedAmmo(MAX_AMMO){
	m_weapon = COPY_RESOURCE(name, CModelObj);
	m_scope = COPY_RESOURCE("Scope", CModelObj);
}

void Gun::UpdateGun() {
	if (Player* p = ObjectManager::FindObject<Player>(ObjectType::ePlayer)) {
		if (Camera* c = ObjectManager::FindObject<Camera>(ObjectType::eCamera)) {
			m_pos = CVector3D(5.92f, 11.66f, 0.0f);
			//銃行列
			switch (p->GetState()) {
			case p->SAiming:
				m_weaponMat = c->GetMatrix()
					* CMatrix::MTranselate(0.0f, -0.075f, -0.01f)
					* CMatrix::MRotation(CVector3D(DtoR(180.2f), DtoR(0), DtoR(180)))
					* CMatrix::MScale(1, 1, 1);
				/* p->GetModel()->GetFrameMatrix(7)
					* CMatrix::MTranselate(p->GetModel()->GetFrameMatrix(7).GetPosition().GetNormalize().x,
						3.0f,
						p->GetModel()->GetFrameMatrix(7).GetPosition().GetNormalize().z)
					* CMatrix::MTranselate(p->GetModel()->GetFrameMatrix(7).GetPosition() + CVector3D(0,3,0))
					* CMatrix::MTranselate(m_pos + CVector3D(-5.92f,-3.0f,20.0f))*/
				break;
			default:
				/*m_weaponMat = p->GetModel()->GetFrameMatrix(36)
					* CMatrix::MTranselate(m_pos)
					* CMatrix::MRotation(DtoR(145), DtoR(110), DtoR(55))
					* CMatrix::MScale(60.0f, 60.0f, 60.0f);*/
				//カメラの姿勢行列
				CMatrix cam_rot = CMatrix::MRotation(c->m_rot);
				//手の行列
				CMatrix hand = p->GetModel()->GetFrameMatrix(36);
				//回転行列のみに（平行移動0,スケール1倍に）
				hand.NormalizeRot();
				//カメラ行列と手の行列の差分の行列
				CMatrix diff_rot = hand.GetInverse() * cam_rot;
				//リロードのときのみ固定
				CMatrix diffOrReloadRot = (p->GetState() == p->SReloaded) ?
					CMatrix::MRotation(DtoR(143.992157), DtoR(-73.678619), DtoR(129.535187)) :
					diff_rot;
				//オイラー角を取得
				CVector3D elur = diff_rot.GetEuler(CMatrix::eZYX);
				//現在の調整角度を表示
				//if (PUSH(CInput::eButton1))
				//	printf("%f %f %f", RtoD(elur.x), RtoD(elur.y), RtoD(elur.z));
				//手のボーンからカメラ方向への差分回転行列をかけて、銃をカメラへ真っすぐ向ける
				m_weaponMat = p->GetModel()->GetFrameMatrix(36)
					* CMatrix::MTranselate(m_pos)
					* diffOrReloadRot
					* CMatrix::MRotation(DtoR(0), DtoR(180), DtoR(0))
					* CMatrix::MScale(60.0f, 60.0f, 60.0f);
				break;
			}
		}
	}
	//スコープ行列
	m_scopeMat = m_weaponMat
		* CMatrix::MTranselate(0, 0.075f, 0)
		* CMatrix::MScale(1, 1, 1);
}

void Gun::Render(){
	m_weapon.Render(m_weaponMat);
	m_scope.Render(m_scopeMat);
}

void Gun::Reloaded(){
	//MAX_AMMOと同量になるように装填する
	m_loadedAmmo += MAX_AMMO - m_loadedAmmo;
	SOUND("Reloaded")->Play3D(m_weaponMat.GetPosition(), CVector3D(1, 1, 1));
}
