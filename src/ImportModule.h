#pragma once

#include <array>
#include <unordered_map>
#include <Windows.h>
#include <string>
#include <stdio.h>

namespace ImportModule
{
	template <typename Module, size_t SymbolsNum>
	class ImportModule
	{
	public:
		static void LoadModule() { GetInstance().Load() };
		static void ReloadModule() { GetInstance().Reload() };
		static void UnloadModule() { GetInstance().Unload() };
		ImportModule() { mSymbolsMap = new std::unordered_map<std::string, FARPROC>(); }
		virtual ~ImportModule() { delete mSymbolsMap; }

	protected:
		static Module &GetInstance()
		{
			static Module instance;
			return instance;
		}

		virtual const char *GetModulePath() const = 0;

		virtual std::array<const char *, SymbolsNum> &GetSymbolNames() const = 0;

		template <typename Result, typename... Args>
		Result Run(const char *name, Args... args)
		{
			auto symbol = mSymbolsMap->find(name);
			if (symbol != mSymbolsMap->end())
			{
				return reinterpret_cast<Result(*)(Args...)>(symbol->second)(args...);
			}
			printf("Function not found: [%s]", name);
		}

		template <typename T>
		T *GetVar(const char *name)
		{
			auto symbol = mSymbolsMap->find(name);
			if (symbol != mSymbolsMap->end())
			{
				return static_cast<T*>(symbol->second);
			}
			printf("Variable not found: [%s]", name);
			return nullptr;
		}

	private:
		void Load()
		{
			std::string			libPath(GetModulePath());
			std::wstring		wlibPath(libPath.begin(), libPath.end());
			mLibHandle = LoadLibrary(wlibPath.data());
			if (!mLibHandle)
			{
				printf("Load lib error code: [%lu], lib path: [%s]\n", GetLastError(), libPath.c_str());
			}
			LoadSymbols();
		}

		bool Unload()
		{
			BOOL ret = FreeLibrary(mLibHandle);
			if (ret == 0)
			{
				printf("Release lib failed: [%s]\n", GetModulePath());
				return false
			}

			return true;
		}

		void Reload()
		{
			if (!Unload())
			{
				return;
			}
			if (mSymbolsMap)
			mSymbolsMap->clear();
			Load();
		}

		void LoadSymbols()
		{
			for (const char *symbol : GetSymbolNames())
			{
				FARPROC symAddress = GetProcAddress(libHandle, symbol);
				(*mSymbolsMap)[symbol] = symAddress;
			}
		}

	private:
		HMODULE										mLibHandle;
		std::unordered_map<std::string, FARPROC>	*mSymbolsMap = nullptr;
	};
}