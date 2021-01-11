// Copyright � Jacob Snyder, Billy O'Neal III
// This is under the 2 clause BSD license.
// See the included LICENSE.TXT file for more details.

#include "stdafx.h"

using namespace Microsoft::WRL;
namespace Instalog
{
	namespace SystemFacades
	{
		std::exception_ptr Win32Exception::FromLastError() noexcept
		{
			return FromWinError(::GetLastError());
		}

		std::exception_ptr Win32Exception::FromWinError(DWORD errorCode) noexcept
		{
			switch (errorCode)
			{
			case ERROR_SUCCESS:
				return std::make_exception_ptr(ErrorSuccessException());
			case ERROR_FILE_NOT_FOUND:
				return std::make_exception_ptr(ErrorFileNotFoundException());
			case ERROR_PATH_NOT_FOUND:
				return std::make_exception_ptr(ErrorPathNotFoundException());
			case ERROR_ACCESS_DENIED:
				return std::make_exception_ptr(ErrorAccessDeniedException());
			case ERROR_ALREADY_EXISTS:
				return std::make_exception_ptr(ErrorAlreadyExistsException());
			case ERROR_INVALID_PARAMETER:
				return std::make_exception_ptr(ErrorInvalidParameterException());
			case ERROR_MOD_NOT_FOUND:
				return std::make_exception_ptr(ErrorModuleNotFoundException());
			case ERROR_PROC_NOT_FOUND:
				return std::make_exception_ptr(ErrorProcedureNotFoundException());
			default:
				return std::make_exception_ptr(Win32Exception(errorCode));
			}
		}

		/*
		// Not using it anyways
		std::exception_ptr Win32Exception::FromNtError(NTSTATUS errorCode) noexcept
		{
			return FromWinError(GetWin32ErrorFromNtError(errorCode));
		}
		*/
		void __declspec(noreturn) Win32Exception::Throw(DWORD lastError)
		{
			std::rethrow_exception(FromWinError(lastError));
		}

		struct LocalFreeHelper
		{
			void operator()(void* toFree)
			{
				::LocalFree(reinterpret_cast<HLOCAL>(toFree));
			}
			;
		};

		std::string Win32Exception::GetCharMessage() const
		{
			return GetWin32ErrorMessage(this->GetErrorCode());
		}

		/*
		// Not using it anyways
		void __declspec(noreturn) Win32Exception::ThrowFromNtError(NTSTATUS errorCode)
		{
			std::rethrow_exception(FromNtError(errorCode));
		}
		*/
		void ThrowIfFailed(HRESULT hRes)
		{
			if (FAILED(hRes))
			{
				ThrowFromHResult(hRes);
			}
		}

		void __declspec(noreturn) ThrowFromHResult(HRESULT hRes)
		{
			ComPtr<IErrorInfo> iei;
			if (S_OK == ::GetErrorInfo(0, iei.GetAddressOf()) &&
				(iei.Get() != nullptr))
			{
				// get the error description from the IErrorInfo
				BSTR bStr;
				iei->GetDescription(&bStr);
				std::wstring errorMessage(bStr);
				std::string narrowMessage(errorMessage.begin(), errorMessage.end());
				throw HresultException(hRes, narrowMessage);
			}
			else if (HRESULT_FACILITY(hRes) == FACILITY_ITF)
			{
				throw HresultException(
					hRes, "Interface Specific");
			}
			else
			{
				Win32Exception::Throw(static_cast<DWORD>(hRes));
			}
		}

		HresultException::HresultException(HRESULT hRes, std::string n)
			: hResult(hRes)
			, narrow(std::move(n))
		{
		}

		HRESULT HresultException::GetErrorCode() const
		{
			return hResult;
		}

		std::string const& HresultException::GetErrorStringA() const
		{
			return narrow;
		}

		char const* HresultException::what()
		{
			return narrow.c_str();
		}

		std::string GetWin32ErrorMessage(DWORD errorCode)
		{
			std::unique_ptr<wchar_t[], LocalFreeHelper> buff;
			LPWSTR buffPtr;
			DWORD bufferLength = ::FormatMessageW(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				NULL,
				errorCode,
				0,
				reinterpret_cast<LPWSTR>(&buffPtr),
				0,
				NULL);
			buff.reset(buffPtr);
			return std::string(buff.get(), buff.get() + bufferLength);
		}
		/*
		// Not using it anyways
		std::uint32_t GetWin32ErrorFromNtError(NTSTATUS errorCode)
		{
			typedef ULONG(WINAPI * RtlNtStatusToDosErrorFunc)(__in NTSTATUS Status);
			RtlNtStatusToDosErrorFunc conv =
				GetNtDll().GetProcAddress<RtlNtStatusToDosErrorFunc>(
					GetThrowingErrorReporter(),
					"RtlNtStatusToDosError");
			return RtlNtStatusToDosError(errorCode);
		}*/

		std::string GetHresultErrorMessage(HRESULT errorCode)
		{
			ComPtr<IErrorInfo> iei;
			if (S_OK == ::GetErrorInfo(0, iei.GetAddressOf()) &&
				(iei.Get() != nullptr))
			{
				// get the error description from the IErrorInfo
				BSTR bStr;
				iei->GetDescription(&bStr);
				std::wstring errorMessage(bStr);
				std::string narrowMessage(errorMessage.begin(), errorMessage.end());
				return narrowMessage;
			}
			else if (HRESULT_FACILITY(errorCode) == FACILITY_ITF)
			{
				return "Interface Specific";
			}
			else
			{
				return GetWin32ErrorMessage(static_cast<DWORD>(errorCode));
			}
		}

	}
} // namespace Instalog::SystemFacades