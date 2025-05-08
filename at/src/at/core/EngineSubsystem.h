#pragma once

#include "atpch.h"

namespace at
{
    template <class T>
    class EngineSubsystem {
    public:
        EngineSubsystem& operator = (const EngineSubsystem&) = delete;
        EngineSubsystem& operator = (EngineSubsystem&&) = delete;

        static T& Get() {
            if (!instance)
                instance = new T_Instance;
            return *instance;
        }
        virtual void Init() {};
        virtual void Shutdown() {};

    protected:
        EngineSubsystem() {}
        static inline T* instance = nullptr;

    private:
        struct T_Instance : public T {
            T_Instance() : T() {}
        };

    };
}