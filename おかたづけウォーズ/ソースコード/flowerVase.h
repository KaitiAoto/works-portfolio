//===============================================
//
// 花瓶処理[flowerVase.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _FLOWERVASE_H_
#define _FLOWERVASE_H_

// インクルード
#include "objectX.h"
#include "meshcircle.h"

// 花瓶クラス
class CFlowerVase :public CObjectX
{
public:

	// メンバ関数
	CFlowerVase(int nPriority = static_cast<int>(Priority::StageModel));
	~CFlowerVase();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定
	bool GetHaveBouquet(void) { return m_bHaveBouquet; }

	// 静的メンバ関数
	static CFlowerVase* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static int GetNumBouqet(void) { return m_nNumBouquet; }
private:

	void Reaction(void);
	// 当たり判定
	void CollRadius(void);
	void CollObjPhysics(void);

	// 定数定義
	static constexpr float Radius = 50.0f;	// 半径
	// モデル名
	static constexpr const char* Filename[2] =
	{
		"data\\MODEL\\flowerVase00.x",
		"data\\MODEL\\bouquet00.x",
	};
	
	// メンバ変数
	CObjectX* m_pBouquet;				// 花束ありモデル用ポインタ
	CMeshCircle* m_pCircle;				// 円へのポインタ
	bool m_bHaveBouquet;				// 花束を持っているか

	// 静的メンバ変数
	static int m_nNumBouquet;			// 花束設置回数カウント用
};

#endif