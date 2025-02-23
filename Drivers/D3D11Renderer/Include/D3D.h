/**
* Copyright (c) 2014-2016 Dallin Wellington
*
*/

#ifndef _MONSOON_D3D_H_
#define _MONSOON_D3D_H_

/**
# Disables a warning about macro redefinitions. There exists a conflict between the old DXSDK
# and the new version of the DXSDK which resides in the Windows Platform libraries.This can be
# safely ignored for now, but it means the program will run off of depreciated macro definitions.
# Read more here : https ://msdn.microsoft.com/en-us/library/windows/desktop/ee663275.aspx
*/
#pragma warning( push )
#pragma warning( disable : 4005 )
#include <d3d11.h>
#include <DXGI.h>
#include <D3DX10math.h>

#include "D3D11Window.h"


#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <memory>

#include <boost/noncopyable.hpp>

namespace Monsoon {
	namespace Renderer {

		/**
		* Initializes and destroys D3D11 Device/Context and provides a wrapper for D3D commands.
		*/
		class D3D : boost::noncopyable
		{
		public:
			D3D();
			~D3D();

			bool Initialize(D3D11Window& renderWindow);
			void Shutdown();

			void BeginScene();
			void EndScene();

			void EnableAlphaBlending();
			void DisableAlphaBlending();

			void TurnOnCulling();
			void TurnOffCulling();

			/**
			 * Instructs the video card to set the current render target to
			 * the Back Buffer.
			 */
			void SetRenderTargetAsBackBuffer();

			/**
			 * Instructs the video card to set the current render target to our
			 * current render texture.
			 */
			void SetRenderTargetAsRenderTexture();

			void TurnOnZBuffer();
			void TurnOffZBuffer();

			void Present();

			bool SetViewport(int x, int y, int width, int height);

			ID3D11Device* GetDevice() { return mDevice; }
			ID3D11DeviceContext* GetContext() { return mContext; }

			ID3D11ShaderResourceView* GetRenderTextureShaderResource() { return mRenderTargetShaderResource; }

		private:
			bool CreateDeviceAndSwapChain(D3D11Window& renderWindow);
			bool CreateRenderTarget(D3D11Window& renderWindow);
			bool CreateDepthStencilBuffer(D3D11Window& renderWindow);
			bool SetRasterState();
			bool CreateRasterStateNoCull();
			bool CreateRenderTexture(D3D11Window& renderWindow);

			ID3D11Device* mDevice;
			ID3D11DeviceContext* mContext;

			IDXGISwapChain* mSwapChain;
			ID3D11RenderTargetView* mRenderTargetView;
			ID3D11Texture2D* mDepthStencilBuffer;
			ID3D11DepthStencilView* mDepthStencilView;
			ID3D11DepthStencilState* mDepthStencilState;
			ID3D11DepthStencilState* mDepthStencilStateNoZBuffer;
			ID3D11RasterizerState* mRasterState;
			ID3D11BlendState* mBlendState;
			ID3D11RasterizerState* mRasterStateNoCulling;
			D3D11_VIEWPORT viewport;

			// VR Distortion Effect
			ID3D11RenderTargetView* mRenderTextureTarget;
			ID3D11Texture2D* mRenderTargetTexture;
			ID3D11ShaderResourceView* mRenderTargetShaderResource;
		};

	} // namespace renderer
} // namespace monsoon

#pragma warning( pop )

#endif // _MONSOON_D3D11_H_