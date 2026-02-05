//==============================
//
// 爆発[explosion.h]
// Author:kaiti
//
//==============================
#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "main.h"
#include "object2D.h"

#define EXPLOSION_SIZE (30)
#define EXPLOSION_LIFE (60)

//オブジェクト2Dクラス
class CExplosion:public CObject2D
{
public:
	//種類
	typedef enum
	{
		TYPE_A = 0,
		TYPE_B,
		TYPE_MAX
	}TYPE;

	//メンバ関数
	CExplosion(int nPriority = 3);
	~CExplosion();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CExplosion* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);

	static HRESULT Load(void);
	static void Unload(void);
	void SetType(TYPE type) { m_type = type; };

private:
	void Animation(void);

	//メンバ変数
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャ

	D3DXVECTOR3 m_move; //移動量

	float m_fLength;		//対角線の長さ
	const float m_fAngle;	//対角線の角度
	int m_nCntAnim;			//アニメーションカウンター
	int m_nPattermAnim;		//アニメーションパターン
	int m_nLife;			//寿命
	bool m_bUse;			//使用しているか
	TYPE m_type;			//種類

};

#endif