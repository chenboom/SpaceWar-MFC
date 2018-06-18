
// ChildView.h : CChildView ��Ľӿ�
//


#pragma once
#include "atltypes.h"
#include "atlimage.h"
#include "afxwin.h"


//�ӵ�
typedef struct blt
{
	CImage	png;
	CRect	rect;
	bool	isOut;
}BULLET;

typedef struct plane
{
	CImage	png;
	CRect	rect;
	int		speed;
	bool	isOn;
	bool    haveblt;
	CRect   bltrect;

}PLANE;

const int	m_max = 30;//��ǹ���������


// CChildView ����

class CChildView : public CWnd
{
// ����
public:
	CChildView();

// ����
public:
	size_t		m_score;

// ����
public:

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//�洢��Ϸ״̬
	enum	STATE { START, RUNNING, END };

// ʵ��
public:
	virtual ~CChildView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	// ���洰������С
	CRect m_client;
	// ����Ӣ������λ��
	CRect m_heroPos;
	// Ӣ��ͼƬ
	CImage m_hero;
	//�ӵ�ͼƬ
	CImage m_MonBlt;
	// ��������ͼƬ��͸�����������ҵĺ���
	void TransparentPNG(CImage *png);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//��Ϸ״̬
	STATE	m_state;
	// ����DC
	CDC m_cacheDC;
	// ����λͼ
	CBitmap m_cacheCBitmap;

	// ��ʼ����
		struct sbk
	{
		CImage bk;
		CImage normal;
		CImage selected;
		CRect  rect;
		bool   isSelected;
	}m_startBk;

	//��Ϸ����
	struct gbk
	{
		CImage	fir;//��һ�ű���
		CImage	sec;//�ڶ��ű���
		BOOL	isFir;//��������Ƿ��ǵ�һ��
		int		cury;//���汳����y����
	}m_gameBk;

	//��������
	struct ebk
	{
		CImage bk;
		CImage gameover;
		CImage againNor;//���¿�ʼ
		CImage againSel;
		CImage exitNor;//�˳�
		CImage exitSel;
		CRect  rGo;//gameover��rect
		CRect  rRes;//restart��rect  
		CRect  rExit;//exit��rect
		bool   isExitSel;
		bool   isResSel;
	}m_endBk;

	//��ͣ�����
	struct sg
	{
		CImage	img;
		BOOL	isStop;
		CRect	rect;
	}m_stopGoOn;


	// ������ͼƬ
	void StickBk();
	vector<BULLET> m_bullet;
	vector<PLANE>	m_vecMonsA;//�����
	vector<PLANE>	m_vecMonsB;//�����


private:
	//��Ϸ������״̬
	void StartUI();//��ʼ
	void RunningUI();//��Ϸ��
	void EndUI();//����

	//��ʼ��Ϸ
	void StartGame();
	//������Ϸ
	void EndGame();

	//
	// ���ӵ�
	void StickBullet();
	//
	void StickMonster();
	// �ų�������ӵ�
	void BulletOut();
	//
	void MonsterOut();
	// �ƶ��ӵ�
	void MoveBullet();
	//
	void MonsterMove();
	//
	void StartMonsterA();
	void StartMonsterB();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
};

