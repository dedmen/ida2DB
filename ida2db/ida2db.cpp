#include "ida2db.h"
#include <QtGui>

#include <ida.hpp>
#include <idp.hpp>
#include <loader.hpp>
#include <kernwin.hpp>
#include "mainwidget.h"
#include <QLayout>
#include <QApplication>
#include <name.hpp>
#include <pro.h>
//#include <QtPlugin>
//Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)
static QApplication* app = nullptr;
static Ida2db* pIda2db = nullptr;
static int argc = 0;
Ida2db::Ida2db() {}

QStringList Ida2db::getAllNamedFunctions() {
	QStringList output;
	int numOfFuncs = get_func_qty();
	if (numOfFuncs <= 0) {
		(void) msg("IDB2SIG: Not found any functions\n");
		return output;
	}
	int i = 0;
	//size_t len = 0;
	func_t* pFunc = NULL;
	for (i = 0; i < numOfFuncs; i++) {
		pFunc = getn_func(i);
		if ((NULL != pFunc) && has_name(getFlags(pFunc->startEA)) && !(pFunc->flags & FUNC_LIB)) {

			char buf[512];
			get_true_name(BADADDR, pFunc->startEA, buf, sizeof(buf));
			//get_segm_name(pFunc->startEA, buf, sizeof buf);
			output.append(buf);
			//len += make_func_sig(pFunc->startEA,
			//	(ulong) (pFunc->endEA - pFunc->startEA),
			//	&pSigBuf[len]);
		}
	}
	return output;
}

//--------------------------------------------------------------------------
static int idaapi ui_callback(void *user_data, int notification_code, va_list va) {
	 if (app == nullptr)
		 app = new QApplication(argc, NULL);
	if (notification_code == ui_tform_visible) {
		TForm *form = va_arg(va, TForm *);
		if (form == user_data) {
			// widget is created, create controls

			QWidget *w = (QWidget *) form;

			QHBoxLayout *mainLayout = new QHBoxLayout();
			mainLayout->setMargin(0);
			pIda2db = new Ida2db();
			mainWidget *userWidget = new mainWidget();

			mainLayout->addWidget(userWidget);
			//w->layout()->addWidget(userWidget);
			//QString obj = w->layout()->objectName();
			w->setLayout(mainLayout);
		}
	}
	if (notification_code == ui_tform_invisible) {
		TForm *form = va_arg(va, TForm *);
		if (form == user_data) {
			// widget is closed, destroy objects (if required)
		}
	}
	return 0;
}

//--------------------------------------------------------------------------
int idaapi init(void) {
	// the plugin works only with idaq
	return is_idaq() ? PLUGIN_OK : PLUGIN_SKIP;		 //#TODO use PLUGIN_KEEP
}

//--------------------------------------------------------------------------
void idaapi term(void) { unhook_from_notification_point(HT_UI, ui_callback); delete app; }

//--------------------------------------------------------------------------
void idaapi run(int /*arg*/) {
	HWND hwnd = NULL;
	TForm *form = create_tform("IDA2DB", &hwnd);
	if (hwnd != NULL) {
		hook_to_notification_point(HT_UI, ui_callback, form);
		//#TODO hook HT_IDB
		open_tform(form,  FORM_MENU | FORM_RESTORE | FORM_QWIDGET);	  //FORM_TAB |
	} else
		close_tform(form, FORM_SAVE);


	 //#TODO hook that stuff
	/*
	   HT_IDB
	
		 changing_cmt
		   extra_cmt_changed
	   HT_UI or something that events when current viewed function is changed.. so that widget can show comments to that function
	
	*/


}

//--------------------------------------------------------------------------
char comment[] = "This is a comment.. ill do that later";

char help[] = "This does stuff";

//--------------------------------------------------------------------------
// This is the preferred name of the plugin module in the menu system
// The preferred name may be overriden in plugins.cfg file

char wanted_name[] = "IDA2DB";

// This is the preferred hotkey for the plugin module
// The preferred hotkey may be overriden in plugins.cfg file
// Note: IDA won't tell you if the hotkey is not correct
//       It will just disable the hotkey.

char wanted_hotkey[] = "";

//--------------------------------------------------------------------------
//
//      PLUGIN DESCRIPTION BLOCK
//
//--------------------------------------------------------------------------
plugin_t PLUGIN = {
  IDP_INTERFACE_VERSION,
  PLUGIN_MOD | PLUGIN_FIX, // plugin flags
  init,                    // initialize

  term, // terminate. this pointer may be NULL.

  run, // invoke plugin

  comment, // long comment about the plugin
  // it could appear in the status line
  // or as a hint

  help, // multiline help about the plugin

  wanted_name,  // the preferred short name of the plugin
  wanted_hotkey // the preferred hotkey to run the plugin
};
