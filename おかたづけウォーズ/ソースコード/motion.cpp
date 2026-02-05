//===============================================
//
//  モーション処理[motion.cpp]
//  Author : Kaiti Aoto
//
//===============================================

// インクルード
#include "motion.h"
#include "renderer.h"
#include "manager.h"

//===============================================
// コンストラクタ
//===============================================
CMotion::CMotion(void)
{
	// 値をクリア
	for (int nCnt = 0; nCnt < MAX_MOTION; nCnt++)
	{
		m_aMotionInfo[nCnt] = {};
	}
	m_nNumMotion = 0;
	m_type = 0;
	m_bLoop = false;
	m_nKey = 0;
	m_nCntMotion = 0;
	m_typeFrom = 0;
	m_typeTo = 0;
	m_bBlend = false;
	m_nBlendFrame = 0;
	m_fBlendRate = 0.0f;
	m_nKeyFrom = 0;
	m_nKeyTo = 0;
	m_nCntFrom = 0;
	m_nCntTo = 0;
}
//===============================================
// デストラクタ
//===============================================
CMotion::~CMotion()
{
}
//===============================================
// モーションブレンド開始設定
//===============================================
void CMotion::StartBlend(int to, int nBlendFrame)
{
	// 値をクリア
	m_bBlend = true;
	m_typeFrom = m_type;
	m_typeTo = to;
	m_nBlendFrame = nBlendFrame;	
	m_fBlendRate = 0.0f;
	m_nKeyFrom = 0;
	m_nKeyTo = 0;
	m_nCntFrom = 0;
	m_nCntTo = 0;
}
//===============================================
// 更新
//===============================================
void CMotion::Update(const int nMaxPart,CModel ** pModel)
{
	// モーションブレンドするなら
	if (m_bBlend == true)
	{
		for (int nCnt = 0; nCnt < nMaxPart; nCnt++)
		{
			// 補間用変数
			D3DXVECTOR3 posA, rotA, posB, rotB, posBlend, rotBlend;
			// 現在のモーション
			int nNextFrom = (m_nKeyFrom + 1) % m_aMotionInfo[m_typeFrom].nNumKey;
			float fDisFrom = (float)m_nCntFrom / m_aMotionInfo[m_typeFrom].aKeyInfo[m_nKeyFrom].nFrame;
			posA = LerpKey(m_typeFrom, nCnt, m_nKeyFrom, nNextFrom, fDisFrom, true);
			rotA = LerpKey(m_typeFrom, nCnt, m_nKeyFrom, nNextFrom, fDisFrom, false);

			// ブレンド先のモーション
			int nNextTo = (m_nKeyTo + 1) % m_aMotionInfo[m_typeTo].nNumKey;
			float fDisTo = (float)m_nCntTo / m_aMotionInfo[m_typeTo].aKeyInfo[m_nKeyTo].nFrame;
			posB = LerpKey(m_typeTo, nCnt, m_nKeyTo, nNextTo, fDisTo, true);
			rotB = LerpKey(m_typeTo, nCnt, m_nKeyTo, nNextTo, fDisTo, false);

			// 最終的なブレンド
			posBlend = LerpVec3(posA, posB, m_fBlendRate);
			rotBlend = LerpVec3(rotA, rotB, m_fBlendRate);

			pModel[nCnt]->SetPos(posBlend);
			pModel[nCnt]->SetRot(rotBlend);
		}

		m_nCntFrom++;
		if (m_nCntFrom >= m_aMotionInfo[m_typeFrom].aKeyInfo[m_nKeyFrom].nFrame)
		{
			m_nCntFrom = 0;
			m_nKeyFrom++;
			if (m_nKeyFrom >= m_aMotionInfo[m_typeFrom].nNumKey - 1)
			{
				m_nKeyFrom = 0;
			}
		}
		m_nCntTo++;
		if (m_nCntTo >= m_aMotionInfo[m_typeTo].aKeyInfo[m_nKeyTo].nFrame)
		{
			m_nCntTo = 0;
			m_nKeyTo++;
			if (m_nKeyTo >= m_aMotionInfo[m_typeTo].nNumKey - 1)
			{
				m_nKeyTo = 0;
			}
		}
		float fDisFrom = (float)m_nCntFrom / m_aMotionInfo[m_typeFrom].aKeyInfo[m_nKeyFrom].nFrame;
		fDisFrom = min(fDisFrom, 1.0f);

		float fDisTo = (float)m_nCntTo / m_aMotionInfo[m_typeTo].aKeyInfo[m_nKeyTo].nFrame;
		fDisTo = min(fDisTo, 1.0f);

		if (m_nBlendFrame > 0)
		{
			m_fBlendRate += 1.0f / m_nBlendFrame;
			if (m_fBlendRate > 1.0f)
			{
				m_fBlendRate = 1.0f;
			}
		}
		else
		{
			m_fBlendRate = 1.0f;
		}

		m_nCntMotion++;

		if (m_fBlendRate >= 1.0f)
		{
			m_bBlend = false;		// ブレンド終了
			m_type = m_typeTo;		// Toモーションに完全移行
			m_nKey = m_nKeyTo;
			m_nCntMotion = m_nCntTo;

			m_fBlendRate = 0.0f;
		}
		return;
	}
	// モーションブレンドしないなら
	else
	{
		// 全モデルの更新
		for (int nCnt = 0; nCnt < nMaxPart; nCnt++)
		{
			// 次のキー
			int nNext = (m_nKey + 1) % m_aMotionInfo[m_type].nNumKey;
			// キー数を戻す
			if (m_nKey >= m_aMotionInfo[m_type].nNumKey || nNext >= m_aMotionInfo[m_type].nNumKey)
			{
				m_nKey = 0;
			}

			D3DXVECTOR3 posMotion, rotMotion, posSabun, rotSabun;

			// 差分計算
			// pos
			posSabun.x = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fPosX - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosX;
			posSabun.y = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fPosY - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosY;
			posSabun.z = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fPosZ - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosZ;
			// rot
			rotSabun.x = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fRotX - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotX;
			rotSabun.y = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fRotY - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotY;
			rotSabun.z = m_aMotionInfo[m_type].aKeyInfo[nNext].aKey[nCnt].fRotZ - m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotZ;

			float fDis = (float)m_nCntMotion / m_aMotionInfo[m_type].aKeyInfo[m_nKey].nFrame;

			// パーツの位置・向きを算出
			posMotion.x = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosX + posSabun.x * fDis);
			posMotion.y = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosY + posSabun.y * fDis);
			posMotion.z = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fPosZ + posSabun.z * fDis);

			rotMotion.x = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotX + rotSabun.x * fDis);
			rotMotion.y = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotY + rotSabun.y * fDis);
			rotMotion.z = (m_aMotionInfo[m_type].aKeyInfo[m_nKey].aKey[nCnt].fRotZ + rotSabun.z * fDis);

			// pos.rot更新
			pModel[nCnt]->SetPos(posMotion);
			pModel[nCnt]->SetRot(rotMotion);
		}

		m_nCntMotion++;

		if (m_nCntMotion >= m_aMotionInfo[m_type].aKeyInfo[m_nKey].nFrame)
		{
			m_nCntMotion = 0;

			m_nKey++;

			if (m_nKey >= m_aMotionInfo[m_type].nNumKey)
			{
				if (m_aMotionInfo[m_type].bLoop)
				{
					m_nKey = 0; // ループ再開
				}
				else
				{
					m_type = 0; // ループしないならニュートラルに
					m_nKey = 0;
				}
			}
		}
	}
}
//===============================================
//
//===============================================
D3DXVECTOR3 CMotion::LerpVec3(const D3DXVECTOR3& posA, const D3DXVECTOR3& posB, float t)
{
	return D3DXVECTOR3(
		posA.x + (posB.x - posA.x) * t,
		posA.y + (posB.y - posA.y) * t,
		posA.z + (posB.z - posA.z) * t
	);
}
//===============================================
//
//===============================================
D3DXVECTOR3 CMotion::LerpKey(int type, int part, int keyA, int keyB, float t, bool isPos)
{
	const auto& a = m_aMotionInfo[type].aKeyInfo[keyA].aKey[part];
	const auto& b = m_aMotionInfo[type].aKeyInfo[keyB].aKey[part];

	if (isPos == true) 
	{
		return LerpVec3({ a.fPosX, a.fPosY, a.fPosZ }, { b.fPosX, b.fPosY, b.fPosZ }, t);
	}
	else
	{
		return LerpVec3({ a.fRotX, a.fRotY, a.fRotZ }, { b.fRotX, b.fRotY, b.fRotZ }, t);
	}
}
//===============================================
// モーション設定
//===============================================
void CMotion::SetType(int type)
{
	m_type = type;

	m_nKey = 0;
	m_nCntMotion = 0;
}
//===============================================
// モーション読み込み
//===============================================
CMotion* CMotion::Load(const char* pFilename)
{
	CMotion* pMotion = new CMotion;

	D3DXVECTOR3 pos[NUM_MODEL];
	D3DXVECTOR3 rot[NUM_MODEL];

	for (int n = 0; n < NUM_MODEL; n++) {
		pos[n] = VECTOR3_NULL;
		rot[n] = VECTOR3_NULL;
	}

	int nLoop = 0;
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntRot = 0;
	int nCntPos = 0;

	FILE* pFile = fopen(pFilename, "r");
	if (!pFile) return nullptr;

	char aString[MAX_WORD];
	char equal[MAX_WORD];

	while (1)
	{
		fscanf(pFile, "%s", &aString[0]);
		if (aString[0] == '#') { fgets(aString, MAX_WORD, pFile); continue; }

		if (strcmp(aString, "SCRIPT") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &aString[0]);

				// コメントスキップ
				if (aString[0] == '#') {
					fgets(aString, MAX_WORD, pFile);
					continue;
				}

				if (strcmp(aString, "MOTIONSET") == 0)
				{
					while (1)
					{
						fscanf(pFile, "%s", &aString[0]);

						if (aString[0] == '#') {
							fgets(aString, MAX_WORD, pFile);
							continue;
						}

						if (strcmp(aString, "LOOP") == 0)
						{
							fscanf(pFile, "%s", equal);
							fscanf(pFile, "%d", &nLoop);
							pMotion->m_aMotionInfo[nCntMotion].bLoop = (nLoop != 0);
						}
						else if (strcmp(aString, "NUM_KEY") == 0)
						{
							int nNumKey = 0;
							fscanf(pFile, "%s", equal);
							fscanf(pFile, "%d", &nNumKey);
							pMotion->m_aMotionInfo[nCntMotion].nNumKey = nNumKey;
						}
						else if (strcmp(aString, "KEYSET") == 0)
						{
							while (1)
							{
								fscanf(pFile, "%s", &aString[0]);

								if (aString[0] == '#') {
									fgets(aString, MAX_WORD, pFile);
									continue;
								}

								if (strcmp(aString, "FRAME") == 0)
								{
									fscanf(pFile, "%s", equal);
									fscanf(pFile, "%d", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
									pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame /= 2;
								}
								else if (strcmp(aString, "KEY") == 0)
								{
									while (1)
									{
										fscanf(pFile, "%s", &aString[0]);

										if (aString[0] == '#') {
											fgets(aString, MAX_WORD, pFile);
											continue;
										}

										if (strcmp(aString, "POS") == 0)
										{
											fscanf(pFile, "%s", equal);
											fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPos].fPosX);
											fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPos].fPosY);
											fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPos].fPosZ);
											nCntPos++;
										}
										else if (strcmp(aString, "ROT") == 0)
										{
											fscanf(pFile, "%s", equal);
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
		else if (feof(pFile))
		{
			break;
		}

	}
	fclose(pFile);
	return pMotion;
}