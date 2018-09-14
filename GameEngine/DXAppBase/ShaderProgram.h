#pragma once

#include<memory>
#include<string>
#include<d3d11.h>
#include<vector>
#include<wrl\client.h>

using namespace std;
using namespace Microsoft::WRL;

namespace dxgame {

	struct ShaderBuffer {
		char*		 bytes;
		uint32_t	 size;

		void resize(uint32_t size);
		ShaderBuffer();
		~ShaderBuffer();
	};

	class ShaderProgram
	{
	public:
		explicit ShaderProgram();
		explicit ShaderProgram(const string& vertPath, const string& fragPath, ComPtr<ID3D11Device> device);
		virtual ~ShaderProgram();

		void LoadShader(const string& vertPath, const string& fragPath);
		void CreateProgram(ComPtr<ID3D11Device> device);
		void CreateLayout(ComPtr<ID3D11Device> device, vector<D3D11_INPUT_ELEMENT_DESC>& pInputElementDescs);
		void ActiveProgram(ComPtr<ID3D11DeviceContext> context);

	private:
		bool LoadFromFile(const string& path, ShaderBuffer& buffer);

		ShaderBuffer				m_VertexBuffer;
		ShaderBuffer				m_FragmentBuffer;
		ComPtr<ID3D11VertexShader>	m_VertexProgram;
		ComPtr<ID3D11PixelShader>	m_FragmentProgram;
		ComPtr<ID3D11InputLayout>	m_InputLayout;
	};

}