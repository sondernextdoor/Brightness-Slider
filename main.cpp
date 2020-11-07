#include "utils.h"

HANDLE hMonitor{ nullptr };

void OnValueChanged(SliderObject* Slider){
	SetMonitorBrightness(hMonitor, Slider->GetValue<DWORD>());
}


void OnTick(SliderObject* Slider) {}
void OnExit(SliderObject* Slider) {}


void BrightnessSlider() {
	DWORD Min{ 0 };
	DWORD Current{ 0 };
	DWORD Max{ 0 };

	GetMonitorBrightness(hMonitor = GetMonitor(), &Min, &Current, &Max);

	SliderObject Slider;
	Slider.Init(OnValueChanged, OnExit, OnTick, StringToColor("White"), Min, Max, Current, "Brightness");
}


int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	BrightnessSlider();
}