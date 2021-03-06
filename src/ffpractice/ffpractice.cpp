// ffpractice.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "ffpractice.h"
#include "base/util/at_exit.h"
#include "base/util/string_number_conversions.h"
#include "mainwindow.h"
//#include <exdisp.h>
//#include <comdef.h>
#include "controlex.h"

void MainThread::Init()
{
	//nbase::ThreadManager::RegisterThread(kThreadUI);
	PreMessageLoop();

	std::wstring theme_dir = QPath::GetAppPath() + _T("skin");
	//DuiLib::CPaintManagerUI::SetInstance(hInstance);

	DuiLib::CPaintManagerUI::SetResourcePath(theme_dir.c_str());
	DuiLib::CPaintManagerUI::SetResourceZip(_T("360SafeRes.zip"));

	C360SafeFrameWnd* pFrame = new C360SafeFrameWnd();
	if (pFrame != NULL)
	{
		pFrame->Create(NULL, _T("360安全卫士"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
		pFrame->CenterWindow();
		::ShowWindow(*pFrame, SW_SHOW);
	}

	//bool adapt_api = ConfigHelper::GetInstance()->IsAdaptDpi();
	//std::wstring language = nbase::UTF8ToUTF16(ConfigHelper::GetInstance()->GetLanguage());
	//ui::GlobalManager::Startup(theme_dir + L"themes\\default", ui::CreateControlCallback(), adapt_api, language);
	//ui::GlobalManager::Startup(theme_dir + L"themes\\default", ExternCtrlManager::CreateExternCtrl, adapt_api, language);
	//nim_ui::UserConfig::GetInstance()->SetDefaultIcon(IDI_ICON);

	//std::wstring app_crash = QCommand::Get(kCmdAppCrash);
	//if (app_crash.empty())
	//{
	//	nim_ui::WindowsManager::SingletonShow<LoginForm>(LoginForm::kClassName);
	//}
	//else
	//{
	//	std::wstring content = nbase::StringPrintf(ui::MutiLanSupport::GetInstance()->GetStringViaID(L"STRID_APP_DUMP_DUMP_TIP").c_str(), app_crash.c_str());
	//	MsgboxCallback cb = nbase::Bind(&MainThread::OnMsgBoxCallback, this, std::placeholders::_1);
	//	ShowMsgBox(NULL, cb, content, false, L"STRING_TIPS", true, L"STRID_APP_DUMP_OPEN", true, L"STRING_NO", true);
	//}
}

void MainThread::Cleanup()
{
	//ui::GlobalManager::Shutdown();

	PostMessageLoop();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}

void MainThread::PreMessageLoop()
{
	//misc_thread_.reset(new MiscThread(kThreadGlobalMisc, "Global Misc Thread"));
	//misc_thread_->Start();
}

void MainThread::PostMessageLoop()
{

	//misc_thread_->Stop();
	//misc_thread_.reset(NULL);
}

void MainThread::OnMsgBoxCallback()
{
	//if (ret == MB_YES)
	//{
	//	std::wstring dir = QPath::GetUserAppDataDir("");
	//	QCommand::AppStartWidthCommand(dir, L"");
	//}
}



int WINAPI MainProc(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow)
{
	// 把cef dll文件的位置添加到程序的"path"环境变量中,这样可以把dll文件放到bin以外的目录，并且不需要手动频繁切换dll文件，这行代码必须写到main的开头
	//nim_cef::CefManager::GetInstance()->AddCefDllToPath();
	nbase::AtExitManager at_manager;

	CComModule _Module;
	_Module.Init(NULL, hInst);

	_wsetlocale(LC_ALL, L"chs");

#ifdef _DEBUG
	AllocConsole();
	FILE* fp = NULL;
	freopen_s(&fp, "CONOUT$", "w+t", stdout);
	wprintf_s(L"Command:\n%s\n\n", lpszCmdLine);
#endif

	srand((unsigned int)time(NULL));

	//::SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);

	//初始化cef
	//CefSettings settings;
	//if (!nim_cef::CefManager::GetInstance()->Initialize(QPath::GetNimAppDataDir(L"Netease"), settings, atoi(GetConfigValue("cef_osr_enabled").c_str()) > 0))
	//	return 0;

	QCommand::ParseCommand(lpszCmdLine);

	HRESULT hr = ::OleInitialize(NULL);
	if (FAILED(hr))
		return 0;

	//g_need_restart_after_dump = true;

	{
		MainThread thread; // 创建主线程
		thread.RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop); // 执行主线程循环
	}
	QLOG_APP(L"exit ui loop");

	//清理cef
	//nim_cef::CefManager::GetInstance()->UnInitialize();

	QLOG_APP(L"app exit");

	// 是否重新运行程序
	/*std::wstring restart = QCommand::Get(kCmdRestart);
	if (!restart.empty())
	{
		std::wstring cmd;
		std::wstring acc = QCommand::Get(kCmdAccount);
		if (!acc.empty())
			cmd.append(nbase::StringPrintf(L" /%s %s ", kCmdAccount.c_str(), acc.c_str()));
		std::wstring exit_why = QCommand::Get(kCmdExitWhy);
		if (!exit_why.empty())
			cmd.append(nbase::StringPrintf(L" /%s %s ", kCmdExitWhy.c_str(), exit_why.c_str()));
		QCommand::RestartApp(cmd);
	}*/

	_Module.Term();
	::OleUninitialize();
	return 0;
}
int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpszCmdLine, int nCmdShow)
{
	int ret = MainProc(hInst, hPrevInst, lpszCmdLine, nCmdShow);
	exit(ret);
	return ret;
}

