/**
* \file hl1_studiomodel_buffer.h
*/
#ifndef HL1_STUDIOMODEL_DEFINES_H__
#define HL1_STUDIOMODEL_DEFINES_H__

// motion flags
#define STUDIO_X        0x0001
#define STUDIO_Y        0x0002  
#define STUDIO_Z        0x0004
#define STUDIO_XR       0x0008
#define STUDIO_YR       0x0010
#define STUDIO_ZR       0x0020
#define STUDIO_LX       0x0040
#define STUDIO_LY       0x0080
#define STUDIO_LZ       0x0100
#define STUDIO_AX       0x0200
#define STUDIO_AY       0x0400
#define STUDIO_AZ       0x0800
#define STUDIO_AXR      0x1000
#define STUDIO_AYR      0x2000
#define STUDIO_AZR      0x4000
#define STUDIO_TYPES    0x7FFF
#define STUDIO_RLOOP    0x8000

// sequence flags
#define STUDIO_LOOPING  0x0001

#define SCRIPT_EVENT_DEAD           1000
#define SCRIPT_EVENT_NOINTERRUPT    1001
#define SCRIPT_EVENT_CANINTERRUPT   1002
#define SCRIPT_EVENT_FIREEVENT      1003
#define SCRIPT_EVENT_SOUND          1004
#define SCRIPT_EVENT_SENTENCE       1005
#define SCRIPT_EVENT_INAIR          1006
#define SCRIPT_EVENT_ENDANIMATION   1007
#define SCRIPT_EVENT_SOUND_VOICE    1008
#define SCRIPT_EVENT_SENTENCE_RND1  1009
#define SCRIPT_EVENT_NOT_DEAD       1010

#endif // HL1_STUDIOMODEL_DEFINES_H__
