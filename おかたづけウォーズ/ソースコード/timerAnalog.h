//===============================================
//
// アナログタイマー[timerAnalog.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _TIMERANALOG_H_
#define _TIMERANALOG_H_

// インクルード
#include "main.h"
#include "number.h"
#include "objectX.h"
#include "file.h"

// アナログタイマークラス
class CAbalogTimer:public CObjectX
{
public:

	// メンバ関数
	CAbalogTimer(int nPriority = static_cast<int>(Priority::UI_Front));
	~CAbalogTimer();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTime);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CntDown(void);
	void Save(const char* pFilename) { saveData.push_back(m_nMaxTime); CFile::IntSave(pFilename, saveData); }

	// 設定
	void SetTime(int nTime);
	// 取得

	// 静的メンバ関数
	static CAbalogTimer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTime);
	static std::vector<int> Load(const char* pFilename) { return CFile::IntLoad(pFilename); }

private:

	// メンバ変数
	CObjectX* m_pClock;		// 時計へのポインタ
	float m_fRotZ;			// 長針の回転用
	int m_nMaxTime;			// 現在の時間
	int m_nCntTime;			// 1秒計測用
	std::vector<int> saveData;
};

#endif