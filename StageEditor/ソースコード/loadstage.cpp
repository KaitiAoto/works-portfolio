//==============================
//
// テクスチャ[texture.cpp]
// Author:kaiti
//
//==============================
#include "loadstage.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "player.h"
#include "savestage.h"
#include "camera.h"
#include "wall.h"
//==================
// コンストラクタ
//==================
CLoadStage::CLoadStage()
{
	m_nCntStage = 0;
	m_nNumModel = 0;
	for (int nCnt = 0; nCnt < MAX_STAGE_MODEL; nCnt++)
	{
		m_pStage[nCnt] = nullptr;
	}
}
//================
// デストラクタ
//================
CLoadStage::~CLoadStage()
{
}
//=======================
// ステージ読み込み
//=======================
void CLoadStage::Load(const char* pFilename)
{
	CSaveStage* pSave = CManager::GetSaveStage();

	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	CCamera* pCamera = CManager::GetCamera();

	int nNumTexture = 0;
	char aTextureName[MAX_STAGE_MODEL][MAX_WORD]; // NUM_STAGE_TEXTURE を適切な定数に
	int nCntTexture = 0;

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	int Index = 0;
	int nCntModel = 0;
	char aModelName[NUM_STAGE_MODEL][MAX_WORD];
	char aPlayer[MAX_WORD];

	int type = 0;
	float degX, degY, degZ;

	int textype = 0;
	D3DXVECTOR3 fieldPos, fieldRot;
	D3DXVECTOR2 fieldBlock, fieldSize;

	char equal[MAX_WORD];

	FILE* pFile;
	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
	{//ファイルが開けたら
		char aString[MAX_WORD];

		while (fscanf(pFile, "%s", aString) != EOF)
		{
			// コメント行のスキップ
			if (aString[0] == '#')
			{
				fgets(aString, MAX_WORD, pFile); // 行末まで読み飛ばす
				continue;
			}

			//fscanf(pFile, "%s", &aString[0]);

			if (strcmp(aString, "SCRIPT") == 0)
			{
				while (fscanf(pFile, "%s", aString) != EOF)
				{
					// コメント行スキップ
					if (aString[0] == '#')
					{
						fgets(aString, MAX_WORD, pFile);
						continue;
					}

					//---------------------------------------------------
					// テクスチャ情報
					//---------------------------------------------------
					// テクスチャ数
					if (strcmp(aString, "NUM_TEXTURE") == 0)
					{
						fscanf(pFile, "%s %d", equal, &nNumTexture);
					}
					// テクスチャファイル名
					else if (strcmp(aString, "TEXTURE_FILENAME") == 0)
					{
						fscanf(pFile, "%s %s", equal, &aTextureName[nCntTexture][0]);

						nCntTexture++;
					}

					//---------------------------------------------------
					// モデル情報
					//---------------------------------------------------
					//モデル数
					else if (strcmp(aString, "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s %d", equal, &m_nNumModel);
					}
					//モデルファイル名
					else if (strcmp(aString, "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s %s", equal, &aModelName[nCntModel][0]);

						nCntModel++;
					}

					//----------------------------------------------------
					// フィールド生成
					//----------------------------------------------------
					else if (strcmp(aString, "FIELDSET") == 0)
					{
						while (fscanf(pFile, "%s", aString) != EOF)
						{
							// コメント行スキップ
							if (aString[0] == '#')
							{
								fgets(aString, MAX_WORD, pFile);
								continue;
							}

							if (strcmp(aString, "TEXTYPE") == 0)
							{
								fscanf(pFile, "%s %d", equal, &textype);
							}
							else if (strcmp(aString, "POS") == 0)
							{
								fscanf(pFile, "%s %f %f %f", equal, &fieldPos.x, &fieldPos.y, &fieldPos.z);
							}
							else if (strcmp(aString, "ROT") == 0)
							{
								float rx, ry, rz;
								fscanf(pFile, "%s %f %f %f", equal, &rx, &ry, &rz);
								fieldRot.x = D3DXToRadian(rx);
								fieldRot.y = D3DXToRadian(ry);
								fieldRot.z = D3DXToRadian(rz);
							}
							else if (strcmp(aString, "SIZE") == 0)
							{
								fscanf(pFile, "%s %f %f", equal, &fieldSize.x, &fieldSize.y);
							}
							else if (strcmp(aString, "END_FIELDSET") == 0)
							{
								// フィールド生成関数を呼び出す（仮）
								CObject3D* pField = CObject3D::Create(&aTextureName[textype][0], fieldPos, fieldRot, fieldSize.x, fieldSize.y, CObject3D::TYPE_FIELD);
								pSave->AddField(pField);
								//CreateField(textype, fieldPos, fieldRot, fieldBlock, fieldSize);
								break;
							}
						}
					}
					//----------------------------------------------------
					// 壁生成
					//----------------------------------------------------
					else if (strcmp(aString, "WALLSET") == 0)
					{
						while (fscanf(pFile, "%s", aString) != EOF)
						{
							// コメント行スキップ
							if (aString[0] == '#')
							{
								fgets(aString, MAX_WORD, pFile);
								continue;
							}

							if (strcmp(aString, "TEXTYPE") == 0)
							{
								fscanf(pFile, "%s %d", equal, &textype);
							}
							else if (strcmp(aString, "POS") == 0)
							{
								fscanf(pFile, "%s %f %f %f", equal, &fieldPos.x, &fieldPos.y, &fieldPos.z);
							}
							else if (strcmp(aString, "ROT") == 0)
							{
								float rx, ry, rz;
								fscanf(pFile, "%s %f %f %f", equal, &rx, &ry, &rz);
								fieldRot.x = D3DXToRadian(rx);
								fieldRot.y = D3DXToRadian(ry);
								fieldRot.z = D3DXToRadian(rz);
							}
							else if (strcmp(aString, "SIZE") == 0)
							{
								fscanf(pFile, "%s %f %f", equal, &fieldSize.x, &fieldSize.y);
							}
							else if (strcmp(aString, "END_WALLSET") == 0)
							{
								CWall::Create(fieldPos, fieldRot, fieldSize.x, fieldSize.y);
								break;
							}
						}
					}

					//----------------------------------------------------
					// モデル配置
					//----------------------------------------------------
					else if (strcmp(aString, "MODELSET") == 0)
					{
						while (fscanf(pFile, "%s", aString) != EOF)
						{
							// コメント行スキップ
							if (aString[0] == '#')
							{
								fgets(aString, MAX_WORD, pFile);
								continue;
							}

							//モデル番号
							if (strcmp(aString, "TYPE") == 0)
							{
								fscanf(pFile, "%s %d", equal, &Index);
							}
							//位置
							else if (strcmp(aString, "POS") == 0)
							{
								fscanf(pFile, "%s %f %f %f", equal, &pos.x, &pos.y, &pos.z);
							}
							//向き
							else if (strcmp(aString, "ROT") == 0)
							{
								fscanf(pFile, "%s %f %f %f", equal, &degX, &degY, &degZ);

								rot.x = degX * (D3DX_PI / 180.0f);
								rot.y = degY * (D3DX_PI / 180.0f);
								rot.z = degZ * (D3DX_PI / 180.0f);
							}
							//種類
							else if (strcmp(aString, "OBJECT") == 0)
							{
								fscanf(pFile, "%s %d", equal, &type);
							}
							else if (strcmp(aString, "END_MODELSET") == 0)
							{
								Create(&aModelName[Index][0], pos, rot, (CStage::TYPE)type, Index);
								m_nCntStage++;
								break;
							}
						}
					}

					//------------------------------------------------------
					// プレイヤー配置
					//------------------------------------------------------
					else if (strcmp(aString, "PLAYERSET") == 0)
					{
						while (fscanf(pFile, "%s", aString) != EOF)
						{
							// コメント行スキップ
							if (aString[0] == '#')
							{
								fgets(aString, MAX_WORD, pFile);
								continue;
							}

							//モデルファイル名
							if (strcmp(aString, "MOTION_FILENAME") == 0)
							{
								fscanf(pFile, "%s %s", equal, &aPlayer[0]);
							}
							//位置
							else if (strcmp(aString, "POS") == 0)
							{
								fscanf(pFile, "%s %f %f %f", equal, &pos.x, &pos.y, &pos.z);
							}
							//向き
							else if (strcmp(aString, "ROT") == 0)
							{
								fscanf(pFile, "%s %f %f %f", equal, &degX, &degY, &degZ);

								rot.x = degX * (D3DX_PI / 180.0f);
								rot.y = degY * (D3DX_PI / 180.0f);
								rot.z = degZ * (D3DX_PI / 180.0f);
							}
							else if (strcmp(aString, "END_PLAYERSET") == 0)
							{
								//プレイヤー情報取得
								CPlayer* pPlayer = CManager::GetPlayer();

								//pPlayer = CPlayer::Create(aPlayer, pos, rot);
								//CManager::SetPlayer(pPlayer);

								break;
							}
						}
					}

					//------------------------------------------------------
					// カメラ配置
					//------------------------------------------------------
					else if (strcmp(aString, "CAMERASET") == 0)
					{
						while (fscanf(pFile, "%s", aString) != EOF)
						{
							// コメント行スキップ
							if (aString[0] == '#')
							{
								fgets(aString, MAX_WORD, pFile);
								continue;
							}

							//視点
							if (strcmp(aString, "POS") == 0)
							{
								fscanf(pFile, "%s %f %f %f", equal, &pos.x, &pos.y, &pos.z);
								pCamera->SetPosV(pos);
							}
							//注視点
							else if (strcmp(aString, "REF") == 0)
							{
								fscanf(pFile, "%s %f %f %f", equal, &pos.x, &pos.y, &pos.z);
								pCamera->SetPosR(pos);
							}
							else if (strcmp(aString, "END_CAMERASET") == 0)
							{
								break;
							}
						}
					}

					else if (strcmp(aString, "END_SCRIPT") == 0)
					{
						break;
					}
				}
			}
		}
		fclose(pFile);//ファイルを閉じる
	}
	else
	{
		// 例外
		return;
	}
}
//=======================
// 全テクスチャ破棄
//=======================
void CLoadStage::Unload(void)
{

}
//==============
// モデル生成
//==============
void CLoadStage::Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CStage::TYPE type,int nIndx)
{
	m_pStage[m_nCntStage] = CStage::Create(pFilename, pos, rot, type, nIndx);
}
