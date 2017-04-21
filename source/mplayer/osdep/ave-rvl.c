/*
        BootMii - a Free Software replacement for the Nintendo/BroadOn bootloader.
        low-level video support for the BootMii UI

Copyright (C) 2008, 2009        Hector Martin "marcan" <marcan@marcansoft.com>
Copyright (C) 2009                      Haxx Enterprises <bushing@gmail.com>
Copyright (c) 2009              Sven Peter <svenpeter@gmail.com>

# This code is licensed to you under the terms of the GNU GPL, version 2;
# see file COPYING or http://www.gnu.org/licenses/old-licenses/gpl-2.0.txt

Some routines and initialization constants originally came from the
"GAMECUBE LOW LEVEL INFO" document and sourcecode released by Titanik
of Crazy Nation and the GC Linux project.
*/

#include <gccore.h>

#define HW_REG_BASE 0xd800000
#define HW_GPIO1BOUT (HW_REG_BASE + 0x0c0)
#define HW_GPIO1BDIR (HW_REG_BASE + 0x0c4)
#define HW_GPIO1BIN (HW_REG_BASE + 0x0c8)

#define SLAVE_AVE 0xe0

static inline u32 read32(u32 addr)
{
        u32 x;

        asm volatile("lwz %0,0(%1) ; sync" : "=r"(x) : "b"(0xc0000000 | addr));

        return x;
}

static inline void write32(u32 addr, u32 x)
{
        asm("stw %0,0(%1) ; eieio" : : "r"(x), "b"(0xc0000000 | addr));
}
 
static inline void aveSetDirection(u32 dir)
{
        u32 val = (read32(HW_GPIO1BDIR)&~0x8000)|0x4000;
        if(dir) val |= 0x8000;
        write32(HW_GPIO1BDIR, val);
}
static inline void aveSetSCL(u32 scl)
{
        u32 val = read32(HW_GPIO1BOUT)&~0x4000;
        if(scl) val |= 0x4000;
        write32(HW_GPIO1BOUT, val);
}
 
static inline void aveSetSDA(u32 sda)
{
        u32 val = read32(HW_GPIO1BOUT)&~0x8000;
        if(sda) val |= 0x8000;
        write32(HW_GPIO1BOUT, val);
}
 
static inline u32 aveGetSDA()
{
        if(read32(HW_GPIO1BIN)&0x8000)
                return 1;
        else
                return 0;
}
 
static u32 __sendSlaveAddress(u8 addr)
{
        u32 i;
 
        aveSetSDA(0);
        udelay(2);
 
        aveSetSCL(0);
        for(i=0;i<8;i++) {
                if(addr&0x80) aveSetSDA(1);
                else aveSetSDA(0);
                udelay(2);
 
                aveSetSCL(1);
                udelay(2);
 
                aveSetSCL(0);
                addr <<= 1;
        }
 
        aveSetDirection(0);
        udelay(2);
 
        aveSetSCL(1);
        udelay(2);
 
        if(aveGetSDA()!=0) {
                return 0;
        }
 
        aveSetSDA(0);
        aveSetDirection(1);
        aveSetSCL(0);
 
        return 1;
}
 
static u32 __VISendI2CData(u8 addr,void *val,u32 len)
{
        u8 c;
        u32 i,j;
        u32 ret;
 
        aveSetDirection(1);
        aveSetSCL(1);
        aveSetSDA(1);
        udelay(4);
 
        ret = __sendSlaveAddress(addr);
        if(ret==0) {
                return 0;
        }
 
        aveSetDirection(1);
        for(i=0;i<len;i++) {
                c = ((u8*)val)[i];
                for(j=0;j<8;j++) {
                        if(c&0x80) aveSetSDA(1);
                        else aveSetSDA(0);
                        udelay(2);
 
                        aveSetSCL(1);
                        udelay(2);
                        aveSetSCL(0);
 
                        c <<= 1;
                }
                aveSetDirection(0);
                udelay(2);
                aveSetSCL(1);
                udelay(2);
 
                if(aveGetSDA()!=0) {
                        return 0;
                }
 
                aveSetSDA(0);
                aveSetDirection(1);
                aveSetSCL(0);
        }
 
        aveSetDirection(1);
        aveSetSDA(0);
        udelay(2);
        aveSetSDA(1);
 
        return 1;
}
 
void VIWriteI2CRegister8(u8 reg, u8 data)
{
        u8 buf[2];
        buf[0] = reg;
        buf[1] = data;
        __VISendI2CData(SLAVE_AVE,buf,2);
        udelay(2);
}
 
void VIWriteI2CRegister16(u8 reg, u16 data)
{
        u8 buf[3];
        buf[0] = reg;
        buf[1] = data >> 8;
        buf[2] = data & 0xFF;
        __VISendI2CData(SLAVE_AVE,buf,3);
        udelay(2);
}
 
void VIWriteI2CRegister32(u8 reg, u32 data)
{
        u8 buf[5];
        buf[0] = reg;
        buf[1] = data >> 24;
        buf[2] = (data >> 16) & 0xFF;
        buf[3] = (data >> 8) & 0xFF;
        buf[4] = data & 0xFF;
        __VISendI2CData(SLAVE_AVE,buf,5);
        udelay(2);
}

void VIWriteI2CRegisterBuf(u8 reg, int size, u8 *data)
{
        u8 buf[0x100];
        buf[0] = reg;
        memcpy(&buf[1], data, size);
        __VISendI2CData(SLAVE_AVE,buf,size+1);
        udelay(2);
}
