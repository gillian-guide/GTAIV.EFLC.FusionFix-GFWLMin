module;

#include <common.hxx>

export module frameratevigilante;

import common;
import comvars;

injector::hook_back<double(__fastcall*)(void* _this, void* edx, void* a2, void* a3)> hbsub_A18510;
double __fastcall sub_A18510(void* _this, void* edx, void* a2, void* a3)
{
    return hbsub_A18510.fun(_this, edx, a2, a3) * (*CTimer::fTimeStep / (1.0f / 30.0f));
}

class FramerateVigilante
{
public:
    FramerateVigilante()
    {
        FusionFix::onInitEventAsync() += []()
        {
            // Handbrake Cam (test)
            auto pattern = find_pattern("E8 ? ? ? ? D9 5C 24 7C F3 0F 10 4C 24", "E8 ? ? ? ? D9 5C 24 70 F3 0F 10 44 24 ? F3 0F 58 86");
            hbsub_A18510.fun = injector::MakeCALL(pattern.get_first(0), sub_A18510).get();

            pattern = hook::pattern("F3 0F 58 05 ? ? ? ? F3 0F 2A 0D");
            if (!pattern.empty())
            {
                struct LoadingTextSparks
                {
                    void operator()(injector::reg_pack& regs)
                    {
                        regs.xmm1.f32[0] += 0.085f * *CTimer::fTimeStep;
                    }
                }; injector::MakeInline<LoadingTextSparks>(pattern.get_first(0), pattern.get_first(8));
            }
            else
            {
                pattern = hook::pattern("F3 0F 58 0D ? ? ? ? 0F 5B C0 F3 0F 11 0D");
                struct LoadingTextSparks
                {
                    void operator()(injector::reg_pack& regs)
                    {
                        regs.xmm0.f32[0] += 0.085f * *CTimer::fTimeStep;
                    }
                }; injector::MakeInline<LoadingTextSparks>(pattern.get_first(0), pattern.get_first(8));
            }
        };
    }
} FramerateVigilante;
