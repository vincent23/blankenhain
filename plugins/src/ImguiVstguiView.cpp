#include "ImguiVstguiView.h"

#include "ImguiFrameCallback.h"

#include <glad/glad.h>
#include <imgui.h>

#include <string>

ImguiVstguiView::ImguiVstguiView(ImguiFrameCallback& imguiCallback_, const CRect& size)
	: COpenGLView(size)
	, imguiCallback(imguiCallback_)
	, imguiContext(ImGui::CreateContext())
{
	ImGui::SetCurrentContext(imguiContext);
	ImGui::GetIO().Fonts = &fonts;
}

ImguiVstguiView::~ImguiVstguiView()
{ 
	ImGui::DestroyContext(imguiContext);
}

void ImguiVstguiView::platformOpenGLViewCreated()
{
	ImGui::SetCurrentContext(imguiContext);
	gladLoadGL();

	ImGuiIO& io = ImGui::GetIO();
	// Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
	io.KeyMap[ImGuiKey_Tab] = VKEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = VKEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VKEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VKEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VKEY_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VKEY_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = VKEY_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = VKEY_HOME;
	io.KeyMap[ImGuiKey_End] = VKEY_END;
	io.KeyMap[ImGuiKey_Delete] = VKEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VKEY_BACK;
	io.KeyMap[ImGuiKey_Enter] = VKEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = VKEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = VKEY_EQUALS + 1 + 'a';
	io.KeyMap[ImGuiKey_C] = VKEY_EQUALS + 1 + 'c';
	io.KeyMap[ImGuiKey_V] = VKEY_EQUALS + 1 + 'v';
	io.KeyMap[ImGuiKey_X] = VKEY_EQUALS + 1 + 'x';
	io.KeyMap[ImGuiKey_Y] = VKEY_EQUALS + 1 + 'y';
	io.KeyMap[ImGuiKey_Z] = VKEY_EQUALS + 1 + 'z';

	// Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
	io.RenderDrawListsFn = nullptr;
}

void ImguiVstguiView::platformOpenGLViewWillDestroy()
{
	invalidateDeviceObjects();
	ImGui::Shutdown();
}

void ImguiVstguiView::drawOpenGL(const CRect& updateRect)
{
	ImGui::SetCurrentContext(imguiContext);
	CRect r(getViewSize());
	newFrame(r);

	imguiCallback.imguiFrame();

	glViewport(0, 0, (int)r.getWidth(), (int)r.getHeight());
	ImVec4 clear_color = ImColor(114, 144, 154);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui::Render();
	renderDrawLists(ImGui::GetDrawData());

	getPlatformOpenGLView()->swapBuffers();
	setDirty();
}

CMouseEventResult ImguiVstguiView::onMouseDown(CPoint& where, const CButtonState& buttons)
{
	std::lock_guard<std::mutex> lock(stateLock);
	switch (buttons.getButtonState()) {
	case kLButton:
		g_MousePressed[0] = true;
	case kRButton:
		g_MousePressed[1] = true;
	case kMButton:
		g_MousePressed[2] = true;
		break;
	}
	return kMouseEventHandled;
}

CMouseEventResult ImguiVstguiView::onMouseUp(CPoint& where, const CButtonState& buttons)
{
	std::lock_guard<std::mutex> lock(stateLock);
	switch (buttons.getButtonState()) {
	case kLButton:
		g_MousePressed[0] = false;
	case kRButton:
		g_MousePressed[1] = false;
	case kMButton:
		g_MousePressed[2] = false;
		break;
	}
	return kMouseEventHandled;
}

CMouseEventResult ImguiVstguiView::onMouseMoved(CPoint& where, const CButtonState& buttons)
{
	std::lock_guard<std::mutex> lock(stateLock);
	g_MousePosition = where;
	return kMouseEventHandled;
}

bool ImguiVstguiView::onWheel(const CPoint& where, const float& distance, const CButtonState& buttons)
{
	g_MouseWheel = distance;
	return true;
}

int32_t ImguiVstguiView::onKeyDown(VstKeyCode& keyCode)
{
	/*
	OutputDebugStringA("onKeyDown\n");
	OutputDebugStringA(std::to_string(keyCode.character).c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA(std::to_string(keyCode.modifier).c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA(std::to_string(keyCode.virt).c_str());
	OutputDebugStringA("\n");
	*/
	return 1;
}

int32_t ImguiVstguiView::onKeyUp(VstKeyCode& keyCode)
{
	/*
	OutputDebugStringA("onKeyUp\n");
	OutputDebugStringA(std::to_string(keyCode.character).c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA(std::to_string(keyCode.modifier).c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA(std::to_string(keyCode.virt).c_str());
	OutputDebugStringA("\n");
	*/
	return 1;
}

void ImguiVstguiView::newFrame(CRect size)
{
	if (!g_FontTexture)
		createDeviceObjects();

	ImGuiIO& io = ImGui::GetIO();

	// Setup display size (every frame to accommodate for window resizing)
	int w, h;
	int display_w, display_h;
	w = (int)size.getWidth();
	h = (int)size.getHeight();
	// should be the same on windows
	// glfwGetFramebufferSize(g_Window, &display_w, &display_h);
	display_w = w;
	display_h = h;
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	// Setup time step
	// TODO
	//double current_time = glfwGetTime();
	double current_time = 0;
	io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
	g_Time = current_time;

	// Setup inputs

	stateLock.lock();

	std::copy_n(std::begin(g_KeysDown), 512, std::begin(io.KeysDown));

	//if (GetFocus() == g_Window)

	io.MousePos = ImVec2((float)g_MousePosition.x, (float)g_MousePosition.y);   // Mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)

	int keyCodes[3] = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };
	for (int i = 0; i < 3; i++)
	{
		io.MouseDown[i] = g_MousePressed[i] || (GetKeyState(keyCodes[i]) & 0x80) != 0;    // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
		g_MousePressed[i] = false;
	}

	io.MouseWheel = g_MouseWheel;
	g_MouseWheel = 0.0f;

	stateLock.unlock();

	io.KeyCtrl = io.KeysDown[VKEY_CONTROL];
	io.KeyShift = io.KeysDown[VKEY_ALT];
	io.KeySuper = false; // no super support

	/* TODO
	// Hide OS mouse cursor if ImGui is drawing it
	glfwSetInputMode(g_Window, GLFW_CURSOR, io.MouseDrawCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
	*/
	// Start the frame
	ImGui::NewFrame();
}

void ImguiVstguiView::renderDrawLists(ImDrawData* draw_data)
{
	// Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
	ImGuiIO& io = ImGui::GetIO();
	int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
	int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
	if (fb_width == 0 || fb_height == 0)
		return;
	draw_data->ScaleClipRects(io.DisplayFramebufferScale);

	// Backup GL state
	GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
	GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	GLint last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, &last_active_texture);
	GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	GLint last_element_array_buffer; glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
	GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
	GLint last_blend_src; glGetIntegerv(GL_BLEND_SRC, &last_blend_src);
	GLint last_blend_dst; glGetIntegerv(GL_BLEND_DST, &last_blend_dst);
	GLint last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
	GLint last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
	GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
	GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
	GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
	GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
	GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
	GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	// Setup viewport, orthographic projection matrix
	glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
	const float ortho_projection[4][4] =
	{
		{ 2.0f / io.DisplaySize.x, 0.0f,                   0.0f, 0.0f },
		{ 0.0f,                  2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
		{ 0.0f,                  0.0f,                  -1.0f, 0.0f },
		{ -1.0f,                  1.0f,                   0.0f, 1.0f },
	};
	glUseProgram(g_ShaderHandle);
	glUniform1i(g_AttribLocationTex, 0);
	glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
	glBindVertexArray(g_VaoHandle);

	for (int n = 0; n < draw_data->CmdListsCount; n++)
	{
		const ImDrawList* cmd_list = draw_data->CmdLists[n];
		const ImDrawIdx* idx_buffer_offset = 0;

		glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
		glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ElementsHandle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

		for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
		{
			const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
			if (pcmd->UserCallback)
			{
				pcmd->UserCallback(cmd_list, pcmd);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
				glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
				glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
			}
			idx_buffer_offset += pcmd->ElemCount;
		}
	}

	// Restore modified GL state
	glUseProgram(last_program);
	glActiveTexture(last_active_texture);
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindVertexArray(last_vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
	glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
	glBlendFunc(last_blend_src, last_blend_dst);
	if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
	if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
	if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
	if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);
	glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
	glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}

void ImguiVstguiView::createFontsTexture()
{
	ImGui::SetCurrentContext(imguiContext);

	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

															  // Upload texture to graphics system
	GLint last_texture;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGenTextures(1, &g_FontTexture);
	glBindTexture(GL_TEXTURE_2D, g_FontTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Store our identifier
	io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

	// Restore state
	glBindTexture(GL_TEXTURE_2D, last_texture);

}

void ImguiVstguiView::createDeviceObjects()
{
	// Backup GL state
	GLint last_texture, last_array_buffer, last_vertex_array;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

	const GLchar *vertex_shader =
		"#version 330\n"
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 UV;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = UV;\n"
		"	Frag_Color = Color;\n"
		"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* fragment_shader =
		"#version 330\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
		"}\n";

	g_ShaderHandle = glCreateProgram();
	g_VertHandle = glCreateShader(GL_VERTEX_SHADER);
	g_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(g_VertHandle, 1, &vertex_shader, 0);
	glShaderSource(g_FragHandle, 1, &fragment_shader, 0);
	glCompileShader(g_VertHandle);
	glCompileShader(g_FragHandle);
	glAttachShader(g_ShaderHandle, g_VertHandle);
	glAttachShader(g_ShaderHandle, g_FragHandle);
	glLinkProgram(g_ShaderHandle);

	g_AttribLocationTex = glGetUniformLocation(g_ShaderHandle, "Texture");
	g_AttribLocationProjMtx = glGetUniformLocation(g_ShaderHandle, "ProjMtx");
	g_AttribLocationPosition = glGetAttribLocation(g_ShaderHandle, "Position");
	g_AttribLocationUV = glGetAttribLocation(g_ShaderHandle, "UV");
	g_AttribLocationColor = glGetAttribLocation(g_ShaderHandle, "Color");

	glGenBuffers(1, &g_VboHandle);
	glGenBuffers(1, &g_ElementsHandle);

	glGenVertexArrays(1, &g_VaoHandle);
	glBindVertexArray(g_VaoHandle);
	glBindBuffer(GL_ARRAY_BUFFER, g_VboHandle);
	glEnableVertexAttribArray(g_AttribLocationPosition);
	glEnableVertexAttribArray(g_AttribLocationUV);
	glEnableVertexAttribArray(g_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
	glVertexAttribPointer(g_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(g_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(g_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

	createFontsTexture();

	// Restore modified GL state
	glBindTexture(GL_TEXTURE_2D, last_texture);
	glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
	glBindVertexArray(last_vertex_array);
}

void ImguiVstguiView::invalidateDeviceObjects()
{
	if (g_VaoHandle) glDeleteVertexArrays(1, &g_VaoHandle);
	if (g_VboHandle) glDeleteBuffers(1, &g_VboHandle);
	if (g_ElementsHandle) glDeleteBuffers(1, &g_ElementsHandle);
	g_VaoHandle = g_VboHandle = g_ElementsHandle = 0;

	if (g_ShaderHandle && g_VertHandle) glDetachShader(g_ShaderHandle, g_VertHandle);
	if (g_VertHandle) glDeleteShader(g_VertHandle);
	g_VertHandle = 0;

	if (g_ShaderHandle && g_FragHandle) glDetachShader(g_ShaderHandle, g_FragHandle);
	if (g_FragHandle) glDeleteShader(g_FragHandle);
	g_FragHandle = 0;

	if (g_ShaderHandle) glDeleteProgram(g_ShaderHandle);
	g_ShaderHandle = 0;

	if (g_FontTexture)
	{
		ImGui::SetCurrentContext(imguiContext);
		glDeleteTextures(1, &g_FontTexture);
		ImGui::GetIO().Fonts->TexID = 0;
		g_FontTexture = 0;
	}
}