#pragma once
#include "../Singleton.hpp"
#include "../imgui/imgui.h"
class Glow
    : public Singleton<Glow>
{
    friend class Singleton<Glow>;

    Glow();
    ~Glow();

public:
    void Run();
    void Shutdown();
};