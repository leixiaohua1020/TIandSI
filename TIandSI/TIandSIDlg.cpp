/**
 * TIandSI
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本工具用于计算视频的时间信息（TI，也称为时间复杂度），以及空间
 * 信息（SI，也称为空间复杂度），以上两个指标取自于ITU-R BT.1788标准。
 * 
 * This software is used to calculate raw video's TI (Temporal perceptual 
 * Information) and SI (Spatial perceptual Information) defined in 
 * ITU-R BT.1788.
 */


#include "stdafx.h"
#include "TIandSI.h"
#include "TIandSIDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTIandSIDlg 对话框




CTIandSIDlg::CTIandSIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTIandSIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_inputyuvheight = 0;
	m_inputyuvwidth = 0;
	//  m_radiodraw = 0;
}

void CTIandSIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YUVLIST, m_yuvlist);
	DDX_Control(pDX, IDC_YUVURL, m_yuvurl);
	DDX_Control(pDX, IDC_INPUT_YUV_PIXFMT, m_inputyuvpixfmt);
	DDX_Text(pDX, IDC_INPUT_YUV_HEIGHT, m_inputyuvheight);
	DDX_Text(pDX, IDC_INPUT_YUV_WIDTH, m_inputyuvwidth);
	DDX_Control(pDX, IDC_OUTPUTSI, m_outputsi);
	DDX_Control(pDX, IDC_OUTPUTTI, m_outputti);
	DDX_Control(pDX, IDC_OUTPUTSIYUV, m_outputsiyuv);
	DDX_Control(pDX, IDC_OUTPUTTIYUV, m_outputtiyuv);
	DDX_Control(pDX, IDC_PROGRESS_ALL, m_progressall);
	DDX_Control(pDX, IDC_PROGRESS_ALL_TEXT, m_progressalltext);
	DDX_Control(pDX, IDC_PROGRESS_CUR_TI, m_progresscurti);
	DDX_Control(pDX, IDC_PROGRESS_CUR_SI, m_progresscursi);
	DDX_Control(pDX, IDC_PROGRESS_CUR_SI_TEXT, m_progresscursitext);
	DDX_Control(pDX, IDC_PROGRESS_CUR_TI_TEXT, m_progresscurtitext);
	DDX_Control(pDX, IDC_CURVAL, m_curval);
	DDX_Control(pDX, IDC_COMBO_DRAW, m_combodraw);
}

BEGIN_MESSAGE_MAP(CTIandSIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTIandSIDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_YUVLIST_ADD, &CTIandSIDlg::OnBnClickedYuvlistAdd)
	ON_BN_CLICKED(IDC_YUVLIST_DELETE, &CTIandSIDlg::OnBnClickedYuvlistDelete)
	ON_BN_CLICKED(IDC_YUVLIST_DELETEALL, &CTIandSIDlg::OnBnClickedYuvlistDeleteall)
	ON_BN_CLICKED(IDABOUT, &CTIandSIDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDPAUSE, &CTIandSIDlg::OnBnClickedPause)
	ON_BN_CLICKED(IDSTOP, &CTIandSIDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDRESULT, &CTIandSIDlg::OnBnClickedResult)
	ON_WM_DROPFILES()
	ON_COMMAND(IDADDDLG, &CTIandSIDlg::OnAdddlg)
	ON_COMMAND(IDWEBSITE, &CTIandSIDlg::OnWebsite)
END_MESSAGE_MAP()


// CTIandSIDlg 消息处理程序

BOOL CTIandSIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//---------------
	m_inputyuvwidth=1920;
	m_inputyuvheight=1080;
	m_inputyuvpixfmt.InsertString(0,L"YUV420P");
	m_inputyuvpixfmt.InsertString(1,L"YUV422P");
	m_inputyuvpixfmt.InsertString(2,L"YUV444P");
	m_inputyuvpixfmt.InsertString(3,L"Y");
	m_inputyuvpixfmt.SetCurSel(0);
	
	m_outputsi.SetCheck(TRUE);
	m_outputsiyuv.SetCheck(TRUE);
	m_outputti.SetCheck(TRUE);
	m_outputtiyuv.SetCheck(TRUE);

	UpdateData(FALSE);
	//初始化
	m_progressalltext.SetWindowText(L"0%");
	m_progresscurtitext.SetWindowText(L"0%");
	m_progresscursitext.SetWindowText(L"0%");
	m_progressall.SetRange(0,100);
	m_progresscurti.SetRange(0,100);
	m_progresscursi.SetRange(0,100);

	//SDL==========================
	sdlparam.graphically==true;
	//SDL_putenv()放在前面
	char variable[256];   
	CWnd* pWnd = GetDlgItem(IDC_SCREEN);  //获取图片控件的窗口指针   
	sprintf(variable,"SDL_WINDOWID=0x%1x",pWnd->GetSafeHwnd()); // 格式化字符串      
	SDL_putenv(variable); 

	if(SDL_Init(SDL_INIT_VIDEO)) {
		AfxMessageBox(L"Could not initialize SDL"); 
		return 0;
	} 


	CRect screenrect;
	GetDlgItem(IDC_SCREEN)->GetWindowRect(screenrect);
	sdlparam.screen = SDL_SetVideoMode(screenrect.Width(), screenrect.Height(), 0, 0);
	if(!sdlparam.screen) {  
		AfxMessageBox(L"SDL: could not set video mode");  
		return 0;
	}

	resultdlg=new ResultDlg;
	resultdlg->Create(IDD_RESULT_DIALOG);
	//-----------------------------
	m_combodraw.InsertString(0,L"显示");
	m_combodraw.InsertString(1,L"不显示");
	m_combodraw.SetCurSel(0);

	SetState(SYSTEM_PREPARE);

	//=============================

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTIandSIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTIandSIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTIandSIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


int CTIandSIDlg::TIandSI(YUVInfo yuvinfo,LPVOID lparam)
{
	CTIandSIDlg *dlg=(CTIandSIDlg *)lparam;;
	//----------------
	CFile cfYUV;
	CFile cfSI;//一个yuv序列，y的数据为8字节的si值
	CFile cfTI;//一个yuv序列，y的数据为8字节的ti值
	CFile cfSIData;//一个y序列，y的数据为float型的si值
	CFile cfTIData;//一个y序列，y的数据为float型的ti值
	FILE* fInfoSI;
	FILE* fInfoTI;

	int nHeight, nWidth;
	int nFrameNum;
	int nYSize;	
	int nUVSize;
	int nFrameSize;

	BYTE *pFrame;
	BYTE **pFrame2D;
	BYTE *pNextFrame;
	BYTE **pNextFrame2D;
	BYTE *NewYBuffer;
	BYTE *NewUVBuffer;

	float *NewSIBuffer;
	float *NewTIBuffer;
	float flSI_A, flSI_S, flTI_A, flTI_S;

	int i, j, k, m, n;

	/****************初始化设置**********************/
	nHeight = yuvinfo.h;
	nWidth = yuvinfo.w;
	nYSize = nHeight*nWidth;

	switch (yuvinfo.pixfmt)
	{
	case YUV420P:{
		nUVSize = nYSize*0.5;//4:2:0
				 }
	case YUV422P:{
		nUVSize = nYSize;//4:2:0
				 }
	case YUV444P:{
		nUVSize = nYSize*2;//4:2:0
				 }
	case Y:{
		nUVSize = 0;//4:2:0
				 }
	default:{
		nUVSize = nYSize*0.5;//4:2:0
			}
	}
	
	
	nFrameSize = nYSize + nUVSize;

	i = 0;
	j = 0;
	k = 0;
	m = n = 0;
	flSI_A = flSI_S = flTI_A = flTI_S = 0;

	pFrame = new BYTE [nYSize];
	pFrame2D = new BYTE *[nHeight];
	for(i = 0; i < nHeight; i++)
		pFrame2D[i] = pFrame + i*nWidth;

	pNextFrame = new BYTE [nYSize];
	pNextFrame2D = new BYTE *[nHeight];
	for(i = 0; i < nHeight; i++)
		pNextFrame2D[i] = pNextFrame + i*nWidth;

	NewYBuffer = new BYTE [nYSize];
	NewUVBuffer = new BYTE [nUVSize];
	memset(NewUVBuffer, 0x80, nUVSize);

	NewSIBuffer = new float [nYSize];
	NewTIBuffer = new float [nYSize];

	cfYUV.Open(yuvinfo.url, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead); 
	nFrameNum = cfYUV.GetLength()/nFrameSize;

	char name[255];
	char name1[255];
	DWORD len = WideCharToMultiByte(CP_OEMCP,NULL,(LPCTSTR)yuvinfo.url,-1,NULL,0,NULL,FALSE);
	WideCharToMultiByte(CP_OEMCP,NULL,(LPCTSTR)yuvinfo.url,-1,name,len,NULL,FALSE);
	//创建文件
	if(m_outputsi.GetCheck()!=FALSE){
		sprintf(name1, "%s_SI.csv", name);
		fInfoSI = fopen(name1, "w+");
	}
	if(m_outputti.GetCheck()!=FALSE){
		sprintf(name1,"%s_TI.csv", name);
		fInfoTI = fopen(name1, "w+");
	}
	if(m_outputsiyuv.GetCheck()!=FALSE){
		cfSI.Open(yuvinfo.url + _T("_SI.yuv"), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite); 
	}
	//cfSIData.Open(strPath + _T("_SIData.y"), CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);

	//Draw--------------
	sdlparam.bmp = SDL_CreateYUVOverlay(nWidth, nHeight,SDL_YV12_OVERLAY, sdlparam.screen); 
	//FIX
	dlg->m_progresscurti.SetPos(0);
	dlg->m_progresscurtitext.SetWindowText(L"0%");
	dlg->m_progresscursi.SetPos(0);
	dlg->m_progresscursitext.SetWindowText(L"0%");

	/********************计算SI***********************/
	for(k = 0;k < nFrameNum; k++)//如果n帧，循环n-1次
	{
		//Check State-----
		while(dlg->sysstate==SYSTEM_PAUSE){
			Sleep(1000);
		}
		if(dlg->sysstate==SYSTEM_PREPARE){
			return -1;
		}
		//----------------

		cfYUV.Seek(k*nFrameSize, CFile::begin);
		cfYUV.Read(pFrame, nYSize);

		for(j = 0; j < nHeight; j++)
			for(i = 0; i < nWidth; i++)
			{
				float XSobel, YSobel, SobelValue;

				if((j == 0) && (i == 0))//(0, 0)
				{
					XSobel = -3*pFrame2D[0][0] + 3*pFrame2D[0][i+1]				
							-pFrame2D[j+1][0] + pFrame2D[j+1][i+1];

					YSobel = 3*pFrame2D[0][0] + pFrame2D[0][i+1]
							-3*pFrame2D[j+1][0] - pFrame2D[j+1][i+1];
				}
				else if((j == 0) && (i == (nWidth - 1)))//(nWidth - 1, 0)
				{
					XSobel = -3*pFrame2D[0][i-1] + 3*pFrame2D[0][i]
							-pFrame2D[j+1][i-1] + pFrame2D[j+1][i];

					YSobel = pFrame2D[0][i-1] + 3*pFrame2D[0][i]
							-pFrame2D[j+1][i-1] - 3*pFrame2D[j+1][i];
				}
				else if((j == (nHeight - 1)) && (i == 0))//(0, nHeight - 1)
				{
					XSobel = -pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
							-3*pFrame2D[j][0] + 3*pFrame2D[j][i+1];				

					YSobel = 3*pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
							-3*pFrame2D[j][0] - pFrame2D[j][i+1];
				}
				else if((j == (nHeight - 1)) && (i == (nWidth - 1)))//(nWidth - 1, nHeight - 1)
				{
					XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i]
							-3*pFrame2D[j][i-1] + 3*pFrame2D[j][i];				

					YSobel = pFrame2D[j-1][i-1] + 3*pFrame2D[j-1][i]
							-pFrame2D[j][i-1] - 3*pFrame2D[j][i];
				}
				else if(j == 0)//上面一排
				{
					XSobel = -3*pFrame2D[0][i-1] + 3*pFrame2D[0][i+1]
							-pFrame2D[j+1][i-1] + pFrame2D[j+1][i+1];

					YSobel = pFrame2D[0][i-1] + 2*pFrame2D[0][i] + pFrame2D[0][i+1]
							-pFrame2D[j+1][i-1] - 2*pFrame2D[j+1][i] - pFrame2D[j+1][i+1];
				}
				else if(j == (nHeight - 1))//下面一排
				{
					XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i+1]
							-3*pFrame2D[j][i-1] + 3*pFrame2D[j][i+1];

					YSobel = pFrame2D[j-1][i-1] + 2*pFrame2D[j-1][i] + pFrame2D[j-1][i+1]
							-pFrame2D[j][i-1] - 2*pFrame2D[j][i] - pFrame2D[j][i+1];
				}
				else if(i == 0)//左边一排
				{
					XSobel = -pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
							-2*pFrame2D[j][0] + 2*pFrame2D[j][i+1]				
							-pFrame2D[j+1][0] + pFrame2D[j+1][i+1];

					YSobel = 3*pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
							-3*pFrame2D[j+1][0] - pFrame2D[j+1][i+1];
				}
				else if(i == (nWidth - 1))//右面一排
				{
					XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i]
							-2*pFrame2D[j][i-1] + 2*pFrame2D[j][i]				
							-pFrame2D[j+1][i-1] + pFrame2D[j+1][i];

					YSobel = pFrame2D[j-1][i-1] + 3*pFrame2D[j-1][i]
							-pFrame2D[j+1][i-1] - 3*pFrame2D[j+1][i];
				}
				else
				{
					XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i+1]
							-2*pFrame2D[j][i-1] + 2*pFrame2D[j][i+1]				
							-pFrame2D[j+1][i-1] + pFrame2D[j+1][i+1];

					YSobel = pFrame2D[j-1][i-1] + 2*pFrame2D[j-1][i] + pFrame2D[j-1][i+1]
							-pFrame2D[j+1][i-1] - 2*pFrame2D[j+1][i] - pFrame2D[j+1][i+1];
				}

				SobelValue = sqrt(pow(XSobel, 2) + pow(YSobel, 2));

				NewYBuffer[j*nWidth + i] = (BYTE)SobelValue;
				NewSIBuffer[j*nWidth + i] = SobelValue;
			}

			for(m = 1; m < (nHeight - 1); m++)
				for(n = 1; n < (nWidth - 1); n++)
				{
					flSI_A = flSI_A + NewSIBuffer[m*nWidth + n];
				}
			flSI_A = flSI_A/((nWidth - 2)*(nHeight - 2));

			for(m = 1; m < (nHeight - 1); m++)
				for(n = 1; n < (nWidth - 1); n++)
				{
					flSI_S = flSI_S + pow((NewSIBuffer[m*nWidth+n]-flSI_A), 2);
				}
			flSI_S = sqrt(flSI_S/((nWidth - 2)*(nHeight - 2) - 1));
			//输出SI
			if(m_outputsi.GetCheck()!=FALSE){
			fprintf(fInfoSI, "%f\n", flSI_S);
			}
			si.push_back(flSI_S);

			CString curvalstr;
			curvalstr.Format(L"%.3f",flSI_S);
			dlg->m_curval.SetWindowText(curvalstr);

		if(m_outputsiyuv.GetCheck()!=FALSE){
			cfSI.Write(NewYBuffer, nYSize);
			cfSI.Write(NewUVBuffer, nUVSize);
		}
		//浮点型数据先不输出
		//cfSIData.Write(NewSIBuffer, nYSize*sizeof(float));
		if(dlg->m_combodraw.GetCurSel()==0){
			sdlparam.graphically=true;
		}else{
			sdlparam.graphically=false;
		}

		if(sdlparam.graphically==true){
			SDL_LockYUVOverlay(sdlparam.bmp);
			sdlparam.bmp->pixels[0]=NewYBuffer;
			sdlparam.bmp->pixels[2]=NewUVBuffer;
			sdlparam.bmp->pixels[1]=NewUVBuffer+nUVSize/2;     
			sdlparam.bmp->pitches[0]=nWidth;
			sdlparam.bmp->pitches[2]=nWidth/2;   
			sdlparam.bmp->pitches[1]=nWidth/2;
			SDL_UnlockYUVOverlay(sdlparam.bmp); 
			CRect screenrect;
			dlg->GetDlgItem(IDC_SCREEN)->GetWindowRect(screenrect);
			sdlparam.rect.x = 0;    
			sdlparam.rect.y = 0;    
			sdlparam.rect.w = screenrect.Width();    
			sdlparam.rect.h = screenrect.Height();    
			SDL_DisplayYUVOverlay(sdlparam.bmp, &sdlparam.rect); 
		}
		int progress=k*100/nFrameNum;
		dlg->m_progresscursi.SetPos(progress);
		CString progresstext;
		progresstext.Format(L"%d%%",progress);
		dlg->m_progresscursitext.SetWindowText(progresstext);
	}
	//FIX
	dlg->m_progresscursi.SetPos(100);
	dlg->m_progresscursitext.SetWindowText(L"100%");
	/*******************************计算TI*********************************/
	if(m_outputtiyuv.GetCheck()!=FALSE){
	cfTI.Open(yuvinfo.url + _T("_TI.yuv"),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite); 
	}
	//cfTIData.Open(strPath + _T("_TIData.y"),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite); 

	for(k = 0;k < nFrameNum-1; k++)//如果n帧，循环n-1次
	{
		//Check State-----
		while(dlg->sysstate==SYSTEM_PAUSE){
			Sleep(1000);
		}
		if(dlg->sysstate==SYSTEM_PREPARE){
			return -1;
		}
		//----------------

		cfYUV.Seek(k*nFrameSize, CFile::begin);
		cfYUV.Read(pFrame, nYSize);

		cfYUV.Seek((k+1)*nFrameSize, CFile::begin);
		cfYUV.Read(pNextFrame, nYSize);

		for(j = 0; j < nHeight; j++)
			for(i = 0; i < nWidth; i++)
			{
				NewTIBuffer[j*nWidth + i] = pNextFrame2D[j][i] - pFrame2D[j][i];
				NewYBuffer[j*nWidth + i] = abs(pNextFrame2D[j][i] - pFrame2D[j][i]);

				flTI_A = flTI_A + NewTIBuffer[j*nWidth + i];
			}
			if(m_outputtiyuv.GetCheck()!=FALSE){
				cfTI.Write(NewYBuffer, nYSize);
				cfTI.Write(NewUVBuffer, nUVSize); 
			}
		//浮点型数据先不输出
		//cfTIData.Write(NewTIBuffer, nYSize*sizeof(float));

		flTI_A = flTI_A/nYSize;

		for(j = 0; j < nHeight; j++)
			for(i = 0; i < nWidth; i++)
			{
				flTI_S = flTI_S + pow((NewTIBuffer[j*nWidth + i]-flTI_A), 2);
			}

		flTI_S = sqrt(flTI_S/(nYSize - 1));
		//输出TI
		if(m_outputti.GetCheck()!=FALSE){
			fprintf(fInfoTI, "%f\n", flTI_S);
		}
		ti.push_back(flTI_S);

		CString curvalstr;
		curvalstr.Format(L"%.3f",flTI_S);
		dlg->m_curval.SetWindowText(curvalstr);

		if(dlg->m_combodraw.GetCurSel()==0){
			sdlparam.graphically=true;
		}else{
			sdlparam.graphically=false;
		}


		if(sdlparam.graphically==true){
			SDL_LockYUVOverlay(sdlparam.bmp);
			sdlparam.bmp->pixels[0]=NewYBuffer;
			sdlparam.bmp->pixels[2]=NewUVBuffer;
			sdlparam.bmp->pixels[1]=NewUVBuffer+nUVSize/2;     
			sdlparam.bmp->pitches[0]=nWidth;
			sdlparam.bmp->pitches[2]=nWidth/2;   
			sdlparam.bmp->pitches[1]=nWidth/2;
			SDL_UnlockYUVOverlay(sdlparam.bmp); 

			CRect screenrect;
			dlg->GetDlgItem(IDC_SCREEN)->GetWindowRect(screenrect);
			sdlparam.rect.x = 0;    
			sdlparam.rect.y = 0;    
			sdlparam.rect.w = screenrect.Width();    
			sdlparam.rect.h = screenrect.Height();    
			SDL_DisplayYUVOverlay(sdlparam.bmp, &sdlparam.rect); 
		}

		int progress=k*100/(nFrameNum-1);
		dlg->m_progresscurti.SetPos(progress);
		CString progresstext;
		progresstext.Format(L"%d%%",progress);
		dlg->m_progresscurtitext.SetWindowText(progresstext);
	}
	//Draw
	 SDL_FreeYUVOverlay(sdlparam.bmp);
	//FIX
	dlg->m_progresscurti.SetPos(100);
	dlg->m_progresscurtitext.SetWindowText(L"100%");

	
	//文件名
	CString filename;
	int nPos=yuvinfo.url.ReverseFind('\\');   
	filename=yuvinfo.url.Mid(nPos+1);

	CString resulttistr(filename);
	CString resultsistr(filename);
	resulttistr.AppendFormat(L"\r\n=======\r\n");
	resultsistr.AppendFormat(L"\r\n=======\r\n");
	//计算平均值

	for (int i=0;i<si.size();i++){
		resultsistr.AppendFormat(L"%.3f\r\n",si[i]);
	}

	for (int i=0;i<ti.size();i++){
		resulttistr.AppendFormat(L"%.3f\r\n",ti[i]);
	}

	resultsistr.AppendFormat(L"=======\r\n");
	resulttistr.AppendFormat(L"=======\r\n");

	dlg->resultdlg->AppendTIStr(resulttistr);
	dlg->resultdlg->AppendSIStr(resultsistr);



	//记录TI，SI
	allsi.push_back(si);
	allti.push_back(ti);
	//清空
	si.clear();
	ti.clear();
	//--------
	if(m_outputsiyuv.GetCheck()!=FALSE)
	cfSI.Close();

	if(m_outputtiyuv.GetCheck()!=FALSE)
	cfTI.Close();

	cfYUV.Close();
	//cfSIData.Close();
	//cfTIData.Close();
	if(m_outputsi.GetCheck()!=FALSE)
	fclose(fInfoSI);

	if(m_outputti.GetCheck()!=FALSE)
	fclose(fInfoTI);

	delete []pFrame;
	delete []pFrame2D;
	delete []pNextFrame;
	delete []pNextFrame2D;
	delete []NewYBuffer;
	delete []NewUVBuffer;
	delete []NewTIBuffer;
	delete []NewSIBuffer;
	//完成一次重画窗口
	Invalidate();
	Sleep(1);
	return 0;
}

UINT Thread_Process(LPVOID lpParam){
	CTIandSIDlg *dlg=(CTIandSIDlg *)lpParam;

	dlg->m_progressall.SetPos(0);
	dlg->m_progressalltext.SetWindowText(L"0%");

	dlg->SetState(SYSTEM_PROCESS);
	//载入
	int i,j;
	for(i=0;i<dlg->yuvlistvector.size();i++){
		dlg->m_yuvlist.SetCurSel(i);

		int progress=i/dlg->yuvlistvector.size();
		dlg->m_progressall.SetPos(progress);
		CString progresstext;
		progresstext.Format(L"%d%%",progress);
		dlg->m_progressalltext.SetWindowText(progresstext);
		//------------------------------------
		int retval=dlg->TIandSI(dlg->yuvlistvector[i],dlg);
		if(retval==-1){
			fcloseall();
			return -1;
		}
	}
	dlg->m_progressall.SetPos(100);
	dlg->m_progressalltext.SetWindowText(L"100%");

	dlg->SetState(SYSTEM_PREPARE);

	AfxMessageBox(L"处理完毕！");
	return 0;
}

void CTIandSIDlg::OnBnClickedOk()
{
	//更新参数
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	SystemClear();
	if(yuvlistvector.size()<1){
		AfxMessageBox(L"请在列表中添加序列！");
		return;
	}
	pThreadProcess=AfxBeginThread(Thread_Process,this);//开启线程

}


void CTIandSIDlg::OnBnClickedYuvlistAdd()
{
	UpdateData(TRUE);
	//获取地址加入列表
	CString yuvurl;
	YUVInfo yuvinfo={NULL,0,0,YUV420P};
	m_yuvurl.GetWindowText(yuvurl);

	if(yuvurl.IsEmpty()==TRUE){
		AfxMessageBox(L"输入地址为空！");
		return;
	}

	yuvinfo.url.Format(L"%s",yuvurl);
	yuvinfo.w=m_inputyuvwidth;
	yuvinfo.h=m_inputyuvheight;

	yuvlistvector.push_back(yuvinfo);

	RefreshYuvlist();
}


void CTIandSIDlg::OnBnClickedYuvlistDelete()
{
	//删除元素
	int urlindex=m_yuvlist.GetCurSel();
	if(urlindex!=-1){
	yuvlistvector.erase(yuvlistvector.begin()+urlindex);
	}else{
		AfxMessageBox(L"没有选中任何元素！");
	}

	RefreshYuvlist();
}


void CTIandSIDlg::OnBnClickedYuvlistDeleteall()
{
	if(yuvlistvector.size()==0){
		AfxMessageBox(L"列表已经为空！");
	}
	//清空元素
	yuvlistvector.clear();
	RefreshYuvlist();
}

void CTIandSIDlg::RefreshYuvlist(){
	//重新载入列表
	m_yuvlist.ResetContent();
	int i=0;
	for(i=0;i<yuvlistvector.size();i++){
		CString record;
		//文件名
		CString filename;
		int nPos=yuvlistvector[i].url.ReverseFind('\\');   
		filename=yuvlistvector[i].url.Mid(nPos+1);   
		record.Format(L"%s,%dx%d",filename,yuvlistvector[i].w,yuvlistvector[i].h);
		m_yuvlist.InsertString(i,record);
	}
}

void CTIandSIDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}



void CTIandSIDlg::OnBnClickedPause()
{
	if(sysstate==SYSTEM_PROCESS){
		SetState(SYSTEM_PAUSE);
	}else{
		SetState(SYSTEM_PROCESS);
	}
}


void CTIandSIDlg::OnBnClickedStop()
{
	SetState(SYSTEM_PREPARE);
}


void CTIandSIDlg::OnBnClickedResult()
{
	resultdlg->ShowWindow(TRUE);
}


void CTIandSIDlg::SystemClear()
{
	resultdlg->SystemClear();
}

void CTIandSIDlg::SetState(Systemstate state)
{
	sysstate=state;
	switch(state){
	case SYSTEM_PREPARE:{
		m_outputti.EnableWindow(TRUE);
		m_outputtiyuv.EnableWindow(TRUE);
		m_outputsi.EnableWindow(TRUE);
		m_outputsiyuv.EnableWindow(TRUE);

		GetDlgItem(IDC_YUVLIST_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDC_YUVLIST_DELETEALL)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDPAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDSTOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_YUVLIST_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDPAUSE)->SetWindowText(L"暂停");
		break;
						}
	case SYSTEM_PAUSE:{
		GetDlgItem(IDPAUSE)->SetWindowText(L"继续");
		break;
					  }
	case SYSTEM_PROCESS:{
		m_outputti.EnableWindow(FALSE);
		m_outputtiyuv.EnableWindow(FALSE);
		m_outputsi.EnableWindow(FALSE);
		m_outputsiyuv.EnableWindow(FALSE);

		GetDlgItem(IDC_YUVLIST_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_YUVLIST_DELETEALL)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDPAUSE)->EnableWindow(TRUE);
		GetDlgItem(IDSTOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_YUVLIST_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDPAUSE)->SetWindowText(L"暂停");
		state=SYSTEM_PROCESS;
		break;
						}
	}
}

void CTIandSIDlg::OnDropFiles(HDROP hDropInfo)
{
	LPTSTR pFilePathName =(LPTSTR)malloc(500);
	::DragQueryFile(hDropInfo, 0, pFilePathName,500);  // 获取拖放文件的完整文件名，最关键！

	m_yuvurl.SetWindowText(pFilePathName);

	::DragFinish(hDropInfo);   // 注意这个不能少，它用于释放Windows 为处理文件拖放而分配的内存
	free(pFilePathName);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CTIandSIDlg::OnAdddlg()
{
		AddDlg adddlg;
		adddlg.maindlg=this;
		adddlg.DoModal();
		
}


void CTIandSIDlg::OnWebsite()
{
	ShellExecuteA(NULL, "open","http://blog.csdn.net/leixiaohua1020",NULL,NULL,SW_SHOWNORMAL);

}
