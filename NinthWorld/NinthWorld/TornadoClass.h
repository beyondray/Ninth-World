/*--------------------------------------------------------------
��ͷ�ļ�����TornadoClass.h    Create by Beyond Ray,2013��2��
����������һ����װ����ת�Ĳ�ɫ�������Ⱦ����
----------------------------------------------------------------*/

#pragma once

#define EXIST_WIDTH		50000
#define EXIST_HEIGHT	50000
#define EXIST_DEPTH		50000
#define TORNADONUMS		1801

#include"D3DComMacro.h"
#include<d3d9.h>
#include<d3dx9.h>

struct TORNADOVERTEX
{
	float x, y, z;		//��������
	float u, v;			//��������
	TORNADOVERTEX(float _x, float _y, float _z, float _u, float _v)
		:x(_x), y(_y), z(_z), u(_u), v(_v){}
};

#define D3DFVF_TORNADO (D3DFVF_XYZ | D3DFVF_TEX1)

class TornadoClass
{
private:
	LPDIRECT3DDEVICE9		m_pd3dDevice9;				//Direct3D�豸�ӿڣ�COM�ӿڶ���
	LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer9;			//���㻺�����COM�ӿڶ���
	LPDIRECT3DTEXTURE9		* m_ppTexture9;				//ָ����������ָ�루COM�ӿڶ���
	D3DXMATRIX				m_matWorld;					//����任����
	int						m_iTextures;				//��¼���������Ŀ

	struct Tornado
	{
		float   x, y, z;				//�������
		float	fR;						//�����ת�뾶��������������ת��			
		float	fFallSpeed;				//�����ٶ�
		float	fRotateRSpeed;			//����ת�뾶��ת���ٶ�
		float	fRotateYSpeed;			//������ת���ٶ�
		float   fRAngle;				//����ת�뾶��ת�ĽǶ�
		float	fYAngle;				//������ת�ĽǶ�
		D3DXMATRIX matTrans;			//ƽ�Ʊ任����
		D3DXMATRIX matRRotation;		//����ת�뾶��ת����ת�任����
		D3DXMATRIX matYRotation;		//����������ת����ת�뾶
		LPDIRECT3DTEXTURE9 pTexture9;	//��̬�л������������ͼ
	};

	Tornado	m_Tornado[TORNADONUMS];		//�������������

public:
	TornadoClass(LPDIRECT3DDEVICE9 pd3dDevice9);		//���캯��
	~TornadoClass();									//��������

public:
	VOID LoadTextureFromFile(TCHAR *pFileName[], int iNums = 10);		//���ļ��м��ض�̬�������ɼ���iNums��
	HRESULT TornadoInit(float fSize, float fSpeed);						//��������ӵĳ�ʼ��
	VOID TornadoUpdate(float fElapsedTime);								//��������ӵĸ���
	VOID TornadoRender();												//��������ӵ���Ⱦ

};