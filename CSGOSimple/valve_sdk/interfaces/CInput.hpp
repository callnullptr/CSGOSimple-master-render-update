#pragma once

#include "../Misc/CUserCmd.hpp"

#define MULTIPLAYER_BACKUP 150

class bf_write;
class bf_read;

class CInput
{
public:

    CVerifiedUserCmd* GetVerifiedCmd(int sequence_number)
    {
        return &m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
    }

    CUserCmd* GetUserCmd(int sequence_number)
    {
        using OriginalFn = CUserCmd * (__thiscall*)(void*, int, int);
        return CallVFunction<OriginalFn>(this, 8)(this, 0, sequence_number);
    }
public:

    char            pad0[0xC];            // 0x0
    bool                m_trackir_available;        // 0xC
    bool                m_mouse_initialized;        // 0xD
    bool                m_mouse_active;            // 0xE
    char            pad1[0x9A];            // 0xF
    bool                m_fCameraInThirdPerson;    // 0xA9
    char            pad2[0x2];            // 0xAA
    Vector                m_vecCameraOffset;        // 0xAC
    std::byte            pad3[0x38];            // 0xB8
    CUserCmd* m_commands;            // 0xF0
    CVerifiedUserCmd* m_pVerifiedCommands;    // 0xF4
};
