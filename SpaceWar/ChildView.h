
// ChildView.h : CChildView 类的接口
//


#pragma once
#include "atltypes.h"
#include "atlimage.h"
#include "afxwin.h"


//子弹
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

const int	m_max = 30;//标记怪物最多个数


// CChildView 窗口

class CChildView : public CWnd
{
// 构造
public:
	CChildView();

// 特性
public:
	size_t		m_score;

// 操作
public:

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
//存储游戏状态
	enum	STATE { START, RUNNING, END };

// 实现
public:
	virtual ~CChildView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	// 保存窗口区大小
	CRect m_client;
	// 保存英雄所在位置
	CRect m_heroPos;
	// 英雄图片
	CImage m_hero;
	//子弹图片
	CImage m_MonBlt;
	// 用来处理图片的透明区域，网上找的函数
	void TransparentPNG(CImage *png);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//游戏状态
	STATE	m_state;
	// 缓冲DC
	CDC m_cacheDC;
	// 缓冲位图
	CBitmap m_cacheCBitmap;

	// 开始背景
		struct sbk
	{
		CImage bk;
		CImage normal;
		CImage selected;
		CRect  rect;
		bool   isSelected;
	}m_startBk;

	//游戏背景
	struct gbk
	{
		CImage	fir;//第一张背景
		CImage	sec;//第二张背景
		BOOL	isFir;//标记下面是否是第一张
		int		cury;//下面背景的y坐标
	}m_gameBk;

	//结束背景
	struct ebk
	{
		CImage bk;
		CImage gameover;
		CImage againNor;//重新开始
		CImage againSel;
		CImage exitNor;//退出
		CImage exitSel;
		CRect  rGo;//gameover的rect
		CRect  rRes;//restart的rect  
		CRect  rExit;//exit的rect
		bool   isExitSel;
		bool   isResSel;
	}m_endBk;

	//暂停与继续
	struct sg
	{
		CImage	img;
		BOOL	isStop;
		CRect	rect;
	}m_stopGoOn;


	// 贴背景图片
	void StickBk();
	vector<BULLET> m_bullet;
	vector<PLANE>	m_vecMonsA;//怪物机
	vector<PLANE>	m_vecMonsB;//怪物机


private:
	//游戏的三种状态
	void StartUI();//开始
	void RunningUI();//游戏中
	void EndUI();//结束

	//开始游戏
	void StartGame();
	//结束游戏
	void EndGame();

	//
	// 贴子弹
	void StickBullet();
	//
	void StickMonster();
	// 排除出界的子弹
	void BulletOut();
	//
	void MonsterOut();
	// 移动子弹
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

