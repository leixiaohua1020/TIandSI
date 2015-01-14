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
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
public:
	void SystemClear();
	CString resulttistr;
	CString resultsistr;
	CEdit m_resultsi;
	CEdit m_resultti;

	void AppendTIStr(CString str);
	void AppendSIStr(CString str);
};
