#include "ImguiEffectEditor.h"

#include "PluginBase.h"
#include "ImguiVstguiView.h"

ImguiEffectEditor::ImguiEffectEditor(PluginBase* plugin, int width, int height)
	: AEffGUIEditor(plugin)
{
	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
}

bool ImguiEffectEditor::open(void* ptr)
{
	CRect frameSize(rect.left, rect.top, rect.right, rect.bottom);
	CFrame* newFrame = new CFrame(frameSize, this);
	newFrame->open(ptr);

	ImguiVstguiView* openglview = new ImguiVstguiView(*this, frameSize);
	newFrame->addView(openglview);
	newFrame->setFocusView(openglview);

	frame = newFrame;
	return true;
}

void ImguiEffectEditor::close()
{
	//-- on close we need to delete the frame object.
	//-- once again we make sure that the member frame variable is set to zero before we delete it
	//-- so that calls to setParameter won't crash.
	CFrame* oldFrame = frame;
	frame = 0;
	oldFrame->forget();
}

void ImguiEffectEditor::imguiFrame()
{ }