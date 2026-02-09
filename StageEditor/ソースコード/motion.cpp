//==============================
//
//  モーション処理[motion.cpp]
//  Author:kaiti
//
//==============================
#include "motion.h"
#include "renderer.h"
#include "manager.h"
//================
// コンストラクタ
//================
CMotion::CMotion(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_aMotionInfo[nCnt] = {};
	}
	m_nNumMotion = 0;
	m_type = TYPE_NEUTRAL;
	m_bLoop = false;
	m_nKey = 0;
	m_nCntMotion = 0;
}
//================
// デストラクタ
//================
CMotion::~CMotion()
{
}
//========
// 更新
//========
void CMotion::Update(const int nMaxPart,CModel ** pModel)
{
	//全モデルの更新
	for (int nCnt = 0; nCnt < nMaxPart; nCnt++)
	{
		//次のキー
		int nNext = (m_nKey + 1) % m_aMotionInfo[m_type].nNumKey;
		//キー数を戻す
		if (m_nKey >= m_aMotionInfo[m_type].nNumKey || nNext >= m_aMotionInfo[m_type].nNumKey)
		{
			m_nKey = 0;
		}

		D3DXVECTOR3 posMotion, rotMotion, posSabun, rotSabun;

		//差分計算
		//pos
		posSabun.x = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fPosX - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosX;
		posSabun.y = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fPosY - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosY;
		posSabun.z = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fPosZ - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosZ;
		//rot
		rotSabun.x = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fRotX - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotX;
		rotSabun.y = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fRotY - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotY;
		rotSabun.z = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fRotZ - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotZ;

		float fDis = (float)m_nCntMotion / m_aMotionInfo[m_type].aKeyInfo[m_nKey].nFrame;

		//パーツの位置・向きを算出
		posMotion.x = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosX + posSabun.x * fDis);
		posMotion.y = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosY + posSabun.y * fDis);
		posMotion.z = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosZ + posSabun.z * fDis);

		rotMotion.x = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotX + rotSabun.x * fDis);
		rotMotion.y = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotY + rotSabun.y * fDis);
		rotMotion.z = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotZ + rotSabun.z * fDis);

		//pos.rot更新
		pModel[nCnt]->SetPos(posMotion);
		pModel[nCnt]->SetRot(rotMotion);
	}

	m_nCntMotion++;

	if (m_nCntMotion >= m_aMotionInfo[m_type].aKeyInfo[m_nKey].nFrame)
	{
		m_nCntMotion = 0;

		m_nKey++;

		if (m_nCntMotion >= m_aMotionInfo[m_type].aKeyInfo[m_aMotionInfo[m_type].nNumKey].nFrame)
		{//モーションをニュートラルに
			m_type = TYPE_NEUTRAL;
		}
	}
}
//=====================
// モーション読み込み
//=====================
CMotion* CMotion::Load(const char* pFilename, CModel** model)
{
	// インスタンス生成
	CMotion* pMotion = new CMotion;
	//デバイスの取得
	CRenderer* pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	D3DXVECTOR3 pos[NUM_MODEL];
	D3DXVECTOR3 rot[NUM_MODEL];

	for (int n = 0; n < NUM_MODEL; n++)
	{
		pos[n] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot[n] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	int nNumModel = 0;
	int Index = 0;
	int nLoop;
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntRot = 0;
	int nCntPos = 0;

	FILE* pFile;
	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
	{//ファイルが開けたら
		char aString[MAX_WORD];

		fscanf(pFile, "%s", &aString[0]);

		if (strcmp(aString, "SCRIPT") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &aString[0]);

				//モーション
				if (strcmp(aString, "MOTIONSET") == 0)
				{
					while (1)
					{
						fscanf(pFile, "%s", &aString[0]);
						//ループ
						if (strcmp(aString, "LOOP") == 0)
						{
							fscanf(pFile, "%d", &nLoop);
						}
						//キー数
						else if (strcmp(aString, "NUM_KEY") == 0)
						{
							int nNumKey = 0;
							fscanf(pFile, "%d", &nNumKey);
							pMotion->m_aMotionInfo[nCntMotion].nNumKey = nNumKey;
						}
						//キー情報
						else if (strcmp(aString, "KEYSET") == 0)
						{
							while (1)
							{
								fscanf(pFile, "%s", &aString[0]);
								//フレーム数
								if (strcmp(aString, "FRAME") == 0)
								{
									fscanf(pFile, "%d", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
								}
								//キー
								else if (strcmp(aString, "KEY") == 0)
								{
									while (1)
									{
										fscanf(pFile, "%s", &aString[0]);
										//位置
										if (strcmp(aString, "POS") == 0)
										{
											fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPos].fPosX);
											fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPos].fPosY);
											fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPos].fPosZ);

											nCntPos++;
										}
										//向き
										else if (strcmp(aString, "ROT") == 0)
										{
											fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRot].fRotX);
											fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRot].fRotY);
											fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntRot].fRotZ);

											nCntRot++;
										}
										else if (strcmp(aString, "END_KEY") == 0)
										{
											break;
										}
									}
								}
								else if (strcmp(aString, "END_KEYSET") == 0)
								{
									nCntKey++;
									nCntRot = 0;
									nCntPos = 0;
									break;
								}
							}
						}
						else if (strcmp(aString, "END_MOTIONSET") == 0)
						{
							nCntMotion++;
							nCntKey = 0;
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
		fclose(pFile);//ファイルを閉じる
	}
	else
	{
		// 例外
		return nullptr;
	}

	return pMotion;
}
