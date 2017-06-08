#pragma once

// As stated previously the ModelClass is responsible for encapsulating the geometry for 3D models.
// In this tutorial we will manually setup the data for a single green triangle. We will also create
// a vertex and index buffer for the triangle so that it can be rendered.

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

class ModelClass
{
public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	// The functions here handle initializing and shutdown of the model's vertex and index buffers. The Render function puts the model
	// geometry on the video card to prepare it for drawing by the color shader.
	bool Initialize(ID3D11Device*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	// Here is the definition of our vertex type that will be used with the vertex buffer in this ModelClass.
	// Also take note that this typedef must match the layout in the ColorShaderClass that will be looked at later in the tutorial.
	struct VertexType
	{
		// XMFLOAT3: Describes a 3D vector consisting of three single-precision floating-point values.
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	
};