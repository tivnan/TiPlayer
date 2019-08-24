
// TiPlayerDlg.h: 头文件
//

#pragma once
#include <string>
#include "vlc/vlc.h"
#include "vlc/libvlc.h"



//枚举视频状态:未读取，播放，暂停
enum VideoState {
	STATE_UN,
	STATE_PLAY,
	STATE_PAUSE
};


// CTiPlayerDlg 对话框
class CTiPlayerDlg : public CDialogEx
{
// 构造
public:
	CTiPlayerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TIPLAYER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	//视频实例
	libvlc_instance_t * m_pVlc_Inst;

	//当前视频播放环境
	libvlc_media_t *m_pVlc_media;

	//视频播放流
	libvlc_media_player_t *m_pVlc_player;

/****************************************************************************************/

	//用于显示当前时间
	CEdit m_curtime;

	//用于显示视频总长度
	CEdit m_duration;

	//进度条，用于显示时间以及拖动播放
	CSliderCtrl m_slider;

	//用于转码
	CEdit m_url;

	//用于设置窗口句柄
	CStatic m_show;

	VideoState playerstate;

	//窗口背景
	CBrush m_brush;

	//菜单
	CMenu m_Menu;

	//音量控制条
	CSliderCtrl m_VSlider;



/****************************************************************************************/

	//暂停播放按钮及其作用
	afx_msg void OnBnClickedBtnPlay();

	//停止按钮及其作用
	afx_msg void OnBnClickedBtnStop();

	//打开按钮及其作用
	afx_msg void OnBnClickedBtnOpen();

	//信息按钮及其作用
	afx_msg void OnBnClickedBtnInfo();

	//拖拽文件及其事件
	afx_msg void OnDropFiles(HDROP hDropInfo);

	//解除事件及其作用
	afx_msg void OnDestroy();

	//进度条滑动事件
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//进度条调节事件
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

/****************************************************************************************/

	void Init();

	//播放视频
	bool Play(const char* szVideo, HWND hWnd);

	//初始化界面
	void ScrRec();

	//判断按钮是否可用
	void SetBtnState(VideoState state);

	//编码转换，以支持中文路径
	void UNICODE_to_UTF8(CStringW& unicodeString, std::string& str);
	char* UnicodeToUtf8(const wchar_t* unicode);

	//播放暂停按钮
	void Continue();
	void Pause();

/****************************************************************************************/
	
	//用于设置图标
	CButton m_BtnPic_Play;
	CButton m_BtnPic_Stop;
	CButton m_BtnPic_Open;
	CButton m_BtnPic_Info;	
	CButton m_ScreenShot;

	//导入Icon资源，利用m_hIconBtn来存储句柄。
	HICON m_IconBtnContinue = AfxGetApp()->LoadIcon(IDI_CONTINUE);
	HICON m_IconBtnPause = AfxGetApp()->LoadIcon(IDI_PAUSE);
	HICON m_IconBtnOpen = AfxGetApp()->LoadIcon(IDI_OPEN);
	HICON m_IconBtnStop = AfxGetApp()->LoadIcon(IDI_STOP);
	HICON m_IconBtnInfo = AfxGetApp()->LoadIcon(IDI_INFO);
	HICON m_IconBtnScrShot= AfxGetApp()->LoadIcon(IDI_SCREENSHOT);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnChangeCurtime();

	afx_msg void On32771();
	afx_msg void On32774();
	afx_msg void On32780();
	afx_msg void On32776();
	afx_msg void On32777();
	afx_msg void On32781();

	afx_msg void OnBnClickedScreenshot();
};


