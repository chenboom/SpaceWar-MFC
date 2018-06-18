
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "SpaceWar.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TIMER_PAINT 1
#define	BGMOVE      2
#define BULLET      3
#define SHOOT       4
#define	MOSTERMOVE  5

// CChildView

CChildView::CChildView()
{
	m_state = START;
	/************************开始界面工作********************/
	//加载png
	m_startBk.bk.Load(_T("res\\img\\STartpng.png"));
	m_startBk.normal.Load(_T("res\\startNormal.png"));
	m_startBk.selected.Load(_T("res\\startSelected.png"));


	//初始化rect
	m_startBk.rect.left =250 ;
	m_startBk.rect.right =330 ;
	m_startBk.rect.top =380;
	m_startBk.rect.bottom = 420;

	//一开始默认未选择
	m_startBk.isSelected = false;

	//初始化暂停、继续位图
	m_stopGoOn.img.Load(_T("res\\play.bmp"));
	m_stopGoOn.isStop = FALSE;
	m_stopGoOn.rect.SetRect(0, 0, m_stopGoOn.img.GetWidth(),
		m_stopGoOn.img.GetHeight() / 2);


	/************************结束界面准备工作********************/
	m_endBk.bk.Load(_T("res\\overbg.png"));
	m_endBk.gameover.Load(_T("res\\gameover.png"));
	m_endBk.againNor.Load(_T("res\\exitNor.bmp"));
	m_endBk.againSel.Load(_T("res\\againSel.bmp"));
	m_endBk.exitNor.Load(_T("res\\againNor.bmp"));
	m_endBk.exitSel.Load(_T("res\\exitSel.bmp"));

	//初始化rGo
	m_endBk.rGo.left = 200;
	m_endBk.rGo.right = 260;
	m_endBk.rGo.top = 330;
	m_endBk.rGo.bottom = 360;

	//初始化rRes
	m_endBk.rRes.left = 240;
	m_endBk.rRes.right = 280;
	m_endBk.rRes.top = 380;
	m_endBk.rRes.bottom = 420;


	//开始默认为未选择
	m_endBk.isExitSel = false;
	m_endBk.isResSel = false;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()



// CChildView 消息处理程序

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	
	//加载背景
	m_gameBk.fir.Load("res\\img\\bg1 582x1600.png");
	m_gameBk.sec.Load("res\\img\\bg2 582x1600.png");
	m_gameBk.isFir = TRUE;
	m_gameBk.cury = 0;
	//加载英雄图
	m_hero.Load("res\\img\\hero 97x97.png");
	//处理英雄图透明区域，否则部分不显示为透明
	TransparentPNG(&m_hero);
	//加载子弹
	m_bullet.resize(5);
	for (int i = 0; i<m_bullet.size(); i++)
	{
		m_bullet[i].png.Load("res\\img\\bullet 60x56.png");

		TransparentPNG(&m_bullet[i].png);
	}
	m_MonBlt.Load("res\\img\\monsterBlt 10x10.png");
	//加载怪物机图
	//A
	m_vecMonsA.resize(30);
	for (int i = 0; i<m_vecMonsA.size(); i++)
	{
		m_vecMonsA[i].png.Load("res\\img\\monsterA 90x68.png");
		TransparentPNG(&m_vecMonsA[i].png);
		m_vecMonsA[i].isOn = false;
		m_vecMonsA[i].haveblt = false;
	}
	//B
	m_vecMonsB.resize(5);
	for (int i = 0; i<m_vecMonsB.size(); i++)
	{
		m_vecMonsB[i].png.Load("res\\img\\monsterB 90x68.png");
		TransparentPNG(&m_vecMonsB[i].png);
		m_vecMonsB[i].isOn = false;
		m_vecMonsB[i].haveblt = false;
	}

	//设置英雄初始位置
	m_heroPos.left = 240;
	m_heroPos.right = 240 + 97;
	m_heroPos.top = 650;
	m_heroPos.bottom = 650 + 97;


	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	
	// TODO: 在此处添加消息处理程序代码
	
	// 不要为绘制消息而调用 CWnd::OnPaint()
	CDC *cDC = this->GetDC();   //获得当前窗口的DC  
	GetClientRect(&m_client);   //获得窗口的尺寸
	//创建内存位图、内存DC以便在内存中绘图, 实现双缓冲
	m_cacheDC.CreateCompatibleDC(NULL);
	m_cacheCBitmap.CreateCompatibleBitmap(cDC, m_client.Width(), m_client.Height());
	m_cacheDC.SelectObject(&m_cacheCBitmap);
	switch (m_state)
	{
		//绘制开始界面
	case START:
		StartUI();
		break;

		//绘制游戏界面
	case RUNNING:
		RunningUI();
		break;

		//绘制结束界面
	case END:
		EndUI();
		EndGame();
		break;
	}

	  
	//将内存中的图贴到客户区中
	cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);

	//释放内存位图和内存DC
	m_cacheCBitmap.DeleteObject();
	m_cacheDC.DeleteDC();
}



// 用来处理png图片的透明区域，网上教程上的函数
void CChildView::TransparentPNG(CImage *png)
{
	for (int i = 0; i <png->GetWidth(); i++)
	{
		for (int j = 0; j <png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char*>(png->GetPixelAddress(i, j));
			pucColor[0] = pucColor[0] * pucColor[3] / 255;
			pucColor[1] = pucColor[1] * pucColor[3] / 255;
			pucColor[2] = pucColor[2] * pucColor[3] / 255;
		}
	}
}


void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nChar)
	{
		//实现英雄左移
		case 37:
		case 'A':
			if (m_heroPos.left >= 0)
			{
				m_heroPos.left -= 10;
				m_heroPos.right -= 10;
			}
			break;
		//实现英雄右移
		case 39:
		case 'D':
			if (m_heroPos.right <= 570)
			{
				m_heroPos.left += 10;
				m_heroPos.right += 10;
			}
			break;
		case 38:
		case 'W':
			if (m_heroPos.top >= 5)
			{
				m_heroPos.bottom -= 10;
				m_heroPos.top -= 10;
			}
			break;
		case 40:
		case 'S':
			if (m_heroPos.bottom <= 770)
			{
				m_heroPos.bottom += 10;
				m_heroPos.top += 10;
			}
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
		case(TIMER_PAINT):OnPaint();break;        //调用绘制画面来刷新
		case(BGMOVE):m_gameBk.cury += 10;break;   //使背景的位置+10
		case(BULLET):
			BulletOut();
			MoveBullet();
			break;
		case(SHOOT):
			for (size_t i = 0; i < m_bullet.size(); i++)
			{
				if (m_bullet[i].isOut)
				{
					int x = m_heroPos.left + 18;
					int y = m_heroPos.top;
					m_bullet[i].rect.SetRect(x, y,
						m_bullet[i].png.GetWidth() + x,
						m_bullet[i].png.GetHeight() + y);

					m_bullet[i].isOut = false;
					break;
				}
				PlaySound (TEXT("res\\music\\attack.wav"),
					NULL, SND_FILENAME | SND_ASYNC);
				//mciSendString("play res\\music\\attack.wav", "", 0, NULL);
			}
			break;
		case(MOSTERMOVE):
			if (rand()%10==1) StartMonsterA();
			if(m_score>200&& rand() % 100 == 1) StartMonsterB();
			MonsterMove();
			MonsterOut();
			
			
			break;
	}
		
	CWnd::OnTimer(nIDEvent);
}


// 贴背景图片
void CChildView::StickBk()
{
	//如果到了下边界，就换图
	if (m_gameBk.cury == m_gameBk.fir.GetHeight())
	{
		m_gameBk.cury = 0;
		m_gameBk.isFir = m_gameBk.isFir ? 0 : 1;
	}

	int height = m_gameBk.fir.GetHeight();

	//如果第一张图在下面，就先绘制第一张
	if (m_gameBk.isFir)
	{
		m_gameBk.fir.BitBlt(m_cacheDC, 0,
			m_gameBk.cury, SRCCOPY);

		m_gameBk.sec.BitBlt(m_cacheDC, 0,
			m_gameBk.cury - height, SRCCOPY);
	}

	//第二张图在下面，先绘制第二张
	else
	{
		m_gameBk.sec.BitBlt(m_cacheDC, 0,
			m_gameBk.cury, SRCCOPY);

		m_gameBk.fir.BitBlt(m_cacheDC, 0,
			m_gameBk.cury - height, SRCCOPY);
	}
}

void CChildView::StartUI()
{
	//播放开始背景音乐
	mciSendString(_T("Open res\\music\\startbg.mp3 alias startbg"), NULL, 0, NULL);
	mciSendString(_T("Play startbg repeat"), NULL, 0, NULL);


	//贴开始背景
	m_startBk.bk.BitBlt(m_cacheDC, 0, 0, SRCCOPY);
	//m_startBk.bk.Load("Open res\\img\\STartpng.png");

	//贴"开始"样字的png
	if (m_startBk.isSelected)
	{
		m_startBk.selected.TransparentBlt(m_cacheDC,
			m_startBk.rect, RGB(255, 255, 255));
	}
	else
	{
		m_startBk.normal.TransparentBlt(m_cacheDC,
			m_startBk.rect, RGB(255, 255, 255));
	}
	
}

void CChildView::RunningUI()
{
	CDC *cDC = this->GetDC();   //获得当前窗口的DC  
	GetClientRect(&m_client);   //获得窗口的尺寸
	//————————————————————开始绘制——————————————————————  
	//贴背景图							 
	//m_bg1.Draw(m_cacheDC, m_client);
	StickBk();
	//贴英雄图
	m_hero.Draw(m_cacheDC, m_heroPos);
	//贴子弹
	StickBullet();
	//贴怪物机图
	StickMonster();
	

	//贴暂停、继续位图
	int w = 30;
	int h = 60;
	m_stopGoOn.img.TransparentBlt(m_cacheDC, 0, 0, w, h,
		0, m_stopGoOn.isStop * h, w, h, RGB(255, 255, 255));


	//***************贴分数*********************
	CString str;

	//修改字体大小
	LOGFONT	lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -25;

	CFont font;
	font.CreateFontIndirect(&lf);

	str.Format(_T("Score: %d"), m_score);


	//设置文本属性
	m_cacheDC.SelectObject(font);
	m_cacheDC.SetTextColor(RGB(255, 127, 39));
	m_cacheDC.SetBkMode(TRANSPARENT);

	m_cacheDC.TextOut(30 + 20, 0, str);

	//最后将缓冲DC内容输出到窗口DC中  
	cDC->BitBlt(0, 0, m_client.Width(), m_client.Height(), &m_cacheDC, 0, 0, SRCCOPY);

	//释放字体资源
	font.DeleteObject();

	//————————————————————绘制结束—————————————————————  

	//在绘制完图后,使窗口区有效  
	ValidateRect(&m_client);
	//释放缓冲DC  
	m_cacheDC.DeleteDC();
	//释放对象  
	m_cacheCBitmap.DeleteObject();
	//释放窗口DC  
	ReleaseDC(cDC);
}

void CChildView::EndUI()
{
	//播放开始背景音乐
	mciSendString(_T("Open res\\music\\overbg.mp3 alias overbg"), NULL, 0, NULL);
	mciSendString(_T("Play overbg"), NULL, 0, NULL);

	//贴背景
	m_endBk.bk.BitBlt(m_cacheDC, 0, 0, SRCCOPY);

	//***************贴分数*********************
	CString str;
	CString strg;

	//修改字体大小
	LOGFONT	lf;
	ZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -25;

	CFont font;
	font.CreateFontIndirect(&lf);

	str.Format(_T("Your Score: %d "), m_score);
	strg.Format(_T("GAME OVER! "));

	//设置文本属性
	m_cacheDC.SelectObject(font);
	m_cacheDC.SetTextColor(RGB(0, 0, 0));
	m_cacheDC.SetBkMode(TRANSPARENT);

	m_cacheDC.TextOut(m_endBk.rGo.left -30, m_endBk.rGo.top - 50, str);
	m_cacheDC.TextOut(m_endBk.rGo.left - 20, m_endBk.rGo.top , strg);
	//释放字体资源
	font.DeleteObject();

	//贴"游戏结束"字样的png
	//m_endBk.gameover.TransparentBlt(m_cacheDC, m_endBk.rGo, RGB(0, 0, 0));
	m_endBk.exitSel.TransparentBlt(m_cacheDC, m_endBk.rRes, RGB(0, 0, 0));
	//m_endBk.gameover.TransparentBlt(m_cacheDC, m_endBk.rRes, RGB(0, 0, 0));
}

void CChildView::StartGame()
{

	//先关闭开始、结束背景音乐
	mciSendString(_T("Close startbg"), NULL, 0, NULL);
	mciSendString(_T("Close overbg"), NULL, 0, NULL);

	//开始游戏, 分数为0
	m_score = 0;

	//初始化英雄机
	m_heroPos.left = 240;
	m_heroPos.right = 240 + 97;
	m_heroPos.top = 650;
	m_heroPos.bottom = 650 + 97;

	//初始化怪物机
	srand(GetTickCount());

	/*for (int i = 0; i<m_max; i++)
	{
		StartMonsterA();
		StartMonsterB();
	}*/


	//设置定时器
	//刷新页面的计时器
	SetTimer(TIMER_PAINT, 10, NULL);
	//实现背景滚动的计时器
	SetTimer(BGMOVE, 10, NULL);
	//移动子弹的计时器
	SetTimer(BULLET, 10, NULL);
	//发射子弹的定时器
	SetTimer(SHOOT, 150, NULL);
	//怪物相关的计时器
	SetTimer(MOSTERMOVE, 10, NULL);

	//打开背景音乐
	mciSendString(_T("Open res\\music\\gamebg.mp3 alias gamebg"), NULL, 0, NULL);
	//播放背景音乐(重复播放)
	mciSendString(_T("Play gamebg repeat"), NULL, 0, NULL);

}

void CChildView::EndGame()
{

	//刷新页面的计时器
	KillTimer(TIMER_PAINT);
	//实现背景滚动的计时器
	KillTimer(BGMOVE);
	//移动子弹的计时器
	KillTimer(BULLET);
	//发射子弹的定时器
	KillTimer(SHOOT);
	//怪物相关的计时器
	KillTimer(MOSTERMOVE);

	//关闭音乐
	mciSendString(_T("Close attack"), NULL, 0, NULL);
	mciSendString(_T("Close die"), NULL, 0, NULL);
	mciSendString(_T("Close gamebg"), NULL, 0, NULL);

}



// 贴子弹
void CChildView::StickBullet()
{
	for (size_t i = 0; i<m_bullet.size(); i++)
	{
		//如果为被移除, 就贴
		if (!m_bullet[i].isOut)
		{
			m_bullet[i].png.TransparentBlt(m_cacheDC,
				m_bullet[i].rect, RGB(0, 0, 0));
		}
	}
}

void CChildView::StickMonster()
{
	for (size_t i = 0; i<m_vecMonsA.size(); i++)
	{
		//如果为被移除, 就贴
		if (m_vecMonsA[i].isOn)
		{
			m_vecMonsA[i].png.TransparentBlt(m_cacheDC,
				m_vecMonsA[i].rect, RGB(0, 0, 0)
			);
		}
	}
	for (size_t i = 0; i<m_vecMonsB.size(); i++)
	{
		//如果为被移除, 就贴
		if (m_vecMonsB[i].isOn)
		{
			m_vecMonsB[i].png.TransparentBlt(m_cacheDC,
				m_vecMonsB[i].rect, RGB(0, 0, 0)
			);
		}
		if (m_vecMonsB[i].haveblt) {
			m_MonBlt.TransparentBlt(m_cacheDC,
				m_vecMonsB[i].bltrect, RGB(0, 0, 0)
			);
		}
	}
}



// 排除出界的子弹
void CChildView::BulletOut()
{
	for (size_t i = 0; i<m_bullet.size(); i++)
	{
		if (m_bullet[i].isOut) continue;
		//如果移到客户区边界, 就移除
		if (m_bullet[i].rect.left < 0 ||
			m_bullet[i].rect.right > m_client.right||
			m_bullet[i].rect.top < 0 ||
			m_bullet[i].rect.bottom > m_client.bottom)
		{
			m_bullet[i].isOut = true;
		}
	}
}

// 排除出界的敌机
void CChildView::MonsterOut()
{
	//A
	for (size_t i = 0; i<m_vecMonsA.size() ; i++)
	{
		if(!m_vecMonsA[i].isOn) continue;
		//如果移到客户区边界, 就移除
		if (m_vecMonsA[i].rect.left < 0 ||
			m_vecMonsA[i].rect.right > m_client.right ||
			m_vecMonsA[i].rect.top < 0 ||
			m_vecMonsA[i].rect.bottom > m_client.bottom)
		{
			m_vecMonsA[i].isOn = false;
		}
		for (size_t j = 0; j < m_bullet.size() && !m_bullet[j].isOut; j++) {
			
			if (m_bullet[j].isOut) continue;
			//子弹和飞机左右边距
			int biuLR = m_bullet[j].rect.left - m_vecMonsA[i].rect.left;
			//子弹和飞机上下边距
			int biuUD= m_bullet[j].rect.top - m_vecMonsA[i].rect.top;
			//子弹打中飞机
			if (biuUD > -50 && biuUD < 68) {
				if (biuLR > -60 && biuLR < 90) {
					m_vecMonsA[i].isOn = false;
					m_bullet[j].isOut = true;
					m_score += 10;
				}
			}
			if (m_vecMonsA[i].rect.left - m_heroPos.left > -90 && m_vecMonsA[i].rect.left - m_heroPos.left < 97) {
				if (m_vecMonsA[i].rect.bottom - m_heroPos.bottom > -17 && m_vecMonsA[i].rect.bottom - m_heroPos.bottom < 18) {
					m_state = END;
				}
			}

		}
	}
	//B 
	for (size_t i = 0; i<m_vecMonsB.size(); i++)
	{
		//飞机子弹撞上英雄机
		if (m_vecMonsB[i].bltrect.left - m_heroPos.left > -10 && m_vecMonsB[i].bltrect.left - m_heroPos.left < 97) {
			if (m_vecMonsB[i].bltrect.bottom - m_heroPos.bottom > -10 && m_vecMonsB[i].bltrect.bottom - m_heroPos.bottom < 18) {
				m_state = END;
			}
		}
		if (!m_vecMonsB[i].isOn) continue;
		//如果移到客户区边界, 就移除
		if (m_vecMonsB[i].rect.left < 0 ||
			m_vecMonsB[i].rect.right > m_client.right ||
			m_vecMonsB[i].rect.top < 0 ||
			m_vecMonsB[i].rect.bottom > m_client.bottom)
		{
			m_vecMonsB[i].isOn = false;
		}
		for (size_t j = 0; j < m_bullet.size() && !m_bullet[j].isOut; j++) {

			if (m_bullet[j].isOut) continue;
			//子弹和飞机左右边距
			int biuLR = m_bullet[j].rect.left - m_vecMonsB[i].rect.left;
			//子弹和飞机上下边距
			int biuUD = m_bullet[j].rect.top - m_vecMonsB[i].rect.top;
			//子弹打中飞机
			if (biuUD > -50 && biuUD < 68) {
				if (biuLR > -60 && biuLR < 90) {
					m_vecMonsB[i].isOn = false;
					m_bullet[j].isOut = true;
					m_score += 10;
				}
			}
			
			
		}
		//飞机撞上英雄机
		if (m_vecMonsB[i].rect.left - m_heroPos.left > -90 && m_vecMonsB[i].rect.left - m_heroPos.left < 97) {
			if (m_vecMonsB[i].rect.bottom - m_heroPos.bottom > -17 && m_vecMonsB[i].rect.bottom - m_heroPos.bottom < 18) {
				m_state = END;
			}
		}
		
	}
}


// 移动子弹
void CChildView::MoveBullet()
{
	for (size_t i = 0; i < m_bullet.size(); i++)
	{
		//移动子弹
		m_bullet[i].rect.top -= 20;
		m_bullet[i].rect.bottom -= 20;
	}
	
}

// 移动敌机
void CChildView::MonsterMove()
{
	//A
	for (size_t i = 0; i < m_vecMonsA.size(); i++)
	{
		int rand1 = rand() % 3 + m_vecMonsA[i].speed-1, rand2 = rand() % 11;
		//移动飞机
		m_vecMonsA[i].rect.top += rand1;
		m_vecMonsA[i].rect.bottom += rand1;
		//m_vecMonsA[i].rect.left += rand2*4-20;
		//m_vecMonsA[i].rect.right += rand2*4-20;
	}
	//B
	for (size_t i = 0; i < m_vecMonsB.size(); i++)
	{
		int rand1 = rand() % 3 + m_vecMonsB[i].speed - 1, rand2 = rand() % 11;
		//移动飞机
		m_vecMonsB[i].rect.top += rand1;
		m_vecMonsB[i].rect.bottom += rand1;
		m_vecMonsB[i].bltrect.top += m_vecMonsB[i].speed+5;
		m_vecMonsB[i].bltrect.bottom += m_vecMonsB[i].speed + 5;
		if (m_vecMonsB[i].bltrect.bottom > 770)m_vecMonsB[i].haveblt = false;
		//m_vecMonsB[i].rect.left += rand2*4-20;
		//m_vecMonsB[i].rect.right += rand2*4-20;
	}

}




void CChildView::StartMonsterA()
{
	for (size_t i = 0; i < m_vecMonsA.size(); i++)
	{
		if (!m_vecMonsA[i].isOn)
		{
			int x = rand() % (582 - m_vecMonsA[i].png.GetWidth());
			int y = 0;
			m_vecMonsA[i].rect.SetRect(x, y,
				m_vecMonsA[i].png.GetWidth() + x,
				m_vecMonsA[i].png.GetHeight() + y);
			m_vecMonsA[i].speed = m_score / 1000 + 4;
			m_vecMonsA[i].isOn = true;
			break;
		}
		PlaySound(TEXT("res\\music\\die.wav"),
			NULL, SND_FILENAME | SND_ASYNC);
	}
}
void CChildView::StartMonsterB()
{
	for (size_t i = 0; i < m_vecMonsB.size(); i++)
	{
		if (!m_vecMonsB[i].isOn)
		{
			int x = rand() % (582 - m_vecMonsB[i].png.GetWidth());
			int y = 0;
			m_vecMonsB[i].rect.SetRect(x, y,
				m_vecMonsB[i].png.GetWidth() + x,
				m_vecMonsB[i].png.GetHeight() + y);
			m_vecMonsB[i].bltrect.SetRect(m_vecMonsB[i].png.GetWidth() / 2 - 5 + x, m_vecMonsB[i].png.GetHeight() / 2 - 5 + y,
				m_vecMonsB[i].png.GetWidth()/2+5 + x,
				m_vecMonsB[i].png.GetHeight()/2+5 + y);
			m_vecMonsB[i].speed = m_score / 1000 + 4;
			m_vecMonsB[i].isOn = true;
			m_vecMonsB[i].haveblt = true;
			break;
		}
		PlaySound(TEXT("res\\music\\die.wBv"),
			NULL, SND_FILENAME | SND_ASYNC);
	}
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch (m_state)
	{
	case START:
	{
		//如果点击了开始
		if (m_startBk.rect.PtInRect(point))
		{
			//进入游戏状态
			m_state = RUNNING;
			StartGame();
		}
	}
	break;

	case RUNNING:
	{
		//如果点击暂停或继续
		if (m_stopGoOn.rect.PtInRect(point))
		{
			if (m_stopGoOn.isStop)
			{
				StartGame();
				m_stopGoOn.isStop = FALSE;
			}
			else
			{
				EndGame();
				m_stopGoOn.isStop = TRUE;
			}
			InvalidateRect(NULL, FALSE);
		}
	}
	break;

	case END:
	{
		if (m_endBk.rRes.PtInRect(point))
		{
			for (size_t i = 0; i<m_vecMonsA.size(); i++)
			{
				if (m_vecMonsA[i].isOn)
				{
					m_vecMonsA[i].isOn = false;
				}
			}
			m_state = RUNNING;
			StartGame();
		}

		else if (m_endBk.rExit.PtInRect(point))
		{
			if (IDYES == MessageBox(_T("\t真的要离开吗?"), _T("SpaceWar"), MB_YESNO))
			{
				//退出程序
				exit(0);
			}
		}
	}
	break;

	CWnd::OnLButtonDown(nFlags, point);
	}
}


void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	/*
		程序退出之前, 释放所有内存资源
		*/

		EndGame();

	//释放开始界面资源
	m_startBk.bk.ReleaseGDIPlus();
	m_startBk.normal.ReleaseGDIPlus();
	m_startBk.selected.ReleaseGDIPlus();

	//释放游戏界面资源
	m_gameBk.fir.ReleaseGDIPlus();
	m_gameBk.sec.ReleaseGDIPlus();

	m_hero.ReleaseGDIPlus();

	for (size_t i = 0; i<m_bullet.size(); i++)
	{
		m_bullet[i].png.ReleaseGDIPlus();
	}

	for (size_t i = 0; i<m_vecMonsA.size(); i++)
	{
		m_vecMonsA[i].png.ReleaseGDIPlus();
	}

	//释放结束界面资源
	m_endBk.bk.ReleaseGDIPlus();
	m_endBk.gameover.ReleaseGDIPlus();
}
