//==============================
//
//  モーション処理[motion.cpp]
//  Author : Kaiti Aoto
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

	m_typeFrom = TYPE_NEUTRAL;
	m_typeTo = TYPE_NEUTRAL;
	m_bBlend = false;
	m_nBlendFrame = 0;
	m_fBlendRate = 0.0f;

	m_nKeyFrom = 0;
	m_nKeyTo = 0;
	m_nCntFrom = 0;
	m_nCntTo = 0;
}
//================
// デストラクタ
//================
CMotion::~CMotion()
{
}
//
//
//
void CMotion::StartBlend(TYPE from, TYPE to, int nBlendFrame)
{
	m_typeFrom = from;
	m_typeTo = to;
	m_bBlend = true;
	m_nBlendFrame = nBlendFrame;
	m_fBlendRate = 0.0f;

	m_nKeyFrom = 0;
	m_nKeyTo = 0;
	m_nCntFrom = 0;
	m_nCntTo = 0;
}
//========
// 更新
//========
void CMotion::Update(const int nMaxPart,CModel ** pModel)
{
	if (m_bBlend == true)
	{
		for (int nCnt = 0; nCnt < nMaxPart; nCnt++)
		{
			// 補間用変数
			D3DXVECTOR3 posA, rotA, posB, rotB, posBlend, rotBlend;
			// From モーション
			int nNextFrom = (m_nKeyFrom + 1) % m_aMotionInfo[m_typeFrom].nNumKey;
			float fDisFrom = (float)m_nCntFrom / m_aMotionInfo[m_typeFrom].aKeyInfo[m_nKeyFrom].nFrame;
			posA = LerpKey(m_typeFrom, nCnt, m_nKeyFrom, nNextFrom, fDisFrom, true);
			rotA = LerpKey(m_typeFrom, nCnt, m_nKeyFrom, nNextFrom, fDisFrom, false);

			// To モーション
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
			m_bBlend = false;              // ブレンド終了
			m_type = m_typeTo;       // Toモーションに完全移行
			m_nKey = m_nKeyTo;
			m_nCntMotion = m_nCntTo;

			m_fBlendRate = 0.0f;
		}
		return;
	}
	else
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

			if (m_nKey >= m_aMotionInfo[m_type].nNumKey)
			{
				if (m_bLoop)
				{
					m_nKey = 0; // ループ再開
				}
				else
				{
					m_type = TYPE_NEUTRAL; // ループしないならニュートラルに
					m_nKey = 0;
				}
			}
		}
	}
}
//
//
//
D3DXVECTOR3 CMotion::LerpVec3(const D3DXVECTOR3& posA, const D3DXVECTOR3& posB, float t)
{
	return D3DXVECTOR3(
		posA.x + (posB.x - posA.x) * t,
		posA.y + (posB.y - posA.y) * t,
		posA.z + (posB.z - posA.z) * t
	);
}
//
//
//
D3DXVECTOR3 CMotion::LerpKey(TYPE type, int part, int keyA, int keyB, float t, bool isPos)
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
CMotion* CMotion::Load(const char* pFilename)
{
	CMotion* pMotion = new CMotion;

	D3DXVECTOR3 pos[NUM_MODEL];
	D3DXVECTOR3 rot[NUM_MODEL];

	for (int n = 0; n < NUM_MODEL; n++) {
		pos[n] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		rot[n] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	int nLoop = 0;
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntRot = 0;
	int nCntPos = 0;

	FILE* pFile = fopen(pFilename, "r");
	if (!pFile) return nullptr;

	char aString[MAX_WORD];
	fscanf(pFile, "%s", &aString[0]);

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
						fscanf(pFile, "%d", &nLoop);
						pMotion->m_bLoop = (nLoop != 0);
					}
					else if (strcmp(aString, "NUM_KEY") == 0)
					{
						int nNumKey = 0;
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
										fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPos].fPosX);
										fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPos].fPosY);
										fscanf(pFile, "%f", &pMotion->m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntPos].fPosZ);
										nCntPos++;
									}
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
	fclose(pFile);
	return pMotion;
}