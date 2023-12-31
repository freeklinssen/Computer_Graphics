// generated by Fast Light User Interface Designer (fluid) version 1.0106

#ifndef modelerui_h
#define modelerui_h
#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_Menu_Bar.H"
#include "ModelerView.h"
#include "FL/Fl_File_Chooser.H"
#include "FL/fl_message.H"
#include "bitmap.h"
#include "iostream"
#include "fstream"
#include "camera.h"
#include "FL/Fl_Browser.H"
#include "FL/Fl_Scroll.H"
#include "FL/Fl_Pack.H"
#include "modelerapp.h"

class ModelerUserInterface {
public:
  ModelerUserInterface();
  Fl_Double_Window *m_controlsWindow;
private:
  inline void cb_m_controlsWindow_i(Fl_Double_Window*, void*);
  static void cb_m_controlsWindow(Fl_Double_Window*, void*);
public:
  Fl_Menu_Bar *m_controlsMenuBar;
  static Fl_Menu_Item menu_m_controlsMenuBar[];
private:
  inline void cb_Save_i(Fl_Menu_*, void*);
  static void cb_Save(Fl_Menu_*, void*);
  inline void cb_Open_i(Fl_Menu_*, void*);
  static void cb_Open(Fl_Menu_*, void*);
  inline void cb_Save1_i(Fl_Menu_*, void*);
  static void cb_Save1(Fl_Menu_*, void*);
  inline void cb_Exit_i(Fl_Menu_*, void*);
  static void cb_Exit(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *m_controlsAnimOnMenu;
private:
  inline void cb_m_controlsAnimOnMenu_i(Fl_Menu_*, void*);
  static void cb_m_controlsAnimOnMenu(Fl_Menu_*, void*);
public:
  Fl_Browser *m_controlsBrowser;
private:
  inline void cb_m_controlsBrowser_i(Fl_Browser*, void*);
  static void cb_m_controlsBrowser(Fl_Browser*, void*);
public:
  Fl_Scroll *m_controlsScroll;
  Fl_Pack *m_controlsPack;
  Fl_Double_Window *m_modelerWindow;
private:
  inline void cb_m_modelerWindow_i(Fl_Double_Window*, void*);
  static void cb_m_modelerWindow(Fl_Double_Window*, void*);
public:
  ModelerView *m_modelerView;
  void show();
};
#endif
