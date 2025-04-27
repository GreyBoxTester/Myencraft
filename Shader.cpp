#include "Shader.h"
#include <d3dcompiler.h>

#pragma comment(lib, "d3dcompiler.lib")

cl::VertexShader::VertexShader(Graphics& gfx, const wchar_t* filename)
{
	wrl::ComPtr<ID3DBlob> blob;
	D3DReadFileToBlob(filename, &blob);
	getDevice(gfx)->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);

	wrl::ComPtr<ID3D11ShaderReflection> reflection;
	D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), __uuidof(ID3D11ShaderReflection), &reflection);

	D3D11_SHADER_DESC reflDesc = {};
	reflection->GetDesc(&reflDesc);
	D3D11_INPUT_ELEMENT_DESC* ieds = new D3D11_INPUT_ELEMENT_DESC[reflDesc.InputParameters]();
	unsigned int byteOffset = 0;

	for (unsigned int i = 0; i < reflDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc = {};
		reflection->GetInputParameterDesc(i, &paramDesc);
		ieds[i].SemanticName = paramDesc.SemanticName;
		ieds[i].SemanticIndex = paramDesc.SemanticIndex;
		ieds[i].InputSlot = 0;
		ieds[i].AlignedByteOffset = byteOffset;
		ieds[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ieds[i].InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			switch (paramDesc.ComponentType)
			{
			case D3D_REGISTER_COMPONENT_UINT32:  ieds[i].Format = DXGI_FORMAT_R32_UINT;  break;
			case D3D_REGISTER_COMPONENT_SINT32:  ieds[i].Format = DXGI_FORMAT_R32_SINT;  break;
			case D3D_REGISTER_COMPONENT_FLOAT32: ieds[i].Format = DXGI_FORMAT_R32_FLOAT; break;
			}

			byteOffset += 4;
		}
		else if (paramDesc.Mask <= 3)
		{
			switch (paramDesc.ComponentType)
			{
			case D3D_REGISTER_COMPONENT_UINT32:  ieds[i].Format = DXGI_FORMAT_R32G32_UINT;  break;
			case D3D_REGISTER_COMPONENT_SINT32:  ieds[i].Format = DXGI_FORMAT_R32G32_SINT;  break;
			case D3D_REGISTER_COMPONENT_FLOAT32: ieds[i].Format = DXGI_FORMAT_R32G32_FLOAT; break;
			}

			byteOffset += 8;
		}
		else if (paramDesc.Mask <= 7)
		{
			switch (paramDesc.ComponentType)
			{
			case D3D_REGISTER_COMPONENT_UINT32:  ieds[i].Format = DXGI_FORMAT_R32G32B32_UINT;  break;
			case D3D_REGISTER_COMPONENT_SINT32:  ieds[i].Format = DXGI_FORMAT_R32G32B32_SINT;  break;
			case D3D_REGISTER_COMPONENT_FLOAT32: ieds[i].Format = DXGI_FORMAT_R32G32B32_FLOAT; break;
			}

			byteOffset += 12;
		}
		else if (paramDesc.Mask <= 15)
		{
			switch (paramDesc.ComponentType)
			{
			case D3D_REGISTER_COMPONENT_UINT32:  ieds[i].Format = DXGI_FORMAT_R32G32B32A32_UINT;  break;
			case D3D_REGISTER_COMPONENT_SINT32:  ieds[i].Format = DXGI_FORMAT_R32G32B32A32_SINT;  break;
			case D3D_REGISTER_COMPONENT_FLOAT32: ieds[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			}

			byteOffset += 16;
		}
	}

	getDevice(gfx)->CreateInputLayout(ieds, reflDesc.InputParameters, blob->GetBufferPointer(), blob->GetBufferSize(), &inpLayout);

	delete[] ieds;
}

cl::VertexShader::VertexShader(Graphics& gfx, const wchar_t* filename, D3D11_INPUT_ELEMENT_DESC* inputLayout, unsigned int count)
{
	wrl::ComPtr<ID3DBlob> blob;
	D3DReadFileToBlob(filename, &blob);
	getDevice(gfx)->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
	getDevice(gfx)->CreateInputLayout(inputLayout, count, blob->GetBufferPointer(), blob->GetBufferSize(), &inpLayout);
}

void cl::VertexShader::bind(Graphics& gfx)
{
	getContext(gfx)->IASetInputLayout(inpLayout.Get());
	getContext(gfx)->VSSetShader(shader.Get(), nullptr, 0);
}

cl::PixelShader::PixelShader(Graphics& gfx, const wchar_t* filename)
{
	wrl::ComPtr<ID3DBlob> blob;
	D3DReadFileToBlob(filename, &blob);
	getDevice(gfx)->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &shader);
}

void cl::PixelShader::bind(Graphics& gfx)
{
	getContext(gfx)->PSSetShader(shader.Get(), nullptr, 0);
}
