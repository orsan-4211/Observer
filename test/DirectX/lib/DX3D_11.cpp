#include <windows.h>
#include <WindowsX.h>
#include <stdio.h>
#include <shlobj.h>
#include <crtdbg.h>
#include <iostream>
#include <direct.h>

#include "DX3D_11.h"

ID3D11ShaderResourceView* nullview = { nullptr };

//DX113D
DX113D::DX113D()
{
	pDevice = NULL;
	pImmediateContext = NULL;
	pSwapChain = NULL;
	pRenderTargetView = NULL;
}

DX113D::~DX113D()
{
	Release();
}

HRESULT DX113D::Create(HWND hwnd, int Width, int Height, int windowf)
{
	HRESULT  hr;


	UINT flags = 0;
#ifdef _DEBUG
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL pLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL level;

	DXGI_SWAP_CHAIN_DESC scDesc;

//	DXGI_SWAP_CHAIN_FULLSCREEN_DESC scfDesc;



	ZeroMemory(&scDesc, sizeof(scDesc));
	scDesc.BufferCount = 1;
	scDesc.BufferDesc.Width = Width;
	scDesc.BufferDesc.Height = Height;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.OutputWindow = hwnd;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.Windowed = windowf;
	
	//scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;	// ストアアプリではDISCARDじゃなくSEQUENTIAL限定か？.
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		pLevels,
		1,
		D3D11_SDK_VERSION,
		&scDesc,
		&pSwapChain,
		&pDevice,
		&level,
		&pImmediateContext);
	if(FAILED(hr)) return hr;


	ID3D11Texture2D* pBackBuffer;

	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if(FAILED(hr)) return hr;

	hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
	pBackBuffer->Release();

	if(FAILED(hr)) return hr;

	hr = pDevice->CreatePixelShader(
		g_ntxps_main,
		sizeof(g_ntxps_main),
		NULL,
		&nottexps
	);

	return hr;
}

void DX113D::Release()
{
	if(pImmediateContext) {
		pImmediateContext->ClearState();
	}

	if(NULL != pRenderTargetView) {
		pRenderTargetView->Release();
		pRenderTargetView = NULL;
	}
	if(NULL != pSwapChain) {
		pSwapChain->Release();
		pSwapChain = NULL;
	}
	if(NULL != pImmediateContext) {
		pImmediateContext->Release();
		pImmediateContext = NULL;
	}
	if(NULL != pDevice) {
		pDevice->Release();
		pDevice = NULL;
	}
}

void DX113D::RenderSet(/*int R, int G, int B, int A*/)
{
	if(NULL == pDevice) return;
	if(NULL == pImmediateContext) return;
	if(NULL == pRenderTargetView) return;
	if(NULL == pSwapChain) return;

	/*float clearColor[4]; 

	ColorconvF(clearColor, R, G, B, A);
*/
	/*pImmediateContext->OMSetRenderTargets(1, &pRenderTargetView, NULL);
	pImmediateContext->ClearRenderTargetView(pRenderTargetView, clearColor);*/
	
}

void DX113D::RenderTargetSet(int numviews, ID3D11RenderTargetView* RTView, ID3D11DepthStencilView *DSview, float * col, UINT clearflag, float depth) {
	pImmediateContext->OMSetRenderTargets(numviews/*1*/, &RTView, DSview);
	pImmediateContext->ClearRenderTargetView(RTView, col);
	if(DSview != NULL)pImmediateContext->ClearDepthStencilView(DSview, clearflag, depth , 0);
}


//DX11DSBuffer

HRESULT DX11DSBuffer::Create(ID3D11Device *pDevice, int Width, int Height) {

	HRESULT  hr;

	ZeroMemory(&txDesc, sizeof(txDesc));
	txDesc.Width = Width;
	txDesc.Height = Height;
	txDesc.MipLevels = 1;
	txDesc.ArraySize = 1;
	txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	txDesc.SampleDesc.Count = 1;
	txDesc.SampleDesc.Quality = 0;
	txDesc.Usage = D3D11_USAGE_DEFAULT;
	txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	txDesc.CPUAccessFlags = 0;
	txDesc.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&txDesc, NULL, &pDepthStencilTexture);
	if (FAILED(hr))
		return hr;

	
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Format = txDesc.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(pDepthStencilTexture, &dsDesc, &pDepthStencilView);
	if (FAILED(hr))
		return hr;
}

void DX11DSBuffer::ReleseBuffer() {
	
	pDepthStencilTexture->Release();
	pDepthStencilView->Release();

}

//シェーダーコンパイル

void shadererror(ID3DBlob* pErrorMsg) {
	const char *cc = (const char *)pErrorMsg->GetBufferPointer();
	char c[2048];
	TCHAR tc[2048];
	size_t leng;

	leng = 0;

	strcpy_s(c, cc);

	//WideCharToMultiByte(CP_ACP, 0, tc, -1, c, sizeof(c), NULL, NULL);
	mbstowcs_s(&leng, tc, sizeof(c), c, 2048);
	MessageBox(NULL, tc, TEXT("Compile Error"), MB_OK);
}

void DX11Shader::Shadercompile(LPCSTR entrypoint, LPCSTR target, TCHAR *file){
	ID3DBlob* pErrorMsg;
	HRESULT hr = D3DCompileFromFile(
		file,
		NULL,
		NULL,
		entrypoint,
		target,
		0,
		0,
		&pBlob,
		&pErrorMsg
	);
	

	if (FAILED(hr))
	{
		// シェーダのエラー内容を表示
		switch (hr) {
		case 0x80070002:
			MessageBox(NULL, TEXT("指定されたファイルは見つかりません。"), TEXT("Compile Error"), MB_OK);
			break;
		case 0x80070003:
			MessageBox(NULL, TEXT("指定されたパスは見つかりません。"), TEXT("Compile Error"), MB_OK);
			break;
		case E_FAIL:
			shadererror(pErrorMsg);
			break;
		default:
			MessageBox(NULL, TEXT("Error"), TEXT("Compile Error"), MB_OK);
			break;
		}

	}

}

bool DX11PShader::CreateShaderP(ID3D11Device &pDevice, const void *pointer, size_t size){
	
	HRESULT hr;
	
	hr = pDevice.CreatePixelShader(
		pointer,
		size,
		NULL,
		&Shader
	);

	return SUCCEEDED(hr);
}

void DX11PShader::ReleaseShader() {
	Shader->Release();
}

bool DX11VShader::CreateShaderV(ID3D11Device &pDevice, const void *pointer, size_t size) {

	HRESULT hr;
	
	hr = pDevice.CreateVertexShader(
		pointer,
		size,
		NULL,
		&Shader
	);

	return SUCCEEDED(hr);
}

void DX11VShader::ReleaseShader() {
	Shader->Release();
}

HRESULT DX11Buffeer::CreateBuffer(ID3D11Device *pDevice, UINT ByteWidth, D3D11_USAGE Usage, UINT BindFlags, UINT CPUAccessFlags, UINT MiscFlags, UINT StructureByteStride
									/*, const void* pSysMem, UINT SysMemPitch, UINT SysMemSlicePitch*/) {

	HRESULT hr;

	memset(&bufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.ByteWidth = ByteWidth;
	bufferDesc.Usage = Usage;
	bufferDesc.BindFlags = BindFlags;
	bufferDesc.CPUAccessFlags = CPUAccessFlags;
	bufferDesc.MiscFlags = MiscFlags;
	bufferDesc.StructureByteStride = StructureByteStride;


	/*subResourceData.pSysMem = pSysMem;
	subResourceData.SysMemPitch = SysMemPitch;
	subResourceData.SysMemSlicePitch = SysMemSlicePitch;*/

	hr = pDevice->CreateBuffer(&bufferDesc, NULL/*&subResourceData*/, &Buffer);
	if (FAILED(hr))	return hr;

}

void DX11Buffeer::Dataset(ID3D11DeviceContext* pImmediateContext, const void* pSysMem, size_t size) {
	pImmediateContext->Map(Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, pSysMem, size); 
	pImmediateContext->Unmap(Buffer, 0);

}

void DX11Buffeer::ReleaseBuffer() {
	Buffer->Release();
}

DX11InputLayout::DX11InputLayout() {
	vertDesc = (D3D11_INPUT_ELEMENT_DESC *)malloc(sizeof(D3D11_INPUT_ELEMENT_DESC) * 1);
}

void DX11InputLayout::DescSet(LPCSTR SemanticName, UINT SemanticIndex, DXGI_FORMAT Format, UINT InputSlot,
								UINT AlignedByteOffset, D3D11_INPUT_CLASSIFICATION InputSlotClass, UINT InstanceDataStepRate) {

	vertDesc = (D3D11_INPUT_ELEMENT_DESC *)realloc(vertDesc, sizeof(D3D11_INPUT_ELEMENT_DESC) * (arraysize + 1));

	vertDesc[arraysize].SemanticName = SemanticName;
	vertDesc[arraysize].SemanticIndex = SemanticIndex;
	vertDesc[arraysize].Format = Format;
	vertDesc[arraysize].InputSlot = InputSlot;
	vertDesc[arraysize].AlignedByteOffset = AlignedByteOffset;
	vertDesc[arraysize].InputSlotClass = InputSlotClass;
	vertDesc[arraysize].InstanceDataStepRate = InstanceDataStepRate;

	arraysize++;
}

void DX11InputLayout::DescDelete() {

	arraysize = 0;
	free(vertDesc);
	Layout->Release();

}

HRESULT DX11InputLayout::CreateIL(ID3D11Device *pDevice, const void *pointer, size_t size) {

	HRESULT hr;

	hr = pDevice->CreateInputLayout(vertDesc, arraysize, pointer, size, &Layout);
	if (FAILED(hr))
		return hr;
}

HRESULT DX11RendTex::CreateTex(ID3D11Device *pDevice, D3D11_USAGE Usage, DXGI_FORMAT Format, UINT BindFlags, UINT Width, UINT Height,
	UINT CPUAccessFlags, UINT MipLevels, UINT ArraySize, UINT Count, UINT Quality, DXGI_FORMAT RtFormat) {
	D3D11_TEXTURE2D_DESC texDesc;
	HRESULT hr;

	memset(&texDesc, 0, sizeof(texDesc));
	texDesc.Usage = Usage;//D3D11_USAGE_DEFAULT;
	texDesc.Format = Format;// DXGI_FORMAT_R8G8B8A8_TYPELESS;
	texDesc.BindFlags = BindFlags;// D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = Width;
	texDesc.Height = Height;
	texDesc.CPUAccessFlags = CPUAccessFlags;// 0;
	texDesc.MipLevels = MipLevels;// 1;
	texDesc.ArraySize = ArraySize;// 1;
	texDesc.SampleDesc.Count = Count;// 1;
	texDesc.SampleDesc.Quality = Quality;// 0;

	hr = pDevice->CreateTexture2D(&texDesc, NULL, &rtex);
	if (FAILED(hr))
	{
		OutputDebugString(TEXT("Error : ID3D11Device::CreateTexture2D() Failed."));
		return false;
	}


	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	memset(&rtvDesc, 0, sizeof(rtvDesc));
	rtvDesc.Format = RtFormat;//DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;


	hr = pDevice->CreateRenderTargetView(rtex, &rtvDesc, &rtview);
	if (FAILED(hr))
	{
		OutputDebugString(TEXT("Error : ID3D11Device::CreateRenderTargetView() Failed."));
		return false;
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	memset(&srvDesc, 0, sizeof(srvDesc));
	srvDesc.Format = rtvDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;


	hr = pDevice->CreateShaderResourceView(rtex, &srvDesc, &view);
	if (FAILED(hr))
	{
		OutputDebugString(TEXT("Error : ID3D11Device::CreateShaderResourceView() Failed."));
		return false;
	}

	texDesc.Width = Width;
	texDesc.Height = Height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = Count;// 1;
	texDesc.SampleDesc.Quality = Quality;// 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&texDesc, NULL, &dstex);
	if (FAILED(hr)) {
		OutputDebugString(TEXT("Error : ID3D11Device::CreateShaderResourceView() Failed."));
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = texDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Flags = 0;
	hr = pDevice->CreateDepthStencilView(dstex, &dsvDesc, &dsview);
	if (FAILED(hr)) {
		OutputDebugString(TEXT("Error : ID3D11Device::CreateShaderResourceView() Failed."));
		return false;
	}

	/*D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Format = texDesc.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(rtex, &dsDesc, &dsview);
	if (FAILED(hr))
		return hr;*/

}

HRESULT DX11Tex::TexLoadFromFile(ID3D11Device *pDevice, const wchar_t *path) {
	
	HRESULT hr;

	hr = CreateWICTextureFromFile(pDevice, path, &tex, &view);
	if (FAILED(hr)) {
		switch (hr) {
		case 0x80070002:
			OutputDebugString(TEXT("TexLoadFromFile: 指定されたファイルが見つかりません。\n"));
			break;
		default:
			OutputDebugString(TEXT("TexLoadFromFile: テクスチャが読み込めませんでした。\n"));
			break;
		}
	}
		return hr;
}

void DX11Tex::SetTex(ID3D11DeviceContext* pImmediateContext) {
	pImmediateContext->PSSetShaderResources(0, 1, &view);
}

HRESULT DX11SAMPLER::SamplerCreate(ID3D11Device *pDevice, D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU,
									D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW, D3D11_COMPARISON_FUNC ComparisonFunc,
									float MinLOD, float MaxLOD) {
	D3D11_SAMPLER_DESC smpDesc;
	HRESULT hr;

	::ZeroMemory(&smpDesc, sizeof(D3D11_SAMPLER_DESC));
	smpDesc.Filter = Filter/*D3D11_FILTER_MIN_MAG_MIP_LINEAR*/;
	smpDesc.AddressU = AddressU/*D3D11_TEXTURE_ADDRESS_CLAMP*/;
	smpDesc.AddressV = AddressV/*D3D11_TEXTURE_ADDRESS_CLAMP*/;
	smpDesc.AddressW = AddressW/*D3D11_TEXTURE_ADDRESS_CLAMP*/;
	smpDesc.ComparisonFunc = ComparisonFunc;//D3D11_COMPARISON_NEVER;
	smpDesc.MinLOD = MinLOD;//0;
	smpDesc.MaxLOD = MaxLOD;//D3D11_FLOAT32_MAX;
	hr = pDevice->CreateSamplerState(&smpDesc, &pSampler);
	if (FAILED(hr))
		return hr;
}

void DX11SAMPLER::SetSampler(ID3D11DeviceContext* pImmediateContext) {
	pImmediateContext->PSSetSamplers(0, 1, &pSampler);
}


void DX11RSState::CreateState(ID3D11Device *pDevice, CD3D11_RASTERIZER_DESC rsdesc) {
	
	pDevice->CreateRasterizerState(&rsdesc, &pRsState);
}

void DX11RSState::SetState(ID3D11DeviceContext* pImmediateContext) {
	pImmediateContext->RSSetState(pRsState);
}

void DX11DSState::CreateState(ID3D11Device *pDevice, CD3D11_DEPTH_STENCIL_DESC dsdesc) {

	pDevice->CreateDepthStencilState(&dsdesc, &pDsState);
}

void DX11DSState::SetState(ID3D11DeviceContext* pImmediateContext) {
	pImmediateContext->OMSetDepthStencilState(pDsState, 0);
}

void DX11BDState::CreateState(ID3D11Device *pDevice, CD3D11_BLEND_DESC bddesc) {

	pDevice->CreateBlendState(&bddesc, &pBdState);
}

void DX11BDState::SetState(ID3D11DeviceContext* pImmediateContext) {
	pImmediateContext->OMSetBlendState(pBdState, NULL, 0xfffffff);
}

//描画

void DrawSet(ID3D11DeviceContext* pImmediateContext, ID3D11InputLayout *Layout, ID3D11Buffer *Vbuff, ID3D11Buffer *Ibuff,
	ID3D11VertexShader* vShader, ID3D11PixelShader* pShader, ID3D11Buffer *pConstantBuffer, D3D11_VIEWPORT *Viewport) {
	UINT strides = sizeof(Vertex);
	UINT offsets = 0;

	pImmediateContext->IASetInputLayout(Layout);
	pImmediateContext->IASetVertexBuffers(0, 1, &Vbuff, &strides, &offsets);
	pImmediateContext->IASetIndexBuffer(Ibuff, DXGI_FORMAT_R16_UINT, 0);
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pImmediateContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
	pImmediateContext->VSSetShader(vShader, NULL, 0);
	pImmediateContext->RSSetViewports(1, Viewport);
	pImmediateContext->PSSetShader(pShader, NULL, 0);
	
}

void DrawBuffer(ID3D11DeviceContext* pImmediateContext, UINT indexcount, UINT startind, int startvert){

	pImmediateContext->DrawIndexed(indexcount, startind, startvert);

}

//色コード変換
void ColorconvF(float color[], int R, int G, int B, int A) {
	color[0] = R / 255.0;
	color[1] = G / 255.0;
	color[2] = B / 255.0;
	color[3] = A / 255.0;
}

void ColorconvF(float color[], DWORD colorbase) {
	DWORD ibuff;
	ibuff = colorbase;

	color[3] = (int)(ibuff / 0x01000000) / 255.0;
	ibuff = (int)(ibuff % 0x01000000);
	color[0] = (int)(ibuff / 0x00010000) / 255.0;
	ibuff = (int)(ibuff % 0x00010000);
	color[1] = (int)(ibuff / 0x00000100) / 255.0;
	ibuff = (int)(ibuff % 0x00000100);
	color[2] = (int)(ibuff / 0x00000001) / 255.0;
}


float* ColorconvDF(DWORD color) {
	float buff[4];//RGBA
	int ibuff;

	ibuff = color;

	buff[3] = (int)(ibuff / 0x01000000) / 255.0;
	ibuff = (int)(ibuff % 0x01000000);
	buff[0] = (int)(ibuff / 0x00010000) / 255.0;
	ibuff = (int)(ibuff % 0x00010000);
	buff[1] = (int)(ibuff / 0x00000100) / 255.0;
	ibuff = (int)(ibuff % 0x00000100);
	buff[2] = (int)(ibuff / 0x00000001) / 255.0;

	return buff;
}

//座標変換

void Set2DPosvert(Vertex vert[], float w, float h, int count, int startcount, XMFLOAT4X4 &view, XMFLOAT4X4 &proj) {
	XMFLOAT4X4 buff;
	XMFLOAT3 posbuf;
	XMVECTOR buffv;
	XMVECTOR resultbuff;

	XMMATRIX viewm;
	XMMATRIX projm;

	XMMATRIX port = XMMatrixIdentity();
	XMStoreFloat4x4(&buff, port);

	viewm = XMLoadFloat4x4(&view);
	projm = XMLoadFloat4x4(&proj);

	viewm = XMMatrixInverse(NULL, viewm);
	projm = XMMatrixInverse(NULL, projm);

	buff._11 = w / 2.0f;	buff._22 = h / 2.0f*-1;
	buff._41 = w / 2.0f;	buff._42 = h / 2.0f;
	
	port = XMLoadFloat4x4(&buff);
	port = XMMatrixInverse(NULL, port);

	XMMATRIX tmp;
	tmp = XMMatrixMultiply(port, projm);
	tmp = XMMatrixMultiply(tmp, viewm);

	for (int i = startcount; i < count; i++) {
		
		posbuf.x = vert[i].x;
		posbuf.y = vert[i].y;
		posbuf.z = vert[i].z;

		buffv = XMLoadFloat3(&posbuf);
		resultbuff = XMVector3TransformCoord(buffv, tmp);
		
		XMStoreFloat3(&posbuf, resultbuff);
		
		vert[i].x = posbuf.x;
		vert[i].y = posbuf.y;
		vert[i].z = posbuf.z;
	}

}

void SetTexnull(ID3D11DeviceContext* pImmediateContext) {
	nullview = NULL;
	pImmediateContext->PSSetShaderResources(0, 1, &nullview);
}
