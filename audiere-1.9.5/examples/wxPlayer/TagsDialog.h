#ifndef ADR_WX_TAGS_DIALOG_H
#define ADR_WX_TAGS_DIALOG_H


#include <audiere.h>
#include <algorithm>
#include "wx.h"

class TagsDialog : public wxDialog
{
public: 

TagsDialog(wxWindow* parent,audiere::SampleSourcePtr source);

void OnButton(wxCommandEvent& event);
void OnClose(wxCloseEvent& event);

protected:
wxListBox *m_tags;
wxButton  *m_close;
DECLARE_EVENT_TABLE()

};
#endif
