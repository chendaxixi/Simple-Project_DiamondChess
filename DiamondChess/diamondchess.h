#ifndef DIAMONDCHESS_H
#define DIAMONDCHESS_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets\qpushbutton.h>
#include <QtWidgets\qlabel.h>
#include <QtWidgets\qframe.h>
#include <QtWidgets\qmenu.h>
#include <QtWidgets\qaction.h>
#include <qpainter.h>
#include <qevent.h>
#include <qmessagebox.h>
#include <qfont.h>
#include <qpalette.h>
#include <tchar.h>
#include "ui_diamondchess.h"

#define ICONAME "DiamondChess.ico"
#define BOARDWID1 8
#define BOARDHEI1 8
#define BOARDWID2 10
#define BOARDHEI2 10
#define CHESSWID 30
#define CHESSHEI 30
#define GRID 40
#define DISAPPEARTIME 30
#define MODE1 3
#define MODE2 4
#define MODE3 5

class DiamondChess : public QMainWindow
{
	Q_OBJECT

public:
	DiamondChess(QWidget *parent = 0);
	~DiamondChess();

public:
class Block
{
public:
	Block(QWidget *parent = 0):m_parent(parent), m_disappear(DISAPPEARTIME),
		m_x(0), m_y(0), m_wid(CHESSWID), m_hei(CHESSHEI), m_state(0)
	{
	}

public:
	void SetFather(QWidget* parent){m_parent = parent;}
	void SetState(int state){m_state = state;}
	void SetPos(int x, int y){m_x = x;m_y = y;}
	int State(){return m_state;}
	void Show()
	{
		QPainter painter(m_parent);
		painter.setRenderHint(QPainter::Antialiasing, true);
		QPen pen(QColor(255,255,255));
		QBrush brush;
		QRadialGradient radialGradient
			(QPointF(m_x+m_wid/2, m_y+m_hei/2), CHESSWID/2, QPointF(m_x+m_wid/2, m_y+m_hei/2));
		switch (m_state)
		{
		case 0:
//			radialGradient.setColorAt(0, QColor(255,255,255));
		//	radialGradient.setColorAt(0.5, QColor(0,191,0));
	//		radialGradient.setColorAt(1, QColor(0,0,0));
	//		brush = QBrush(radialGradient);
			break;
		case 1:
			radialGradient.setColorAt(0, QColor(0, 191, 191));
		//	radialGradient.setColorAt(1, QColor(255,255,255));
			radialGradient.setColorAt(1, QColor(0,0,0));
			brush = QBrush(radialGradient);
			break;
		case 2:
			radialGradient.setColorAt(0, QColor(255,86,86));
		//	radialGradient.setColorAt(1, QColor(255,255,255));
			radialGradient.setColorAt(1, QColor(0,0,0));
			brush = QBrush(radialGradient);
			break;
		case 4:
			radialGradient.setColorAt(0, QColor(0, 191, 191));
		//	radialGradient.setColorAt(1, QColor(255,255,255));
			radialGradient.setColorAt((double)m_disappear/disappearTime, QColor(0,0,0));
			brush = QBrush(radialGradient);
			m_disappear--;
			if (m_disappear < 0)
				if (gameOver != 1)
				{
					m_disappear = disappearTime;
					m_state = 0;
				}
				else
					m_disappear = 0;
			break;
		case 3:
			radialGradient.setColorAt(0, QColor(255,86,86));
		//	radialGradient.setColorAt(1, QColor(255,255,255));
			radialGradient.setColorAt((double)m_disappear/disappearTime, QColor(0,0,0));
			brush = QBrush(radialGradient);
			m_disappear--;
			if (m_disappear < 0)
				if (gameOver != 1)
				{
					m_disappear = disappearTime;
					m_state = 0;
				}
				else
					m_disappear = 0;
			break;
		case 5:
			radialGradient.setColorAt(0, QColor(255,255,255));
		//	radialGradient.setColorAt(1, QColor(255,255,255));
			radialGradient.setColorAt((double)m_disappear/disappearTime, QColor(0,0,0));
			brush = QBrush(radialGradient);
			m_disappear--;
			if (m_disappear < 0)
				if (gameOver != 1)
				{
					m_disappear = disappearTime;
					m_state = 0;
				}
				else
					m_disappear = 0;
			break;
		default:
			break;
		}	
		painter.setPen(pen);
		painter.setBrush(brush);
		painter.drawRect(QRect(m_x, m_y, m_wid, m_hei));
		painter.end();
	}

public:
	int m_disappear;
	static int gameOver;
	static int disappearTime;

private:
	int m_x;
	int m_y;
	int m_wid;
	int m_hei;
	int m_state; //0为空,1为蓝，2为红, 4为蓝消失，3为红消失, 5为空消失
	QWidget* m_parent;
};

private slots:
	void InitGame();
	void ShowHelp();
	void ShowVersionHelp();
	void ChooseSize1();
	void ChooseSize2();
	void ChooseNum1();
	void ChooseNum2();
	void ChooseNum3();
	void Lose();

public:
	void InitWindow();
	void EndGame();
	void SetSize();
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	QPoint ChessFind(QPoint& p);
	void ChessDown(int i, int j);

private:
	QMenu* menuGame;
	QAction* newGame;
	QMenu* g_Size;
	QAction* g_Size1;
	QAction* g_Size2;
	QMenu* g_Num;
	QAction* g_Num1;
	QAction* g_Num2;
	QAction* g_Num3;
	QAction* quit;
	QMenu* menuHelp;
	QAction* versionHelp;
	QAction* help;
	QAction* lose;
	QPushButton* b_newGame;
	QPushButton* b_quit;
	QLabel* hint;
	QLabel* scoreRed;
	QLabel* scoreBlue;
	QLabel* blockLeft;
	QFrame* chessFrame;
	QFrame* buttonFrame;
	QFrame* scoreFrame;
	Block block[BOARDHEI2][BOARDWID2];
	Block blockHint;

private:
	int m_turn; //0->Blue, 1->Red
	int m_score[2];
	int m_chess0n;
	int boardWid;
	int boardHei;
	int modeNumber;
	bool flag_Size1;
	bool flag_Size2;
	bool flag_Num1;
	bool flag_Num2;
	bool flag_Num3;

private:
	Ui::DiamondChessClass ui;
};

#endif // DIAMONDCHESS_H
