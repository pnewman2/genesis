#include <stdint.h>

struct SystemIoRegs {
    uint8_t __space0;
    uint8_t VersionNumber;
    uint8_t __space2;
    uint8_t Data1;
    uint8_t __space4;
    uint8_t Data2;
    uint8_t __space6;
    uint8_t Data3;
    uint8_t __space8;
    uint8_t Ctrl1;
    uint8_t __space10;
    uint8_t Ctrl2;
    uint8_t __space12;
    uint8_t Ctrl3;
    uint8_t __space14;
};

struct VDP_Port {
    uint16_t DataPort;
    uint16_t AlsoDataPort;
    uint16_t ControlPort;
    uint16_t AlsoControlPort;
    uint16_t HvCounter;
    uint8_t  __prohibited_10_a[0x10 - 0xa];
    uint8_t  PSG;
};

union VDP_Reg_BackgroundColor {
    struct {
        uint8_t _zero:2;
        uint8_t ColorPalette:2;
        uint8_t ColorCode:4;
    };
    uint8_t V;
};

union VDP_Reg_ModeSet2 {
    struct {
        uint8_t _zero:1;
        uint8_t DISP:1;
        uint8_t IEO:1;
        uint8_t M:2;
        uint8_t One:1;
        uint8_t _zeroes:2;
    };
    uint8_t V;
};

union VDP_ControlRegWrite {
    struct {
        uint8_t One:1;
        uint8_t _zero:2;
        uint8_t RegisterNo:5;
        uint8_t Data;
    };
    uint16_t V;
};

union VDP_DataWrite1 {
    struct {
        uint16_t CD:2;
        uint16_t A:14;
    };
    uint16_t V;
};

union VDP_DataWrite2 {
    struct {
        uint8_t _zero_15_8;
        uint8_t CD:4;
        uint8_t _zero_4_2:2;
        uint8_t A:2;
    };
    uint16_t V;
};

enum VDP_RegIndex {
    RegIndex_ModeSet1 = 0,
    RegIndex_ModeSet2 = 1,
    RegIndex_BackgroundColor = 7
};

union VDP_Color {
    struct {
        uint16_t __zero_15_12:4;
        uint16_t B:3;
        uint16_t __zero_8:1;
        uint16_t G:3;
        uint16_t __zero_4:1;
        uint16_t R:3;
        uint16_t __zero_0:1;
    };
    uint16_t V;
};

static void VDP_CramWrite(volatile struct VDP_Port *VDP, uint16_t address,
        union VDP_Color color) {
    // Two control word writes to initiate the CRAM write cycle.
    VDP->ControlPort = (union VDP_DataWrite1) {
        .CD = 3,
        .A = address
    }.V;
    VDP->ControlPort = (union VDP_DataWrite2) {
        .CD = 0,
        .A = address >> 14
    }.V;
    VDP->DataPort = color.V;
}

static void VDP_RegWrite(volatile struct VDP_Port *VDP, uint8_t regIndex,
        uint8_t value) {
    VDP->ControlPort = (union VDP_ControlRegWrite) {
        .Data = value,
        .RegisterNo = regIndex,
        .One = 1
    }.V;
}

void start() {
    volatile struct SystemIoRegs *sys =
            (volatile struct SystemIoRegs *)0xa10000;

    // The manual says to do this for some reason.
    if ((sys->VersionNumber & 0xf) != 0) {
        const char sega[] = {'S', 'E', 'G', 'A'};
        *(volatile uint32_t *)0xa14000 = (uintptr_t)sega;
    }

    volatile struct VDP_Port *VDP = (struct VDP_Port *)0xc00000;

    VDP_RegWrite(VDP, RegIndex_BackgroundColor,
        (union VDP_Reg_BackgroundColor) {
            .ColorCode = 0,
            .ColorPalette = 0
        }.V);

    VDP_RegWrite(VDP, RegIndex_ModeSet2,
            (union VDP_Reg_ModeSet2) {
                .DISP = 1,
                .One = 1
            }.V);

    VDP_CramWrite(VDP, 0x00, (union VDP_Color) { .B = 7 });

    while (1);
}