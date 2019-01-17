#pragma once

#include <chrono>

#include <wx/glcanvas.h>
#include <wx/taskbar.h>
#include <wx/wx.h>
#include <wx/hyperlink.h>
#include <wx/aboutdlg.h>


#include <glm/glm.hpp>


// the rendering context used by all GL canvases
class TestGLContext : public wxGLContext
{
public:
	TestGLContext(wxGLCanvas *canvas);

	// render the cube showing it at given angles
	void DrawRotatedCube(float xangle = 0, float yangle = 0);

	void DrawRotatedCube(glm::fquat q);

	void DrawRotatedCube(float xangle = 0, float yangle = 0, float zangle = 0);

private:
	// textures for the cube faces
	GLuint m_textures[6];
};


class TestGLCanvas : public wxGLCanvas
{
public:
	TestGLCanvas(wxWindow *parent, int *attribList = NULL);

	// angles of rotation around x- and y- axis
	float m_xangle;
	float m_yangle;

private:
	void OnPaint(wxPaintEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnSpinTimer(wxTimerEvent& WXUNUSED(event));



	wxTimer m_spinTimer;
	bool m_useStereo,
		m_stereoWarningAlreadyDisplayed;

	wxDECLARE_EVENT_TABLE();
};

enum
{
	NEW_STEREO_WINDOW = wxID_HIGHEST + 1
};


// Define a new application type
class MyApp : public wxApp
{
public:
	MyApp() {};

    // virtual wxApp methods
    virtual bool OnInit();

	virtual int OnExit();

	// Returns the shared context used by all frames and sets it as current for
	// the given canvas.
	TestGLContext& GetContext(wxGLCanvas *canvas, bool useStereo);

private:
	// the GL context we use for all our mono rendering windows
	TestGLContext *m_glContext;
	// the GL context we use for all our stereo rendering windows
	TestGLContext *m_glStereoContext;

	wxTimer m_myTimer;
};


class MyTaskBarIcon;
class MyLoopThread;

// Define a new frame type
class MainFrame : public wxFrame
{
public:

	wxApp *parent;


	wxCheckBox* CB1;
	wxCheckBox* CB2;
	wxCheckBox* CB3;
	wxCheckBox* CB4;
	wxCheckBox* CB5;
	wxCheckBox* CB6;
	wxCheckBox* CB7;
	wxCheckBox* CB8;
	wxCheckBox* CB9;
	wxCheckBox* CB10;
	wxCheckBox* CB11;
	wxCheckBox* CB12;
	wxCheckBox* CB13;
	wxCheckBox* CB14;
	wxCheckBox* CB15;
	wxCheckBox* CB16;



	wxStaticText* st1;
	wxStaticText* st2;
	wxStaticText* st3;
	wxStaticText* st4;

	wxSlider* slider1;
	wxSlider* slider2;
	wxStaticText* slider1Text;
	wxStaticText* slider2Text;


	wxButton* startButton;
	wxButton* quitButton;
	wxButton* updateButton;
	wxButton* donateButton;


	MainFrame();

	void onStart(wxCommandEvent&);
	void DoWork();
	void onQuit(wxCommandEvent&);
	
	void onQuit2(wxCloseEvent&);

	void onUpdate(wxCommandEvent&);
	void onDonate(wxCommandEvent &);

	void toggleCombine(wxCommandEvent&);

	void toggleGyro(wxCommandEvent&);
	void toggleGyroWindow(wxCommandEvent &);
	void toggleBatteryLed(wxCommandEvent& e);
	void toggleMario(wxCommandEvent&);

	void toggleReverseX(wxCommandEvent&);
	void toggleReverseY(wxCommandEvent&);

	void togglePreferLeftJoyCon(wxCommandEvent&);
	void toggleQuickToggleGyro(wxCommandEvent&);
	void toggleInvertQuickToggle(wxCommandEvent&);
	void toggleDolphinPointerMode(wxCommandEvent&);

	void toggleDebugMode(wxCommandEvent&);
	void toggleWriteDebug(wxCommandEvent&);

	void toggleForcePoll(wxCommandEvent&);

	void setGyroSensitivityX(wxCommandEvent&);
	void setGyroSensitivityY(wxCommandEvent&);


	void toggleBroadcastMode(wxCommandEvent&);
	void toggleWriteCastToFile(wxCommandEvent&);


	void checkForUpdate();

protected:
	MyTaskBarIcon* taskBarIcon;
	MyLoopThread* loopThread;
private:
	void DoQuit();
};






// Define a new frame type
class MyFrame : public wxFrame
{
public:
	MyFrame(bool stereoWindow = false);

private:
	void OnClose(wxCommandEvent& event);
	void OnNewWindow(wxCommandEvent& event);
	void OnNewStereoWindow(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};


class MyTaskBarIcon : public wxTaskBarIcon {
public:
	MyTaskBarIcon(MainFrame* pParent)
		: wxTaskBarIcon()
		, m_pParent(pParent)
		, m_notification(false)
		, m_lastBatteryNotification()
	{}

	void StartNotification();
	void SetJoyConStatus(int lcounter, int rcounter, uint8_t lbattery, uint8_t rbattery);
	void SetTitle(const wxString& title);
	const wxString GetTooltip() const;
	virtual bool SetIcon(const wxIcon &icon, const wxString& tooltip=wxEmptyString);
protected:
	virtual wxMenu* CreatePopupMenu();
	void OnDoubleClick(wxTaskBarIconEvent& event);
	void OnMenuOpen(wxCommandEvent& event);
	void OnMenuExit(wxCommandEvent& event);
	void OnMenuGameController(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
private:
	MainFrame* m_pParent;
	wxString m_title;
	wxIcon m_icon;
	bool m_notification;
	int m_lcounter;
	int m_rcounter;
	uint8_t m_lbattery;
	uint8_t m_rbattery;
	std::chrono::high_resolution_clock::time_point m_lastBatteryNotification;

	enum {
		MENUID_GAME_CONTROLLER = 10001,
	};

	void NotifyInfo(const wxString& message);
	void NotifyWarning(const wxString& message);
	void NotifyIfBatteryIsLow();
};

class MyLoopThread : public wxThread {
public:
	MyLoopThread(MainFrame* pMainFrame)
		: wxThread(wxTHREAD_JOINABLE)
		, m_pMainFrame(pMainFrame)
	{}
protected:
	virtual ExitCode Entry() {
		while (!TestDestroy()) {
			m_pMainFrame->DoWork();
		}
		return 0;
	}
private:
	MainFrame* m_pMainFrame;
};
