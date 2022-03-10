#ifndef INCLUDED_DX3D_11
#define INCLUDED_DX3D_11

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl/client.h>

#include "WICTextureLoader.h"

using namespace DirectX;

#define PI 3.141592


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "ps.h"
#include "ntxps.h"
#include "vs.h"


extern ID3D11ShaderResourceView *nullview;

class DX113D {
public:
	ID3D11Device*           pDevice;
	ID3D11DeviceContext*    pImmediateContext;
	IDXGISwapChain*         pSwapChain;
	ID3D11RenderTargetView* pRenderTargetView;

	ID3D11PixelShader* nottexps;

public:
	DX113D();
	virtual ~DX113D();
	virtual HRESULT Create(HWND hwnd, int Width, int Height, int windowf);
	virtual void Release();
	virtual void RenderSet(/*int R, int G, int B, int A*/);

	void RenderTargetSet(int numviews, ID3D11RenderTargetView* RTView, ID3D11DepthStencilView *DSview, float * col, UINT clearflag, float depth);

};

class DX11DSBuffer{
public:
	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
	D3D11_TEXTURE2D_DESC txDesc;
	ID3D11Texture2D *pDepthStencilTexture;
	ID3D11DepthStencilView *pDepthStencilView;

	HRESULT Create(ID3D11Device *pDevice, int Width, int Height);
	void ReleseBuffer();

};

class DX11Shader {
public:
	ID3DBlob* pBlob;

	void Shadercompile(LPCSTR entrypoint, LPCSTR target, TCHAR *file);
};

class DX11PShader :public DX11Shader {
public:
	ID3D11PixelShader* Shader;

	bool CreateShaderP(ID3D11Device &pDevice, const void *pointer, size_t size);
	void ReleaseShader();
};

class DX11VShader :public DX11Shader {
public:
	ID3D11VertexShader* Shader;

	bool CreateShaderV(ID3D11Device &pDevice, const void *pointer, size_t size);
	void ReleaseShader();
};

class Pos {
public:
	float x, y, z;
};

class TexPos {
public:
	float tu, tv;
};

class Vertex : public Pos , public TexPos{
public:
	float color[4];
	
};

class vertclass {
public:
	Vertex *vert;
	WORD *ind;
	int vertc;
	int vertcmax;
	int indc;
	int indcmax;
};


class ConstantBuffer {
public:
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
	XMVECTOR Diffuse;
};

class DX11Buffeer {
public:
	D3D11_BUFFER_DESC bufferDesc;
	D3D11_SUBRESOURCE_DATA subResourceData;
	ID3D11Buffer *Buffer;

	D3D11_MAPPED_SUBRESOURCE msr;
	

	HRESULT CreateBuffer(ID3D11Device *pDevice, UINT ByteWidth, D3D11_USAGE Usage, UINT BindFlags,
							UINT CPUAccessFlags, UINT MiscFlags, UINT StructureByteStride/*,
							const void* pSysMem, UINT SysMemPitch, UINT SysMemSlicePitch*/);
	void Dataset(ID3D11DeviceContext* pImmediateContext, const void* pSysMem, size_t size);
	void ReleaseBuffer();

};

class DX11InputLayout {
public:
	D3D11_INPUT_ELEMENT_DESC *vertDesc;
	int arraysize;

	ID3D11InputLayout *Layout;

	DX11InputLayout();
	void DescSet(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, UINT InputSlot, UINT AlignedByteOffset, D3D11_INPUT_CLASSIFICATION InputSlotClass, UINT InstanceDataStepRate);
	void DescDelete();

	HRESULT CreateIL(ID3D11Device *pDevice, const void *pointer, size_t size);
};

class DX11Tex {
public:
	
	ID3D11Resource *tex;
	ID3D11ShaderResourceView *view;

	
	HRESULT TexLoadFromFile(ID3D11Device *pDevice, const wchar_t *path);
	void SetTex(ID3D11DeviceContext* pImmediateContext);

};

class DX11RendTex :public DX11Tex {
public:
	ID3D11Texture2D *rtex;
	ID3D11RenderTargetView *rtview;
	ID3D11Texture2D *dstex;
	ID3D11DepthStencilView *dsview;

	HRESULT CreateTex(ID3D11Device *pDevice, D3D11_USAGE Usage, DXGI_FORMAT Format, UINT BindFlags, UINT Width, UINT Height,
		UINT CPUAccessFlags, UINT MipLevels, UINT ArraySize, UINT Count, UINT Quality, DXGI_FORMAT RtFormat);
};

class DX11SAMPLER {
public:
	ID3D11SamplerState *pSampler;

	HRESULT SamplerCreate(ID3D11Device *pDevice, D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU,
		D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW, D3D11_COMPARISON_FUNC ComparisonFunc,
		float MinLOD, float MaxLOD);

	void SetSampler(ID3D11DeviceContext* pImmediateContext);

	void ReleaseSampler() {
		pSampler->Release();
	}
};

class DX11RSState {
public:

	ID3D11RasterizerState *pRsState;

	void CreateState(ID3D11Device *pDevice, CD3D11_RASTERIZER_DESC rsdesc);
	void SetState(ID3D11DeviceContext* pImmediateContext);

	void ReleaseState() {
		pRsState->Release();
	}

};

class DX11DSState {
public:
	ID3D11DepthStencilState *pDsState;

	void CreateState(ID3D11Device *pDevice, CD3D11_DEPTH_STENCIL_DESC dsdesc);
	void SetState(ID3D11DeviceContext* pImmediateContext);

	void ReleaseState() {
		pDsState->Release();
	}
};

class DX11BDState {
public:
	ID3D11BlendState *pBdState;
	

	void CreateState(ID3D11Device *pDevice, CD3D11_BLEND_DESC bddesc);
	void SetState(ID3D11DeviceContext* pImmediateContext);

	void ReleaseState() {
		pBdState->Release();
	}
};

extern void DrawSet(ID3D11DeviceContext* pImmediateContext, ID3D11InputLayout *Layout, ID3D11Buffer *Vbuff, ID3D11Buffer *Ibuff,
	ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11Buffer *pConstantBuffer, D3D11_VIEWPORT *Viewport);

extern void DrawBuffer(ID3D11DeviceContext* pImmediateContext, UINT indexcount, UINT startind, int startvert);

extern void ColorconvF(float color[], int R, int G, int B, int A);
extern void ColorconvF(float color[], DWORD colorbase);

extern float* ColorconvDF(DWORD color);

extern void Set2DPosvert(Vertex vert[], float w, float h, int count, int startcount, XMFLOAT4X4 &view, XMFLOAT4X4 &proj);

extern void SetTexnull(ID3D11DeviceContext* pImmediateContext);

#endif
