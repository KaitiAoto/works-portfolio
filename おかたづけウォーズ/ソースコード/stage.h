//===============================================
//
// ステージモデル[stage.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _STAGE_H_
#define _STAGE_H_

// インクルード
#include "main.h"
#include "objectX.h"

// ステージモデルクラス
class CStage :public CObjectX
{
public:
	// 種類
	typedef enum
	{
		TYPE_MODEL = 0,
		TYPE_DUSTBOX,
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CStage(int nPriority = static_cast<int>(Priority::StageUI));
	~CStage();
	HRESULT Init(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, bool bShadow, TYPE type = TYPE_MODEL);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	// 取得
	TYPE GetType(void) { return m_type; }
	// 静的メンバ関数
	static CStage* Create(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, bool bShadow, TYPE type = TYPE_MODEL);
	static void ShakeAll(void);
private:
	// メンバ変数
	TYPE m_type;				// 種類
};

#endif