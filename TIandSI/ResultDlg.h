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


// ResultDlg 对话框

class ResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ResultDlg)

public:
	ResultDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ResultDlg();

// 对话框数据
	enum { IDD = IDD_RESULT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_resultsi;
	CEdit m_resultti;

	void SystemClear();
	CString resulttistr;
	CString resultsistr;

	void AppendTIStr(CString str);
	void AppendSIStr(CString str);
	afx_msg void OnDestroy();
};
