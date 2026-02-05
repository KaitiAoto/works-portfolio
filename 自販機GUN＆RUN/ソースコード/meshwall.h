//===================================
//
// メッシュ壁表示処理[meshWall.h]
// Author:kaiti
//
//===================================
#ifndef _MESHWALL_H
#define _MESHWALL_H_

#include "main.h"

//マクロ定義
#define WALLMESHVTX_X (50)//横
#define WALLMESHVTX_Y (50)//縦

#define WALLMAX_VTX ((WALLMESHVTX_X + 1) * (WALLMESHVTX_Y + 1))//頂点数

#define WALLPOLYGON_NO (WALLMESHVTX_X * WALLMESHVTX_Y * 2 + (WALLMESHVTX_Y - 1) * 4)//ポリゴン数

#define WALLINDEX_NO ((WALLMESHVTX_X + 1) * 2 * WALLMESHVTX_Y + (WALLMESHVTX_Y - 1) * 2)//インデックス数

#define WALLMESH_SIZE (150.0f)

#define MAX_MESHWALL (4)

//影の構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置
	D3DXVECTOR3 rot;//向き
	D3DXMATRIX mtxWorld;//ワールドマトリックス
	bool bUse;
}MeshWall;

//プロトタイプ宣言
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

#endif