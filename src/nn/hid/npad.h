#pragma once
#include <cstddef>
#include <cstdint>
#include "nn/util/bit_flag_set.h"

namespace nn::hid
{
    using NpadIdType = uint32_t;
    namespace NpadId {
        constexpr NpadIdType No1 = 0, No2 = 1, No3 = 2, No4 = 3, No5 = 4, No6 = 5, No7 = 6, No8 = 7;
        constexpr NpadIdType Handheld = 0x20;
    }

    struct NpadStyleTag {};
    using NpadStyleSet = nn::util::BitFlagSet<32, NpadStyleTag>;
    namespace NpadStyle {
        constexpr int FullKey = 0, Handheld = 1, JoyDual = 2, JoyLeft = 3, JoyRight = 4;
    }

    struct NpadButtonTag {};
    using NpadButtonSet = nn::util::BitFlagSet<64, NpadButtonTag>;
    namespace NpadButton {
        constexpr int A = 0, B = 1, X = 2, Y = 3, StickL = 4, StickR = 5, L = 6, R = 7, ZL = 8, ZR = 9;
        constexpr int Plus = 10, Minus = 11, Left = 12, Up = 13, Right = 14, Down = 15;
        constexpr int StickLLeft = 16, StickLUp = 17, StickLRight = 18, StickLDown = 19;
        constexpr int StickRLeft = 20, StickRUp = 21, StickRRight = 22, StickRDown = 23;
    }

    struct NpadAttributeTag {};
    using NpadAttributesSet = nn::util::BitFlagSet<32, NpadAttributeTag>;
    namespace NpadAttribute {
        constexpr int IsConnected = 0;
    }

    constexpr int32_t AnalogStickMax = 0x7FFF;
    struct AnalogStickState {
        int32_t x = 0;   // -AnalogStickMax (left) .. +AnalogStickMax (right)
        int32_t y = 0;   // -AnalogStickMax (down) .. +AnalogStickMax (up)
    };

    struct NpadCommonState {
        int64_t samplingNumber = 0;
        NpadButtonSet buttons;
        AnalogStickState analogStickL;
        AnalogStickState analogStickR;
        NpadAttributesSet attributes;
    };
    struct NpadFullKeyState  : NpadCommonState {};   // Pro Controller
    struct NpadHandheldState : NpadCommonState {};   // console in handheld mode
    struct NpadJoyDualState  : NpadCommonState {};   // two Joy-Con

    void InitializeNpad();
    void SetSupportedNpadStyleSet(NpadStyleSet style);
    void SetSupportedNpadIdType(const NpadIdType* ids, size_t count);
    NpadStyleSet GetNpadStyleSet(const NpadIdType& id);
    void GetNpadState(NpadFullKeyState* outState, const NpadIdType& id);
    void GetNpadState(NpadHandheldState* outState, const NpadIdType& id);
    void GetNpadState(NpadJoyDualState* outState, const NpadIdType& id);
}
