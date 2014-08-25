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
#include "ResultDlg.h"
#include "afxdialogex.h"


// ResultDlg 对话框

IMPLEMENT_DYNAMIC(ResultDlg, CDialogEx)

ResultDlg::ResultDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ResultDlg::IDD, pParent)
{

}

ResultDlg::~ResultDlg()
{
}

void ResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULT_SI, m_resultsi);
	DDX_Control(pDX, IDC_RESULT_TI, m_resultti);
}


BEGIN_MESSAGE_MAP(ResultDlg, CDialogEx)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// ResultDlg 消息处理程序
void ResultDlg::SystemClear()
{
	resulttistr.Format(L"");
	resultsistr.Format(L"");
	m_resultti.SetWindowText(L"");
	m_resultsi.SetWindowText(L"");
}

void ResultDlg::AppendTIStr(CString str)
{
	resulttistr.AppendFormat(L"%s\r\n",str);
	m_resultti.SetWindowText(resulttistr);
}

void ResultDlg::AppendSIStr(CString str)
{
	resultsistr.AppendFormat(L"%s\r\n",str);
	m_resultsi.SetWindowText(resultsistr);
}

void ResultDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	delete this;
}
