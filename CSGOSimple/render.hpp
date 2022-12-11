#pragma once
#include <string>
#include <sstream>
#include <stdint.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <shared_mutex>
#include <deque>
#include <any>
#include <utility>
#include <vector>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

#include "singleton.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/impl/imgui_impl_dx9.h"
#include "imgui/impl/imgui_impl_win32.h"

#include "valve_sdk/misc/Color.hpp"
#include "valve_sdk/sdk.hpp"

extern ImFont* g_pDefaultFont;
extern ImFont* g_pSecondFont;


class Vector;


enum class EDrawType : int
{
	NONE = 0,
	LINE,
	RECT,
	RECT_MULTICOLOR,
	CIRCLE,
	TRIANGLE,
	POLYGON,
	TEXT,
	IMAGE
};

struct DrawObject_t
{
	DrawObject_t(const EDrawType nType, std::any&& pObject) :
		nType(nType), pObject(std::move(pObject)) { }

	EDrawType nType = EDrawType::NONE;
	std::any pObject = { };
};

enum ERectRenderFlags : unsigned int
{
	DRAW_RECT_NONE = 0,
	DRAW_RECT_OUTLINE = (1 << 0),
	DRAW_RECT_BORDER = (1 << 1),
	DRAW_RECT_FILLED = (1 << 2)
};

enum ECircleRenderFlags : unsigned int
{
	DRAW_CIRCLE_NONE = 0,
	DRAW_CIRCLE_OUTLINE = (1 << 0),
	DRAW_CIRCLE_FILLED = (1 << 1)
};

enum ETriangleRenderFlags : unsigned int
{
	DRAW_TRIANGLE_NONE = 0,
	DRAW_TRIANGLE_OUTLINE = (1 << 0),
	DRAW_TRIANGLE_FILLED = (1 << 1)
};

enum EPolygonRenderFlags : unsigned int
{
	DRAW_POLYGON_NONE = 0,
	DRAW_POLYGON_OUTLINE = (1 << 0),
	DRAW_POLYGON_FILLED = (1 << 1)
};

enum ETextRenderFlags : unsigned int
{
	DRAW_TEXT_NONE = 0,
	DRAW_TEXT_DROPSHADOW = (1 << 0),
	DRAW_TEXT_OUTLINE = (1 << 1)
};

struct LineObject_t
{
	ImVec2 vecStart = { };
	ImVec2 vecEnd = { };
	ImU32 colLine = 0x0;
	float flThickness = 0.f;
};

struct RectObject_t
{
	ImVec2 vecMin = { };
	ImVec2 vecMax = { };
	ImU32 colRect = 0x0;
	unsigned int uFlags = 0x0;
	ImU32 colOutline = 0x0;
	float flRounding = 0.f;
	ImDrawCornerFlags roundingCorners = ImDrawCornerFlags_None;
	float flThickness = 0.f;
};

struct RectMultiImColorObject_t
{
	ImVec2 vecMin = { };
	ImVec2 vecMax = { };
	ImU32 colUpperLeft = 0x0;
	ImU32 colUpperRight = 0x0;
	ImU32 colBottomRight = 0x0;
	ImU32 colBottomLeft = 0x0;
};

struct CircleObject_t
{
	ImVec2 vecCenter = { };
	float flRadius = 0.f;
	ImU32 colCircle = 0x0;
	int nSegments = 0;
	unsigned int uFlags = 0x0;
	ImU32 colOutline = 0x0;
	float flThickness = 0.f;
};

struct TriangleObject_t
{
	ImVec2 vecFirst = { };
	ImVec2 vecSecond = { };
	ImVec2 vecThird = { };
	ImU32 colTriangle = 0x0;
	unsigned int uFlags = 0x0;
	ImU32 colOutline = 0x0;
	float flThickness = 0.f;
};

struct PolygonObject_t
{
	std::vector<ImVec2> vecPoints = { };
	ImU32 colPolygon = 0x0;
	unsigned int uFlags = 0x0;
	ImU32 colOutline = 0x0;
	bool bClosed = false;
	float flThickness = 0.f;
};

struct TextObject_t
{
	const ImFont* pFont = nullptr;
	float flFontSize = 0.f;
	ImVec2 vecPosition = { };
	std::string szText = { };
	ImU32 colText = 0x0;
	unsigned int uFlags = 0x0;
	ImU32 colOutline = 0x0;
};

struct ImageObject_t
{
	ImTextureID pTexture = nullptr;
	ImVec2 vecMin = { };
	ImVec2 vecMax = { };
	ImU32 colImage = 0x0;
	float flRounding = 0.f;
	ImDrawCornerFlags roundingCorners = ImDrawCornerFlags_None;
};


class Render
	: public Singleton<Render>
{
	friend class Singleton<Render>;

private:
	std::deque<DrawObject_t> vecDrawData = { };
	std::deque<DrawObject_t> vecSafeDrawData = { };
	std::shared_mutex drawMutex = { };

	ImU32 GetU32(Color _color)
	{
		return ((_color[3] & 0xff) << 24) + ((_color[2] & 0xff) << 16) + ((_color[1] & 0xff) << 8)
			+ (_color[0] & 0xff);
	}
public:
	
	void Initialize();
	void GetFonts();
	void BeginScene();


	void RenderImage(ImTextureID user_texture_id, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a = ImVec2(0, 0), const ImVec2& uv_b = ImVec2(1, 1), ImU32 col = 0xFFFFFFFF) {
		AddImage(user_texture_id, a, b, ImColor(255, 255, 255), 0.f, 15);
	}

	template <class T>
	inline void RenderBoxByType(T x1, T y1, T x2, T y2, ImColor color, float thickness = 1.f, int type = 0)
	{
		if (type == 0)
			RenderBox(x1, y1, x2, y2, color, thickness);
		else if (type == 1)
			RenderCoalBox(x1, y1, x2, y2, color);
		else if (type == 2)
			RenderBox(x1, y1, x2, y2, color, thickness, 8.f);
	}

	template <class T>
	inline void RenderBoxFilledByType(T x1, T y1, T x2, T y2, ImColor color, float thickness = 1.f, int type = 0) {
		if (type == 0 || type == 1)
			RenderBoxFilled(x1, y1, x2, y2, color, thickness);
		else if (type == 2)
			RenderBoxFilled(x1, y1, x2, y2, color, thickness, 8.f);
	}

	template <class T>
	inline void RenderCoalBox(T x1, T y1, T x2, T y2, ImColor color, float th = 1.f) {
		int w = x2 - x1;
		int h = y2 - y1;

		int iw = w / 4;
		int ih = h / 4;
		// top
		RenderLine(x1, y1, x1 + iw, y1, color, th);					// left
		RenderLine(x1 + w - iw, y1, x1 + w, y1, color, th);			// right
		RenderLine(x1, y1, x1, y1 + ih, color, th);					// top left
		RenderLine(x1 + w - 1, y1, x1 + w - 1, y1 + ih, color, th);	// top right
																	// bottom
		RenderLine(x1, y1 + h, x1 + iw, y1 + h, color, th);			// left
		RenderLine(x1 + w - iw, y1 + h, x1 + w, y1 + h, color, th);	// right
		RenderLine(x1, y1 + h - ih, x1, y1 + h, color, th);			// bottom left
		RenderLine(x1 + w - 1, y1 + h - ih, x1 + w - 1, y1 + h, color, th);	// bottom right
	}

	template <class T>
	inline void RenderBox(T x1, T y1, T x2, T y2, ImColor color, float thickness = 1.f, float rounding = 0.f) {

		ImVec2 a(x1, y1), b(x2, y2);
		AddRect(a, b, color, 0, ImColor(0, 0, 0, 0), rounding, 15, thickness);
	}
	inline void RenderBox(RECT r, ImColor color, float thickness = 1.f, float rounding = 0.f) {
		RenderBox(r.left, r.top, r.right, r.bottom, color, thickness, rounding);
	}
	template <class T>
	inline void RenderBoxFilled(T x1, T y1, T x2, T y2, ImColor color, float thickness = 1.f, float rounding = 0.f) {
		AddRect(ImVec2(x1, y1), ImVec2( x2,  y2), color, DRAW_RECT_FILLED, ImColor(0, 0, 0, 0), rounding, 15, thickness);
	}
	template <class T>
	inline void RenderLine(T x1, T y1, T x2, T y2, ImColor color, float thickness = 1.f) {
		AddLine(ImVec2(x1, y1), ImVec2(x2, y2), color, thickness);
	}

	template <class T>
	inline void RenderText(std::string text, T x, T y, float size, ImColor clr,ImFont* pFont) {
		AddText(pFont, size, ImVec2(x, y), text, clr, DRAW_TEXT_OUTLINE, ImColor(0, 0, 0));
	}

	void RenderDrawData(ImDrawList* pDrawList)
	{
		std::unique_lock<std::shared_mutex> lock(drawMutex);

		// prevent render in main menu
		if (!g_EngineClient->IsInGame())
			return;

		if (vecSafeDrawData.empty())
			return;

		for (const auto& data : vecSafeDrawData)
		{
			if (!data.pObject.has_value())
				continue;

			switch (data.nType)
			{
			case EDrawType::LINE:
			{
				const auto& pObject = std::any_cast<LineObject_t>(data.pObject);
				pDrawList->AddLine(pObject.vecStart, pObject.vecEnd, pObject.colLine, pObject.flThickness);
				break;
			}
			case EDrawType::RECT:
			{
				const auto& pObject = std::any_cast<RectObject_t>(data.pObject);
				AddDrawListRect(pDrawList, pObject.vecMin, pObject.vecMax, pObject.colRect, pObject.uFlags, pObject.colOutline, pObject.flRounding, pObject.roundingCorners, pObject.flThickness);
				break;
			}
			case EDrawType::RECT_MULTICOLOR:
			{
				const auto& pObject = std::any_cast<RectMultiImColorObject_t>(data.pObject);
				pDrawList->AddRectFilledMultiColor(pObject.vecMin, pObject.vecMax, pObject.colUpperLeft, pObject.colUpperRight, pObject.colBottomRight, pObject.colBottomLeft);
				break;
			}
			case EDrawType::CIRCLE:
			{
				const auto& pObject = std::any_cast<CircleObject_t>(data.pObject);

				if (pObject.uFlags & DRAW_CIRCLE_FILLED)
					pDrawList->AddCircleFilled(pObject.vecCenter, pObject.flRadius, pObject.colCircle, pObject.nSegments);
				else
					pDrawList->AddCircle(pObject.vecCenter, pObject.flRadius, pObject.colCircle, pObject.nSegments, pObject.flThickness);

				if (pObject.uFlags & DRAW_CIRCLE_OUTLINE)
					pDrawList->AddCircle(pObject.vecCenter, pObject.flRadius + 1.0f, pObject.colOutline, pObject.nSegments, pObject.flThickness + 1.0f);

				break;
			}
			case EDrawType::TRIANGLE:
			{
				const auto& pObject = std::any_cast<TriangleObject_t>(data.pObject);

				if (pObject.uFlags & DRAW_TRIANGLE_FILLED)
					pDrawList->AddTriangleFilled(pObject.vecFirst, pObject.vecSecond, pObject.vecThird, pObject.colTriangle);
				else
					pDrawList->AddTriangle(pObject.vecFirst, pObject.vecSecond, pObject.vecThird, pObject.colTriangle, pObject.flThickness);

				if (pObject.uFlags & DRAW_TRIANGLE_OUTLINE)
					pDrawList->AddTriangle(pObject.vecFirst, pObject.vecSecond, pObject.vecThird, pObject.colOutline, pObject.flThickness + 1.0f);

				break;
			}
			case EDrawType::POLYGON:
			{
				const auto& pObject = std::any_cast<PolygonObject_t>(data.pObject);

				if (pObject.uFlags & DRAW_POLYGON_FILLED)
					pDrawList->AddConvexPolyFilled(pObject.vecPoints.data(), pObject.vecPoints.size(), pObject.colPolygon);
				else
					pDrawList->AddPolyline(pObject.vecPoints.data(), pObject.vecPoints.size(), pObject.colPolygon, pObject.bClosed, pObject.flThickness);

				if (pObject.uFlags & DRAW_POLYGON_OUTLINE)
					pDrawList->AddPolyline(pObject.vecPoints.data(), pObject.vecPoints.size(), pObject.colOutline, pObject.bClosed, pObject.flThickness + 1.0f);

				break;
			}
			case EDrawType::TEXT:
			{
				const auto& pObject = std::any_cast<TextObject_t>(data.pObject);
				AddDrawListText(pDrawList, pObject.pFont, pObject.flFontSize, pObject.vecPosition, pObject.szText, pObject.colText, pObject.uFlags, pObject.colOutline);
				break;
			}
			case EDrawType::IMAGE:
			{
				const auto& pObject = std::any_cast<ImageObject_t>(data.pObject);
				pDrawList->AddImageRounded(pObject.pTexture, pObject.vecMin, pObject.vecMax, ImVec2(0, 0), ImVec2(1, 1), pObject.colImage, pObject.flRounding, pObject.roundingCorners);
				break;
			}
			default:
				break;
			}
		}
	}

	void ClearDrawData()
	{
		if (!vecDrawData.empty())
			vecDrawData.clear();
	}

	void SwapDrawData()
	{
		std::unique_lock<std::shared_mutex> lock(drawMutex);
		vecDrawData.swap(vecSafeDrawData);
	}

	void AddLine(const ImVec2& vecStart, const ImVec2& vecEnd, const ImColor& colLine, float flThickness)
	{
		vecDrawData.emplace_back(EDrawType::LINE, std::make_any<LineObject_t>(LineObject_t{ vecStart, vecEnd, colLine, flThickness }));
	}

	void AddRect(const ImVec2& vecMin, const ImVec2& vecMax, const ImColor& colRect, unsigned int uFlags, const ImColor& colOutline, float flRounding, ImDrawCornerFlags roundingCorners, float flThickness)
	{
		vecDrawData.emplace_back(EDrawType::RECT, std::make_any<RectObject_t>(RectObject_t{ vecMin, vecMax, (colRect), uFlags, (colOutline), flRounding, roundingCorners, flThickness }));
	}

	void AddRectMultiImColor(const ImVec2& vecMin, const ImVec2& vecMax, const ImColor& colUpperLeft, const ImColor& colUpperRight, const ImColor& colBottomRight, const ImColor& colBottomLeft)
	{
		vecDrawData.emplace_back(EDrawType::RECT_MULTICOLOR, std::make_any<RectMultiImColorObject_t>(RectMultiImColorObject_t{ vecMin, vecMax, (colUpperLeft), (colUpperRight), (colBottomRight), (colBottomLeft) }));
	}

	void AddCircle(const ImVec2& vecCenter, float flRadius, const ImColor& colCircle, int nSegments, unsigned int uFlags, const ImColor& colOutline, float flThickness)
	{
		vecDrawData.emplace_back(EDrawType::CIRCLE, std::make_any<CircleObject_t>(CircleObject_t{ vecCenter, flRadius, (colCircle), nSegments, uFlags, (colOutline), flThickness }));
	}

	void AddTriangle(const ImVec2& vecFirst, const ImVec2& vecSecond, const ImVec2& vecThird, const ImColor& colTriangle, unsigned int uFlags, const ImColor& colOutline, float flThickness)
	{
		vecDrawData.emplace_back(EDrawType::TRIANGLE, std::make_any<TriangleObject_t>(TriangleObject_t{ vecFirst, vecSecond, vecThird, (colTriangle), uFlags, (colOutline), flThickness }));
	}

	void AddPolygon(std::vector<ImVec2>& vecPoints, const ImColor& colPolygon, unsigned int uFlags, const ImColor& colOutline, bool bClosed, float flThickness)
	{
		vecDrawData.emplace_back(EDrawType::POLYGON, std::make_any<PolygonObject_t>(PolygonObject_t{ std::move(vecPoints), (colPolygon), uFlags, (colOutline), bClosed, flThickness }));
	}

	void AddText(const ImFont* pFont, float flFontSize, const ImVec2& vecPosition, const std::string& szText, const ImColor& colText, unsigned int uFlags, const ImColor& colOutline)
	{
		if (pFont->ContainerAtlas == nullptr)
			return;

		// check is only one flag selected
		IM_ASSERT(ImIsPowerOfTwo(uFlags == DRAW_TEXT_NONE || uFlags & (DRAW_TEXT_DROPSHADOW | DRAW_TEXT_OUTLINE)));

		vecDrawData.emplace_back(EDrawType::TEXT, std::make_any<TextObject_t>(TextObject_t{ pFont, flFontSize, vecPosition, szText, (colText), uFlags, (colOutline) }));
	}

	void AddText(const ImVec2& vecPosition, const std::string& szText, const ImColor& colText, int iFlags, const ImColor& colOutline)
	{
		AddText(nullptr, 0.f, vecPosition, szText, colText, iFlags, colOutline);
	}

	void AddImage(ImTextureID pTexture, const ImVec2& vecMin, const ImVec2& vecMax, const ImColor& colImage, float flRounding, ImDrawCornerFlags roundingCorners)
	{
		vecDrawData.emplace_back(EDrawType::IMAGE, std::make_any<ImageObject_t>(ImageObject_t{ pTexture, vecMin, vecMax, (colImage), flRounding, roundingCorners }));
	}

	void AddDrawListRect(ImDrawList* pDrawList, const ImVec2& vecMin, const ImVec2& vecMax, ImU32 colRect, unsigned int uFlags, ImU32 colOutline, float flRounding, ImDrawCornerFlags roundingCorners, float flThickness)
	{
		if (uFlags & DRAW_RECT_FILLED)
			pDrawList->AddRectFilled(vecMin, vecMax, colRect, flRounding, roundingCorners);
		else
			pDrawList->AddRect(vecMin, vecMax, colRect, flRounding, roundingCorners, flThickness);

		if (uFlags & DRAW_RECT_BORDER)
			pDrawList->AddRect(vecMin + ImVec2(1.0f, 1.0f), vecMax - ImVec2(1.0f, 1.0f), colOutline, flRounding, roundingCorners, 1.0f);

		if (uFlags & DRAW_RECT_OUTLINE)
			pDrawList->AddRect(vecMin - ImVec2(1.0f, 1.0f), vecMax + ImVec2(1.0f, 1.0f), colOutline, flRounding, roundingCorners, 1.0f);
	}

	void AddDrawListText(ImDrawList* pDrawList, const ImFont* pFont, float flFontSize, const ImVec2& vecPosition, const std::string& szText, ImU32 colText, unsigned int uFlags, ImU32 colOutline)
	{
		// set font texture
		pDrawList->PushTextureID(pFont->ContainerAtlas->TexID);

		if (uFlags & DRAW_TEXT_DROPSHADOW)
			pDrawList->AddText(pFont, flFontSize, vecPosition + ImVec2(1.0f, -1.0f), colOutline, szText.c_str());
		else if (uFlags & DRAW_TEXT_OUTLINE)
		{
			pDrawList->AddText(pFont, flFontSize, ImVec2(vecPosition.x + 1, vecPosition.y + 1), colOutline, szText.c_str());
			pDrawList->AddText(pFont, flFontSize, ImVec2(vecPosition.x - 1, vecPosition.y - 1), colOutline, szText.c_str());
			pDrawList->AddText(pFont, flFontSize, ImVec2(vecPosition.x + 1, vecPosition.y - 1), colOutline, szText.c_str());
			pDrawList->AddText(pFont, flFontSize, ImVec2(vecPosition.x - 1, vecPosition.y + 1), colOutline, szText.c_str());
			pDrawList->AddText(pFont, flFontSize, ImVec2(vecPosition.x + 1, vecPosition.y), colOutline, szText.c_str());
			pDrawList->AddText(pFont, flFontSize, ImVec2(vecPosition.x - 1, vecPosition.y), colOutline, szText.c_str());
			pDrawList->AddText(pFont, flFontSize, ImVec2(vecPosition.x, vecPosition.y - 1), colOutline, szText.c_str());
			pDrawList->AddText(pFont, flFontSize, ImVec2(vecPosition.x, vecPosition.y + 1), colOutline, szText.c_str());

			//pDrawList->AddText(pFont, flFontSize, vecPosition + ImVec2(1.0f, -1.0f), colOutline, szText.c_str());
			//pDrawList->AddText(pFont, flFontSize, vecPosition + ImVec2(-1.0f, 1.0f), colOutline, szText.c_str());
		}

		pDrawList->AddText(pFont, flFontSize, vecPosition, colText, szText.data());
		pDrawList->PopTextureID();
	}
};