//===============================================
//
// タイマー[timer.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _TIMER_H_
#define _TIMER_H_

// インクルード
#include "main.h"
#include "number.h"
#include "object.h"
#include "file.h"

// タイマークラス
class CTimer:public CObject
{
public:

	// メンバ関数
	CTimer(int nPriority = static_cast<int>(Priority::UI_Front));
	~CTimer();	
	HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Second(void);
	void Minute(void);
	void CntDown(void);
	void AddTime(int nAdd);
	void Save(const char* pFilename) { saveData.push_back(m_nTimer); CFile::IntSave(pFilename, saveData); }

	// 設定
	void SetLess(bool bLess) { m_bLess = bLess; }
	void SetTime(int nTime);
	// 取得
	bool GetLess(void) { return m_bLess; }
	int GetTime(void) { return m_nTimer; }

	// 静的メンバ関数
	static CTimer* Create(D3DXVECTOR3 pos, bool bLess);
	static std::vector<int> Load(const char* pFilename) { return CFile::IntLoad(pFilename); }

private:

	static constexpr float TIMER_SIZE = 25.0f;	// サイズ
	static constexpr int TIME_DIGIT = 2;		// タイムの桁数
	static constexpr int TIMERDATA = 10;		// テクスチャ変更用

	// メンバ変数
	CNumber* m_pSecond[TIME_DIGIT];	// 秒数字へのポインタ
	CNumber* m_pMinute[TIME_DIGIT];	// 分数字へのポインタ
	int m_nTimer;					// 現在の時間
	int m_nCntTime;					// 1秒計測用
	int m_nDrawSecond;				// 描画する秒数
	int m_nDrawMinute;				// 描画する分数
	float m_fWidth;					// 横幅
	float m_fHeight;				// 縦幅
	bool m_bLess;					// 時間が減るかどうか
	std::vector<int> saveData;
};

#endif