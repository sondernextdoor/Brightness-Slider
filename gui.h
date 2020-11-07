#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include <CommCtrl.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")


class SliderObject;
typedef void(*SliderCallback)(PVOID);
std::unique_ptr<SliderObject> gSliderObject{ nullptr };
WNDPROC gSliderObjectProcedure{ nullptr };
LRESULT CALLBACK SliderObjectProcedure(HWND hWindow, UINT Message, WPARAM WP, LPARAM LP);


class SliderObject {
public:

	SliderCallback OnValueChanged{ nullptr };
	SliderCallback OnExit{ nullptr };
	SliderCallback OnTick{ nullptr };

	HWND Parent{ nullptr };
	HWND Display{ nullptr };
	HWND Slider{ nullptr };

	int Value{ 0 };
	int MinimumValue{ 0 };
	int MaximumValue{ 0 };
	int DefaultValue{ 0 };
	LPCSTR WindowName{ nullptr };
	DWORD SliderStyle{ 0 };
	COLORREF BkgColor{ 0 };


	template <typename T = SliderCallback>
	void Init(
		T OnValueChanged,
		T OnExit,
		T OnTick,
		COLORREF BkgColor,
		int MinimumValue = 1,
		int MaximumValue = 100,
		int DefaultValue = 100,
		LPCSTR WindowName = "", 
		DWORD SliderStyle = WS_CHILD | TBS_HORZ | WS_VISIBLE | TBS_TOOLTIPS,
		DWORD ParentStyle = WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, 
		int X = -1,
		int Y = -1,
		int Width = 300,
		int Height = 40
	) 
	{
		this->OnValueChanged = reinterpret_cast<SliderCallback>(OnValueChanged);
		this->OnExit = reinterpret_cast<SliderCallback>(OnExit);
		this->OnTick = reinterpret_cast<SliderCallback>(OnTick);
		this->BkgColor = BkgColor;
		this->Value = DefaultValue;
		this->MinimumValue = MinimumValue;
		this->MaximumValue = MaximumValue;
		this->DefaultValue = DefaultValue;
		this->WindowName = WindowName;
		this->SliderStyle = SliderStyle;
		
		this->Parent = CreateWindowExA(0, (LPCSTR)WC_DIALOG, WindowName, ParentStyle, 0, 0, 0, 0, nullptr, nullptr, nullptr, nullptr);
		this->Slider = CreateWindowExW(0, TRACKBAR_CLASSW, nullptr, SliderStyle, X, Y, Width, Height, this->Parent, nullptr, nullptr, nullptr);

		gSliderObject = std::make_unique<SliderObject>(*this);
		gSliderObjectProcedure = (WNDPROC)SetWindowLongPtrW(gSliderObject->Parent, GWLP_WNDPROC, (LONG_PTR)SliderObjectProcedure);
	
		SendMessageW(gSliderObject->Slider, TBM_SETRANGEMIN, gSliderObject->MinimumValue, gSliderObject->MinimumValue);
		SendMessageW(gSliderObject->Slider, TBM_SETRANGEMAX, gSliderObject->MinimumValue, gSliderObject->MaximumValue);
		SendMessageW(gSliderObject->Slider, TBM_SETPOS, gSliderObject->DefaultValue, gSliderObject->DefaultValue);

		SetWindowPos(gSliderObject->Parent, HWND_TOP, 0, 0, 312, 60, SWP_SHOWWINDOW);
		ShowWindow(gSliderObject->Parent, SW_SHOW);
		
		MSG Message = { 0 };

		do {
			DispatchMessageW(&Message);
			this->OnTick(this);
		} while (GetMessageW(&Message, nullptr, 0, 0));
	}

	void SetWindowName(std::string_view Name) {
		SetWindowTextA(this->Parent, Name.data());
		this->WindowName = Name.data();
	}

	template <typename T>
	T GetValue() {
		return *reinterpret_cast<T*>(&this->Value);
	}

	template <typename T>
	void SetValue(T Value) {
		this->Value = *reinterpret_cast<int*>(&Value);
	}
};


LRESULT CALLBACK SliderObjectProcedure(HWND hWindow, UINT Message, WPARAM WP, LPARAM LP) {
	switch (Message) {


		case WM_HSCROLL: {
			if (reinterpret_cast<HWND>(LP) == gSliderObject->Slider) {

				SendMessageW(gSliderObject->Display, 
					     WM_SETTEXT, 
					     NULL, 
					     (LPARAM)std::to_wstring(
					     gSliderObject->Value = SendMessage(
					     gSliderObject->Slider, 
					     TBM_GETPOS, 
					     0, 
					     0)).c_str());

				gSliderObject->OnValueChanged(gSliderObject.get());
			}
		} break;


		case WM_CLOSE: {
			gSliderObject->OnExit(gSliderObject.get());
			PostQuitMessage(0);
		} break;


		case WM_CTLCOLORSTATIC: {

			if (RGB(255, 255, 255) == gSliderObject->BkgColor) {
				SetTextColor((HDC)WP, RGB(0, 0, 0));
			} else {
				SetTextColor((HDC)WP, RGB(255, 255, 255));
			}

			SetBkColor((HDC)WP, gSliderObject->BkgColor);
			return (BOOL)CreateSolidBrush(gSliderObject->BkgColor);

		} break;


		case WM_CTLCOLORDLG: {

			if (RGB(255, 255, 255) == gSliderObject->BkgColor) {
				SetTextColor((HDC)WP, RGB(0, 0, 0));
			} else {
				SetTextColor((HDC)WP, RGB(255, 255, 255));
			}

			SetBkColor((HDC)WP, gSliderObject->BkgColor);
			return (BOOL)CreateSolidBrush(gSliderObject->BkgColor);

		} break;
	}
	
	return CallWindowProcW(gSliderObjectProcedure, hWindow, Message, WP, LP);
}
