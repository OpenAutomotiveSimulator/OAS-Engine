#include <atlbase.h>        // Common COM helpers.
#include "dxc/dxcapi.h"     // Be sure to link with dxcompiler.lib.
#include <d3d12shader.h>    // Shader reflection.

// Code from https://github.com/microsoft/DirectXShaderCompiler/wiki/Using-dxc.exe-and-dxcompiler.dll
// Included here to verify compilation success. If this needs to be altered, please update the wiki as well

int filter(unsigned int code, struct _EXCEPTION_POINTERS *pExceptionInfo) {
  static char scratch[32];
  // report all errors with fputs to prevent any allocation
  if (code == EXCEPTION_ACCESS_VIOLATION) {
    // use pExceptionInfo to document and report error
    fputs("access violation. Attempted to ", stderr);
    if (pExceptionInfo->ExceptionRecord->ExceptionInformation[0])
      fputs("write", stderr);
    else
      fputs("read", stderr);
    fputs(" from address ", stderr);
    sprintf_s(scratch, _countof(scratch), "0x%p\n", (void*)pExceptionInfo->ExceptionRecord->ExceptionInformation[1]);
    fputs(scratch, stderr);
    return EXCEPTION_EXECUTE_HANDLER;
  }
  if (code == EXCEPTION_STACK_OVERFLOW) {
    // use pExceptionInfo to document and report error
    fputs("stack overflow\n", stderr);
    return EXCEPTION_EXECUTE_HANDLER;
  }
  fputs("Unrecoverable Error ", stderr);
  sprintf_s(scratch, _countof(scratch), "0x%08x\n", code);
  fputs(scratch, stderr);
  return EXCEPTION_CONTINUE_SEARCH;
}


HRESULT Compile(IDxcCompiler3 *pCompiler, DxcBuffer *pSource, LPCWSTR pszArgs[],
             int argCt, IDxcIncludeHandler *pIncludeHandler, IDxcResult **pResults) {

  __try {
      return pCompiler->Compile(
               pSource,                // Source buffer.
               pszArgs,                // Array of pointers to arguments.
               argCt,                  // Number of arguments.
               pIncludeHandler,        // User-provided interface to handle #include directives (optional).
               IID_PPV_ARGS(pResults) // Compiler output status, buffer, and errors.
      );
  } __except(filter(GetExceptionCode(), GetExceptionInformation())) {
    // UNRECOVERABLE ERROR!
    // At this point, state could be extremely corrupt. Terminate the process
    return E_FAIL;
  }
}


int main()
{
    //
    // Create compiler and utils.
    //
    CComPtr<IDxcUtils> pUtils;
    CComPtr<IDxcCompiler3> pCompiler;
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&pUtils));
    DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&pCompiler));

    //
    // Create default include handler. (You can create your own...)
    //
    CComPtr<IDxcIncludeHandler> pIncludeHandler;
    pUtils->CreateDefaultIncludeHandler(&pIncludeHandler);


    //
    // COMMAND LINE: dxc myshader.hlsl -E main -T ps_6_0 -Zi -D MYDEFINE=1 -Fo myshader.bin -Fd myshader.pdb -Qstrip_reflect
    //
   LPCWSTR pszArgs[] =
    {
        L"myshader.hlsl",            // Optional shader source file name for error reporting and for PIX shader source view.
        L"-E", L"main",              // Entry point.
        L"-T", L"ps_6_0",            // Target.
        L"-Zi",                      // Enable debug information.
        L"-D", L"MYDEFINE=1",        // A single define.
        L"-Fo", L"myshader.bin",     // Optional. Stored in the pdb.
        L"-Fd", L"myshader.pdb",     // The file name of the pdb. This must either be supplied or the autogenerated file name must be used.
        L"-Qstrip_reflect",          // Strip reflection into a separate blob.
    };


    //
    // Open source file.
    //
    CComPtr<IDxcBlobEncoding> pSource = nullptr;
    pUtils->LoadFile(L"myshader.hlsl", nullptr, &pSource);
    DxcBuffer Source;
    Source.Ptr = pSource->GetBufferPointer();
    Source.Size = pSource->GetBufferSize();
    Source.Encoding = DXC_CP_ACP; // Assume BOM says UTF8 or UTF16 or this is ANSI text.

    //
    // Compile it with specified arguments.
    //
    CComPtr<IDxcResult> pResults;
    if (FAILED(Compile(pCompiler, &Source, pszArgs, _countof(pszArgs), pIncludeHandler, &pResults)))
    {

      // Either an unrecoverable error exception was caught or a failing HRESULT was returned
      // Use fputs to prevent any chance of new allocations
      // Terminate the process
      puts("Internal error or API misuse! Compile Failed\n");
      return 1;
    }

    //
    // Print errors if present.
    //
    CComPtr<IDxcBlobUtf8> pErrors = nullptr;
    // Note that d3dcompiler would return null if no errors or warnings are present.
    // IDxcCompiler3::Compile will always return an error buffer, but its length will be zero if there are no warnings or errors.
    if (SUCCEEDED(pResults->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&pErrors), nullptr)) &&
        pErrors != nullptr && pErrors->GetStringLength() != 0)
        wprintf(L"Warnings and Errors:\n%S\n", pErrors->GetStringPointer());

    //
    // Quit if the compilation failed.
    //
    HRESULT hrStatus;
    if (FAILED(pResults->GetStatus(&hrStatus)) || FAILED(hrStatus))
    {
        // Compilation failed, but successful HRESULT was returned.
        // Could reuse the compiler and allocator objects. For simplicity, exit here anyway
        wprintf(L"Compilation Failed\n");
        return 1;
    }

    //
    // Save shader binary.
    //
    CComPtr<IDxcBlob> pShader = nullptr;
    CComPtr<IDxcBlobWide> pShaderName = nullptr;
    if (SUCCEEDED(pResults->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&pShader), &pShaderName)) &&
        pShader != nullptr)
    {
        FILE* fp = NULL;

        _wfopen_s(&fp, pShaderName->GetStringPointer(), L"wb");
        fwrite(pShader->GetBufferPointer(), pShader->GetBufferSize(), 1, fp);
        fclose(fp);
    }

    //
    // Save pdb.
    //
    CComPtr<IDxcBlob> pPDB = nullptr;
    CComPtr<IDxcBlobWide> pPDBName = nullptr;
    if(SUCCEEDED(pResults->GetOutput(DXC_OUT_PDB, IID_PPV_ARGS(&pPDB), &pPDBName)))
    {
        FILE* fp = NULL;

        // Note that if you don't specify -Fd, a pdb name will be automatically generated. Use this file name to save the pdb so that PIX can find it quickly.
        _wfopen_s(&fp, pPDBName->GetStringPointer(), L"wb");
        fwrite(pPDB->GetBufferPointer(), pPDB->GetBufferSize(), 1, fp);
        fclose(fp);
    }

    //
    // Print hash.
    //
    CComPtr<IDxcBlob> pHash = nullptr;
    if (SUCCEEDED(pResults->GetOutput(DXC_OUT_SHADER_HASH, IID_PPV_ARGS(&pHash), nullptr)) &&
        pHash != nullptr)
    {
        wprintf(L"Hash: ");
        DxcShaderHash* pHashBuf = (DxcShaderHash*)pHash->GetBufferPointer();
        for (int i = 0; i < _countof(pHashBuf->HashDigest); i++)
            wprintf(L"%x", pHashBuf->HashDigest[i]);
        wprintf(L"\n");
    }


    //
    // Get separate reflection.
    //
    CComPtr<IDxcBlob> pReflectionData;
    if (SUCCEEDED(pResults->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(&pReflectionData), nullptr)) &&
        pReflectionData != nullptr)
    {
        // Optionally, save reflection blob for later here.

        // Create reflection interface.
        DxcBuffer ReflectionData;
        ReflectionData.Encoding = DXC_CP_ACP;
        ReflectionData.Ptr = pReflectionData->GetBufferPointer();
        ReflectionData.Size = pReflectionData->GetBufferSize();

        CComPtr< ID3D12ShaderReflection > pReflection;
        pUtils->CreateReflection(&ReflectionData, IID_PPV_ARGS(&pReflection));

        // Use reflection interface here.

    }
}