//==============================
//
// プレイヤー[player.cpp]
// Author:kaiti
//
//==============================
#include "modelset.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "objectBillboard.h"
#include "collision.h"
#include "savestage.h"
//==================
// コンストラクタ
//==================
CModelSet::CModelSet(int nPriority):CObject(nPriority)
{
	//値をクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_type = TYPE_MODEL;

	m_nLife = 0;
	m_fSpeed = 0;
	m_nNumModel = 0;
	m_mtxWorld = {};

	for (int nCnt = 0; nCnt < NUM_STAGE_MODEL; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}	
}
//================
// デストラクタ
//================
CModelSet::~CModelSet()
{
}
//===========
// 生成処理
//===========
CModelSet* CModelSet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CModelSet* pPlayer;
	//インスタンス生成
	pPlayer = new CModelSet;
	//初期化
	if (FAILED(pPlayer->Init(pos, rot)))
	{
		delete pPlayer;
		return nullptr;
	}
	pPlayer->SetType(type);
	return pPlayer;
}
//===============
// 初期化処理
//===============
HRESULT CModelSet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//位置・向き設定
	m_pos = pos;
	m_rot = rot;
	m_fSpeed = 0.1f;
	m_nDraw = 0;
	m_ModelType = CStage::TYPE_MODEL;

	for (int nCnt = 0; nCnt < NUM_STAGE_MODEL; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}

	//m_pModelName[NUM_STAGE_MODEL] =
	const char* apFileName[NUM_STAGE_MODEL] =
	{
		"data\\MODEL\\vending_machine00.x",
		"data\\MODEL\\vending_machine01.x",
		"data\\MODEL\\vending_machine02.x",
		"data\\MODEL\\vending_machine03.x",
		"data\\MODEL\\vending_machine04.x",
		"data\\MODEL\\vending_machine05.x",
		"data\\MODEL\\vending_machine06.x",
		"data\\MODEL\\dustbox00.x",
	
		"data\\MODEL\\convenience_store00.x",
		"data\\MODEL\\convenience_store02.x",

		"data\\MODEL\\house00.x",
		"data\\MODEL\\house01.x",
		"data\\MODEL\\mansion00.x",
		"data\\MODEL\\mansion01.x",
		"data\\MODEL\\wall01.x",

		"data\\MODEL\\bluecar.x",
		"data\\MODEL\\vending_machine07.x",
	};
	
	//モデル生成
	for (int nCnt = 0; nCnt < NUM_STAGE_MODEL; nCnt++)
	{
		if (m_apModel[nCnt] == NULL)
		{
			if (apFileName[nCnt] != NULL)
			{
				strcpy(m_pModelName[nCnt], apFileName[nCnt]);

				m_apModel[nCnt] = CModel::Create(m_pModelName[nCnt], m_pos, m_rot);
				m_nNumModel++;
			}
		}
	}

	const char* apFieldTextures[NUM_STAGE_MODEL] =
	{
		"data\\TEXTURE\\field00.jpeg",
		"data\\TEXTURE\\field01.jpg",
		"data\\TEXTURE\\gauge00.jpeg",

		//"data\\TEXTURE\\field01.jpeg",
		//"data\\TEXTURE\\field02.jpeg"
	};

	// 保存しておく
	for (int i = 0; i < NUM_STAGE_MODEL; i++)
	{
		if (apFieldTextures[i] != NULL)
		{
			strcpy(m_apFieldTexture[i], apFieldTextures[i]);
			m_nNumTex++;
		}
	}
	m_nDrawField = 0;

	fX = 100.0f;
	fZ = 100.0f;

	m_pField = CObject3D::Create(m_apFieldTexture[m_nDrawField], m_pos, m_rot, fX, fZ, CObject3D::TYPE_FIELD);
	m_pWall = CObject3D::Create(m_apFieldTexture[m_nDrawField], m_pos, m_rot, fX, fZ, CObject3D::TYPE_WALL);
	SetObjType(TYPE_PLAYER);//オブジェクトのタイプ
	return S_OK;
}
//============
// 終了処理
//============
void CModelSet::Uninit(void)
{
	//モデル全終了
	for (int nCnt = 0; nCnt < NUM_STAGE_MODEL; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{
			m_apModel[nCnt]->Uninit();
			delete m_apModel[nCnt];
			m_apModel[nCnt] = nullptr;
		}
	}

	CObject::Release();
}
//============
// 更新処理
//============
void CModelSet::Update(void)
{
	//行動
	Action();

	//移動
	Move();

	if (m_type == TYPE_FIELD)
	{
		m_pField->SetDraw(true);
	}
	else
	{
		m_pField->SetDraw(false);
	}

	if (m_type == TYPE_WALL)
	{
		m_pWall->SetDraw(true);
	}
	else
	{
		m_pWall->SetDraw(false);
	}

	//CEffect::Create(m_pos, m_rot, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DCOLOR_RGBA(255, 255, 255, 255), 60, EFFECT_SIZE);

}
//============
// 描画処理
//============
void CModelSet::Draw(void)
{
	{
		CDebugProc* pDegub = CManager::GetDebug();
		pDegub->Print("\n<<操作>>");
		pDegub->Print("書き出し：F9");
		pDegub->Print("移動：W/A/S/D/V/B");
		pDegub->Print("角度：R/T");
		pDegub->Print("移動速度：LSHIFT/LCTRL");
		pDegub->Print("モデル切り替え：1/2");
		pDegub->Print("オブジェクトタイプ変更：3/4");
		pDegub->Print("配置タイプ変更：F3");
		pDegub->Print("生成：Ｆ\n");

		if (m_type == TYPE_MODEL)
		{
			pDegub->Print("配置の種類： モデル");
		}
		else if (m_type == TYPE_FIELD)
		{
			pDegub->Print("拡大縮小：U/J/I/K");
			pDegub->Print("配置の種類： フィールド");
			pDegub->Print("現在サイズ： %.1f x %.1f", fX, fZ);
		}
		else if (m_type == TYPE_WALL)
		{
			pDegub->Print("拡大縮小：U/J/I/K");
			pDegub->Print("配置の種類： 壁");
			pDegub->Print("現在サイズ： %.1f x %.1f", fX, fZ);
		}

		pDegub->Print("位置：%.1f,%.1f,%.1f", m_pos.x, m_pos.y, m_pos.z);
		pDegub->Print("角度：%.1f,%.1f,%.1f", m_rot.x, m_rot.y, m_rot.z);
		pDegub->Print("移動速度： %.2f", m_fSpeed);
		if (m_ModelType == CStage::TYPE_MODEL)
		{
			pDegub->Print("オブジェクトの種類： モデル");
		}
		else if (m_ModelType == CStage::TYPE_GIMMICK)
		{
			pDegub->Print("オブジェクトの種類： ギミック");
		}

		pDegub->Print("モデルの種類： %d", m_nDraw);
	}
	
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反転
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	if (m_type == TYPE_MODEL)
	{
		//モデル全描画
		if (m_apModel[m_nDraw] != NULL)
		{
			m_apModel[m_nDraw]->Draw();
		}
	}
	else if (m_type == TYPE_FIELD)
	{
		m_pField->Set(m_apFieldTexture[m_nDrawField], m_pos, m_rot, fX, fZ);
		m_pField->Draw();
	}
	else if (m_type == TYPE_WALL)
	{
		m_pWall->Set(m_apFieldTexture[m_nDrawField], m_pos, m_rot, fX, fZ);
		m_pWall->Draw();
	}

}
//===========
// 移動処理
//===========
void CModelSet::Move(void)
{
	//キー取得
	CInputKey* pInputKey = CManager::GetInputKey();

	CCamera* pCamera = CManager::GetCamera();
	D3DXVECTOR3 CameraRot = pCamera->GetRot();

	//キー移動
	if (pInputKey->GetPress(DIK_A) == true)
	{//左
		m_move.x += cosf(CameraRot.y - D3DX_PI) * m_fSpeed;
		m_move.z -= sinf(CameraRot.y - D3DX_PI) * m_fSpeed;
	}
	if (pInputKey->GetPress(DIK_D) == true)
	{//右
		m_move.x -= cosf(CameraRot.y - D3DX_PI) * m_fSpeed;
		m_move.z += sinf(CameraRot.y - D3DX_PI) * m_fSpeed;
	}
	if (pInputKey->GetPress(DIK_W) == true)
	{//前
		m_move.x -= sinf(CameraRot.y - D3DX_PI) * m_fSpeed;
		m_move.z -= cosf(CameraRot.y - D3DX_PI) * m_fSpeed;
	}
	if (pInputKey->GetPress(DIK_S) == true)
	{//後
		m_move.x += sinf(CameraRot.y - D3DX_PI) * m_fSpeed;
		m_move.z += cosf(CameraRot.y - D3DX_PI) * m_fSpeed;
	}
	if (pInputKey->GetPress(DIK_V) == true)
	{//前
		m_move.y += m_fSpeed;
	}
	if (pInputKey->GetPress(DIK_B) == true)
	{//後
		m_move.y -= m_fSpeed;
	}

	if (pInputKey->GetPress(DIK_R) == true)
	{
		m_rotDest.y += 0.01f;
	}
	if (pInputKey->GetTrigger(DIK_T) == true)
	{
		m_rotDest.y += D3DX_PI / 4.0f;
	}

	if (pInputKey->GetPress(DIK_0) == true)
	{
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}


	//角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}

	m_rot += (m_rotDest - m_rot) * 0.5f;


	//前回の位置保存
	m_posOld = m_pos;
	
	//位置更新
	m_pos += m_move;

	//当たり判定
	Collision();
		
	//移動量初期化
	m_move.x = 0.0f;
	m_move.y = 0.0f;
	m_move.z = 0.0f;
}
//==================
// アクション処理
//==================
void CModelSet::Action(void)
{
	CInputKey* pInputKey;
	//キー取得
	pInputKey = CManager::GetInputKey();

	CSaveStage* pSave = CManager::GetSaveStage();

	//生成
	if (pInputKey->GetTrigger(DIK_F) == true)
	{
		if (m_type == TYPE_MODEL)
		{
			CStage* pStage = CStage::Create(m_pModelName[m_nDraw], m_pos, m_rot, m_ModelType, m_nDraw);
		}
		else if (m_type == TYPE_FIELD)
		{
			CObject3D* pObj3d = CObject3D::Create(m_apFieldTexture[m_nDrawField], m_pos, m_rot, fX, fZ, CObject3D::TYPE_FIELD);
			pSave->AddField(pObj3d);
		}
		else if (m_type == TYPE_WALL)
		{
			CObject3D* pObj3d = CObject3D::Create(m_apFieldTexture[m_nDrawField], m_pos, m_rot, fX, fZ, CObject3D::TYPE_WALL);
			pSave->AddWall(pObj3d);

		}
	}

	if (m_type == TYPE_MODEL)
	{
		//
		if (pInputKey->GetTrigger(DIK_1))
		{
			m_nDraw = (m_nDraw - 1 + m_nNumModel) % m_nNumModel;
		}
		if (pInputKey->GetTrigger(DIK_2))
		{
			m_nDraw = (m_nDraw + 1) % m_nNumModel;
		}

		if (pInputKey->GetTrigger(DIK_3))
		{
			m_ModelType = static_cast<CStage::TYPE>((m_ModelType + 1) % CStage::TYPE_MAX);
		}
		if (pInputKey->GetTrigger(DIK_4))
		{
			m_ModelType = static_cast<CStage::TYPE>((m_ModelType - 1 + CStage::TYPE_MAX) % CStage::TYPE_MAX);
		}
	}
	else if (m_type == TYPE_FIELD || m_type == TYPE_WALL)
	{
		if (pInputKey->GetTrigger(DIK_1))
		{
			m_nDrawField = (m_nDrawField - 1 + NUM_STAGE_MODEL) % NUM_STAGE_MODEL;
		}
		if (pInputKey->GetTrigger(DIK_2))
		{
			m_nDrawField = (m_nDrawField + 1) % NUM_STAGE_MODEL;
		}

		if (pInputKey->GetPress(DIK_U)) { fX += 1.0f; }
		if (pInputKey->GetPress(DIK_J)) { fX -= 1.0f; }
		if (pInputKey->GetPress(DIK_I)) { fZ += 1.0f; }
		if (pInputKey->GetPress(DIK_K)) { fZ -= 1.0f; }

		// 縮小制限をかける（最小10.0fなど）
		const float MIN_FIELD_SIZE = 10.0f;
		if (fX < MIN_FIELD_SIZE) fX = MIN_FIELD_SIZE;
		if (fZ < MIN_FIELD_SIZE) fZ = MIN_FIELD_SIZE;
	}

	//配置の種類
	if (pInputKey->GetTrigger(DIK_F3))
	{
		m_type = static_cast<TYPE>((m_type + 1) % TYPE_MAX);
	}

	// Shift でスピードアップ（最大 1.0f まで）
	if (pInputKey->GetPress(DIK_LSHIFT))
	{
		m_fSpeed += 0.01f;
		if (m_fSpeed > 1.5f)
		{
			m_fSpeed = 1.5f;
		}
	}
	// Ctrl でスピードダウン（最小 0.05f まで）
	if (pInputKey->GetPress(DIK_LCONTROL))
	{
		m_fSpeed -= 0.01f;
		if (m_fSpeed < 0.05f)
		{
			m_fSpeed = 0.05f;
		}
	}
	if (pInputKey->GetTrigger(DIK_0))
	{
		m_pos= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	//保存
	if (pInputKey->GetTrigger(DIK_F9))
	{
		pSave->Save("data\\STAGE\\model01.txt");
	}
#ifdef _DEBUG
	//テスト

#endif
}
const char* CModelSet::GetModelFilename(int index)
{
	if (index >= 0 && index < NUM_STAGE_MODEL)
	{
		return m_pModelName[index];
	}
	return nullptr;
}
const char* CModelSet::GetTexFilename(int index)
{
	if (index >= 0 && index < NUM_STAGE_MODEL)
	{
		return m_apFieldTexture[index];
	}
	return nullptr;

	return nullptr;
}
//====================
// 当たり判定
//====================
bool CModelSet::Collision(void)
{
	CDebugProc* pDegub = CManager::GetDebug();

	//当たり判定のポインタ取得
	CCollision* pColl = CManager::GetColl();

	//return bColl;
	return false;
}