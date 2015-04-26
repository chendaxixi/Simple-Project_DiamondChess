#include "diamondchess.h"

int DiamondChess::Block::gameOver = 0;
int DiamondChess::Block::disappearTime = DISAPPEARTIME;

DiamondChess::DiamondChess(QWidget *parent)
	: QMainWindow(parent), 
	menuGame(NULL), newGame(NULL), g_Size(NULL), g_Size1(NULL), g_Size2(NULL),
	g_Num(NULL), g_Num1(NULL), g_Num2(NULL), g_Num3(NULL), quit(NULL), menuHelp(NULL),
	versionHelp(NULL), help(NULL), b_newGame(NULL), b_quit(NULL), hint(NULL),
	scoreRed(NULL), scoreBlue(NULL), blockLeft(NULL), chessFrame(NULL),
	buttonFrame(NULL), scoreFrame(NULL)
{
	ui.setupUi(this);
	InitWindow();
}

DiamondChess::~DiamondChess()
{

}

void DiamondChess::InitWindow()
{
	menuGame = new QMenu(tr("&Game"), this);
	menuHelp = new QMenu(tr("&Help"), this);
	g_Size = new QMenu(tr("&Size"), menuGame);
	g_Num = new QMenu(tr("&Num"), menuGame);
	lose = new QAction(tr("&End Game"), menuGame);
	newGame = new QAction(tr("&New"), menuGame);
	quit = new QAction(tr("&Quit"), menuGame);
	help = new QAction(tr("&Help"), menuHelp);
	versionHelp = new QAction(tr("&Version"), menuHelp);
	g_Size1 = new QAction(tr("8"), g_Size);
	g_Size2 = new QAction(tr("10"), g_Size);
	g_Num1 = new QAction(tr("3"), g_Num);
	g_Num2 = new QAction(tr("4"), g_Num);
	g_Num3 = new QAction(tr("5"), g_Num);
	buttonFrame = new QFrame(this);
	chessFrame = new QFrame(this);
	scoreFrame = new QFrame(this);
	b_newGame = new QPushButton(tr("New Game"), buttonFrame);
	b_quit = new QPushButton(tr("Quit"), buttonFrame);
	hint = new QLabel(buttonFrame);
	scoreRed = new QLabel(scoreFrame);
	scoreBlue = new QLabel(scoreFrame);
	blockLeft = new QLabel(scoreFrame);

	connect(newGame, SIGNAL(triggered()), this, SLOT(InitGame()));
	connect(quit, SIGNAL(triggered()), this, SLOT(close()));
	connect(help, SIGNAL(triggered()), this, SLOT(ShowHelp()));
	connect(versionHelp, SIGNAL(triggered()), this, SLOT(ShowVersionHelp()));
	connect(b_newGame, SIGNAL(clicked()), this, SLOT(InitGame()));
	connect(b_quit, SIGNAL(clicked()), this, SLOT(close()));
	connect(g_Size1, SIGNAL(triggered()), this, SLOT(ChooseSize1()));
	connect(g_Size2, SIGNAL(triggered()), this, SLOT(ChooseSize2()));
	connect(g_Num1, SIGNAL(triggered()), this, SLOT(ChooseNum1()));
	connect(g_Num2, SIGNAL(triggered()), this, SLOT(ChooseNum2()));
	connect(g_Num3, SIGNAL(triggered()), this, SLOT(ChooseNum3()));
	connect(lose, SIGNAL(triggered()), this, SLOT(Lose()));

	g_Size->addAction(g_Size1);
	g_Size->addAction(g_Size2);
	g_Num->addAction(g_Num1);
	g_Num->addAction(g_Num2);
	g_Num->addAction(g_Num3);
	menuBar()->addMenu(menuGame);
	menuBar()->addSeparator();
	menuBar()->addMenu(menuHelp);
	menuGame->addAction(newGame);
	menuGame->addMenu(g_Size);
	menuGame->addMenu(g_Num);
	menuGame->addAction(lose);
	menuGame->addSeparator();
	menuGame->addAction(quit);
	menuHelp->addAction(help);
	menuHelp->addSeparator();
	menuHelp->addAction(versionHelp);

	newGame->setShortcut(QKeySequence(Qt::Key_F5));
	quit->setShortcut(QKeySequence(Qt::Key_Escape));
	help->setShortcut(QKeySequence(Qt::Key_F7));
	versionHelp->setShortcut(QKeySequence(Qt::Key_F8));
	lose->setShortcut(QKeySequence(Qt::Key_F2));

	g_Num1->setIcon(QIcon(ICONAME));
	g_Num2->setIcon(QIcon(ICONAME));
	g_Num3->setIcon(QIcon(ICONAME));
	g_Size1->setIcon(QIcon(ICONAME));
	g_Size2->setIcon(QIcon(ICONAME));
/*	g_Size1->setCheckable(true);
	g_Size2->setCheckable(true);
	g_Num1->setCheckable(true);
	g_Num2->setCheckable(true);
	g_Num3->setCheckable(true);
	if (i == 1) g_Size1->setChecked(true);
	else g_Size2->setChecked(true);
	if (j == 1) g_Num1->setChecked(true);
	else if (j == 2) g_Num2->setChecked(true);
	else g_Num3->setChecked(true);	*/
	flag_Size1 = true;
	flag_Size2 = false;
	flag_Num1 = true;
	flag_Num2 = false;
	flag_Num3 = false;
	g_Num1->setIconVisibleInMenu(flag_Num1);
	g_Num2->setIconVisibleInMenu(flag_Num2);
	g_Num3->setIconVisibleInMenu(flag_Num3);
	g_Size1->setIconVisibleInMenu(flag_Size1);
	g_Size2->setIconVisibleInMenu(flag_Size2);

	QPalette pal;
	pal.setColor(QPalette::WindowText, Qt::white);
//	hint->setPalette(pal);
	scoreRed->setPalette(pal);
	scoreBlue->setPalette(pal);
	blockLeft->setPalette(pal);
	modeNumber = MODE1;
	boardHei = BOARDHEI1;
	boardWid = BOARDWID1;
	InitGame();
}

void DiamondChess::SetSize()
{
	QSize qSize;
	QSize sizeMenu = menuBar()->size();
	qSize.setWidth((boardWid + 4) * CHESSWID);
	qSize.setHeight((boardHei + 4) * CHESSHEI + 3 * GRID + sizeMenu.height());
	this->setFixedSize(qSize);
	
	buttonFrame->setGeometry(0, sizeMenu.height(), qSize.width(), GRID);
	scoreFrame->setGeometry(0, sizeMenu.height() + GRID, qSize.width(), 2 * GRID);
	chessFrame->setGeometry(0, sizeMenu.height() + 3*GRID, qSize.width(), 
		qSize.height() - sizeMenu.height() - 3*GRID);
	b_newGame->setGeometry(0.25 * CHESSWID, (GRID - CHESSHEI) / 2, 
		((boardWid+4)/4 - 1) * CHESSWID, CHESSHEI);
	b_quit->setGeometry((boardWid+4)/4 * CHESSWID, (GRID - CHESSHEI) / 2, 
		((boardWid+4)/4 - 1) * CHESSWID, CHESSHEI);
	hint->setGeometry((boardWid+4)/2 * CHESSWID, (GRID - CHESSHEI) / 2, 
		(boardWid+4)/2 * CHESSWID, CHESSHEI);
	scoreRed->setGeometry(CHESSWID, (GRID - CHESSHEI) / 2, (boardWid/2 - 1) * CHESSWID, CHESSHEI);
	scoreBlue->setGeometry(CHESSWID, GRID + (GRID - CHESSHEI) / 2, 
		(boardWid/2 - 1) * GRID, CHESSHEI);
	blockLeft->setGeometry((boardWid/2 + 1) * CHESSWID, (2*GRID - CHESSHEI) / 2, 
		(boardWid/2 - 1) * GRID, CHESSHEI);
	blockHint.SetFather(this);
	blockHint.SetPos((boardWid+4-1.5)*CHESSWID, sizeMenu.height() + (GRID - CHESSHEI) / 2);
	int tempY = chessFrame->y();
	for (int i = 0;i < boardHei;i++)
		for (int j = 0;j < boardWid;j++)
		{
			block[i][j].SetFather(this);
			block[i][j].SetPos((2+j) * CHESSWID, tempY + (2+i) * CHESSHEI);
//			block[i][j].setGeometry((2+j) * CHESSWID, tempY + (2+i) * CHESSHEI,
//				CHESSWID, CHESSHEI);
		}
}

void DiamondChess::InitGame()
{
	QFont font;
	font.setPixelSize(boardHei/2 + 10);
//	hint->setFont(font);
	scoreRed->setFont(font);
	scoreBlue->setFont(font);
	blockLeft->setFont(font);
	hint->setText(tr("Player Blue First."));
	scoreRed->setText(tr("Red Score: 0"));
	scoreBlue->setText(tr("Blue Score: 0"));
	if (boardHei == BOARDHEI1)
	{
		Block::disappearTime = DISAPPEARTIME;
		blockLeft->setText(tr("Block Left: 64"));
	}
	else if (boardHei == BOARDHEI2)
	{
		Block::disappearTime = DISAPPEARTIME / 2;
		blockLeft->setText(tr("Block Left: 100"));
	}
	Block::gameOver = 0;
	m_chess0n = 0;
	m_turn = 0;
	m_score[0] = 0;
	m_score[1] = 0;
	for (int i = 0;i < boardHei;i++)
		for (int j = 0;j < boardWid;j++)
		{
			block[i][j].SetState(0);
			block[i][j].m_disappear = Block::disappearTime;
		}
	SetSize();
	this->update();
}

void DiamondChess::ShowHelp()
{
	QMessageBox::about(this, tr("Help"), 
		tr("Play chess in turn.One player is Blue and the other is Red\n"
			"Rules:\n\t1.Blue First\n"
			"\t2.When your continuous diamonds in certain row or line are no less than Num"
			"(3/4/5,you can set it at the menu),"
			"they will disappear and you will get the equal score.\n"
			"So let's begin the fight!"));
}

void DiamondChess::ShowVersionHelp()
{
	QMessageBox::about(this, tr("Version"), 
		tr("The game version is 1.2.\n"
		"\t\tauthor:Chendaxixi\n"
		"\t\t2014.7.26"));
}

void DiamondChess::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QBrush brush;
	QLinearGradient* linearGradient;
/*	linearGradient = new QLinearGradient(scoreFrame->x(), scoreFrame->y(), 
		scoreFrame->x() + 6*CHESSWID, scoreFrame->y() + GRID); 
	linearGradient->setColorAt(0, Qt::red); 
	linearGradient->setColorAt(1.0, Qt::black); 
	brush = QBrush(*linearGradient);
	painter.setBrush(brush);
	painter.drawRect(scoreFrame->x(), scoreFrame->y(), 6*CHESSWID, GRID);	
	delete linearGradient;

	linearGradient = new QLinearGradient(scoreFrame->x(), scoreFrame->y()+GRID,
		scoreFrame->x() + 6*CHESSWID, scoreFrame->y() + 2 * GRID);
	linearGradient->setColorAt(0, Qt::black); 
	linearGradient->setColorAt(1.0, Qt::blue); 
	brush = QBrush(*linearGradient);
	painter.setBrush(brush);
	painter.drawRect(scoreFrame->x(), scoreFrame->y()+GRID, 6*CHESSWID, GRID);
	delete linearGradient;	*/

	linearGradient = new QLinearGradient(scoreFrame->x()+(boardWid+4)/2*CHESSWID, scoreFrame->y(),
		scoreFrame->x() + (boardWid+4)/2*CHESSWID, scoreFrame->y() + 2 * GRID);
	linearGradient->setColorAt(0, Qt::red); 
	linearGradient->setColorAt(1, Qt::blue); 
	brush = QBrush(*linearGradient);
	painter.setBrush(brush);
	painter.drawRect(scoreFrame->x(), scoreFrame->y(), (boardWid+4)*CHESSWID, 2*GRID);
	delete linearGradient;	

	QRadialGradient* radialGradient = new QRadialGradient
		(QPointF(chessFrame->x()+chessFrame->width()/2, chessFrame->y()+chessFrame->height()/2),
		(boardWid+4)/2*1.414*CHESSHEI, 
		QPointF(chessFrame->x()+chessFrame->width()/2, chessFrame->y()+chessFrame->height()/2));
	radialGradient->setColorAt(0, QColor(255,0,0));
	radialGradient->setColorAt(1, QColor(0,0,0));
	brush = QBrush(*radialGradient);
	painter.setBrush(brush);
	painter.drawRect(chessFrame->x(),chessFrame->y(),chessFrame->width(),chessFrame->height());
	delete radialGradient;

	radialGradient = new QRadialGradient
		(QPointF(chessFrame->x()+chessFrame->width()/2, chessFrame->y()+chessFrame->height()/2),
		boardWid/2*1.414*CHESSHEI, 
		QPointF(chessFrame->x()+chessFrame->width()/2, chessFrame->y()+chessFrame->height()/2));
	radialGradient->setColorAt(0, QColor(0,0,0));
	radialGradient->setColorAt(1, QColor(0,0,255));
	brush = QBrush(*radialGradient);
	painter.setBrush(brush);
	painter.drawRect(
		chessFrame->x() + 2*CHESSWID, 
		chessFrame->y() + 2*CHESSHEI, 
		boardWid*CHESSWID,
		boardHei*CHESSHEI); 
	delete radialGradient;
	painter.end();	
	for (int i = 0;i < boardHei;i++)
		for (int j = 0;j < boardWid;j++)
			block[i][j].Show();
	if (Block::gameOver == 0)
	{
		blockHint.SetState(m_turn+1);
		blockHint.Show();
	}
}

void DiamondChess::mousePressEvent(QMouseEvent *e)
{
	if (e->button() != Qt::LeftButton) return;
	QPoint qPos = e->pos();
	QPoint chess = ChessFind(qPos);
	if (chess.x() < 0) return;
	ChessDown(chess.x(), chess.y());
}

QPoint DiamondChess::ChessFind(QPoint& qPos)
{
	int j = qPos.x() / CHESSWID - 2;
	if (j < 0) return QPoint(-1,-1);
	if (j >= boardWid) return QPoint(-1,-1);
	int tempY = chessFrame->y();
	int i = (qPos.y() - tempY) / CHESSHEI - 2;
	if (i < 0) return QPoint(-1,-1);
	if (i >= boardHei) return QPoint(-1,-1);
	return QPoint(i,j);
}

void DiamondChess::ChessDown(int i, int j)
{
	if (block[i][j].State() > 0) return;
	if (m_turn == 0) 
		block[i][j].SetState(1);
	else 
		block[i][j].SetState(2);
	m_chess0n++;
	int num = 1;
	int state = block[i][j].State();
	int temp1 = i - 1, temp2 = i + 1;
	int flag = 0;
	for (int k = i-1;k >= -1;k--)
	{
		if (k == -1)
		{
			temp1 = k;
			break;
		}
		if (block[k][j].State() != state) 
		{
			temp1 = k;
			break;
		}
		num++;
	}
	for (int k = i+1;k <= boardHei;k++)
	{
		if (k == boardHei)
		{
			temp2 = k;
			break;
		}
		if (block[k][j].State() != state)
		{
			temp2 = k;
			break;
		}
		num++;
	}
	if (num >= modeNumber)
	{
		for (int k = temp1 + 1;k < temp2;k++)
			block[k][j].SetState(5 - state);
		m_score[m_turn] += num;
		flag = 1;
		m_chess0n -= num;
	}
	num = 1;
	temp1 = j-1;
	temp2 = j+1;
	for (int k = j-1;k >= -1;k--)
	{
		if (k == -1)
		{
			temp1 = k;
			break;
		}
		if (block[i][k].State() != state) 
		{
			temp1 = k;
			break;
		}
		num++;
	}
	for (int k = j+1;k <= boardWid;k++)
	{
		if (k == boardWid)
		{
			temp2 = k;
			break;
		}
		if (block[i][k].State() != state)
		{
			temp2 = k;
			break;
		}
		num++;
	}
	if (num >= modeNumber)
	{
		for (int k = temp1 + 1;k < temp2;k++)
			block[i][k].SetState(5 - state);
		m_score[m_turn] += num;
		m_chess0n -= num;
		if (flag == 1) m_chess0n++;
		flag = 1;
	}
	char str[50];
	if (flag == 1)
	{
		if (m_turn == 1)
		{
			sprintf(str, "Red Score: %d", m_score[1]);
			scoreRed->setText(str);
		}
		else
		{
			sprintf(str, "Blue Score: %d", m_score[0]);
			scoreBlue->setText(str);
		}
		hint->setText("Good Job!!!");
	}
	m_turn = 1 - m_turn;
	sprintf(str, "Block Left: %d", boardHei * boardWid - m_chess0n);
	blockLeft->setText(str);
	this->repaint();
	if(block[i][j].m_disappear != DISAPPEARTIME)
		for (int k = 0;k < DISAPPEARTIME;k++)
			this->repaint();
	if (m_turn == 0)
		hint->setText(tr("Now turn to Blue."));
	else
		hint->setText(tr("Now turn to Red."));
	this->repaint();
	if (m_chess0n == boardHei * boardWid)
		EndGame();
}

void DiamondChess::EndGame()
{
	Block::gameOver = 1;
	if (m_score[0] > m_score[1])
		hint->setText(tr("Game Over.The Winner is Blue!"));
	else if (m_score[0] < m_score[1])
		hint->setText(tr("Game Over.The Winner is Red!"));
	else
		hint->setText(tr("Game Over.A draw in chess!"));
	for (int i = boardHei/2 -1;i >= 0;i--)
	{
		for (int j = i;j <  boardWid - i;j++)
		{
			block[i][j].SetState(block[i][j].State()+2);
			block[boardHei-i-1][j].SetState(block[boardHei-i-1][j].State()+2);
		}
		for (int j = i+1;j < boardHei - i - 1;j++)
		{
			block[j][i].SetState(block[j][i].State()+2);
			block[j][boardHei-i-1].SetState(block[j][boardHei-i-1].State()+2);
		}
		this->repaint();
		for (int k = 0;k < DISAPPEARTIME;k++)
			this->repaint();
	}
}

void DiamondChess::ChooseNum1()
{
/*	if (g_Num1->isChecked()) return;
	g_Num2->setChecked(false);
	g_Num3->setChecked(false);
	g_Num1->setChecked(true);	*/
	if (flag_Num1) return;
	flag_Num3 = false;
	flag_Num2 = false;
	flag_Num1 = true;
	g_Num1->setIconVisibleInMenu(flag_Num1);
	g_Num2->setIconVisibleInMenu(flag_Num2);
	g_Num3->setIconVisibleInMenu(flag_Num3);
	modeNumber = MODE1;
	InitGame();
}

void DiamondChess::ChooseNum2()
{
/*	if (g_Num2->isChecked()) return;
	g_Num1->setChecked(false);
	g_Num3->setChecked(false);
	g_Num2->setChecked(true);	*/
	if (flag_Num2) return;
	flag_Num3 = false;
	flag_Num1 = false;
	flag_Num2 = true;
	g_Num1->setIconVisibleInMenu(flag_Num1);
	g_Num2->setIconVisibleInMenu(flag_Num2);
	g_Num3->setIconVisibleInMenu(flag_Num3);
	modeNumber = MODE2;
	InitGame();
}

void DiamondChess::ChooseNum3()
{
/*	if (g_Num3->isChecked()) return;
	g_Num2->setChecked(false);
	g_Num1->setChecked(false);
	g_Num3->setChecked(true);	*/
	if (flag_Num3) return;
	flag_Num1 = false;
	flag_Num2 = false;
	flag_Num3 = true;
	g_Num1->setIconVisibleInMenu(flag_Num1);
	g_Num2->setIconVisibleInMenu(flag_Num2);
	g_Num3->setIconVisibleInMenu(flag_Num3);
	modeNumber = MODE3;
	InitGame();
}

void DiamondChess::ChooseSize1()
{
/*	if(g_Size1->isChecked()) return;
	g_Size2->setChecked(false);
	g_Size1->setChecked(true);	*/
	if (flag_Size1) return;
	flag_Size2 = false;
	flag_Size1 = true;
	g_Size1->setIconVisibleInMenu(flag_Size1);
	g_Size2->setIconVisibleInMenu(flag_Size2);
	boardHei = BOARDHEI1;
	boardWid = BOARDWID1;
	InitGame();
}

void DiamondChess::ChooseSize2()
{
/*	if(g_Size2->isChecked()) return;
	g_Size1->setChecked(false);
	g_Size2->setChecked(true);	*/
	if (flag_Size2) return;
	flag_Size1 = false;
	flag_Size2 = true;
	g_Size1->setIconVisibleInMenu(flag_Size1);
	g_Size2->setIconVisibleInMenu(flag_Size2);
	boardHei = BOARDHEI2;
	boardWid = BOARDWID2;
	InitGame();
}

void DiamondChess::Lose()
{
	if (Block::gameOver == 1) return;
	Block::gameOver = 1;
	if (m_score[0] > m_score[1])
		hint->setText(tr("Game Over.The Winner is Blue!"));
	else if (m_score[0] < m_score[1])
		hint->setText(tr("Game Over.The Winner is Red!"));
	else
		hint->setText(tr("Game Over.A draw in chess!"));
	for (int i = boardHei/2 -1;i >= 0;i--)
	{
		for (int j = i;j <  boardWid - i;j++)
		{
			block[i][j].SetState(5-block[i][j].State());
			block[boardHei-i-1][j].SetState(5-block[boardHei-i-1][j].State());
		}
		for (int j = i+1;j < boardHei - i - 1;j++)
		{
			block[j][i].SetState(5-block[j][i].State());
			block[j][boardHei-i-1].SetState(5-block[j][boardHei-i-1].State());
		}
		this->repaint();
		for (int k = 0;k < DISAPPEARTIME;k++)
			this->repaint();
	}
}
