#ifndef _VM_EEPROM_CONFIG_H_
#define _VM_EEPROM_CONFIG_H_

#rom int 0xf00000={1,2,3,4}

void WriteEEPROMInt8( unsigned int16 uiAddress, unsigned int8 uiValue )
{
    write_eeprom(uiAddress,uiValue);
}

void WriteEEPROMInt16( unsigned int16 uiAddress, unsigned int16 uiValue )
{
    unsigned int8 uiTemp = 0;

    // Write first byte.
    uiTemp = uiValue>>8;
    write_eeprom(uiAddress,uiTemp);
    // Increment write address.
    uiAddress++;
    // Write second byte.
    uiTemp = 0;
    uiTemp = uiValue & 0x00FF;
    write_eeprom(uiAddress,uiTemp);
}

unsigned int8 ReadEEPROMInt8( unsigned int16 uiAddress )
{
    unsigned int8 uiValue = 0;

    uiValue = read_eeprom(uiAddress);

    return uiValue;
}

unsigned int16 ReadEEPROMInt16( unsigned int16 uiAddress )
{
    unsigned int16 uiValue = 0;
    unsigned int16 uiTemp  = 0;

    // Read first byte.
    uiTemp = read_eeprom(uiAddress);
    uiValue = uiTemp << 8;
    // Increment read address.
    uiAddress++;
    // Read second byte.
    uiTemp = read_eeprom(uiAddress);
    uiValue = uiValue | uiTemp;

    return uiValue;
}

#endif /* _VM_EEPROM_CONFIG_H_ */