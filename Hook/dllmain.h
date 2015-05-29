// dllmain.h : Declaration of module class.

class CHookModule : public ATL::CAtlDllModuleT< CHookModule >
{
public :
	DECLARE_LIBID(LIBID_HookLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_HOOK, "{39120C6B-70D9-4DFE-91FA-E44A6D97510C}")
};

extern class CHookModule _AtlModule;
