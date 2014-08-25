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
#include "TIandSIDlg.h"


// AddDlg 对话框
class CTIandSIDlg;

class AddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AddDlg)

public:
	AddDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~AddDlg();

// 对话框数据
	enum { IDD = IDD_ADD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChoosefile();
	CComboBox m_inputyuvpixfmt;
	CEdit m_yuvurls;

	CTIandSIDlg *maindlg;
	vector<CString> urls; 
	afx_msg void OnBnClickedYuvlistAdd();
	int m_inputyuvheight;
	int m_inputyuvwidth;
};
