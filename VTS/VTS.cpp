#pragma once

#include <cppfmu_cs.hpp>
#include <cmath>

#define FMU_UUID "22467b28-0bfa-11ec-9a03-0242ac130003"

class MNav : public cppfmu::SlaveInstance
{
public:
    MNav()
    {
        MNav::Reset();
    }

    void Reset() override
    {
        output = "";
        input = "";
    }

    void SetString(
        const cppfmu::FMIValueReference vr[],
        std::size_t nvr,
        const cppfmu::FMIString value[]) override
    {
        input = value[0];
    }

    void GetString(
        const cppfmu::FMIValueReference vr[],
        std::size_t nvr,
        cppfmu::FMIString value[]) const override
    {
        value[1] = output;
    }

    bool DoStep(
        cppfmu::FMIReal currentCommunicationPoint,
        cppfmu::FMIReal dt,
        cppfmu::FMIBoolean /*newStep*/,
        cppfmu::FMIReal& /*endOfStep*/) override
    {
        return true;
    }

private:
    cppfmu::FMIString output;
    cppfmu::FMIString input;
};

cppfmu::UniquePtr<cppfmu::SlaveInstance> CppfmuInstantiateSlave(
    cppfmu::FMIString instanceName,
    cppfmu::FMIString fmuGUID,
    cppfmu::FMIString fmuResourceLocation,
    cppfmu::FMIString mimeType,
    cppfmu::FMIReal timeout,
    cppfmu::FMIBoolean visible,
    cppfmu::FMIBoolean interactive,
    cppfmu::Memory memory,
    cppfmu::Logger logger)
{
    if (std::strcmp(fmuGUID, FMU_UUID) != 0) {
        throw std::runtime_error("FMU GUID mismatch");
    }
    return cppfmu::AllocateUnique<MNav>(memory);
}