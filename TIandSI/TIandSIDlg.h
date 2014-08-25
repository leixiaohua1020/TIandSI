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

#pragma once
#include "ResultDlg.h"
#include "AddDlg.h"

class AddDlg;
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
	afx_msg void OnBnClickedYuvlistAdd();
	afx_msg void OnBnClickedYuvlistDelete();
	CListBox m_yuvlist;
	CMFCEditBrowseCtrl m_yuvurl;

	vector<YUVInfo> yuvlistvector;
	void RefreshYuvlist();
	afx_msg void OnBnClickedYuvlistDeleteall();

	CComboBox m_inputyuvpixfmt;
	int m_inputyuvheight;
	int m_inputyuvwidth;
	int TIandSI(YUVInfo yuvinfo,LPVOID dlg);
	vector<float> si;
	vector<vector<float>> allsi;
	vector<float> ti;
	vector<vector<float>> allti;
	CButton m_outputsi;
	CButton m_outputti;
	CButton m_outputsiyuv;
	CButton m_outputtiyuv;
	afx_msg void OnBnClickedAbout();
	CWinThread *pThreadProcess;
	CProgressCtrl m_progressall;
	CEdit m_progressalltext;
	//SDL
	SDLParam sdlparam;
	CProgressCtrl m_progresscurti;
	CProgressCtrl m_progresscursi;
	CEdit m_progresscursitext;
	CEdit m_progresscurtitext;
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedResult();
	CEdit m_curval;
	CComboBox m_combodraw;


	ResultDlg *resultdlg;

	//
	void SystemClear();
	void SetState(Systemstate state);
	Systemstate sysstate;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnAdddlg();
	afx_msg void OnWebsite();
};
