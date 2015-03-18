/*--------------------------------------------------------------
��Cpp�ļ�����TornadoClass.cpp    Create by Beyond Ray,2013��2��
����������һ����װ����ת�Ĳ�ɫ�������Ⱦ�����ʵ��
----------------------------------------------------------------*/
#include "stdafx.h"
#include"TornadoClass.h"

//------------------------------------------��TornadoClass()������----------------------------------------------
// Desc:���캯��
//--------------------------------------------------------------------------------------------------------------
TornadoClass::TornadoClass(LPDIRECT3DDEVICE9 pd3dDevice9)
{
	m_pd3dDevice9 = pd3dDevice9;
	m_pVertexBuffer9 = NULL;
	m_ppTexture9 = NULL;
	for (int i = 0; i < TORNADONUMS; i++)
	{
		::ZeroMemory(&m_Tornado[i], sizeof(Tornado));
	}
}

//---------------------------------------��LoadTextureFromFile()������------------------------------------------
// Desc:���ļ��м��ض�̬�������ɼ���10��
//--------------------------------------------------------------------------------------------------------------
VOID TornadoClass::LoadTextureFromFile(TCHAR *pFileName[], int iNums /* = 10 */)
{
	m_iTextures = iNums;
	m_ppTexture9 = new LPDIRECT3DTEXTURE9[m_iTextures];
	for (int i = 0; i < m_iTextures; i++)
		D3DXCreateTextureFromFile(m_pd3dDevice9, pFileName[i], &m_ppTexture9[i]);
}

//------------------------------------------��TornadoInit()������-----------------------------------------------
// Desc:��������ӵĳ�ʼ�����������Ӹ������ԣ� �������㻺���������ö������飩
//--------------------------------------------------------------------------------------------------------------
HRESULT TornadoClass::TornadoInit(float fSize,float fSpeed)		//���ӳߴ��С��������ת�ٶ�
{
	srand(GetTickCount());		//��ʼ���������

	float fMaxR = std::min(EXIST_WIDTH, EXIST_DEPTH) / 2.0f;

	for (int i = 0; i < TORNADONUMS; i++)
	{
		//������ĳ�ʼ������޶��ڹ涨�Ŀռ䷶Χ��
		m_Tornado[i].x = ((float)EXIST_WIDTH * rand() / RAND_MAX) -EXIST_WIDTH / 2.0f;
		m_Tornado[i].z = ((float)EXIST_DEPTH * rand() / RAND_MAX) - EXIST_DEPTH / 2.0f;	
		m_Tornado[i].y = ((float)EXIST_HEIGHT * rand() / RAND_MAX) ;

		//�����������ת�뾶
		m_Tornado[i].fR = fMaxR * rand() / RAND_MAX;

		//������ת�ٶȺ�һ�������ٶ�
		m_Tornado[i].fFallSpeed = 500.0f + (float)(rand() % 1000);
		m_Tornado[i].fRotateRSpeed = 1.0f * rand() / RAND_MAX;
		m_Tornado[i].fRotateYSpeed = fSpeed;			//�������ת�ٶ�Ϊ�̶�ֵ

		//�趨ƽ�Ʊ任����
		static float fAngle = 2 * D3DX_PI * ((float)rand() / RAND_MAX);
		D3DXMatrixTranslation(&m_Tornado[i].matTrans, m_Tornado[i].fR * cos(fAngle), 
			0.0f, m_Tornado[i].fR * sin(fAngle));

		//�趨��ʼ��������ͼ
		m_Tornado[i].pTexture9 = m_ppTexture9[0];
	}
	//*
	HR(m_pd3dDevice9->CreateVertexBuffer(4 * sizeof(TORNADOVERTEX), D3DUSAGE_WRITEONLY, D3DFVF_TORNADO,
		D3DPOOL_MANAGED, &m_pVertexBuffer9, NULL));

	TORNADOVERTEX * pVertices = NULL;
	HR(m_pVertexBuffer9->Lock(0, 0, (void**)&pVertices, 0));

	pVertices[0] = TORNADOVERTEX(-fSize, -fSize, 0.0f, 0.0f, 1.0f);
	pVertices[1] = TORNADOVERTEX(-fSize, fSize, 0.0f, 0.0f, 0.0f);
	pVertices[2] = TORNADOVERTEX(fSize, -fSize, 0.0f, 1.0f, 1.0f);
	pVertices[3] = TORNADOVERTEX(fSize, fSize, 0.0f, 1.0f, 0.0f);

	m_pVertexBuffer9->Unlock();
	//*/
	return S_OK;
}

//-------------------------------------------��TornadoUpdate()������------------------------------------------
// Desc:����λ�ø��º���������������ת����ĸ��£�����λ�õĸ��£������г����߽��жϣ�
//------------------------------------------------------------------------------------------------------------
VOID TornadoClass::TornadoUpdate(float fElapsedTime)
{
	static int iIndex ;
	iIndex = 0;
	for (int i = 0; i < TORNADONUMS; i++)
	{
		(i % 2 == 1) ? m_Tornado[i].y -= m_Tornado[i].fFallSpeed * fElapsedTime
			: m_Tornado[i].y += m_Tornado[i].fFallSpeed * fElapsedTime;	//�߶�ֵ����
		if (m_Tornado[i].y < 0.0f || m_Tornado[i].y > EXIST_HEIGHT)
			m_Tornado[i].y = ((float)EXIST_HEIGHT * rand() / RAND_MAX);

		m_Tornado[i].pTexture9 = m_ppTexture9[iIndex++];			//�л���̬����

		if (iIndex >= m_iTextures)
			iIndex = 0;

		//������ת���������
		(i % 2) == 0 ? m_Tornado[i].fRAngle += fElapsedTime * m_Tornado[i].fRotateRSpeed
			: m_Tornado[i].fRAngle -= fElapsedTime * m_Tornado[i].fRotateRSpeed;
		(i % 3) == 0 ? m_Tornado[i].fYAngle += fElapsedTime * m_Tornado[i].fRotateYSpeed
			: m_Tornado[i].fYAngle -= fElapsedTime * m_Tornado[i].fRotateYSpeed;
		D3DXMatrixRotationY(&m_Tornado[i].matRRotation, m_Tornado[i].fRAngle);
		D3DXMatrixRotationY(&m_Tornado[i].matYRotation, m_Tornado[i].fYAngle);
	}
	
}

//-------------------------------------------��TronadoRender()������-------------------------------------------
// Desc:��������ӵ���Ⱦ������Alpha��ϣ�������ˣ��������޳�ģʽ��
//-------------------------------------------------------------------------------------------------------------
VOID TornadoClass::TornadoRender()
{

	//����ɫ��ϵĵ�һ����������ɫֵ��Ϊ���
	m_pd3dDevice9->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	//����ɫ��ϵĵ�һ����������ɫֵȡΪ����ֵ
	m_pd3dDevice9->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	//��������������˷�ʽ
	m_pd3dDevice9->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pd3dDevice9->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	//����Alpha��ɫ���
	m_pd3dDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pd3dDevice9->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pd3dDevice9->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�����޳�ģʽΪ���޳�
	m_pd3dDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//׼����ʼ��Ⱦ����
	m_pd3dDevice9->SetStreamSource(0, m_pVertexBuffer9, 0, sizeof(TORNADOVERTEX));
	m_pd3dDevice9->SetFVF(D3DFVF_TORNADO);

	static D3DXMATRIX matTransXYZ;
	for (unsigned int i = 0; i < TORNADONUMS; i++)
	{
		D3DXMatrixTranslation(&matTransXYZ, m_Tornado[i].x, m_Tornado[i].y, m_Tornado[i].z);
		m_matWorld = m_Tornado[i].matYRotation  * m_Tornado[i].matTrans * m_Tornado[i].matRRotation * matTransXYZ;
		m_pd3dDevice9->SetTransform(D3DTS_WORLD, &m_matWorld);

		m_pd3dDevice9->SetTexture(0, m_Tornado[i].pTexture9);
		m_pd3dDevice9->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	//�����տ������޳�ģʽ��Ϊ���������� �ر�Alpha���
	m_pd3dDevice9->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

//----------------------------------------��~TornadoClass()������-----------------------------------------------
// Desc:��������
//--------------------------------------------------------------------------------------------------------------
TornadoClass::~TornadoClass()
{
	for (int i = 0; i < TORNADONUMS; i++)
		m_Tornado[i].pTexture9 = NULL;

	for (int i = 0; i < m_iTextures; i++)
		SAFE_RELEASE(m_ppTexture9[i]);

	SAFE_DELETE_ARRAY(m_ppTexture9);
	SAFE_RELEASE(m_pVertexBuffer9);
}