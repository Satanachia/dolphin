// Copyright (C) 2003-2008 Dolphin Project.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 2.0.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License 2.0 for more details.

// A copy of the GPL 2.0 should have been included with the program.
// If not, see http://www.gnu.org/licenses/

// Official SVN repository and contact information can be found at
// http://code.google.com/p/dolphin-emu/

#ifndef _EMU_DECLARATIONS_
#define _EMU_DECLARATIONS_

#include "pluginspecs_wiimote.h"

#include <vector>
#include <string>
#include "Common.h"
#include "wiimote_hid.h"
#include "EmuDefinitions.h"
#include "Encryption.h"
#include "Console.h" // for startConsoleWin, wprintf, GetConsoleHwnd

extern SWiimoteInitialize g_WiimoteInitialize;
//extern void __Log(int log, const char *format, ...);
//extern void __Log(int log, int v, const char *format, ...);

namespace WiiMoteEmu
{
	
//******************************************************************************
// Definitions and variable declarations
//******************************************************************************


extern u8 g_Leds;
extern u8 g_IR;

extern u8 g_Eeprom[WIIMOTE_EEPROM_SIZE];

extern u8 g_RegSpeaker[WIIMOTE_REG_SPEAKER_SIZE];
extern u8 g_RegExt[WIIMOTE_REG_EXT_SIZE];
extern u8 g_RegExtTmp[WIIMOTE_REG_EXT_SIZE];
extern u8 g_RegIr[WIIMOTE_REG_IR_SIZE];

extern u8 g_ReportingMode;
extern u16 g_ReportingChannel;

extern wiimote_key g_ExtKey; // extension encryption key


static const u8 EepromData_0[] = {
	0xA1, 0xAA, 0x8B, 0x99, 0xAE, 0x9E, 0x78, 0x30,
	0xA7, 0x74, 0xD3, 0xA1, 0xAA, 0x8B, 0x99, 0xAE,
	0x9E, 0x78, 0x30, 0xA7, 0x74, 0xD3, 0x82, 0x82,
	0x82, 0x15, 0x9C, 0x9C, 0x9E, 0x38, 0x40, 0x3E,
	0x82, 0x82, 0x82, 0x15, 0x9C, 0x9C, 0x9E, 0x38,
	0x40, 0x3E
};

static const u8 EepromData_16D0[] = {
	0x00, 0x00, 0x00, 0xFF, 0x11, 0xEE, 0x00, 0x00,
	0x33, 0xCC, 0x44, 0xBB, 0x00, 0x00, 0x66, 0x99,
	0x77, 0x88, 0x00, 0x00, 0x2B, 0x01, 0xE8, 0x13
};


/* Default calibration for the nunchuck. It should be written to 0x20 - 0x3f of the
   extension register. 0x80 is the neutral x and y accelerators and 0xb3 is the
   neutral z accelerometer that is adjusted for gravity. */
static const u8 nunchuck_calibration[] =
{
	0x80,0x80,0x80,0x00, 0xb3,0xb3,0xb3,0x00,
	0xe0,0x20,0x80,0xe0, 0x20,0x80,0xee,0x43,
	0x80,0x80,0x80,0x00, 0xb3,0xb3,0xb3,0x00,
	0xe0,0x20,0x80,0xe0, 0x20,0x80,0xee,0x43
};

/* Classic Controller calibration. 0x80 is the neutral for the analog triggers and
   sticks. The left analog range is 0x1c - 0xe4 and the right is 0x28 - 0xd8.
   We use this range because it's closest to the GC controller range. */
static const u8 classic_calibration[] =
{
	0xe4,0x1c,0x80,0xe4, 0x1c,0x80,0xd8,0x28,
	0x80,0xd8,0x28,0x80, 0x20,0x20,0x95,0xea,
	0xe4,0x1c,0x80,0xe4, 0x1c,0x80,0xd8,0x28,
	0x80,0xd8,0x28,0x80, 0x20,0x20,0x95,0xea
};



/* The Nunchuck id. It should be written to the last bytes of the
   extension register */
static const u8 nunchuck_id[] =
{
	0x00, 0x00, 0xa4, 0x20, 0x00, 0x00
};

/* The Classic Controller id. It should be written to the last bytes of the
   extension register */
static const u8 classic_id[] =
{
	0x00, 0x00, 0xa4, 0x20, 0x01, 0x01
};

/* The id for nothing inserted */
static const u8 nothing_id[] =
{
	0x00, 0x00, 0x00, 0x00, 0x2e, 0x2e
};

/* The id for a partially inserted extension */
static const u8 partially_id[] =
{
	0x00, 0x00, 0x00, 0x00, 0xff, 0xff
};


void HidOutputReport(u16 _channelID, wm_report* sr);

void WmLeds(u16 _channelID, wm_leds* leds);
void WmReadData(u16 _channelID, wm_read_data* rd);
void WmWriteData(u16 _channelID, wm_write_data* wd);
void WmRequestStatus(u16 _channelID, wm_request_status* rs);
void WmRequestStatus_(u16 _channelID, int a);
void WmDataReporting(u16 _channelID, wm_data_reporting* dr);

void SendReadDataReply(u16 _channelID, void* _Base, u16 _Address, u8 _Size);
void SendReportCoreAccel(u16 _channelID);
void SendReportCoreAccelIr12(u16 _channelID);
void SendReportCore(u16 _channelID);
void SendReportCoreAccelExt16(u16 _channelID);
void SendReportCoreAccelIr10Ext(u16 _channelID);

int WriteWmReport(u8* dst, u8 channel);
void WmSendAck(u16 _channelID, u8 _reportID, u32 address);


void FillReportAcc(wm_accel& _acc);
void FillReportInfo(wm_core& _core);
void FillReportIR(wm_ir_extended& _ir0, wm_ir_extended& _ir1);
void FillReportIRBasic(wm_ir_basic& _ir0, wm_ir_basic& _ir1);
void FillReportExtension(wm_extension& _ext);
void FillReportClassicExtension(wm_classic_extension& _ext);

u32 convert24bit(const u8* src);
u16 convert16bit(const u8* src);
void GetMousePos(float& x, float& y);

} // namespace

#endif	//_EMU_DECLARATIONS_