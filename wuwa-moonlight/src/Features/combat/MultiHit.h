#pragma once
#include <includes.h>
#include "SDK.hpp"
#include <globals.h>
#include "TsAnimNotifyReSkillEvent_parameters.hpp"
#include <codecvt>
#include <locale>

class HitMultiplier
{
private:
    bool m_bBlockEnemyHit{true};

public:
    void HandleKeys();
    void Draw();
    void Call(UObject *Object, UFunction *Function, void *Parms);
};

inline void HitMultiplier::Draw()
{
    ImGui::Checkbox("Attack Modifier", &config::multihit::enabled);
    ImGui::SameLine();
    ImGui::WarningTooltip("is the character's attack modifier (don't set it over 20 otherwise there is a high risk of being banned)");

    ImGui::Checkbox("Block enemy hit", &m_bBlockEnemyHit);
    ImGui::SameLine();
    ImGui::Tooltip("I'm really don't know how it works (not tested)");

    ImGui::BeginDisabled(!config::multihit::enabled);
    {
        ImGui::Text("Hit multiplier");
        ImGui::SliderInt("##Hit multiplier", &config::multihit::hits, config::multihit::min_hits, config::multihit::max_hits);
    }
    ImGui::EndDisabled();
}

inline void HitMultiplier::Call(UObject *Object, UFunction *Function, void *Parms)
{
    // Check if the function is enabled
    if (!config::multihit::enabled)
    {
        return;
    }

    if (Function->Index != globals::FN_TsAnimNotifyReSkillEvent_C)
    {
        return;
    }

    // Get parameters and check their validity
    auto parameters = static_cast<SDK::Params::TsAnimNotifyReSkillEvent_C_K2_Notify *>(Parms);
    if (!parameters)
    {
        return;
    }

    USkeletalMeshComponent *meshComp = parameters->MeshComp;
    if (!meshComp)
    {
        return;
    }

    AActor *owner = meshComp->GetOwner();

    // Check if the action belongs to our character or an enemy
    if (owner == globals::pawn)
    {
        FVectorDouble actorLocation = owner->D_K2_GetActorLocation();
        LOG_INFO("Location: X=%.2f, Y=%.2f, Z=%.2f", actorLocation.X, actorLocation.Y, actorLocation.Z);
        
        auto kCharacter = reinterpret_cast<ATsBaseCharacter_C *>(owner);
        if (kCharacter) {
            LOG_INFO("kCharacter class: %s", kCharacter->Class->GetFullName().c_str());
            DebugBreak();

            //LOG_INFO("kCharacter DebugCreatureId: %d", kCharacter->TsCharacterDebugComponent->DebugCreatureId);

            //auto cData = kCharacter->CharacterData;

            

            /*while (property) {
                auto pname = property->Name.ToString();
                
                LOG_INFO("CharacterData Property: %s", pname.c_str());

                property = property->Next;
            }*/
        }
        
        for (int i = 1; i < config::multihit::hits; ++i)
        {
            globals::oProcessEvent(Object, Function, Parms);
        }

        LOG_INFO("MultiHit: executed %d additional hits", config::multihit::hits - 1);
        return; // Return as hits have already been executed
    }
    else if (m_bBlockEnemyHit && owner != globals::pawn)
    {
        // If enemy attack blocking is enabled and the attacker is not the player
        LOG_INFO("Enemy attack blocked");
        return; // Don't call the original function, blocking the attack
    }

    // Do not call globals::oProcessEvent at the end of the function,
    // cuz this is will be in hooked func (hkProcessEvent)
}

//std::string ConvertToUTF8(const std::string& str, UINT fromCodePage = CP_ACP) {
//    // Step 1: ±¾µØ±àÂë/UTF-16 ¡ú std::wstring
//    int wlen = MultiByteToWideChar(fromCodePage, 0, str.c_str(), -1, nullptr, 0);
//    std::wstring wstr(wlen, 0);
//    MultiByteToWideChar(fromCodePage, 0, str.c_str(), -1, &wstr[0], wlen);
//
//    // Step 2: std::wstring ¡ú UTF-8
//    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
//    return converter.to_bytes(wstr);
//}