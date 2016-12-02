#pragma once

#include <vstgui/vstgui.h>
#include <mutex>

struct ImDrawData;
class ImguiFrameCallback;

class ImguiVstguiView : public COpenGLView
{
public:
	ImguiVstguiView(ImguiFrameCallback& imguiCallback, const CRect& size);

protected:
	void platformOpenGLViewCreated() override;
	void platformOpenGLViewWillDestroy() override;
	void drawOpenGL(const CRect& updateRect) override;
	CMouseEventResult onMouseDown(CPoint& where, const CButtonState& buttons) override;
	CMouseEventResult onMouseUp(CPoint& where, const CButtonState& buttons) override;
	CMouseEventResult onMouseMoved(CPoint& where, const CButtonState& buttons) override;
	bool onWheel(const CPoint& where, const float& distance, const CButtonState& buttons) override;
	int32_t onKeyDown(VstKeyCode& keyCode) override;
	int32_t onKeyUp(VstKeyCode& keyCode) override;

private:
	void newFrame(CRect size);
	void renderDrawLists(ImDrawData* draw_data);
	void createFontsTexture();
	void createDeviceObjects();
	void invalidateDeviceObjects();

	bool initialized = false;

	// imgui
	double g_Time = 0.0f;
	bool g_MousePressed[3] = { false, false, false };
	float g_MouseWheel = 0.0f;
	unsigned int g_FontTexture = 0;
	int g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
	int g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
	int g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
	unsigned int g_VboHandle = 0, g_VaoHandle = 0, g_ElementsHandle = 0;
	bool g_KeysDown[512] = {};
	CPoint g_MousePosition = { -1, -1 };

	std::mutex stateLock;

	ImguiFrameCallback& imguiCallback;
};