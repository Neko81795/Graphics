#include "Texture.h"

#include "GraphicsEngine.h"


namespace Graphics
{
	Texture::Texture(Graphics::GraphicsEngine& graphics, std::wstring path) : Graphics(graphics), Path(path)
	{
		ULONG_PTR gdiplustoken;
		Gdiplus::GdiplusStartupInput inp;
		Gdiplus::GdiplusStartup(&gdiplustoken, &inp, nullptr);
		Gdiplus::Bitmap bitmap(path.c_str());

		if (bitmap.GetLastStatus() == Gdiplus::Status::InvalidParameter)
			throw std::exception("Failed to load image");

		Width = bitmap.GetWidth();
		Height = bitmap.GetHeight();
		Format = bitmap.GetPixelFormat();

		Gdiplus::Rect rect(0, 0, static_cast<int>(Width), static_cast<int>(Height));
		Gdiplus::BitmapData data;

		if (Gdiplus::Ok == bitmap.LockBits(&rect, Gdiplus::ImageLockModeRead, Format, &data))
		{
			size_t len = data.Height * std::abs(data.Stride);
			Bytes.resize(len);

			memcpy(&Bytes[0], data.Scan0, len);

			bitmap.UnlockBits(&data);
		}
		else
		{
			throw std::exception("NOT SURE WHAT HAPPEN");
		}
	}

	void Texture::Create()
	{
		D3D11_TEXTURE2D_DESC1 texDesc{};
		texDesc.Width = Width;
		texDesc.Height = Height;
		texDesc.MipLevels = 1;

		if (Format == PixelFormat32bppARGB)
		{

			texDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UINT;
			texDesc.ArraySize = 4 * texDesc.Width * texDesc.Height;
		}
		else
		{
			throw std::exception("FORMAT UNSUPORTED");
		}

		texDesc.Usage = D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
		texDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data{};
		data.pSysMem = &Bytes[0];

		Graphics.Device->CreateTexture2D1(&texDesc, &data, Tex.ReleaseAndGetAddressOf());

	}

	void Texture::Use()
	{
	}
}

