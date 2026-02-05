//==============================
//
// スタート[start.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef _START_H_
#define _START_H_

// インクルード
#include "main.h"
#include "object.h"
#include "model.h"

//スタートクラス
class CStart:public CObject
{
public:
	// 種類
	typedef enum
	{
		TYPE_MODEL = 0,
		TYPE_VENDER,
		TYPE_ENEMY_BASE,
		TYPE_GIMMICK,
		TYPE_MAX
	}TYPE;

	// メンバ関数
	CStart(int nPriority = PRIORITY_START);
	~CStart();

	HRESULT Init(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	
	// 取得
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }
	bool GetStart(void) { return m_bDraw; }
	// 設定
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }

	// 静的メンバ関数
	static CStart* Create(const char* pFilename, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);

private:
	// メンバ変数
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_size;			// サイズ
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	CModel* m_pModel;			// モデルへのポインタ
	bool m_bDraw;				// 描画するかどうか
};

#endif