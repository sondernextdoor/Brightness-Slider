#pragma once
#include "gui.h"
#include <physicalmonitorenumerationapi.h>
#include <highlevelmonitorconfigurationapi.h>

#pragma comment(lib, "Dxva2.lib")


HANDLE GetMonitor() {
	DWORD NumberOfPhysicalMonitors{ 0 };
	PHYSICAL_MONITOR* PhysicalMonitors{ nullptr };

	HMONITOR hMonitor{ MonitorFromWindow(GetTopWindow(nullptr), NULL) };

	if (!hMonitor || hMonitor == INVALID_HANDLE_VALUE) {
		return INVALID_HANDLE_VALUE;
	}

	if (!GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &NumberOfPhysicalMonitors)) {
		return INVALID_HANDLE_VALUE;
	}

	PhysicalMonitors = static_cast<PHYSICAL_MONITOR*>(malloc(
		NumberOfPhysicalMonitors * sizeof(PHYSICAL_MONITOR)
	));

	if (!GetPhysicalMonitorsFromHMONITOR(hMonitor, 
					     NumberOfPhysicalMonitors, 
					     PhysicalMonitors)) {
		free(PhysicalMonitors);
		return INVALID_HANDLE_VALUE;
	}

	return PhysicalMonitors->hPhysicalMonitor;
}


COLORREF StringToColor(std::string SourceString) {
	if (SourceString == "white" || SourceString == "White" || SourceString == "WHITE")
		return RGB(255, 255, 255);
	if (SourceString == "black" || SourceString == "Black" || SourceString == "BLACK")
		return RGB(0, 0, 0);
	if (SourceString == "red" || SourceString == "Red" || SourceString == "RED")
		return RGB(255, 0, 0);
	if (SourceString == "blue" || SourceString == "Blue" || SourceString == "BLUE")
		return RGB(0, 0, 255);
	if (SourceString == "green" || SourceString == "Green" || SourceString == "GREEN")
		return RGB(0, 128, 0);
	if (SourceString == "purple" || SourceString == "Purple" || SourceString == "PURPLE")
		return RGB(100, 0, 135);
	if (SourceString == "navy" || SourceString == "Navy" || SourceString == "NAVY")
		return RGB(0, 0, 128);
	if (SourceString == "maroon" || SourceString == "Maroon" || SourceString == "MAROON")
		return RGB(128, 0, 0);

	return RGB(255, 255, 255);
}
