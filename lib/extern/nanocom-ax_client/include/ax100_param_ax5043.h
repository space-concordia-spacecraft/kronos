#ifndef AX100_PARAM_AX5043_H_
#define AX100_PARAM_AX5043_H_
/* Copyright (c) 2013-2018 GomSpace A/S. All rights reserved. */
/*
 * ax100_param_ax5043.h
 *
 */

#include <stdint.h>
#include <param/param_types.h>

#define AX5043_PARAM_SIZE	0xFFF

static const param_table_t ax5043_config[] = {

		/* Revision & interface probing */
		{.name = "revision", 		.addr = 0x000, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "scratch", 		.addr = 0x001, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Operating mode */
		{.name = "pwrmode", 		.addr = 0x002, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Voltage regulator */
		{.name = "powstat", 		.addr = 0x003, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "powststat",		.addr = 0x004, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "powirqmask", 		.addr = 0x005, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Interrupt control */
		{.name = "irqmask", 		.addr = 0x006, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "rdevenmask", 		.addr = 0x008, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "irqinversi", 		.addr = 0x00A, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "irqrequest", 		.addr = 0x00C, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "rdevenrequ", 		.addr = 0x00E, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},

		/* Modulation and framing */
		{.name = "modulation", 		.addr = 0x010, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "encoding", 		.addr = 0x011, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "framing", 		.addr = 0x012, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "crcinit", 		.addr = 0x014, 					.type = PARAM_X32, 	.size = sizeof(uint32_t)},

		/* Forward error correction */
		{.name = "fec", 			.addr = 0x018, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "fecsync", 		.addr = 0x019, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "fecstatus", 		.addr = 0x01A, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Status */
		{.name = "radiostate", 		.addr = 0x01C, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "xtalstatus", 		.addr = 0x01D, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Pin configuration */
		{.name = "pinstate", 		.addr = 0x020, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pinfunsysc", 		.addr = 0x021, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pinfundclk", 		.addr = 0x022, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pinfundata", 		.addr = 0x023, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pinfunirq", 		.addr = 0x024, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pinfunants", 		.addr = 0x025, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pinfunpwra", 		.addr = 0x026, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pwramp",	 		.addr = 0x027, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* FIFO */
		{.name = "fifostat",	 	.addr = 0x028, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "fifodata",	 	.addr = 0x029, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "fifocount",	 	.addr = 0x02A, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "fifofree",	 	.addr = 0x02C, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "fifothresh",	 	.addr = 0x02E, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},

		/* Synthesizer */
		{.name = "pllloop",	 		.addr = 0x030, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pllcpi",	 		.addr = 0x031, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pllvcodiv",	 	.addr = 0x032, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pllrnga",	 		.addr = 0x033, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "freqa",	 		.addr = 0x034, 					.type = PARAM_X32, 	.size = sizeof(uint32_t)},
		{.name = "pllloopbst",	 	.addr = 0x038, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pllcpibst",	 	.addr = 0x039, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pllrngb",	 		.addr = 0x03B, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "freqb",	 		.addr = 0x03C, 					.type = PARAM_X32, 	.size = sizeof(uint32_t)},

		/* Signal strength */
		{.name = "rssi",		 	.addr = 0x040, 					.type = PARAM_INT8, .size = sizeof(int8_t)},
		{.name = "bgndrssi",	 	.addr = 0x041, 					.type = PARAM_INT8, .size = sizeof(int8_t)},
		{.name = "diversity",	 	.addr = 0x042, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "agccounter",	 	.addr = 0x043, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Receiver tracking */
		{.name = "trkdatart2",	 	.addr = 0x045, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "trkdatart1",	 	.addr = 0x046, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "trkdatart0",	 	.addr = 0x047, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "trkampl",	 		.addr = 0x048, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "trkphase",	 	.addr = 0x04A, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "trkrffreq2",	 	.addr = 0x04D, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "trkrffreq1",	 	.addr = 0x04E, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "trkrffreq0",	 	.addr = 0x04F, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "trkfreq",		 	.addr = 0x050, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "trkfskdem",	 	.addr = 0x052, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "trkafskdem",	 	.addr = 0x054, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},

		/* Timer */
		{.name = "timer2",		 	.addr = 0x059, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "timer1",	 		.addr = 0x05A, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "timer0",	 		.addr = 0x05B, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Wakeup Timer */
		{.name = "wkuptimer",	 	.addr = 0x068, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "wkup",	 		.addr = 0x06A, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "wkupfrq",	 		.addr = 0x06C, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "wkupxoearl",	 	.addr = 0x06E, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/**
		 *  Physical layer parameters
		 **/
		{.name = "iffreq",	 		.addr = 0x100, 					.type = PARAM_UINT16,.size = sizeof(uint16_t)},
		{.name = "decimation", 		.addr = 0x102, 					.type = PARAM_UINT8,.size = sizeof(uint8_t)},
		{.name = "rxdatrate2",	 	.addr = 0x103, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "rxdatrate1",	 	.addr = 0x104, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "rxdatrate0",	 	.addr = 0x105, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "maxdroffs2",	 	.addr = 0x106, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "maxdroffs1",	 	.addr = 0x107, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "maxdroffs0",	 	.addr = 0x108, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "maxrfoffs2",	 	.addr = 0x109, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "maxrfoffs1",	 	.addr = 0x10A, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "maxrfoffs0",	 	.addr = 0x10B, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "fskdmax",	 		.addr = 0x10C, 					.type = PARAM_UINT16,.size = sizeof(uint16_t)},
		{.name = "fskdmin",	 		.addr = 0x10E, 					.type = PARAM_UINT16,.size = sizeof(uint16_t)},
		{.name = "afskspace",		.addr = 0x110, 					.type = PARAM_UINT16,.size = sizeof(uint16_t)},
		{.name = "afskmark",		.addr = 0x112, 					.type = PARAM_UINT16,.size = sizeof(uint16_t)},
		{.name = "afskctrl",	 	.addr = 0x114, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "amplfilter",	 	.addr = 0x115, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "freqleak",	 	.addr = 0x116, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "rxparamsets",	 	.addr = 0x117, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "rxcurparamcur", 	.addr = 0x118, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Parameter set 0 */
		{.name = "0agcgain",	 	.addr = 0x120, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0agctarget",	 	.addr = 0x121, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0agcahyst",	 	.addr = 0x122, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0agcminmax",	 	.addr = 0x123, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0timegain",	 	.addr = 0x124, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0drgain",	 		.addr = 0x125, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0phasegain",	 	.addr = 0x126, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0freqgaina",	 	.addr = 0x127, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0freqgainb",	 	.addr = 0x128, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0freqgainc",	 	.addr = 0x129, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0freqgaind",	 	.addr = 0x12A, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0amplgain",	 	.addr = 0x12B, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0freqdev",	 	.addr = 0x12C, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "0fourfsk",	 	.addr = 0x12E, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "0bboffsres",	 	.addr = 0x12F, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Parameter set 1 */
		{.name = "1agcgain",	 	.addr = 0x130, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1agctarget",	 	.addr = 0x131, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1agcahyst",	 	.addr = 0x132, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1agcminmax",	 	.addr = 0x133, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1timegain",	 	.addr = 0x134, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1drgain",	 		.addr = 0x135, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1phasegain",	 	.addr = 0x136, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1freqgaina",	 	.addr = 0x137, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1freqgainb",	 	.addr = 0x138, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1freqgainc",	 	.addr = 0x139, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1freqgaind",	 	.addr = 0x13A, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1amplgain",	 	.addr = 0x13B, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1freqdev",	 	.addr = 0x13C, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "1fourfsk",	 	.addr = 0x13E, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "1bboffsres",	 	.addr = 0x13F, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Parameter set 2 */
		{.name = "2agcgain",	 	.addr = 0x140, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2agctarget",	 	.addr = 0x141, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2agcahyst",	 	.addr = 0x142, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2agcminmax",	 	.addr = 0x143, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2timegain",	 	.addr = 0x144, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2drgain",	 		.addr = 0x145, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2phasegain",	 	.addr = 0x146, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2freqgaina",	 	.addr = 0x147, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2freqgainb",	 	.addr = 0x148, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2freqgainc",	 	.addr = 0x149, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2freqgaind",	 	.addr = 0x14A, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2amplgain",	 	.addr = 0x14B, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2freqdev",	 	.addr = 0x14C, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "2fourfsk",	 	.addr = 0x14E, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "2bboffsres",	 	.addr = 0x14F, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Parameter set 3 */
		{.name = "3agcgain",	 	.addr = 0x150, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3agctarget",	 	.addr = 0x151, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3agcahyst",	 	.addr = 0x152, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3agcminmax",	 	.addr = 0x153, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3timegain",	 	.addr = 0x154, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3drgain",	 		.addr = 0x155, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3phasegain",	 	.addr = 0x156, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3freqgaina",	 	.addr = 0x157, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3freqgainb",	 	.addr = 0x158, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3freqgainc",	 	.addr = 0x159, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3freqgaind",	 	.addr = 0x15A, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3amplgain",	 	.addr = 0x15B, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3freqdev",	 	.addr = 0x15C, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "3fourfsk",	 	.addr = 0x15E, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "3bboffsres",	 	.addr = 0x15F, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Transmitter parameters */
		{.name = "modcfgf",	 		.addr = 0x160, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "fskdev2",	 		.addr = 0x161, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "fskdev1",	 		.addr = 0x162, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "fskdev0",	 		.addr = 0x163, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "modcfga",	 		.addr = 0x164, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "txrate2",	 		.addr = 0x165, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "txrate1",	 		.addr = 0x166, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "txrate0",	 		.addr = 0x167, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "txpwrcofa",		.addr = 0x168, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "txpwrcofb",		.addr = 0x16A, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "txpwrcofc",		.addr = 0x16C, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "txpwrcofd",		.addr = 0x16E, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "txpwrcofe",		.addr = 0x170, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},

		/* PLL parameters */
		{.name = "pllvcoi",	 		.addr = 0x180, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pllvcoir",		.addr = 0x181, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "plllockdet",		.addr = 0x182, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pllrngclk",		.addr = 0x183, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Crystal oscillator */
		{.name = "xtalcap",	 		.addr = 0x184, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Baseband */
		{.name = "bbtune",	 		.addr = 0x188, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "bboffscap", 		.addr = 0x189, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/**
		 *  MAC layer parameters
		 **/

		/* Packet format */
		{.name = "pktaddrcfg",	 	.addr = 0x200, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pktlencfg",	 	.addr = 0x201, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pktlenoffst",	 	.addr = 0x202, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pktmaxlen",	 	.addr = 0x203, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pktaddr",	 		.addr = 0x204, 					.type = PARAM_X32, 	.size = sizeof(uint32_t)},
		{.name = "pktaddrmsk",	 	.addr = 0x208, 					.type = PARAM_X32, 	.size = sizeof(uint32_t)},

		/* Pattern match */
		{.name = "match0pat",	 	.addr = 0x210, 					.type = PARAM_X32, 	.size = sizeof(uint32_t)},
		{.name = "match0len",	 	.addr = 0x214, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "match0min",	 	.addr = 0x215, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "match0max",	 	.addr = 0x216, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "match1pat",	 	.addr = 0x218, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "match1len",	 	.addr = 0x21C, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "match1min",	 	.addr = 0x21D, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "match1max",	 	.addr = 0x21E, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Packet controller */
		{.name = "tmgtxboost",	 	.addr = 0x220, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "tmgtxsettle",	 	.addr = 0x221, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "tmgrxboost",	 	.addr = 0x223, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "tmgrxsettle",	 	.addr = 0x224, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "tmgrxoffsacq",	.addr = 0x225, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "tmgrxcoaragc",	.addr = 0x226, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "tmgrxagc",	 	.addr = 0x227, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "tmgrxrssi",	 	.addr = 0x228, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "tmgrxpreamb1",	.addr = 0x229, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "tmgrxpreamb2",	.addr = 0x22A, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "tmgrxpreamb3",	.addr = 0x22B, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "rssireference",	.addr = 0x22C, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "rssiabsthr",	 	.addr = 0x22D, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "bgndrssigain",	.addr = 0x22E, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "bgndrssithr",	 	.addr = 0x22F, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pktchunksize",	.addr = 0x230, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pktmiscflags",	.addr = 0x231, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pktstoreflag",	.addr = 0x232, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},
		{.name = "pktaccptflag",	.addr = 0x233, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* DAC */
		{.name = "dacvalue",		.addr = 0x330, 					.type = PARAM_X16, 	.size = sizeof(uint16_t)},
		{.name = "dacconfig",		.addr = 0x332, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

		/* Performance tuning parameters */
		{.name = "f1c",				.addr = 0xF1C, 					.type = PARAM_X8, 	.size = sizeof(uint8_t)},

};

static const int ax5043_config_count = sizeof(ax5043_config) / sizeof(ax5043_config[0]);

void param_ax5043_write(uint16_t addr, size_t size, void * item, uint8_t flags);
void * param_ax5043_read(uint16_t addr, size_t size);

#endif /* AX100_PARAM_AX5043_H_ */
