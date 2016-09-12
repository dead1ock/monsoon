/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#include "D3D.h"
#include "D3D11Window.h"

#include <platform/types.h>

using namespace Monsoon::Renderer;

const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const float RENDERSCALE = 1.0f;

D3D::D3D()
:
mDevice(nullptr),
mContext(nullptr),
mSwapChain(nullptr),
mRenderTargetTexture(nullptr),
mRenderTargetView(nullptr),
mRenderTargetShaderResource(nullptr),
mDepthStencilBuffer(nullptr),
mDepthStencilView(nullptr),
mRasterStateNoCulling(nullptr),
mRenderTextureTarget(nullptr)
{

}

D3D::~D3D() {

}

bool D3D::Initialize(D3D11Window& renderWindow) {

	if (!CreateDeviceAndSwapChain(renderWindow))
		return false;

	if (!CreateRenderTarget(renderWindow))
		return false;

	if (!CreateRenderTexture(renderWindow))
		return false;

	if (!CreateDepthStencilBuffer(renderWindow))
		return false;

	if (!SetRasterState())
		return false;

	if (!SetViewport(0, 0, renderWindow.getWidth(), renderWindow.getHeight()))
		return false;

	if (!CreateRasterStateNoCull())
		return false;

	//
	// Describe BlendState
	//
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (int i = 0; i < 8; i++)
	{
		blendDesc.RenderTarget[i].BlendEnable = true;
		blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}

	HRESULT result = mDevice->CreateBlendState(&blendDesc, &mBlendState);
	if (FAILED(result))
		return false;

	return true;
}

void D3D::Shutdown() {

	if (mRasterStateNoCulling != nullptr)
		mRasterStateNoCulling->Release();

	if (mBlendState != nullptr)
		mBlendState->Release();

	if (mDepthStencilView != nullptr)
		mDepthStencilView->Release();

	if (mDepthStencilBuffer != nullptr)
		mDepthStencilBuffer->Release();

	if (mRenderTargetShaderResource != nullptr)
		mRenderTargetShaderResource->Release();

	if (mRenderTextureTarget != nullptr)
		mRenderTextureTarget->Release();

	if (mRenderTargetView != nullptr)
		mRenderTargetView->Release();

	if (mRenderTargetTexture != nullptr)
		mRenderTargetTexture->Release();

	if (mSwapChain != nullptr)
		mSwapChain->Release();

	if (mContext != nullptr)
		mContext->Release();

	if (mDevice != nullptr)
		mDevice->Release();
}

void D3D::Present() {
	mSwapChain->Present(1, 0);
}

bool D3D::CreateDeviceAndSwapChain(D3D11Window& renderWindow) {
	HRESULT result;
	U32 createDeviceFlags = 0;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = renderWindow.getWidth();
	swapChainDesc.BufferDesc.Height = renderWindow.getHeight();
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = renderWindow.getHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	if (renderWindow.isFullscreen())
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

#ifdef _DEBUG
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, NULL, &mContext);

	if (FAILED(result))
	{
		std::string errorMessage = "Unable to create D3D Device or Context. Error code: ";
		
		errorMessage.append(std::to_string(result));
		MessageBox(0, errorMessage.c_str(), 0, 0);
		return false;
	}

	if (featureLevel != D3D_FEATURE_LEVEL_11_0)
	{ 
		MessageBox(0, "DirectX 11 Unsupported!", 0, 0);
		return false;
	}

	return true;
}

bool D3D::CreateRenderTarget(D3D11Window& renderWindow) {
	ID3D11Texture2D* backBufferPtr;
	HRESULT result;

	// Get the pointer to the back buffer.
	result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = mDevice->CreateRenderTargetView(backBufferPtr, NULL, &mRenderTargetView);
	if (FAILED(result))
	{
		return false;
	}

	backBufferPtr->Release();
	backBufferPtr = 0;
	return true;
}

bool D3D::CreateDepthStencilBuffer(D3D11Window& renderWindow) {
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDescNoZBuffer;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	HRESULT result;


	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = (float)ceil(renderWindow.getWidth() * RENDERSCALE);
	depthBufferDesc.Height = (float)ceil(renderWindow.getHeight() * RENDERSCALE);
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = mDevice->CreateTexture2D(&depthBufferDesc, NULL, &mDepthStencilBuffer);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(result))
	{
		return false;
	}

	mContext->OMSetDepthStencilState(mDepthStencilState, 1);

	ZeroMemory(&depthStencilDescNoZBuffer, sizeof(depthStencilDescNoZBuffer));

	depthStencilDescNoZBuffer.DepthEnable = false;
	depthStencilDescNoZBuffer.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDescNoZBuffer.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDescNoZBuffer.StencilEnable = true;
	depthStencilDescNoZBuffer.StencilReadMask = 0xFF;
	depthStencilDescNoZBuffer.StencilWriteMask = 0xFF;
	depthStencilDescNoZBuffer.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescNoZBuffer.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDescNoZBuffer.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescNoZBuffer.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDescNoZBuffer.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescNoZBuffer.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDescNoZBuffer.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDescNoZBuffer.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = mDevice->CreateDepthStencilState(&depthStencilDescNoZBuffer, &mDepthStencilStateNoZBuffer);
	if (FAILED(result))
	{
		return false;
	}

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool D3D::SetViewport(int x, int y, int width, int height) {

	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = x;
	viewport.TopLeftY = y;

	mContext->RSSetViewports(1, &viewport);
	return true;
}

bool D3D::SetRasterState() {
	D3D11_RASTERIZER_DESC rasterDesc;
	HRESULT result;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	if (FAILED(result))
	{
		return false;
	}

	mContext->RSSetState(mRasterState);
	return true;
}

bool D3D::CreateRasterStateNoCull() {
	D3D11_RASTERIZER_DESC rasterDesc;
	HRESULT result;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = mDevice->CreateRasterizerState(&rasterDesc, &mRasterStateNoCulling);
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

void D3D::BeginScene() {
	float color[4];
	color[0] = 0.0f;
	color[1] = 0.0f;
	color[2] = 0.0f;
	color[3] = 1.0f;

	mContext->ClearRenderTargetView(mRenderTextureTarget, color);

	mContext->ClearRenderTargetView(mRenderTargetView, color);
	mContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void D3D::EndScene() {

}

void D3D::SetRenderTargetAsBackBuffer()
{
	mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void D3D::SetRenderTargetAsRenderTexture()
{
	mContext->OMSetRenderTargets(1, &mRenderTextureTarget, mDepthStencilView);
}

void D3D::EnableAlphaBlending() {
	mContext->OMSetBlendState(mBlendState, NULL, 0xffffffff);
}

void D3D::DisableAlphaBlending() {
	mContext->OMSetBlendState(NULL, NULL, 0xffffffff);
}

void D3D::TurnOnCulling()
{
	mContext->RSSetState(mRasterState);
}

void D3D::TurnOffCulling()
{
	mContext->RSSetState(mRasterStateNoCulling);
}

void D3D::TurnOnZBuffer()
{
	mContext->OMSetDepthStencilState(mDepthStencilState, 1);
}

void D3D::TurnOffZBuffer()
{
	mContext->OMSetDepthStencilState(mDepthStencilStateNoZBuffer, 1);
}

bool D3D::CreateRenderTexture(D3D11Window& renderWindow)
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC renderTextureDesc;
	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC renderTextureShaderResDesc;

	ZeroMemory(&renderTextureDesc, sizeof(renderTextureDesc));

	renderTextureDesc.Width = (int)ceil(renderWindow.getWidth() * RENDERSCALE);
	renderTextureDesc.Height = (int)ceil(renderWindow.getHeight() * RENDERSCALE);
	renderTextureDesc.MipLevels = 1;
	renderTextureDesc.ArraySize = 1;
	renderTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	renderTextureDesc.SampleDesc.Count = 1;
	renderTextureDesc.SampleDesc.Quality = 0;
	renderTextureDesc.Usage = D3D11_USAGE_DEFAULT;
	renderTextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	renderTextureDesc.CPUAccessFlags = 0;
	renderTextureDesc.MiscFlags = 0;

	result = mDevice->CreateTexture2D(&renderTextureDesc, NULL, &mRenderTargetTexture);

	if (FAILED(result))
		return false;

	renderTargetViewDesc.Format = renderTextureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetViewDesc.Texture2D.MipSlice = 0;

	result = mDevice->CreateRenderTargetView(mRenderTargetTexture, NULL, &mRenderTextureTarget);
	if (FAILED(result))
		return false;

	renderTextureShaderResDesc.Format = renderTextureDesc.Format;
	renderTextureShaderResDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	renderTextureShaderResDesc.Texture2D.MostDetailedMip = 0;
	renderTextureShaderResDesc.Texture2D.MipLevels = 1;

	result = mDevice->CreateShaderResourceView(mRenderTargetTexture, &renderTextureShaderResDesc, &mRenderTargetShaderResource);
	if (FAILED(result))
		return false;

	return true;
}