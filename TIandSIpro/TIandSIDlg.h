/**
 * TIandSIpro
 * (TIandSI Professional)
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 * 
 * 本工程可以计算一个压缩视频的时间信息TI（Temporal perceptual Information，
 * 也可以称时间复杂度）和空间信息SI（Spatial perceptual Information，也可以
 * 称空间复杂度）。计算方法出自标准：ITU-R BT.1788
 *
 * Professional: 支持压缩码流（而不是像素数据比如YUV，RGB）作为输入
 *
 * This software can calculate a video bitstream's TI(Temporal perceptual Information) 
 * and SI(Spatial perceptual Information) according to ITU-R BT.1788.
 *
 * Professional: Support bitstreams (not raw data such as YUV, RGB, etc.) as Input.
 *
 */

#pragma once
//使用STL
#include <vector>
using std::vector;
#include <math.h>
#include "ResultDlg.h"

extern "C"
{
#include "sdl/SDL.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
};


typedef struct SDLParam{
	SDL_Surface *screen; 
	SDL_Overlay *bmp; 
	SDL_Rect rect;
	bool graphically;
}SDLParam;

typedef enum Systemstate{
	SYSTEM_PREPARE,
	SYSTEM_PAUSE,
	SYSTEM_PROCESS
};

// CTIandSIDlg 对话框
class CTIandSIDlg : public CDialogEx
{
// 构造
public:
	CTIandSIDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TIANDSI_DIALOG };

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
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedUrllistAdd();
	afx_msg void OnBnClickedUrllistDelete();
	CListBox m_urllist;
	CMFCEditBrowseCtrl m_inputurl;

	vector<CString> urllist;
	void RefreshUrllist();
	afx_msg void OnBnClickedUrllistDeleteall();

	int TIandSI(CString urllist,LPVOID dlg);
	int TIandSICal(LPVOID lparam,char* ydata,char* prev_ydata,int width,int height,SDLParam sdlparam,float &ti,float &si);

//	CButton m_outputsi;
//	CButton m_outputti;
	afx_msg void OnBnClickedAbout();
	CWinThread *pThreadProcess;
	CProgressCtrl m_progressall;
	CEdit m_progressalltext;
	//SDL
	SDLParam sdlparam;
	CProgressCtrl m_progresscur;
	CEdit m_progresscurtext;
//	afx_msg void OnClickedRadioDraw();
	//
	ResultDlg *resultdlg;
	//
	void SystemClear();
	afx_msg void OnBnClickedResult();
	CButton m_output;

	void SetState(Systemstate state);
	Systemstate sysstate;

	CComboBox m_draw;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	CEdit m_curval;
	afx_msg void OnWebsite();
	afx_msg void OnUrllistQuickadd();

	afx_msg void OnCancel();
};
