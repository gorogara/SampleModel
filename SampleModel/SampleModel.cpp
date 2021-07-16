#pragma once

#include <cppfmu_cs.hpp>
#include <cmath>

#define FMU_UUID "3147a610-e5da-11eb-ba80-0242ac130004"

class SampleModel : public cppfmu::SlaveInstance
{
public:
    SampleModel()
    {
        SampleModel::Reset();
    }

    void Reset() override
    {
        inputReal = 0;
        outputReal = 0;
    }

    void SetReal(
        const cppfmu::FMIValueReference vr[],
        std::size_t nvr,
        const cppfmu::FMIReal value[]) override
    {
        inputReal = value[0];
    }

    void GetReal(
        const cppfmu::FMIValueReference vr[],
        std::size_t nvr,
        cppfmu::FMIReal value[]) const override
    {
        value[1] = outputReal;
    }

    bool DoStep(
        cppfmu::FMIReal currentCommunicationPoint,
        cppfmu::FMIReal dt,
        cppfmu::FMIBoolean /*newStep*/,
        cppfmu::FMIReal& /*endOfStep*/) override
    {
        outputReal = sin(currentCommunicationPoint);
        return true;
    }

private:
    cppfmu::FMIReal inputReal;
    cppfmu::FMIReal outputReal;
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
    return cppfmu::AllocateUnique<SampleModel>(memory);
}