
// TiPlayerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TiPlayer.h"
#include "TiPlayerDlg.h"
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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTiPlayerDlg 对话框



CTiPlayerDlg::CTiPlayerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TIPLAYER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTiPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CURTIME, m_curtime);
	DDX_Control(pDX, IDC_DURATION, m_duration);
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	DDX_Control(pDX, IDC_SCREEN, m_show);
	DDX_Control(pDX, IDC_BTN_PLAY, m_BtnPic_Play);
	DDX_Control(pDX, IDC_BTN_STOP, m_BtnPic_Stop);
	DDX_Control(pDX, IDC_BTN_OPEN, m_BtnPic_Open);
	DDX_Control(pDX, IDC_BTN_INFO, m_BtnPic_Info);
	DDX_Control(pDX, IDC_VOLUMESLIDER, m_VSlider);
	DDX_Control(pDX, IDC_SCREENSHOT, m_ScreenShot);
}

BEGIN_MESSAGE_MAP(CTiPlayerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_PLAY, &CTiPlayerDlg::OnBnClickedBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STOP, &CTiPlayerDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_OPEN, &CTiPlayerDlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_INFO, &CTiPlayerDlg::OnBnClickedBtnInfo)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
	ON_EN_CHANGE(IDC_CURTIME, &CTiPlayerDlg::OnEnChangeCurtime)
	ON_COMMAND(ID_32771, &CTiPlayerDlg::On32771)
	ON_COMMAND(ID_32774, &CTiPlayerDlg::On32774)
	ON_COMMAND(ID_32780, &CTiPlayerDlg::On32780)
	ON_COMMAND(ID_32776, &CTiPlayerDlg::On32776)
	ON_COMMAND(ID_32777, &CTiPlayerDlg::On32777)
	ON_COMMAND(ID_32781, &CTiPlayerDlg::On32781)
	ON_BN_CLICKED(IDC_SCREENSHOT, &CTiPlayerDlg::OnBnClickedScreenshot)
END_MESSAGE_MAP()


// CTiPlayerDlg 消息处理程序

BOOL CTiPlayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	//界面初始化
	ScrRec();

	m_pVlc_Inst = NULL;
	m_pVlc_media = NULL;
	m_pVlc_player=NULL;

	//播放进度条
	m_slider.SetRange(0, 1000);

	//音量进度条
	m_VSlider.SetRange(0, 200);       //设置滑动范围为1到20
	m_VSlider.SetTicFreq(1);			//每1个单位画一刻度
	m_VSlider.SetPos(100);


	//初始化视频流
	Init();
	
	//按钮图标设置
	m_BtnPic_Open.SetIcon(m_IconBtnOpen);
	m_BtnPic_Play.SetIcon(m_IconBtnContinue);
	m_BtnPic_Stop.SetIcon(m_IconBtnStop);
	m_BtnPic_Info.SetIcon(m_IconBtnInfo);
	m_ScreenShot.SetIcon(m_IconBtnScrShot);

	//背景颜色
	m_brush.CreateSolidBrush(RGB(214, 214, 214));

	//菜单
	m_Menu.LoadMenu(IDR_MENU);
	SetMenu(&m_Menu);

	//禁止调整窗口大小
	ModifyStyle(WS_THICKFRAME, 0, SWP_FRAMECHANGED | SWP_DRAWFRAME);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTiPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTiPlayerDlg::OnPaint()
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
HCURSOR CTiPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//暂停播放
void CTiPlayerDlg::OnBnClickedBtnPlay()
{
	if (playerstate == STATE_PLAY) {
		libvlc_media_player_set_pause(m_pVlc_player, 1);
		playerstate = STATE_PAUSE;
		m_BtnPic_Play.SetIcon(m_IconBtnContinue);
		GetDlgItem(IDC_BTN_PLAY)->SetWindowText(_T("Resume"));
	}
	else if (playerstate == STATE_PAUSE) {
		libvlc_media_player_set_pause(m_pVlc_player, 0);
		playerstate = STATE_PLAY;
		m_BtnPic_Play.SetIcon(m_IconBtnPause);
		GetDlgItem(IDC_BTN_PLAY)->SetWindowText(_T("Pause"));
	}
}



void CTiPlayerDlg::OnBnClickedBtnStop()
{
	//暂停并释放资源
	if (m_pVlc_player != NULL) {
		libvlc_media_player_stop(m_pVlc_player);
		libvlc_media_player_release(m_pVlc_player);
		KillTimer(1);
	}

	ScrRec();


	//清空界面
	Invalidate(TRUE);
	UpdateWindow();
}


void CTiPlayerDlg::OnBnClickedBtnOpen()
{
	//打开文件对话框，获取相应视频文件
	CString FilePathName;
	CString strfilter;
	strfilter.Append(_T("Common media formats|*.avi;*.wmv;*.wmp;*.wm;*.asf;*.rm;*.ram;*.rmvb;*.ra;*.mpg;*.mpeg;*.mpe;*.m1v;*.m2v;*.mpv2;"));
	strfilter.Append(_T("*.mp2v;*.dat;*.mp4;*.m4v;*.m4p;*.vob;*.ac3;*.dts;*.mov;*.qt;*.mr;*.3gp;*.3gpp;*.3g2;*.3gp2;*.swf;*.ogg;*.wma;*.wav;"));
	strfilter.Append(_T("*.mid;*.midi;*.mpa;*.mp2;*.mp3;*.m1a;*.m2a;*.m4a;*.aac;*.mkv;*.ogm;*.m4b;*.tp;*.ts;*.tpr;*.pva;*.pss;*.wv;*.m2ts;*.evo;"));
	strfilter.Append(_T("*.rpm;*.realpix;*.rt;*.smi;*.smil;*.scm;*.aif;*.aiff;*.aifc;*.amr;*.amv;*.au;*.acc;*.dsa;*.dsm;*.dsv;*.dss;*.pmp;*.smk;*.flic|"));
	strfilter.Append(_T("Windows Media Video(*.avi;*wmv;*wmp;*wm;*asf)|*.avi;*.wmv;*.wmp;*.wm;*.asf|"));
	strfilter.Append(_T("Windows Media Audio(*.wma;*wav;*aif;*aifc;*aiff;*mid;*midi;*rmi)|*.wma;*.wav;*.aif;*.aifc;*.aiff;*.mid;*.midi;*.rmi|"));
	strfilter.Append(_T("Real(*.rm;*ram;*rmvb;*rpm;*ra;*rt;*rp;*smi;*smil;*.scm)|*.rm;*.ram;*.rmvb;*.rpm;*.ra;*.rt;*.rp;*.smi;*.smil;*.scm|"));
	strfilter.Append(_T("MPEG Video(*.mpg;*mpeg;*mpe;*m1v;*m2v;*mpv2;*mp2v;*dat;*mp4;*m4v;*m4p;*m4b;*ts;*tp;*tpr;*pva;*pss;*.wv;)|"));
	strfilter.Append(_T("*.mpg;*.mpeg;*.mpe;*.m1v;*.m2v;*.mpv2;*.mp2v;*.dat;*.mp4;*.m4v;*.m4p;*.m4b;*.ts;*.tp;*.tpr;*.pva;*.pss;*.wv;|"));
	strfilter.Append(_T("MPEG Audio(*.mpa;*mp2;*m1a;*m2a;*m4a;*aac;*.m2ts;*.evo)|*.mpa;*.mp2;*.m1a;*.m2a;*.m4a;*.aac;*.m2ts;*.evo|"));
	strfilter.Append(_T("DVD(*.vob;*ifo;*ac3;*dts)|*.vob;*.ifo;*.ac3;*.dts|MP3(*.mp3)|*.mp3|CD Tracks(*.cda)|*.cda|"));
	strfilter.Append(_T("Quicktime(*.mov;*qt;*mr;*3gp;*3gpp;*3g2;*3gp2)|*.mov;*.qt;*.mr;*.3gp;*.3gpp;*.3g2;*.3gp2|"));
	strfilter.Append(_T("Flash Files(*.flv;*swf;*.f4v)|*.flv;*.swf;*.f4v|Playlist(*.smpl;*.asx;*m3u;*pls;*wvx;*wax;*wmx;*mpcpl)|*.smpl;*.asx;*.m3u;*.pls;*.wvx;*.wax;*.wmx;*.mpcpl|"));
	strfilter.Append(_T("Others(*.ivf;*au;*snd;*ogm;*ogg;*fli;*flc;*flic;*d2v;*mkv;*pmp;*mka;*smk;*bik;*ratdvd;*roq;*drc;*dsm;*dsv;*dsa;*dss;*mpc;*divx;*vp6;*.ape;*.flac;*.tta;*.csf)"));
	strfilter.Append(_T("|*.ivf;*.au;*.snd;*.ogm;*.ogg;*.fli;*.flc;*.flic;*.d2v;*.mkv;*.pmp;*.mka;*.smk;*.bik;*.ratdvd;*.roq;*.drc;*.dsm;*.dsv;*.dsa;*.dss;*.mpc;*.divx;*.vp6;*.ape;*.amr;*.flac;*.tta;*.csf|"));
	strfilter.Append(_T("All Files(*.*)|*.*||"));

	LPCTSTR lpszfilter = strfilter;

	//TRUE为OPEN对话框，FALSE为SAVE AS对话框 
	CFileDialog dlg(TRUE, NULL, NULL, NULL, lpszfilter);

	if (dlg.DoModal() == IDOK) {
		FilePathName = dlg.GetPathName();
	}

	//获取所需数组大小
	DWORD CHtemp = WideCharToMultiByte(CP_UTF8, NULL, FilePathName, -1, NULL, NULL, 0, NULL);
	//分配内存
	char* CszVideoPath = new char[CHtemp];
	//编码
	WideCharToMultiByte(CP_UTF8, NULL, FilePathName, -1, CszVideoPath, CHtemp, NULL, NULL);

	if (CszVideoPath==NULL||CszVideoPath==0)
	{
		AfxMessageBox(_T("Input URL is NULL!"));
		return;
	}

	//播放视频
	Play(CszVideoPath, m_show.m_hWnd);


	playerstate = STATE_PLAY;
	SetBtnState(STATE_PLAY);
	SetTimer(1, 1000, NULL);

	//设置音量显示
	int volume = libvlc_audio_get_volume(m_pVlc_player);
	m_VSlider.SetPos(100);

}

//视频信息展示
void CTiPlayerDlg::OnBnClickedBtnInfo()
{
	CString infostr;
	infostr.AppendFormat(_T("========视频信息========\r\n"));

	//视频长宽
	unsigned video_w = 0, video_h = 0;

	//视频帧速率
	float framerate = 0;
	libvlc_video_get_size(m_pVlc_player, 0, &video_w, &video_h);
	framerate = libvlc_media_player_get_fps(m_pVlc_player);

	infostr.AppendFormat(_T("视频宽度:%d\r\n视频高度:%d\r\n视频帧速率:%f\r\n")
		, video_w, video_h, framerate);

	

	//音频信息
	infostr.AppendFormat(_T("========音频信息========\r\n"));
	int channel = 0;
	channel = libvlc_audio_get_channel(m_pVlc_player);
	infostr.AppendFormat(_T("音轨:%d\r\n"), channel);


	int volume;
	volume = libvlc_audio_get_volume(m_pVlc_player);
	infostr.AppendFormat(_T("音量:%d\r\n"), volume);



	AfxMessageBox(infostr, MB_ICONINFORMATION);
}


void CTiPlayerDlg::OnDropFiles(HDROP hDropInfo)
{
	//获取拖拽进来的文件数量
	UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, NULL);

	//szVideoPath用于储存文件路径
	//MAX_PATH定义了编译器所支持的最长全路径名的长度
	TCHAR TszVideoPath[MAX_PATH];

	//获取最后一个文件路径
	DragQueryFile(hDropInfo, nCount - 1, TszVideoPath, MAX_PATH);

	//Unicode转为UTF-8
	//将TCHAR类型的szvideoPath转换为CHAR类型
	char* CszVideoPath = UnicodeToUtf8(TszVideoPath);

	//播放视频
	Play(CszVideoPath, m_show.m_hWnd);

	// 用于释放Windows 为处理文件拖放而分配的内存
	::DragFinish(hDropInfo);   


	playerstate = STATE_PLAY;
	SetBtnState(STATE_PLAY);
	SetTimer(1, 1000, NULL);

	//设置音量显示
	int volume = libvlc_audio_get_volume(m_pVlc_player);
	m_VSlider.SetPos(100);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CTiPlayerDlg::OnDestroy()
{
	//释放Inst
	libvlc_release(m_pVlc_Inst);

	CDialogEx::OnDestroy();

}

//设置时间显示
void CTiPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) 
	{
		//当前时间
		CString curtimestr, durationstr;
		
		//时分秒
		int curtime;
		int duration;
		int tns, thh, tmm, tss;
		int progress;
		
		curtime = libvlc_media_player_get_time(m_pVlc_player);
		if (curtime != 0) {
			//change to second
			tns = curtime / 1000;
			thh = tns / 3600;
			tmm = (tns % 3600) / 60;
			tss = (tns % 60);
			curtimestr.Format(_T("%02d:%02d:%02d"), thh, tmm, tss);
			m_curtime.SetWindowText(curtimestr);
		}


		duration = libvlc_media_player_get_length(m_pVlc_player);
		if (duration != 0) {
			tns = duration / 1000;
			thh = tns / 3600;
			tmm = (tns % 3600) / 60;
			tss = (tns % 60);
			durationstr.Format(_T("%02d:%02d:%02d"), thh, tmm, tss);
			m_duration.SetWindowText(durationstr);

			progress = curtime * 100 / duration;
			m_slider.SetPos(progress);
		}
	}

	//末尾自动结束
	if (libvlc_media_player_get_state(m_pVlc_player) == libvlc_Ended)
		OnBnClickedBtnStop();


	CDialogEx::OnTimer(nIDEvent);
}

//滚动进度条控制时间进度
void CTiPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER)
	{
		//设置视频进度条滑动事件
		if (pScrollBar->GetSafeHwnd() == m_slider.GetSafeHwnd())
		{
			float posf = 0.0;
			if (nSBCode == SB_THUMBPOSITION) 
			{
				posf = (float)nPos / 100.0;
				libvlc_media_player_set_position(m_pVlc_player, posf);
			}
		}
	}
	else if (pScrollBar->GetDlgCtrlID() == IDC_VOLUMESLIDER)
	{
		//设置音量进度条滑动事件
		if (pScrollBar->GetSafeHwnd() == m_VSlider.GetSafeHwnd())
		{
			float posf = 0.0;
			if (nSBCode == SB_THUMBPOSITION)
			{
				posf = (float)nPos;
				libvlc_audio_set_volume(m_pVlc_player,posf);
			}
		}



	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

//初始化m_pVlc_Inst,获取一个实例
void CTiPlayerDlg::Init()
{
	//LIBVLC_NEW()的作用——
	if (m_pVlc_Inst == NULL)
	{
		m_pVlc_Inst = libvlc_new(0, NULL);
	}
}

//播放视频
bool CTiPlayerDlg::Play(const char* szVideo, HWND hWnd)
{
	//如果文件路径不存在或者为0，则播放失败
	if (szVideo == NULL || szVideo == 0)
	{
		AfxMessageBox(_T("文件路径存在问题"), MB_OK || MB_ICONWARNING, 0);
		return false;
	}

	//初始化
	Init();

	//从路径里创建一个媒体项目，用于后续解码播放的等等
	m_pVlc_media = libvlc_media_new_path(m_pVlc_Inst, szVideo);

	//为媒体项目创建一个播放环境和流
	m_pVlc_player = libvlc_media_player_new_from_media(m_pVlc_media);

	//释放m_pVlc_media
	libvlc_media_release(m_pVlc_media);

	//开始播放
	libvlc_media_player_play(m_pVlc_player);

	//指定窗口播放
	libvlc_media_player_set_hwnd(m_pVlc_player, hWnd);

	SetBtnState(STATE_PLAY);

	m_BtnPic_Play.SetIcon(m_IconBtnPause);

	//表示播放成功
	return true;

}

//初始化屏幕
void CTiPlayerDlg::ScrRec()
{
	//初始化按钮和播放状态
	SetBtnState(STATE_UN);
	playerstate = STATE_UN;

	//初始化时间
	m_curtime.SetWindowText(_T("00:00:00"));
	m_duration.SetWindowText(_T("00:00:00"));

	//视频流置NULL
	m_pVlc_player = NULL;
	m_pVlc_media = NULL;

	//进度条归零
	m_slider.SetPos(0);

}

//编码转换，以支持中文路径
void CTiPlayerDlg::UNICODE_to_UTF8(CStringW& unicodeString, std::string& str)
{
	//获取长度
	int stringLength = ::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), NULL, 0, NULL, NULL);

	//分配内存
	char* buffer = new char[stringLength + 1];

	//编码分配，末端置\0
	::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), buffer, stringLength, NULL, NULL);
	buffer[stringLength] = '\0';

	str = buffer;

	delete[] buffer;

}

char* CTiPlayerDlg::UnicodeToUtf8(const wchar_t* unicode)
{
	int len;

	//获得char数组所需长度
	len = WideCharToMultiByte(CP_UTF8, 0, unicode, -1, NULL, 0, NULL, NULL);

	//分配内存且置零
	char *szUtf8 = (char*)malloc(len + 1);
	memset(szUtf8, 0, len + 1);

	//编码
	WideCharToMultiByte(CP_UTF8, 0, unicode, -1, szUtf8, len, NULL, NULL);
	

	return szUtf8;

}

//判断按钮是否可用
void CTiPlayerDlg::SetBtnState(VideoState state)
{
	if (state == STATE_UN) {
		GetDlgItem(IDC_BTN_OPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_INFO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_PLAY)->EnableWindow(FALSE);
	}
	else {
		GetDlgItem(IDC_BTN_OPEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_INFO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_PLAY)->EnableWindow(TRUE);
	}
}

void CTiPlayerDlg::Continue()
{
	//继续视频
	libvlc_media_player_set_pause(m_pVlc_player, 0);
}

void CTiPlayerDlg::Pause()
{
	//暂停视频
	libvlc_media_player_set_pause(m_pVlc_player, 1);
}



HBRUSH CTiPlayerDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	//

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return m_brush;
}


void CTiPlayerDlg::OnEnChangeCurtime()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}





//菜单各功能

//文件——>打开
void CTiPlayerDlg::On32771()
{
	OnBnClickedBtnOpen();
}

//暂停播放
void CTiPlayerDlg::On32774()
{
	OnBnClickedBtnPlay();
}



void CTiPlayerDlg::On32780()
{
	OnBnClickedBtnStop();
}


void CTiPlayerDlg::On32776()
{
	//打开github地址
	AfxMessageBox(_T("本程序已开源，可于Github下载交流学习"), MB_OK, 0);
	ShellExecuteA(NULL, "open", "https://github.com/Untuto", NULL, NULL, SW_SHOWNORMAL);
}


void CTiPlayerDlg::On32777()
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void CTiPlayerDlg::On32781()
{
	// TODO: 在此添加命令处理程序代码
	AfxMessageBox(_T("如有问题请阅读帮助手册"), MB_OK, 0);
}




void CTiPlayerDlg::OnBnClickedScreenshot()
{
	AfxMessageBox(_T("功能未完成，静待上线"), MB_OK, 0);

	

	////////////////////////////////////////////////////////
	//未完成功能//
	///////////////////////////////////////////////////////


	//TCHAR szFilter[] = _T("*.jpg|所有文件(*.*)|*.*||");

	//CFileDialog fileDlg(FALSE, _T("*.jpg"), "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

	////this->SetFocus();

	//if (IDOK == fileDlg.DoModal())
	//{
	//	//如果点击了文件对话框上的“保存”按钮，则将选择的文件路径显示到编辑框里	 
	//	m_strSnapShotPath = fileDlg.GetPathName();
	//}

	//AfxBeginThread(&TakeSnapThread, this);
}

//流媒体播放
void RTMP()
{






}
