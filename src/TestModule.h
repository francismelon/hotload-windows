#pragma once
#include <array>
#include "ImportModule.h"

// define function name array, the array size is function names num
static std::array<const char *, 2> TestModuleSymbolExportNameArr =
{
	"add",
	"sub"
};

enum TestFuncNameType
{
	FUNCTION_ADD,
	FUNCTION_SUB
};

static std::string mModulePath = "";

class TestModule : public ImportModule::ImportModule<TestModule, TestModuleSymbolExportNameArr.size()>
{
public:
	int Add(int a, int b)
	{
		return GetInstance().Run<int>(TestModuleSymbolExportNameArr[FUNCTION_ADD], a, b);
	}

	int Sub(int a, int b)
	{
		return GetInstance().Run<int>(TestModuleSymbolExportNameArr[FUNCTION_SUB], a, b);
	}

	// the path must use '\\' symbol
	void setModulePath(const std::string &path) { mModulePath = path; }

protected:
	virtual const char *GetModulePath() const override
	{
		return mModulePath.c_str();
	}

	virtual std::array<const char *, TestModuleSymbolExportNameArr.size()> &GetSymbolNames() const override
	{
		return TestModuleSymbolExportNameArr;
	}
};