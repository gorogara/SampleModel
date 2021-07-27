#pragma once

#include <cppfmu_cs.hpp>
#include <cmath>
#include <unordered_map>

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
		for (int i = 0; i < 2; i++)
		{
			inputRealMap.insert({ i, 0.0 });
		}

		for (int i = 2; i < 4; i++)
		{
			inputIntMap.insert({ i, 0 });
		}

		for (int i = 4; i < 6; i++)
		{
			outputRealMap.insert({ i, 0 });
		}

		for (int i = 6; i < 8; i++)
		{
			outputIntMap.insert({ i, 0 });
		}
	}

	// Input 받는 곳
	void SetReal(
		const cppfmu::FMIValueReference vr[],
		std::size_t nvr,
		const cppfmu::FMIReal value[]) override
	{
		for (int i = 0; i < nvr; i++)
		{
			inputRealMap[vr[i]] = value[i];
		}
	}

	void SetInteger(
		const cppfmu::FMIValueReference vr[],
		std::size_t nvr,
		const cppfmu::FMIInteger value[]) override
	{
		for (int i = 0; i < 0 + nvr; i++)
		{
			inputIntMap[vr[i]] = value[i];
		}
	}

	// Output 내보내는 곳
	void GetReal(
		const cppfmu::FMIValueReference vr[],
		std::size_t nvr,
		cppfmu::FMIReal value[]) const override
	{
		for (int i = 0; i < nvr; i++)
		{
			auto item = outputRealMap.find(vr[i]);

			if (item != outputRealMap.end())
			{
				value[i] = item->second;
			}
		}
	}

	void GetInteger(
		const cppfmu::FMIValueReference vr[],
		std::size_t nvr,
		cppfmu::FMIInteger value[]) const override
	{
		for (int i = 0; i < nvr; i++)
		{
			auto item = outputIntMap.find(vr[i]);

			if (item != outputIntMap.end())
			{
				value[i] = item->second;
			}
		}
	}

	bool DoStep(
		cppfmu::FMIReal currentCommunicationPoint,
		cppfmu::FMIReal dt,
		cppfmu::FMIBoolean /*newStep*/,
		cppfmu::FMIReal& /*endOfStep*/) override
	{
		auto value = sin(currentCommunicationPoint);

		for (auto i = outputIntMap.begin(); i != outputIntMap.end(); i++)
		{
			i->second = value * 10;
		}

		for (auto i = outputRealMap.begin(); i != outputRealMap.end(); i++)
		{
			i->second = value;
		}

		return true;
	}

private:
	std::unordered_map<int, cppfmu::FMIReal> inputRealMap;
	std::unordered_map<int, cppfmu::FMIInteger> inputIntMap;
	std::unordered_map<int, cppfmu::FMIReal> outputRealMap;
	std::unordered_map<int, cppfmu::FMIInteger> outputIntMap;
	//cppfmu::FMIReal inputReal;
	//cppfmu::FMIReal outputReal;
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