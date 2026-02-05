//===============================================
//
// モデル表示処理[objectX.h]
// Author : Kaiti Aoto
//
//===============================================

// 二重インクルード防止
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

// インクルード防止
#include "main.h"
#include "object.h"

// オブジェクトXクラス
class CObjectX :public CObject
{
public:

	// メンバ関数
	CObjectX(int nPriority = static_cast<int>(Priority::Character));
	// 仮想関数
	~CObjectX();
	HRESULT Init(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void UpdateOBB(void);
	HRESULT ChangeModel(const char* pNewModel);
	void ReturnRotation(void);
	void Shake(void);

	// 設定
	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetOBB(Obb obb) { m_obb = obb; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }
	void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }
	void SetRealSize(D3DXVECTOR3 size);
	void SetColorChange(bool bUse) { m_bColorChange = bUse; }
	void SetColor(D3DXCOLOR col) { m_forcedColor = col; }
	void SetEmissive(bool bEmissive) { m_bEmissive = bEmissive; }
	void SetQuat(bool bQuat, const D3DXQUATERNION& quat);
	void SetShake(bool bShake, int nShakeTime = 30, D3DXVECTOR3 MaxShake = D3DXVECTOR3(5.0f,5.0f,5.0f)) { m_bShake = bShake, m_nShakeTime = nShakeTime; m_MaxShake = MaxShake; }
	// 取得
	D3DXMATRIX GetMtx(void) { return m_mtxWorld; }
	D3DXVECTOR3 GetPos(void) { return m_pos;}
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	D3DXVECTOR3 GetScale(void) { return m_scale; }
	const Obb& GetOBB() const { return m_obb; }
	D3DXQUATERNION GetQuat(void) { return m_quat; }
	// 静的メンバ関数
	static CObjectX* Create(const char* pFilename, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority = static_cast<int>(Priority::Character));

private:
	// メンバ変数
	LPD3DXMESH m_pMesh;			// メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;	// マテリアルへのポインタ
	DWORD m_dwNumMat;			// マテリアルの数
	D3DXVECTOR3 m_pos;			// 位置
	D3DXVECTOR3 m_rot;			// 向き
	D3DXVECTOR3 m_size;			// サイズ
	D3DXVECTOR3 m_realSize;		// 実際にしたいサイズ
	D3DXVECTOR3 m_scale;		// スケール
	D3DXVECTOR3 m_OffPos;		// 位置（オフセット）
	D3DXVECTOR3 m_MaxShake;		// 最大振動
	D3DXCOLOR m_forcedColor;	// 色
	D3DXMATRIX m_mtxWorld;		// ワールドマトリックス
	D3DXQUATERNION m_quat;
	int* m_nIdxTex;				// テクスチャインデックスへのポインタ
	Obb m_obb;					// OBB情報
	int m_nShakeTime;			// 振動時間
	bool m_bDraw;				// 描画するかどうか
	bool m_bColorChange;		// 色を変えるかどうか
	bool m_bEmissive;			// 発光するかどうか
	bool m_bQuat;
	bool m_bShake;				// 振動するか

};

#endif
