#pragma once

#include "../singleton.hpp"
#include "../imgui/imgui.h"

class IMatRenderContext;
struct DrawModelState_t;
struct ModelRenderInfo_t;
class matrix3x4_t;
class IMaterial;
class ImColor;

class Chams
    : public Singleton<Chams>
{
    friend class Singleton<Chams>;

    Chams();
    ~Chams();

public:
	void OnDrawModelExecute(
        IMatRenderContext* ctx,
        const DrawModelState_t &state,
        const ModelRenderInfo_t &pInfo,
        matrix3x4_t *pCustomBoneToWorld);

private:
    void OverrideMaterial(bool ignoreZ, bool flat, bool wireframe, bool glass, const ImColor& rgba);

    IMaterial* materialRegular = nullptr;
    IMaterial* materialFlat = nullptr;
};