

#define AVE_VI_COMB 0x03
#define AVE_VI_GAMMA 0x10

#define AVE_AI_VOLUME 0x71


void VIWriteI2CRegister8(u8 reg, u8 data);
void VIWriteI2CRegister16(u8 reg, u16 data);
void VIWriteI2CRegister32(u8 reg, u32 data);
void VIWriteI2CRegisterBuf(u8 reg, int size, u8 *data);
