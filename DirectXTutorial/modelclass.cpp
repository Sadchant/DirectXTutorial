#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


// The Initialize function will call the initialization functions for the vertex and index buffers.
bool ModelClass::Initialize(ID3D11Device* device)
{
	bool result;

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}
	return true;
}

// The Shutdown function will call the shutdown functions for the vertex and index buffers.
void ModelClass::Shutdown()
{
	// Shutdown the vertex and index buffers.
	ShutdownBuffers();
	return;
}

// Render is called from the GraphicsClass::Render function.This function calls RenderBuffers to put the vertex and index buffers 
// on the graphics pipeline so the color shader will be able to render them.
void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
	return;
}


// GetIndexCount returns the number of indexes in the model. The color shader will need this information to draw this model.
int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

// The InitializeBuffers function is where we handle creating the vertex and index buffers. Usually you would read in a model
// and create the buffers from that data file. For this tutorial we will just set the points in the vertex and index buffer manually
// since it is only a single triangle.
bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	// das struct mit position und Farbe
	VertexType* vertices;
	unsigned long* indices;
	// D3D11_BUFFER_DESC: Describes a buffer resource.
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	// D3D11_SUBRESOURCE_DATA: Specifies data for initializing a subresource.
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	//First create two temporary arrays to hold the vertex and index data that we will use later to populate the final buffers with.

	// Set the number of vertices in the vertex array.
	m_vertexCount = 3;

	// Set the number of indices in the index array.
	m_indexCount = 3;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if (!indices)
	{
		return false;
	}

	// Now fill both the vertex and index array with the three points of the triangle as well as the index to each of the points.
	// Please note that I create the points in the clockwise order of drawing them. If you do this counter clockwise it will think
	// the triangle is facing the opposite direction and not draw it due to back face culling. Always remember that the order in which
	// you send your vertices to the GPU is very important. The color is set here as well since it is part of the vertex description.
	// I set the color to green.

	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, -1.0f, 0.0f);  // Bottom left.
	vertices[0].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	vertices[1].position = XMFLOAT3(0.0f, 1.0f, 0.0f);  // Top middle.
	vertices[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, 0.0f);  // Bottom right.
	vertices[2].color = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  // Bottom left.
	indices[1] = 1;  // Top middle.
	indices[2] = 2;  // Bottom right.

	// With the vertex array and index array filled out we can now use those to create the vertex buffer and index buffer. Creating both
	// buffers is done in the same fashion. First fill out a description of the buffer. In the description the ByteWidth(size of the buffer)
	// and the BindFlags(type of buffer) are what you need to ensure are filled out correctly. After the description is filled out you need
	// to also fill out a subresource pointer which will point to either your vertex or index array you previously created. With the description
	// and subresource pointer you can call CreateBuffer using the D3D device and it will return a pointer to your new buffer.

	// Set up the description of the static vertex buffer.
	// gibt an, wie Grafikkarte und CPU auf dem Buffer lesen/schreiben sollen, DEFAULT: Grafikkarte kann lesen und schreiben
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//Size of the buffer in bytes.
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	// Identify how the buffer will be bound to the pipeline.
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	// 0 if no CPU access is necessary
	vertexBufferDesc.CPUAccessFlags = 0;
	// sonstige Flags, kann dem Buffer verschiedene Eigenschaften geben
	vertexBufferDesc.MiscFlags = 0;
	// The size of each element in the buffer structure (in bytes) when the buffer represents a structured buffer.
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	// Pointer to the initialization data.
	// Zeiger auf den Buffer
	vertexData.pSysMem = vertices;
	// The distance (in bytes) from the beginning of one line of a texture to the next line.
	vertexData.SysMemPitch = 0;
	// The distance(in bytes) from the beginning of one depth level to the next.
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	// Creates a buffer (vertex buffer, index buffer, or shader-constant buffer).
	// der dritte Parameter wird von CreateBuffer befüllt
	// This method returns E_OUTOFMEMORY if there is insufficient memory to create the buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// After the vertex buffer and index buffer have been created you can delete the vertex and index arrays as they are no longer needed since the data was copied into the buffers.

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

// The ShutdownBuffers function just releases the vertex buffer and index buffer that were created in the InitializeBuffers function.
void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if (m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}
	return;
}

// RenderBuffers is called from the Render function.The purpose of this function is to set the vertex buffer and index buffer as active
// on the input assembler in the GPU. Once the GPU has an active vertex buffer it can then use the shader to render that buffer. This
// function also defines how those buffers should be drawn such as triangles, lines, fans, and so forth. In this tutorial we set the vertex
// buffer and index buffer as active on the input assembler and tell the GPU that the buffers should be drawn as triangles using the IASetPrimitiveTopology DirectX function.

void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	// The Direct3D 10 and higher API separates functional areas of the pipeline into stages; the first stage in the pipeline is the input-assembler (IA) stage. 
	// The purpose of the input - assembler stage is to read primitive data(points, lines and / or triangles) from user - filled buffers and assemble the data 
	// into primitives that will be used by the other pipeline stages.
	deviceContext->IASetVertexBuffers(0, // The first input slot for binding. The first vertex buffer is explicitly bound to the start slot
		1,								// The number of vertex buffers in the array.
		&m_vertexBuffer,				// A pointer to an array of vertex buffers
		&stride,						// Pointer to an array of stride (Schritt) values; one stride value for each buffer in the vertex-buffer array. Each stride is the size (in bytes) of the elements that are to be used from that vertex buffer.
		&offset);						// Pointer to an array of offset values; one offset value for each buffer in the vertex-buffer array. Each offset is the number of bytes between the first element of a vertex buffer and the first element that will be used.

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer,	// A pointer to an ID3D11Buffer object, that contains indices.
		DXGI_FORMAT_R32_UINT,						// A DXGI_FORMAT that specifies the format of the data in the index buffer.
		0);											// Offset (in bytes) from the start of the index buffer to the first index to use.

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	// Bind information about the primitive type, and data order that describes input data for the input assembler stage.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}



