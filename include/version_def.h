#ifndef VERSION_DEF_H
#define VERSION_DEF_H

#define _stringfmt_1(x)         #x

#define _stringfmt_2(y)         _stringfmt_1(y)

#define stringcat_4(a,b,c,d)    a##b##c##d

#define versioncat_4(a,b,c,d)   _stringfmt_2(stringcat_4(a,b,c,d))

#define FIRMWARE_ITEM_TYPE      24

#define FIRMWARE_ITEM_ALGO      07

#define FIRMWARE_ITEM_MAJOR     01

#define FIRMWARE_ITEM_MINOR     01

#define FIRMWARE_VERSION        versioncat_4(FIRMWARE_ITEM_TYPE,FIRMWARE_ITEM_ALGO,FIRMWARE_ITEM_MAJOR,FIRMWARE_ITEM_MINOR)

#endif