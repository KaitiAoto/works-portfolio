//==============================
//
// 自販機[vender.h]
// Author : Kaiti Aoto
//
//==============================

// 二重インクルード防止
#ifndef VENDER_H_
#define VENDER_H_

// インクルード
#include "main.h"
#include "object.h"
#include "model.h"
#include "bullet.h"
#include "meshcylinder.h"

// 前方宣言
class CRestock;
class CMapEnemyBase;

// マクロ定義
#define MAX_RESTOCK (50)
#define NUM_REUSETIME (600)

// 自販機クラス
class CVender:public CObject
{
public:
	// 種類
	typedef enum
	{	
		SHAKE_ROTY = 0,
		SHAKE_ROTZ,

		SHAKE_SCALEX,
		SHAKE_SCALEY,

		SHAKE_MAX
	}SHAKE;

	// メンバ関数
	CVender(int nPriority = PRIORITY_VENDER);
	~CVender();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CBullet::TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SubRestock(int nSub) { m_nRestock -= nSub; if (m_nRestock <= 0) { m_nRestock = 0; } }

	// 設定
	void SetUseRestock(bool bUse) { m_bUseRestock = bUse; }
	void SetRestock(int nRestock) { m_nRestock = nRestock; }
	void SetCanRestock(bool bUse) { m_bCanRestock = bUse; }
	// 取得
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }
	static CRestock* GetpRestock(void) { return m_pRestock; }
	CBullet::TYPE GetType(void) { return m_type; }
	bool GetUse(void) { return m_bUseRestock; }
	int GetRestock(void) { return m_nRestock; }


	// 静的メンバ関数
	static CVender* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, CBullet::TYPE type);
	// 設定
	static CBullet::TYPE SetType(const char* pFileName);

private:
	const char* SetModelName(void);
	void Shake(void);

	// メンバ変数
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_Offrot;		// 向き
	D3DXVECTOR3 m_size;			// サイズ
	D3DXVECTOR3 m_scale;		// サイズ
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	CModel* m_pModel;			// モデルへのポインタ
	CMeshCylinder* m_pCylinder;	// シリンダーへのポインタ
	static CRestock* m_pRestock;		// 補充ＵＩへのポインタ
	CMapEnemyBase* m_pMapIcon;
	CBullet::TYPE m_type;		// 補充する中身の種類
	SHAKE m_ShakeType;			//
	bool m_bUse;				// 使用しているか
	bool m_bUseRestock;			// 補充できるか
	bool m_bCanRestock;
	bool m_bShake;
	bool m_bRLShake;
	int m_nCntReuse;			// 再使用できるまでの時間
	int m_nRestock;				// 補充数


};

#endif